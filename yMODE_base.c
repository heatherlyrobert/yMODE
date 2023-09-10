/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



tMY         myMODE;



/*===[[ FILE-WIDE VARIABLES ]]================================================*/
#define     MAX_STACK   50
char    g_mode_stack    [MAX_STACK]; /* vi-like mode stack             */
int     g_mode_depth;                /* depth of current mode stack    */
char    g_mode_curr    = '-';        /* current mode in stack          */
char    g_mode_exited  = '-';        /* last exited mode               */
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
   /*                                  123    1234567890       123456789-123456789-123456789-123456789-123456    123456789-123456789-123456789-123456789-123456789-       123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-  */
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat  ---expected-status----------------------------   ---description--------------------------------------- ---message----------------------------------------------------------------------------------  */
   /*---(fundamental)---------------- --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { FMOD_STATUS  , MODE_FUND  , '-', "sta", "status"    , 0, "····· p ·b· i   ····· n ····· r ·········· d o", "internal status and locking to prevent trouble"     , ""                                                                                           , "yMODE"     },
   { FMOD_MODE    , MODE_FUND  , '-', "mod", "modes"     , 0, "5···· p ·bh i a ····· n ····· r ·········· d o", "internal mode setting, handling, and tracking"      , ""                                                                                           , "yMODE"     },
   { FMOD_KEYS    , MODE_FUND  , '-', "key", "keys"      , 0, "5···· p ·bh i a ····· n ····· r ·········· d o", "internal looping, key processing, and logging"      , ""                                                                                           , "yKEYS"     },
   { FMOD_VIEW    , MODE_FUND  , '-', "vew", "viewing"   , 0, "5·:·· p ·bh i a ····· n ····· r ·········· d o", "internal window layout and drawing manangement"     , ""                                                                                           , "yVIEW"     },
   { FMOD_FILE    , MODE_FUND  , '-', "fil", "files"     , 0, "5···· p ·bh i a :···· n 1···· r ·········· d o", "internal file reading and writing"                  , ""                                                                                           , "yFILE"     },
   /*---(map)------------------------ --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { MODE_MAP     , MODE_MAJOR , 'y', "MAP", "map"       , 1, "5···· p fbh i a ····· n 1···· r 0········· d o", "map-mode providing 2D review of object collections" , "horz(a)=0HhlL$  horz(g/z)=sh,le  vert(a)=_KkjJG  vert(g/z)=tk.jb  modes=vIFV:{ret}"         , "yMAP"      },
   { UMOD_VISUAL  , MODE_MICRO , 'y', "vis", "visual"    , 1, "5F··· p fb· i   ····· n ····· r 0M········ d o", "visual selection, history, and access"              , "index=a-zA-Z0-9   special=!?"                                                               , "yMAP"      },
   { SMOD_MREG    , MODE_SUB   , 'y', "reg", "register"  , 1, "5F··· p fb· i   ····· n 1···· r 0M········ d o", "selecting specific registers for data movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"              , "yMAP"      },
   { UMOD_MARK    , MODE_MICRO , 'y', "mrk", "mark"      , 1, "5F:·· p fb· i   ····· n ····· r 0M········ d o", "object and location marking"                        , "names=a-zA-Z0-9  actions=#!?_  special='[()]  wander=@  range=<>*"                          , "yMARK"     },
   { SMOD_MACRO   , MODE_SUB   , 'y', "mac", "macro"     , 1, "5F:·· p fbh i a ····· n ····· r 0········· d o", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                    , "yMACRO"    },
   { UMOD_SENDKEYS, MODE_MICRO , 'y', "sky", "sendkeys"  , 1, "5···· p fb· i   ····· n ····· r 0········· d o", "inter-application messaging"                        , ""                                                                                           , "yX11"      },
   { XMOD_FORMAT  , MODE_EXTERN, 'y', "frm", "format"    , 1, "5···· p fb· i   ····· n 1···· r 0········· d o", "content data formatting options"                    , "w=mnNwWhHlL  a=<|>[^]{}:' f=iIfeE ,cCaA$sS; oOxXbBzZrR d=0123456789  f=-=_.+!/@qQ~#"        , "yMAP"      },
   { XMOD_UNITS   , MODE_EXTERN, 'y', "unt", "units"     , 1, "5···· p fb· i   ····· n 1···· r 0········· d o", "content unitizing options"                          , "off -, (+24) Y Z E P T G M K H D . d c m u n p f a z y (-24)"                               , "yMAP"      },
   { XMOD_OBJECT  , MODE_EXTERN, 'y', "obj", "object"    , 1, "5···· p fb· i   ····· n ····· r 0········· d o", "object formatting and sizing options"               , ""                                                                                           , "?"         },
   { XMOD_PALETTE , MODE_EXTERN, 'y', "pal", "palette"   , 1, "5···· p fb· i   ····· n ····· r 0········· d o", "object colorization"                                , ""                                                                                           , "?"         },
   { UMOD_MUNDO   , MODE_MICRO , 'y', "mun", "map-undo"  , 1, "5···· p fb· i   ····· n 1···· r 0········· d o", "map level undo and redo"                            , ""                                                                                           , "yMAP"      },
   /*---(source)--------------------- --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { MODE_SOURCE  , MODE_MAJOR , 'y', "SRC", "source"    , 2, "5···· p fbh i a ····· n 1···· r 0MV······· d o", "textual content review, editing, and manipulation"  , "hor=0HhlL$bBeEwW  g/z=sh,le  sel=vV\"  pul=yYdDxX  put=pP  chg=rRiIaA  fnd=fnN"             , "ySRC"      },
   { SMOD_SREG    , MODE_SUB   , 'y', "srg", "src-reg"   , 2, "5F:·· p fb· i   ····· n ····· r 0········· d o", "selecting specific registers for text movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"              , "ySRC"      },
   { UMOD_REPLACE , MODE_MICRO , 'y', "rpl", "replace"   , 2, "5···· p fb· i   ····· n ····· r 0········· d o", "textual content overtyping in source mode"          , "type over character marked with special marker"                                             , "ySRC"      },
   { UMOD_INPUT   , MODE_MICRO , 'y', "inp", "input"     , 2, "5···· p fb· i   ····· n ····· r 0········· d o", "textual content creation in source mode"            , ""                                                                                           , "ySRC"      },
   { UMOD_WANDER  , MODE_MICRO , 'y', "wdr", "wander"    , 2, "5···· p fb· i   ····· n ····· r 0········· d o", "formula creation by moving to target cells"         , "modes={ret}{esc}"                                                                           , "ySRC"      },
   { UMOD_SUNDO   , MODE_MICRO , 'y', "sun", "src-undo"  , 2, "5···· p fb· i   ····· n ····· r 0········· d o", "source level undo and redo"                         , ""                                                                                           , "ySRC"      },
   /*---(power)---------------------- --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { MODE_COMMAND , MODE_MAJOR , '-', "CMD", "command"   , 3, "5F··· p fbh i a ····· n ····· r 0········· d o", "command line capability for advanced actions"       , ""                                                                                           , "yCMD"      },
   { MODE_SEARCH  , MODE_MAJOR , '-', "SCH", "search"    , 3, "5F··· p fbh i a ····· n 1···· r 0M········ d o", "search mode to find data and objects"               , ""                                                                                           , "yMARK"     },
   { UMOD_HISTORY , MODE_MICRO , 'y', "his", "history"   , 3, "5···· p fb· i   ····· n ····· r 0·V······· d o", "search and command history access"                  , ""                                                                                           , "?"         },
   { SMOD_FILTER  , MODE_SUB   , 'y', "fil", "filter"    , 3, "5···· p fb· i   ····· n ····· r 0········· d o", "process current/selection through external filter"  , "0HhlL$_KkjJG  gz=sh,letk.jb  dxy  !: ~uU /nN oO sS"                                         , "?"         },
   { SMOD_ERROR   , MODE_SUB   , 'y', "err", "errors"    , 3, "5···· p fb· i   ····· n ····· r 0········· d o", "display and action errors"                          , ""                                                                                           , "?"         },
   /*---(overall)-------------------- --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { MODE_GOD     , MODE_MAJOR , 'y', "GOD", "god"       , 4, "5···· p fb· i   ····· n ····· r 0········· d o", "god-mode allowing 3D omnicient viewing"             , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                   , "yMAP"      },
   { MODE_OMNI    , MODE_MAJOR , 'y', "OMN", "omni"      , 4, "5···· p fb· i   ····· n ····· r 0········· d o", "omnipotent 3D manipulation mode"                    , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                   , "?"         },
   { UMOD_UNIVERSE, MODE_MICRO , 'y', "uni", "universe"  , 4, "5···· p fb· i   ····· n 1···· r ·M········ d o", "moving between universes, tabs, or buffers"         , "select=0-9A-Z   move=jk   panel=abdgpqrtxy   cursor=_[<,>]~   search=/   status=_"          , "yMAP"      },
   { SMOD_MENUS   , MODE_SUB   , 'y', "mnu", "menus"     , 4, "5···· p fb· i   ····· n ····· r 0········· d o", "interactive menu system for accessing commands"     , ""                                                                                           , "yCMD"      },
   { SMOD_HINT    , MODE_SUB   , 'y', "hnt", "hint"      , 4, "5···· p fb· i   ····· n 1···· r 0M········ d o", "provides automatic and manual labeling hints"       , ""                                                                                           , "yMARK"     },
   { PMOD_REPEAT  , MODE_MICRO , 'y', "rep", "repeat"    , 4, "5···· p fb· i   ····· n ····· r 0········· d o", "accumulate repetition multipliers"                  , "range 1-99"                                                                                 , "yKEYS"     },
   /*---(time)----------------------- --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { MODE_PROGRESS, MODE_MAJOR , 'y', "PRG", "progress"  , 5, "5···· p fb· i   ····· n 123·· r 0········· d o", "progress timeline adding time dimension"            , "horz=0LlhH$  vert=_KkjJ~  speed=<>  scale=io  play=,."                                      , "yKEYS"     },
   /*---(done)----------------------- --------------------*//* prep--- init--- needs-- conf--- deps-------- -  */
   { '·'          , MODE_NOT   , 'y', "bad", "bad mode"  , 0, "····· - ··· - - ····· - ····· - ·········· - -", "default message when mode is not understood"        , "mode not understood"                                                                        , ""          },
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat  ---expected-status----------------------------   ---description--------------------------------------- ---message----------------------------------------------------------------------------------  */
};
int  g_nmode   = 0;

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
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   ymode_status_init ();
   /*---(defense)------------------------*/
   --rce;  if (!yMODE_check_prep  (FMOD_MODE)) {
      DEBUG_YMODE   yLOG_note    ("status is not ready for init");
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(yVIHUB)-------------------------*/
   yVIHUB_from_yMODE (yMODE_yvihub_set);
   /*---(prepare)------------------------*/
   ystrlcpy (g_majors, "", MAX_MODES);
   myMODE.h_major   = G_KEY_SPACE;
   /*---(identify majors)----------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      g_mesg [i] = g_modes [i].mesg;
      if (g_modes  [i].type != MODE_MAJOR)    continue;
      sprintf (t, "%c", g_modes  [i].abbr);
      ystrlcat (g_majors, t, MAX_MODES);
   }
   /*---(load transitions)---------------*/
   ymode_allow_load ();
   /*---(clear stack)--------------------*/
   for (i = 0; i < MAX_STACK; ++i) {
      g_mode_stack [i] = '-';
   }
   /*---(clear controls)-----------------*/
   g_mode_depth  =  0;
   g_mode_curr   = '-';
   g_mode_exited = '-';
   /*> ystrlcpy (myVIKEYS.mode_text, "--", LEN_TERSE);                                 <*/
   /*---(custom functions)---------------*/
   myMODE.e_format  = NULL;
   myMODE.e_object  = NULL;
   myMODE.e_palette = NULL;
   /*---(update status)------------------*/
   yMODE_init_set   (FMOD_MODE   , NULL, NULL);
   /*---(go to default mode)-------------*/
   if (a_mode == MODE_GOD)  rc = ymode_force (MODE_GOD);
   else                     rc = ymode_force (MODE_MAP);
   DEBUG_YMODE   yLOG_value   ("mode_force", rc);
   --rce;  if (rc < 0) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_init_after        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(other updates)------------------*/
   rc = yVIHUB_yFILE_dump_add ("statuses"  , "", "inventory of mode statuses"  , yMODE_statuses);
   DEBUG_YMODE   yLOG_value   ("dump_add"  , rc);
   rc = yVIHUB_yVIEW_switch_add ('s', "mode", "mod", yMODE_status, "display the mode stack");
   DEBUG_YMODE   yLOG_value   ("switch_add", rc);
   yMODE_after_set  (FMOD_MODE);
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
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

char
yMODE_handle            (uchar a_key)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          = 0;             /* generic return code            */
   uchar       x_minor     = ' ';
   char      (*x_handler) (uchar a_major, uchar a_minor) = NULL;
   int         n           = 0;
   char        x_keys      [LEN_LABEL];
   char        x_prev      =  '-';
   char        x_curr      =  '-';
   char        x_again     =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP   yLOG_value   ("major"     , myMODE.h_major);
   DEBUG_LOOP   yLOG_value   ("a_key"     , a_key);
   /*---(defense)------------------------*/
   if (a_key == 0) {
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_LOOP   yLOG_note    ("REAL KEY");
   x_minor = ychrworking (a_key);
   DEBUG_LOOP   yLOG_char    ("x_minor"   , x_minor);
   /*> myVIKEYS.trouble   = '-';                                                      <*/
   /*---(handle count)-------------------*/
   if (yMODE_curr () == PMOD_REPEAT) {
      rc = yVIHUB_yKEYS_repeat_umode (myMODE.h_major, x_minor);
      if (rc >  0)  myMODE.h_major = G_KEY_SPACE;
   }
   if (yMODE_curr () == PMOD_REPEAT) {
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(handle grouping)----------------*/
   if (strchr ("()", x_minor) != NULL) {
      DEBUG_LOOP   yLOG_complex ("grouping"  , "%c, %3d, %c", yMODE_curr (), x_minor, ychrvisible (x_minor));
      rc = yVIHUB_yKEYS_group_hmode (myMODE.h_major, x_minor);
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
      x_prev = yMODE_curr ();
      DEBUG_LOOP   yLOG_char    ("MODE prev" , x_prev);
      rc = x_handler (myMODE.h_major, x_minor);
      DEBUG_LOOP   yLOG_value   ("rc"        , rc);
      x_curr = yMODE_curr ();
      DEBUG_LOOP   yLOG_char    ("MODE curr" , x_curr);
      /*---(translate unprintable)-------*/
      snprintf (x_keys,   9, "  %c%c", ychrvisible (myMODE.h_major), ychrvisible (x_minor));
      /*---(handle returns)--------------*/
      if (rc <   0) {
         DEBUG_LOOP   yLOG_note    ("negative return, warning");
         yVIHUB_yKEYS_set_warning  ();
      }
      /*---(check repeats)---------------*/
      DEBUG_LOOP   yLOG_note    ("repeat checking");
      x_again = yVIHUB_yKEYS_repeat_check (myMODE.h_major, x_minor, x_prev, x_curr, rc);
      DEBUG_LOOP   yLOG_value   ("x_again"   , x_again);
      if (x_again != 1) break;
      /*---(done)------------------------*/
   }
   /*---(setup for next keystroke)-------*/
   if      (rc >= 10)    myMODE.h_major = rc;
   else if (rc >   0)    myMODE.h_major = G_KEY_SPACE;
   else if (rc ==  0)    myMODE.h_major = G_KEY_SPACE;
   else                  myMODE.h_major = G_KEY_SPACE;
   yVIHUB_yKEYS_check_repeat ();
   yVIHUB_yVIEW_keys      (x_keys);
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return rc;
}


