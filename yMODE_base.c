/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



/*===[[ FILE-WIDE VARIABLES ]]================================================*/
#define     MAX_STACK   50
char    g_mode_stack    [MAX_STACK]; /* vi-like mode stack             */
int     g_mode_depth;                /* depth of current mode stack    */
char    g_mode_curr;                 /* current mode in stack          */
char    g_message       [LEN_RECD];
char    g_last       = '-';
char    g_text          [LEN_TERSE] = "··";

char   *g_allow  [MAX_MODES] = { NULL };
char    g_actual [MAX_MODES] [LEN_DESC];
char   *g_mesg   [MAX_MODES] = { NULL };


const tMODE_INFO  g_modes [MAX_MODES] = {
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat  ---expected-status----------------------   ---description--------------------------------------- ---message-------------------------------------------------------------------------------  */
   /*---(fundamental)---------------- --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { FMOD_STATUS  , MODE_FUND  , '-', "sta", "status"    , 0, "----- p i ----- n ----- r ---------- d o", "internal status and locking to prevent trouble"     , ""                                                                                        },
   { FMOD_MODE    , MODE_FUND  , '-', "mod", "modes"     , 0, "5---- p i ----- n ----- r ---------- d o", "internal mode setting and tracking"                 , ""                                                                                        },
   { FMOD_VIEW    , MODE_FUND  , '-', "vew", "viewing"   , 0, "5-:-- p i ----- n ----- r ---------- d o", "internal opengl and curses drawing manangement"     , ""                                                                                        },
   { FMOD_FILE    , MODE_FUND  , '-', "fil", "files"     , 0, "5---- p i :---- n 1---- r ---------- d o", "internal file reading and writing"                  , ""                                                                                        },
   /*---(map)------------------------ --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { MODE_MAP     , MODE_MAJOR , 'y', "MAP", "map"       , 1, "5---- p i ----- n 1---- r 0--------- d o", "map-mode providing 2D review of object collections" , "horz(a)=0HhlL$  horz(g/z)=sh,le  vert(a)=_KkjJG  vert(g/z)=tk.jb  modes=vIFV:{ret}"      },
   { UMOD_VISUAL  , MODE_MICRO , 'y', "vis", "visual"    , 1, "5f--- p i ----- n ----- r 0M-------- d o", "visual selection history and access"                , "index=a-zA-Z0-9   special=!?"                                                            },
   { SMOD_MREG    , MODE_SUB   , 'y', "reg", "register"  , 1, "5f--- p i ----- n 1---- r 0M-------- d o", "selecting specific registers for data movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           },
   { UMOD_MARK    , MODE_MICRO , 'y', "mrk", "mark"      , 1, "5f:-- p i ----- n ----- r 0M-------- d o", "object and location marking"                        , "names=a-zA-Z0-9  actions=#!?_  special='[()]  wander=@  range=<>*"                       },
   { SMOD_MACRO   , MODE_SUB   , 'y', "mac", "macro"     , 1, "5f:-- p i ----- n ----- r 0--------- d o", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                 },
   { UMOD_SENDKEYS, MODE_MICRO , 'y', "sky", "sendkeys"  , 1, "5---- p i ----- n ----- r 0--------- d o", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                 },
   { XMOD_FORMAT  , MODE_EXTERN, 'y', "frm", "format"    , 1, "5---- p i ----- n ----- r 0--------- d o", "content formatting options"                         , "w=mnNwWhHlL  a=<|>[^]{}:' f=iIfeE ,cCaA$sS; oOxXbBzZrR d=0123456789  f=-=_.+!/@qQ~#"     },
   { XMOD_UNITS   , MODE_EXTERN, 'y', "unt", "units"     , 1, "5---- p i ----- n ----- r 0--------- d o", "content formatting options"                         , "off -, (+24) Y Z E P T G M K H D . d c m u n p f a z y (-24)"                            },
   { XMOD_OBJECT  , MODE_EXTERN, 'y', "obj", "object"    , 1, "5---- p i ----- n ----- r 0--------- d o", "object formatting and sizing options"               , ""                                                                                        },
   { XMOD_PALETTE , MODE_EXTERN, 'y', "pal", "palette"   , 1, "5---- p i ----- n ----- r 0--------- d o", "provides automatic and manual labeling hints"       , ""                                                                                        },
   { UMOD_MAP_UNDO, MODE_SUB   , 'y', "mun", "map-undo"  , 1, "5---- p i ----- n 1---- r 0--------- d o", "map level undo and redo"                            , ""                                                                                        },
   /*---(source)--------------------- --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { MODE_SOURCE  , MODE_MAJOR , 'y', "SRC", "source"    , 2, "5---- p i ----- n 1---- r 0MV------- d o", "linewise review of textual content"                 , "hor=0HhlL$bBeEwW  g/z=sh,le  sel=vV\"  pul=yYdDxX  put=pP  chg=rRiIaA  fnd=fnN"          },
   { SMOD_SREG    , MODE_SUB   , 'y', "srg", "src reg"   , 2, "5f:-- p i ----- n ----- r 0--------- d o", "selecting specific registers for text movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           },
   { UMOD_SRC_REPL, MODE_MICRO , 'y', "rpl", "replace"   , 2, "5---- p i ----- n ----- r 0--------- d o", "linewise overtyping of content in source mode"      , "type over character marked with special marker"                                          },
   { UMOD_SRC_INPT, MODE_MICRO , 'y', "inp", "input"     , 2, "5---- p i ----- n ----- r 0--------- d o", "linewise creation and editing of textual content"   , ""                                                                                        },
   { UMOD_WANDER  , MODE_MICRO , 'y', "wdr", "wander"    , 2, "5---- p i ----- n ----- r 0--------- d o", "formula creation by moving to target cells"         , "modes={ret}{esc}"                                                                        },
   { UMOD_SRC_UNDO, MODE_SUB   , 'y', "sun", "src-undo"  , 2, "5---- p i ----- n ----- r 0--------- d o", "source level undo and redo"                         , ""                                                                                        },
   /*---(power)---------------------- --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { MODE_COMMAND , MODE_MAJOR , '-', "CMD", "command"   , 3, "5f--- p i ----- n ----- r 0--------- d o", "command line capability for advanced actions"       , ""                                                                                        },
   { MODE_SEARCH  , MODE_MAJOR , '-', "SCH", "search"    , 3, "5f--- p i ----- n 1---- r 0M-------- d o", "search mode to find data and objects"               , ""                                                                                        },
   { UMOD_HISTORY , MODE_MICRO , 'y', "his", "history"   , 3, "5---- p i ----- n ----- r 0-V------- d o", "search and command history access"                  , ""                                                                                        },
   { SMOD_FILTER  , MODE_SUB   , 'y', "fil", "filter"    , 3, "5---- p i ----- n ----- r 0--------- d o", "process current/selection through external filter"  , "0HhlL$_KkjJG  gz=sh,letk.jb  dxy  !: ~uU /nN oO sS"                                      },
   { SMOD_ERROR   , MODE_SUB   , 'y', "err", "errors"    , 3, "5---- p i ----- n ----- r 0--------- d o", "display and action errors"                          , ""                                                                                        },
   /*---(overall)-------------------- --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { MODE_GOD     , MODE_MAJOR , 'y', "GOD", "god"       , 4, "5---- p i ----- n ----- r 0--------- d o", "god-mode allowing 3D omnicient viewing"             , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                },
   { MODE_OMNI    , MODE_MAJOR , 'y', "OMN", "omni"      , 4, "5---- p i ----- n ----- r 0--------- d o", "omnipotent 3D manipulation mode"                    , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                },
   { SMOD_BUFFER  , MODE_SUB   , 'y', "buf", "buffer"    , 4, "5---- p i ----- n 1---- r M--------- d o", "moving and selecting between buffers and windows"   , "select=0-9A-Z   move=jk   panel=abdgpqrtxy   cursor=_[<,>]~   search=/   status=_"       },
   { SMOD_MENUS   , MODE_SUB   , 'y', "mnu", "menus"     , 4, "5---- p i ----- n ----- r 0--------- d o", "interactive menu system for accessing commands"     , ""                                                                                        },
   { SMOD_HINT    , MODE_MAJOR , 'y', "hnt", "hint"      , 4, "5---- p i ----- n 1---- r 0M-------- d o", "provides automatic and manuallabeling hints"        , ""                                                                                        },
   { UMOD_REPEAT  , MODE_MICRO , 'y', "rep", "repeat"    , 4, "5---- p i ----- n ----- r 0--------- d o", "accumulate multiplier"                              , "range 1-99"                                                                              },
   /*---(time)----------------------- --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { MODE_PROGRESS, MODE_MAJOR , 'y', "PRG", "progress"  , 5, "5---- p i ----- n ----- r 0--------- d o", "progress timeline adding time dimension"            , "horz=0LlhH$  vert=_KkjJ~  speed=<>  scale=io  play=,."                                   },
   /*---(done)----------------------- --------------------*//* prep--- - needs-- conf--- deps-------- -  */
   { '-'          , MODE_NOT   , 'y', "bad", "bad mode"  , 0, "5---- p i ----- n ----- r 0--------- d o", "default message when mode is not understood"        , "mode not understood"                                                                     },
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat  ---expected-status----------------------   ---description--------------------------------------- ---message-------------------------------------------------------------------------------  */
};
int  g_nmode   = 0;

   /*> { 0            , MODE_NOT   , 'y', "bad", "bad mode"  , 0, "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "default message when mode is not understood"        , "mode not understood"                                                                     ,    0 },   <*/
char        g_majors       [MAX_MODES] = "";



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char        yMODE_ver [200] = "";

char*        /*--> return library versioning info --------[ leaf-- [ ------ ]-*/
yMODE_version           (void)
{
   char        t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 18);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (yMODE_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return yMODE_ver;
}



char         /*--> prepare mode stack for use ------------[--------[--------]-*/
yMODE_init              (char a_mode)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         j           =    0;
   char        n           =    0;
   char        t           [5] = "";
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   ymode_status_init ();
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (FMOD_MODE)) {
      DEBUG_MODE   yLOG_note    ("status is not ready for init");
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (g_majors, "", MAX_MODES);
   /*---(identify majors)----------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      g_mesg [i] = g_modes [i].mesg;
      if (g_modes  [i].type != MODE_MAJOR)    continue;
      sprintf (t, "%c", g_modes  [i].abbr);
      strlcat (g_majors, t, MAX_MODES);
   }
   /*---(load transitions)---------------*/
   ymode_allow_load ();
   /*---(clear stack)--------------------*/
   for (i = 0; i < MAX_STACK; ++i) {
      g_mode_stack [i] = '-';
   }
   /*---(clear controls)-----------------*/
   g_mode_depth =  0;
   g_mode_curr = '-';
   /*> strlcpy (myVIKEYS.mode_text, "--", LEN_TERSE);                                 <*/
   /*---(custom functions)---------------*/
   /*> s_formatter = NULL;                                                            <* 
    *> s_uniter    = NULL;                                                            <* 
    *> s_paletter  = NULL;                                                            <*/
   /*---(update status)------------------*/
   yMODE_init_set   (FMOD_MODE);
   /*---(go to default mode)-------------*/
   if (a_mode == MODE_GOD)  yMODE_enter (MODE_GOD);
   else                     yMODE_enter (MODE_MAP);
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_wrap              (void)
{
   ymode_status_purge ();
   ymode_allow_purge  ();
   ymode_mesg_purge   ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char          unit_answer [LEN_FULL];

char       /*----: set up program urgents/debugging --------------------------*/
ymode__unit_quiet       (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMODE_unit" };
   /*> yURG_logger   (x_narg, x_args);                                                <*/
   /*> yURG_urgs     (x_narg, x_args);                                                <*/
   yMODE_init (MODE_MAP);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ymode__unit_loud        (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMODE_unit" };
   yURG_logger   (x_narg, x_args);
   yURG_urgs     (x_narg, x_args);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("mode"         , YURG_ON);
   yURG_name  ("ystr"         , YURG_ON);
   DEBUG_MODE  yLOG_info     ("yMODE"     , yMODE_version   ());
   yMODE_init (MODE_MAP);
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ymode__unit_end         (void)
{
   DEBUG_MODE  yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_wrap ();                                                               <*/
   DEBUG_MODE  yLOG_exit    (__FUNCTION__);
   yLOGS_end    ();
   return 0;
}

char*        /*-> unit test accessor -----------------[ light  [us.420.111.11]*/ /*-[01.0000.00#.Z]-*/ /*-[--.---.---.--]-*/
ymode__unit             (char *a_question, int n)
{
   /*---(locals)-----------+-----------+-*/
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_FULL]  = "";
   char        t           [LEN_FULL]  = "";
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "MODE unit        : question not understood");
   /*---(selection)----------------------*/
   if      (strcmp (a_question, "stack"        )  == 0) {
      yMODE_status (t);
      snprintf (unit_answer, LEN_FULL, "MODE stack       : %s", t);
   }
   else if (strcmp (a_question, "entry"        )  == 0) {
      if (g_allow [n] == NULL)  strcpy  (r, "åæ");
      else {
         if (strlen (g_allow [n]) <= 10)  sprintf (r, "å%.10sæ", g_allow [n]);
         else                             sprintf (r, "å%.10s>", g_allow [n]);
      }
      sprintf (s, "å%.15sæ", g_modes [n].desc);
      if (g_mesg  [n] == NULL)  strcpy  (t, "åæ");
      else                      sprintf (t, "å%.15sæ", g_mesg [n]);
      snprintf (unit_answer, LEN_FULL, "MODE entry  (%2d) : %c %c %c %-3.3s %-10.10s %1d %-12.12s %-17.17s %s", n, g_modes [n].abbr, g_modes [n].type, g_modes [n].show, g_modes [n].three, g_modes [n].terse, g_modes [n].cat, r, s, t);
   }
   else if (strcmp (a_question, "status"       )  == 0) {
      snprintf (unit_answer, LEN_FULL, "MODE status (%2d) : %c  å%sæ  å%sæ", n, g_modes [n].abbr, g_modes [n].expect, g_actual [n]);
   }
   else if (strcmp (a_question, "text"         )  == 0) {
      snprintf (unit_answer, LEN_FULL, "MODE text        : %s", g_text);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



