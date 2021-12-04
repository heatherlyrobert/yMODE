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

char   *g_allow   [MAX_MODES] = { NULL };
char  (*g_handler [MAX_MODES]) (uchar a_major, uchar a_minor) = { NULL };
char  (*g_prepper [MAX_MODES]) (uchar a_major, uchar a_minor) = { NULL };
char    g_actual  [MAX_MODES] [LEN_DESC];
char   *g_mesg    [MAX_MODES] = { NULL };

char    g_keys    [LEN_TERSE] = "·-·-";


const tMODE_INFO  g_modes [MAX_MODES] = {
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat  ---expected-status------------------------   ---description--------------------------------------- ---message-------------------------------------------------------------------------------  */
   /*---(fundamental)---------------- --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { FMOD_STATUS  , MODE_FUND  , '-', "sta", "status"    , 0, "----- p - i ----- n ----- r ---------- d o", "internal status and locking to prevent trouble"     , ""                                                                                        },
   { FMOD_MODE    , MODE_FUND  , '-', "mod", "modes"     , 0, "5---- p - i ----- n ----- r ---------- d o", "internal mode setting and tracking"                 , ""                                                                                        },
   { FMOD_VIEW    , MODE_FUND  , '-', "vew", "viewing"   , 0, "5-:-- p - i ----- n ----- r ---------- d o", "internal opengl and curses drawing manangement"     , ""                                                                                        },
   { FMOD_FILE    , MODE_FUND  , '-', "fil", "files"     , 0, "5---- p - i :---- n 1---- r ---------- d o", "internal file reading and writing"                  , ""                                                                                        },
   /*---(map)------------------------ --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { MODE_MAP     , MODE_MAJOR , 'y', "MAP", "map"       , 1, "5---- p f i ----- n 1---- r 0--------- d o", "map-mode providing 2D review of object collections" , "horz(a)=0HhlL$  horz(g/z)=sh,le  vert(a)=_KkjJG  vert(g/z)=tk.jb  modes=vIFV:{ret}"      },
   { UMOD_VISUAL  , MODE_MICRO , 'y', "vis", "visual"    , 1, "5f--- p f i ----- n ----- r 0M-------- d o", "visual selection history and access"                , "index=a-zA-Z0-9   special=!?"                                                            },
   { SMOD_MREG    , MODE_SUB   , 'y', "reg", "register"  , 1, "5f--- p f i ----- n 1---- r 0M-------- d o", "selecting specific registers for data movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           },
   { UMOD_MARK    , MODE_MICRO , 'y', "mrk", "mark"      , 1, "5f:-- p f i ----- n ----- r 0M-------- d o", "object and location marking"                        , "names=a-zA-Z0-9  actions=#!?_  special='[()]  wander=@  range=<>*"                       },
   { SMOD_MACRO   , MODE_SUB   , 'y', "mac", "macro"     , 1, "5f:-- p f i ----- n ----- r 0--------- d o", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                 },
   { UMOD_SENDKEYS, MODE_MICRO , 'y', "sky", "sendkeys"  , 1, "5---- p f i ----- n ----- r 0--------- d o", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                 },
   { XMOD_FORMAT  , MODE_EXTERN, 'y', "frm", "format"    , 1, "5---- p f i ----- n ----- r 0--------- d o", "content formatting options"                         , "w=mnNwWhHlL  a=<|>[^]{}:' f=iIfeE ,cCaA$sS; oOxXbBzZrR d=0123456789  f=-=_.+!/@qQ~#"     },
   { XMOD_UNITS   , MODE_EXTERN, 'y', "unt", "units"     , 1, "5---- p f i ----- n ----- r 0--------- d o", "content formatting options"                         , "off -, (+24) Y Z E P T G M K H D . d c m u n p f a z y (-24)"                            },
   { XMOD_OBJECT  , MODE_EXTERN, 'y', "obj", "object"    , 1, "5---- p f i ----- n ----- r 0--------- d o", "object formatting and sizing options"               , ""                                                                                        },
   { XMOD_PALETTE , MODE_EXTERN, 'y', "pal", "palette"   , 1, "5---- p f i ----- n ----- r 0--------- d o", "provides automatic and manual labeling hints"       , ""                                                                                        },
   { UMOD_MUNDO   , MODE_SUB   , 'y', "mun", "map-undo"  , 1, "5---- p f i ----- n 1---- r 0--------- d o", "map level undo and redo"                            , ""                                                                                        },
   /*---(source)--------------------- --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { MODE_SOURCE  , MODE_MAJOR , 'y', "SRC", "source"    , 2, "5---- p f i ----- n 1---- r 0MV------- d o", "linewise review of textual content"                 , "hor=0HhlL$bBeEwW  g/z=sh,le  sel=vV\"  pul=yYdDxX  put=pP  chg=rRiIaA  fnd=fnN"          },
   { SMOD_SREG    , MODE_SUB   , 'y', "srg", "src reg"   , 2, "5f:-- p f i ----- n ----- r 0--------- d o", "selecting specific registers for text movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           },
   { UMOD_REPLACE , MODE_MICRO , 'y', "rpl", "replace"   , 2, "5---- p f i ----- n ----- r 0--------- d o", "linewise overtyping of content in source mode"      , "type over character marked with special marker"                                          },
   { UMOD_INPUT   , MODE_MICRO , 'y', "inp", "input"     , 2, "5---- p f i ----- n ----- r 0--------- d o", "linewise creation and editing of textual content"   , ""                                                                                        },
   { UMOD_WANDER  , MODE_MICRO , 'y', "wdr", "wander"    , 2, "5---- p f i ----- n ----- r 0--------- d o", "formula creation by moving to target cells"         , "modes={ret}{esc}"                                                                        },
   { UMOD_SUNDO   , MODE_SUB   , 'y', "sun", "src-undo"  , 2, "5---- p f i ----- n ----- r 0--------- d o", "source level undo and redo"                         , ""                                                                                        },
   /*---(power)---------------------- --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { MODE_COMMAND , MODE_MAJOR , '-', "CMD", "command"   , 3, "5f--- p f i ----- n ----- r 0--------- d o", "command line capability for advanced actions"       , ""                                                                                        },
   { MODE_SEARCH  , MODE_MAJOR , '-', "SCH", "search"    , 3, "5f--- p f i ----- n 1---- r 0M-------- d o", "search mode to find data and objects"               , ""                                                                                        },
   { UMOD_HISTORY , MODE_MICRO , 'y', "his", "history"   , 3, "5---- p f i ----- n ----- r 0-V------- d o", "search and command history access"                  , ""                                                                                        },
   { SMOD_FILTER  , MODE_SUB   , 'y', "fil", "filter"    , 3, "5---- p f i ----- n ----- r 0--------- d o", "process current/selection through external filter"  , "0HhlL$_KkjJG  gz=sh,letk.jb  dxy  !: ~uU /nN oO sS"                                      },
   { SMOD_ERROR   , MODE_SUB   , 'y', "err", "errors"    , 3, "5---- p f i ----- n ----- r 0--------- d o", "display and action errors"                          , ""                                                                                        },
   /*---(overall)-------------------- --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { MODE_GOD     , MODE_MAJOR , 'y', "GOD", "god"       , 4, "5---- p f i ----- n ----- r 0--------- d o", "god-mode allowing 3D omnicient viewing"             , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                },
   { MODE_OMNI    , MODE_MAJOR , 'y', "OMN", "omni"      , 4, "5---- p f i ----- n ----- r 0--------- d o", "omnipotent 3D manipulation mode"                    , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                },
   { SMOD_BUFFER  , MODE_SUB   , 'y', "buf", "buffer"    , 4, "5---- p f i ----- n 1---- r M--------- d o", "moving and selecting between buffers and windows"   , "select=0-9A-Z   move=jk   panel=abdgpqrtxy   cursor=_[<,>]~   search=/   status=_"       },
   { SMOD_MENUS   , MODE_SUB   , 'y', "mnu", "menus"     , 4, "5---- p f i ----- n ----- r 0--------- d o", "interactive menu system for accessing commands"     , ""                                                                                        },
   { SMOD_HINT    , MODE_MAJOR , 'y', "hnt", "hint"      , 4, "5---- p f i ----- n 1---- r 0M-------- d o", "provides automatic and manuallabeling hints"        , ""                                                                                        },
   { PMOD_REPEAT  , MODE_MICRO , 'y', "rep", "repeat"    , 4, "5---- p f i ----- n ----- r 0--------- d o", "accumulate multiplier"                              , "range 1-99"                                                                              },
   /*---(time)----------------------- --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { MODE_PROGRESS, MODE_MAJOR , 'y', "PRG", "progress"  , 5, "5---- p f i ----- n ----- r 0--------- d o", "progress timeline adding time dimension"            , "horz=0LlhH$  vert=_KkjJ~  speed=<>  scale=io  play=,."                                   },
   /*---(done)----------------------- --------------------*//* prep--- f - needs-- conf--- deps-------- -  */
   { '-'          , MODE_NOT   , 'y', "bad", "bad mode"  , 0, "5---- p f i ----- n ----- r 0--------- d o", "default message when mode is not understood"        , "mode not understood"                                                                     },
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat  ---expected-status------------------------   ---description--------------------------------------- ---message-------------------------------------------------------------------------------  */
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
   char        rc          =    0;
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
   yMODE_init_set   (FMOD_MODE, NULL, NULL);
   /*---(go to default mode)-------------*/
   if (a_mode == MODE_GOD)  rc = ymode_force (MODE_GOD);
   else                     rc = ymode_force (MODE_MAP);
   DEBUG_MODE   yLOG_value   ("mode_force", rc);
   --rce;  if (rc < 0) {
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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

uchar
yMODE_handle            (uchar a_key)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          = 0;             /* generic return code            */
   static char x_major     = G_KEY_SPACE;   /* saved keystroke from last loop */
   uchar       x_minor     = ' ';
   char      (*x_handler) (uchar a_major, uchar a_minor) = NULL;
   int         x_repeat    = 0;             /* store current repeat count     */
   int         n           = 0;
   char        x_keys      [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP   yLOG_value   ("a_key"     , a_key);
   /*---(defense)------------------------*/
   if (a_key == 0) {
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_LOOP   yLOG_note    ("REAL KEY");
   x_minor = chrworking (a_key);
   DEBUG_LOOP   yLOG_char    ("x_minor"   , x_minor);
   /*> myVIKEYS.trouble   = '-';                                                      <*/
   /*---(handle count)-------------------*/
   if (yMODE_curr () == PMOD_REPEAT) {
      rc = yKEYS_repeat_umode (x_major, x_minor);
      if (rc >  0)  x_major = G_KEY_SPACE;
   }
   if (yMODE_curr () == PMOD_REPEAT) {
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle grouping)----------------*/
   if (strchr ("()", x_minor) != NULL) {
      DEBUG_LOOP   yLOG_complex ("grouping"  , "%c, %3d, %c", yMODE_curr (), x_minor, chrvisible (x_minor));
      rc = yKEYS_group_hmode (x_major, x_minor);
      DEBUG_LOOP   yLOG_value   ("group"     , rc);
      if (rc > 0) {
         DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
         return 0;
      }
   }
   /*---(mode)---------------------------*/
   DEBUG_LOOP   yLOG_char    ("mode"      , yMODE_curr ());
   n = ymode_by_abbr (yMODE_curr ());
   --rce;  if (n < 0) {
      yMODE_exit ();
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_LOOP   yLOG_value   ("n"         , n);
   /*---(handler)------------------------*/
   x_handler = g_handler [n];
   DEBUG_LOOP   yLOG_point   ("x_handler" , x_handler);
   --rce;  if (x_handler == NULL) {
      yMODE_exit ();
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(loop)---------------------------*/
   while (1) {
      /*---(run mode)--------------------*/
      DEBUG_LOOP   yLOG_char    ("MODE bef"  , yMODE_curr ());
      rc = x_handler (x_major, x_minor);
      DEBUG_LOOP   yLOG_value   ("rc"        , rc);
      DEBUG_LOOP   yLOG_char    ("MODE aft"  , yMODE_curr ());
      /*---(translate unprintable)-------*/
      snprintf (x_keys,   9, "  %c%c", chrvisible (x_major), chrvisible (x_minor));
      /*---(check repeats)---------------*/
      x_repeat = yKEYS_repeats ();
      DEBUG_LOOP   yLOG_value   ("x_repeat"  , x_repeat);
      /*---(handle returns)--------------*/
      /*> if        (rc == YKEYS_LOCK) {                                                         <* 
       *>    DEBUG_LOOP   yLOG_note    ("negative return YKEYS_LOCK, locking all action");       <* 
       *>    yKEYS_set_lock     ();                                                              <* 
       *>    yKEYS_repeat_reset ();                                                              <* 
       *>    break;                                                                              <* 
       *> } else if (rc == YKEYS_ERROR) {                                                        <* 
       *>    DEBUG_LOOP   yLOG_note    ("negative return YKEYS_ERROR, error breaking repeat");   <* 
       *>    yKEYS_set_error    ();                                                              <* 
       *>    yKEYS_repeat_reset ();                                                              <* 
       *>    break;                                                                              <* 
       *> } else if (rc <   0) {                                                                 <* 
       *>    DEBUG_LOOP   yLOG_note    ("negative return, warning");                             <* 
       *>    yKEYS_set_warning  ();                                                              <* 
       *> }                                                                                      <*/
      /*---(loop repeats)----------------*/
      if (rc >= 0 && x_repeat > 0 && yMODE_curr () != PMOD_REPEAT) {
         DEBUG_LOOP   yLOG_note    ("repeating");
         yKEYS_repeat_dec ();
         continue;
      }
      /*---(loop repeats)----------------*/
      if (rc < 0 && yMODE_curr () != PMOD_REPEAT) {
         DEBUG_LOOP   yLOG_note    ("complete repeat");
         yKEYS_repeat_reset ();
      }
      break;
      /*---(done)------------------------*/
   }
   /*---(setup for next keystroke)-------*/
   if      (rc >   0)    x_major = rc;
   else if (rc ==  0)    x_major = G_KEY_SPACE;
   else                  x_major = G_KEY_SPACE;
   yVIEW_keys (x_keys);
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNIT_TEST_______o () { return; }

char           unit_answer [LEN_FULL];
int            s_xpos      = 0;
int            s_ypos      = 0;

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
   yMODE_wrap ();
   DEBUG_MODE  yLOG_exit    (__FUNCTION__);
   yLOGS_end    ();
   return 0;
}

char yMODE_handler_stub   (uchar a_major, uchar a_minor) { return 0; }

static char  s_keys [LEN_RECD] = "";
static char  s_mode [LEN_RECD] = "";

char
yMODE_handler_reset     (void)
{
   strlcpy (s_keys, "", LEN_RECD);
   strlcpy (s_mode, "", LEN_RECD);
   s_xpos = s_ypos = 0;
   return 0;
}

char
ymode_handler_log       (uchar a_mode, uchar a_key)
{
   char        t           [LEN_TERSE] = "";
   sprintf (t, "%c", a_mode);
   strlcat (s_mode, t, LEN_RECD);
   sprintf (t, "%c", chrvisible (a_key));
   strlcat (s_keys, t, LEN_RECD);
   return 0;
}

char
ymode_handler_map        (uchar a_major, uchar a_minor)
{
   char        rc          =     0;
   ymode_handler_log ('M', a_minor);
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   DEBUG_MODE   yLOG_char    ("a_minor"   , a_minor);
   switch (a_minor) {
   case '1'  : case '2'  : case '3'  : case '4'  :
   case '5'  : case '6'  : case '7'  : case '8'  :
   case '9'  : yMODE_enter  (PMOD_REPEAT);
               rc = a_minor;
               break;
   case '0'  : s_xpos  =  0;  break;
   case 'H'  : s_xpos -=  5;  break;
   case 'h'  : s_xpos -=  1;  break;
   case 'l'  : s_xpos +=  1;  break;
   case 'L'  : s_xpos +=  5;  break;
   case '$'  : s_xpos  = 99;  break;
   case '_'  : s_ypos  =  0;  break;
   case 'K'  : s_ypos -=  5;  break;
   case 'k'  : s_ypos -=  1;  break;
   case 'j'  : s_ypos +=  1;  break;
   case 'J'  : s_ypos +=  5;  break;
   case '~'  : s_ypos  = 99;  break;
   case '¦'  : case G_KEY_RETURN :
               yMODE_enter (MODE_SOURCE);
               yMODE_enter (UMOD_INPUT);
               break;
   case '¥'  : case G_KEY_ESCAPE :
               break;
   case ':'  :
               yMODE_enter (MODE_COMMAND);
               break;
   case '@'  : case 'q'    : case 'Q'      :
               rc = yMACRO_hmode (a_major, a_minor);
               break;
               /*> case '('  : case ')'  :                                                        <* 
                *>             rc = yKEYS_group_hmode (a_major, a_minor);                         <* 
                *>             break;                                                             <*/
   }
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode_handler_source     (uchar a_major, uchar a_minor)
{
   char        rc          =     0;
   ymode_handler_log ('S', a_minor);
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case '1'  : case '2'  : case '3'  : case '4'  : case '5'  :
   case '6'  : case '7'  : case '8'  : case '9'  :
      yMODE_enter (PMOD_REPEAT);
      break;
   case '¦'  : case G_KEY_RETURN :
      yMODE_exit  ();
      break;
   case '¥'  : case G_KEY_ESCAPE :
      yMODE_exit  ();
      break;
   case 'I'  : case 'i'  : case 'a'  : case 'A'  :
      yMODE_enter (UMOD_INPUT);
      break;
   case CASE_MACRO_KEYS :
      rc = yMACRO_hmode (a_major, a_minor);
      break;
      /*> case '('  : case ')'  :                                                        <* 
       *>    rc = yKEYS_group_hmode (a_major, a_minor);                                  <* 
       *>    break;                                                                      <*/
   }
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode_handler_input      (uchar a_major, uchar a_minor)
{
   ymode_handler_log ('i', a_minor);
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case '¥'  : case G_KEY_ESCAPE :
      yMODE_exit  ();
      break;
   case '¦'  : case G_KEY_RETURN :
      yMODE_exit  ();
      yMODE_exit  ();
      break;
   }
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

static  char s_command [LEN_HUND] = "";

char
ymode_handler_command_prep (void)
{
   strlcpy (s_command, ":", LEN_HUND);
   return 0;
}

char
ymode_handler_command    (uchar a_major, uchar a_minor)
{
   char        t           [LEN_SHORT] = "";
   ymode_handler_log (':', a_minor);
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case '¦'  : case G_KEY_RETURN :
      if      (strcmp (":q"     , s_command) == 0)  yKEYS_quit ();
      else if (strcmp (":qa"    , s_command) == 0)  yKEYS_quit ();
      else if (strcmp (":wqa"   , s_command) == 0)  yKEYS_quit ();
      yMODE_exit  ();
      break;
   case '¥'  : case G_KEY_ESCAPE :
      yMODE_exit  ();
      break;
   default   :
      sprintf (t, "%c", a_minor);
      strlcat (s_command, t, LEN_HUND);
      break;
   }
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymode_handler_macro      (uchar a_major, uchar a_minor)
{
   ymode_handler_log ('@', a_minor);
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   yMODE_exit  ();
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_handler_setup     (void)
{
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   yMODE_handler_reset ();
   yMODE_init_set (FMOD_FILE    , NULL, NULL);
   yMODE_init_set (MODE_COMMAND , ymode_handler_command_prep, ymode_handler_command);
   yMODE_conf_set (FMOD_FILE    , '1');
   yMODE_init_set (FMOD_VIEW    , NULL, NULL);
   yMODE_init_set (MODE_MAP     , NULL, ymode_handler_map);
   yMODE_conf_set (MODE_MAP     , '1');
   yMODE_init_set (MODE_SOURCE  , NULL, ymode_handler_source);
   yMODE_conf_set (MODE_SOURCE  , '1');
   yMODE_init_set (UMOD_INPUT   , NULL, ymode_handler_input);
   yMODE_init_set (SMOD_MACRO   , NULL, ymode_handler_macro);
   yMODE_conf_set (SMOD_MACRO   , '1');
   yMODE_init_set (UMOD_SUNDO   , NULL, yMODE_handler_stub);
   yMODE_init_set (SMOD_SREG    , NULL, yMODE_handler_stub);
   DEBUG_PROG   yLOG_info    ("mode"      , yMODE_actual (FMOD_MODE));
   DEBUG_PROG   yLOG_info    ("status"    , yMODE_actual (FMOD_STATUS));
   DEBUG_PROG   yLOG_info    ("view"      , yMODE_actual (FMOD_VIEW));
   DEBUG_PROG   yLOG_info    ("file"      , yMODE_actual (FMOD_FILE));
   DEBUG_PROG   yLOG_info    ("map"       , yMODE_actual (MODE_MAP));
   DEBUG_PROG   yLOG_info    ("comand"    , yMODE_actual (MODE_COMMAND));
   DEBUG_PROG   yLOG_info    ("source"    , yMODE_actual (MODE_SOURCE));
   DEBUG_PROG   yLOG_info    ("input"     , yMODE_actual (UMOD_INPUT));
   DEBUG_PROG   yLOG_info    ("macro"     , yMODE_actual (SMOD_MACRO));
   yKEYS_init ();
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char*        /*-> unit test accessor -----------------[ light  [us.420.111.11]*/ /*-[01.0000.00#.Z]-*/ /*-[--.---.---.--]-*/
yMODE__unit             (char *a_question, int n)
{
   /*---(locals)-----------+-----------+-*/
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_FULL]  = "";
   char        t           [LEN_FULL]  = "";
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "MODE unit        : question not understood");
   /*---(selection)----------------------*/
   if (n > 32)  n = ymode_by_abbr (n);
   if      (strcmp (a_question, "stack"        )  == 0) {
      yMODE_status (0, 0, t);
      snprintf (unit_answer, LEN_FULL, "MODE stack       :%s", t);
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
      snprintf (unit_answer, LEN_FULL, "MODE text        : å%sæ", g_text);
   }
   else if (strcmp (a_question, "u_modes"      )  == 0) {
      snprintf (unit_answer, LEN_FULL, "MODE u_modes     : å%-.80sæ", s_mode);
   }
   else if (strcmp (a_question, "u_keys"       )  == 0) {
      snprintf (unit_answer, LEN_FULL, "MODE u_keys      : å%-.80sæ", s_keys);
   }
   else if (strcmp (a_question, "u_pos"        )  == 0) {
      snprintf (unit_answer, LEN_FULL, "MODE u_pos       : %3dx, %3dy", s_xpos, s_ypos);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



