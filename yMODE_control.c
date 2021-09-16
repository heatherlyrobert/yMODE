/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



/*===[[ FILE-WIDE VARIABLES ]]================================================*/
#define     MAX_STACK   50
static char    s_mode_stack    [MAX_STACK]; /* vi-like mode stack             */
static int     s_mode_depth;                /* depth of current mode stack    */
static char    s_mode_curr;                 /* current mode in stack          */
static char    s_message       [LEN_RECD];

/*    MODE_    major mode, used for most working time
 *    SMOD_    sub-mode, supports major mode(s), used for short-period
 *    UMOD_    micro-mode, very simple supporting capability, used very quickly
 *    XMOD_    external-mode, custom to applicaton supported
 *    FMOD_    fundamental, not a usable mode, but a building block
 */
#define       MODE_FUND     'F'
#define       MODE_MAJOR    'M'
#define       MODE_SUB      's'
#define       MODE_MICRO    'u'
#define       MODE_EXTERN   'x'
#define       MODE_NOT      '-'


tMODE_INFO  g_modes [MAX_MODES] = {
   /*-abbr-------- ---type----- show  -tla- ---terse----- cat allow  ---expected-status----------------------    ---actual-status-------------------------   ---description--------------------------------------- ---message-------------------------------------------------------------------------------    use */
   /*---(fundamental)---------------- --------------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { FMOD_STATUS  , MODE_FUND  , '-', "sta", "status"    , 0, ""  , "----- p i ----- n ----- r ---------- d o", "----- - - ----- - ----- - ---------- - -", "interal status and locking to prevent trouble"      , ""                                                                                        ,    0 },
   { FMOD_MODE    , MODE_FUND  , '-', "mod", "modes"     , 0, ""  , "5---- p i ----- n ----- r ---------- d o", "----- - - ----- - ----- - ---------- - -", "internal mode setting and tracking"                 , ""                                                                                        ,    0 },
   { FMOD_VIEW    , MODE_FUND  , '-', "vew", "viewing"   , 0, ""  , "5-:-- p i ----- n ----- r ---------- d o", "----- - - ----- - ----- - ---------- - -", "internal opengl and curses drawing manangement"     , ""                                                                                        ,    0 },
   { FMOD_FILE    , MODE_FUND  , '-', "fil", "files"     , 0, ""  , "5---- p i :---- n 1---- r ---------- d o", "----- - - ----- - ----- - ---------- - -", "internal file reading and writing"                  , ""                                                                                        ,    0 },
   /*---(map)------------------------ ------ -------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { MODE_MAP     , MODE_MAJOR , 'y', "MAP", "map"       , 1, ""  , "5---- p i ----- n 1---- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "map-mode providing 2D review of object collections" , "horz(a)=0HhlL$  horz(g/z)=sh,le  vert(a)=_KkjJG  vert(g/z)=tk.jb  modes=vIFV:{ret}"      ,    0 },
   { UMOD_VISUAL  , MODE_MICRO , 'y', "vis", "visual"    , 1, ""  , "5f--- p i ----- n ----- r 0M-------- d o", "----- - - ----- - ----- - ---------- - -", "visual selection history and access"                , "index=a-zA-Z0-9   special=!?"                                                            ,    0 },
   { SMOD_MREG    , MODE_SUB   , 'y', "reg", "register"  , 1, ""  , "5f--- p i ----- n 1---- r 0M-------- d o", "----- - - ----- - ----- - ---------- - -", "selecting specific registers for data movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           ,    0 },
   { UMOD_MARK    , MODE_MICRO , 'y', "mrk", "mark"      , 1, ""  , "5f:-- p i ----- n ----- r 0M-------- d o", "----- - - ----- - ----- - ---------- - -", "object and location marking"                        , "names=a-zA-Z0-9  actions=#!?_  special='[()]  wander=@  range=<>*"                       ,    0 },
   { SMOD_MACRO   , MODE_SUB   , 'y', "mac", "macro"     , 1, ""  , "5f:-- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                 ,    0 },
   { UMOD_SENDKEYS, MODE_MICRO , 'y', "sky", "sendkeys"  , 1, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "macro recording, execution, and maintenance"        , "run=a-z"                                                                                 ,    0 },
   { XMOD_FORMAT  , MODE_EXTERN, 'y', "frm", "format"    , 1, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "content formatting options"                         , "w=mnNwWhHlL  a=<|>[^]{}:' f=iIfeE ,cCaA$sS; oOxXbBzZrR d=0123456789  f=-=_.+!/@qQ~#"     ,    0 },
   { XMOD_UNITS   , MODE_EXTERN, 'y', "unt", "units"     , 1, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "content formatting options"                         , "off -, (+24) Y Z E P T G M K H D . d c m u n p f a z y (-24)"                            ,    0 },
   { XMOD_OBJECT  , MODE_EXTERN, 'y', "obj", "object"    , 1, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "object formatting and sizing options"               , ""                                                                                        ,    0 },
   { XMOD_PALETTE , MODE_EXTERN, 'y', "pal", "palette"   , 1, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "provides automatic and manual labeling hints"       , ""                                                                                        ,    0 },
   { UMOD_MAP_UNDO, MODE_SUB   , 'y', "mun", "map-undo"  , 1, ""  , "5---- p i ----- n 1---- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "map level undo and redo"                            , ""                                                                                        ,    0 },
   /*---(source)--------------------- --------------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { MODE_SOURCE  , MODE_MAJOR , 'y', "SRC", "source"    , 2, ""  , "5---- p i ----- n 1---- r 0MV------- d o", "----- - - ----- - ----- - ---------- - -", "linewise review of textual content"                 , "hor=0HhlL$bBeEwW  g/z=sh,le  sel=vV\"  pul=yYdDxX  put=pP  chg=rRiIaA  fnd=fnN"          ,    0 },
   { SMOD_SREG    , MODE_SUB   , 'y', "srg", "src reg"   , 2, ""  , "5f:-- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "selecting specific registers for text movement"     , "regs=\"a-zA-Z-+0  pull=yYxXdD  -/+=vVcCtTsSfF  push=pPrRmMaAiIoObB  mtce=#?!g"           ,    0 },
   { UMOD_SRC_REPL, MODE_MICRO , 'y', "rpl", "replace"   , 2, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "linewise overtyping of content in source mode"      , "type over character marked with special marker"                                          ,    0 },
   { UMOD_SRC_INPT, MODE_MICRO , 'y', "inp", "input"     , 2, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "linewise creation and editing of textual content"   , ""                                                                                        ,    0 },
   { UMOD_WANDER  , MODE_MICRO , 'y', "wdr", "wander"    , 2, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "formula creation by moving to target cells"         , "modes={ret}{esc}"                                                                        ,    0 },
   { UMOD_SRC_UNDO, MODE_SUB   , 'y', "sun", "src-undo"  , 2, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "source level undo and redo"                         , ""                                                                                        ,    0 },
   /*---(power)---------------------- --------------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { MODE_COMMAND , MODE_MAJOR , '-', "CMD", "command"   , 3, ""  , "5f--- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "command line capability for advanced actions"       , ""                                                                                        ,    0 },
   { MODE_SEARCH  , MODE_MAJOR , '-', "SCH", "search"    , 3, ""  , "5f--- p i ----- n 1---- r 0M-------- d o", "----- - - ----- - ----- - ---------- - -", "search mode to find data and objects"               , ""                                                                                        ,    0 },
   { UMOD_HISTORY , MODE_MICRO , 'y', "his", "history"   , 3, ""  , "5---- p i ----- n ----- r 0-V------- d o", "----- - - ----- - ----- - ---------- - -", "search and command history access"                  , ""                                                                                        ,    0 },
   { SMOD_FILTER  , MODE_SUB   , 'y', "fil", "filter"    , 3, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "process current/selection through external filter"  , "0HhlL$_KkjJG  gz=sh,letk.jb  dxy  !: ~uU /nN oO sS"                                      ,    0 },
   { SMOD_ERROR   , MODE_SUB   , 'y', "err", "errors"    , 3, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "display and action errors"                          , ""                                                                                        ,    0 },
   /*---(overall)-------------------- --------------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { MODE_GOD     , MODE_MAJOR , 'y', "GOD", "god"       , 4, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "god-mode allowing 3D omnicient viewing"             , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                ,    0 },
   { MODE_OMNI    , MODE_SUB   , 'y', "OMN", "omni"      , 4, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "omnipotent 3D manipulation mode"                    , "linear=LnhHJjkKIioO  rotate=PpaAYytTRrwW"                                                ,    0 },
   { SMOD_BUFFER  , MODE_SUB   , 'y', "buf", "buffer"    , 4, ""  , "5---- p i ----- n 1---- r M--------- d o", "----- - - ----- - ----- - ---------- - -", "moving and selecting between buffers and windows"   , "select=0-9A-Z   move=jk   panel=abdgpqrtxy   cursor=_[<,>]~   search=/   status=_"       ,    0 },
   { SMOD_MENUS   , MODE_SUB   , 'y', "mnu", "menus"     , 4, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "interactive menu system for accessing commands"     , ""                                                                                        ,    0 },
   { SMOD_HINT    , MODE_MAJOR , 'y', "hnt", "hint"      , 4, ""  , "5---- p i ----- n 1---- r 0M-------- d o", "----- - - ----- - ----- - ---------- - -", "provides automatic and manuallabeling hints"        , ""                                                                                        ,    0 },
   { UMOD_REPEAT  , MODE_MICRO , 'y', "rep", "repeat"    , 4, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "accumulate multiplier"                              , "range 1-99"                                                                              ,    0 },
   /*---(time)----------------------- --------------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { MODE_PROGRESS, MODE_MAJOR , 'y', "PRG", "progress"  , 5, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "progress timeline adding time dimension"            , "horz=0LlhH$  vert=_KkjJ~  speed=<>  scale=io  play=,."                                   ,    0 },
   /*---(done)----------------------- --------------------------*//* prep--- - needs-- conf--- deps-------- -    prep--- - needs-- conf--- deps-------- -*/
   { 0            , MODE_NOT   , 'y', "bad", "bad mode"  , 0, ""  , "5---- p i ----- n ----- r 0--------- d o", "----- - - ----- - ----- - ---------- - -", "default message when mode is not understood"        , "mode not understood"                                                                     ,    0 },
   /*-----------------------------------------------------------*//*-------------------------------------------------------------------------------------*/
};
static int  s_nmode   = 0;

static      char        s_majors       [MAX_MODES] = "";

#define       S_PRE        0
#define       S_PREP       6

#define       S_INIT       8

#define       S_NEEDS     10
#define       S_BASE      16

#define       S_CONF      18
#define       S_READY     24

#define       S_DEPS      26
#define       S_INTEG     37

#define       S_OPER      39



/*====================------------------------------------====================*/
/*===----                         shared functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SHARED__________o () { return; }

static  s_last       = '-';

char
MODE__by_abbr           (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   static int  n           =   -1;
   static char x_abbr      =  '-';
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_schar   (a_abbr);
   DEBUG_MODE   yLOG_schar   (x_abbr);
   /*---(short-cut)----------------------*/
   if (a_abbr == x_abbr) {
      DEBUG_MODE   yLOG_snote   ("fast");
      DEBUG_MODE   yLOG_sint    (n);
      return n;
   }
   /*---(lookup)-------------------------*/
   n = -1;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'    )  break;
      if (g_modes [i].abbr != a_abbr)   continue;
      n = i;
      DEBUG_MODE   yLOG_snote   ("found");
   }
   if (n <  0)  DEBUG_MODE   yLOG_snote   ("FAILED");
   if (n >= 0)  x_abbr = a_abbr;
   DEBUG_MODE   yLOG_sint    (n);
   s_last = n;
   /*---(complete)-----------------------*/
   return n;
}



/*====================------------------------------------====================*/
/*===----                      status checking                         ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_CHECK____o () { return; }

char
STATUS__check           (char a_abbr, char a_target)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_index     =    0;
   char        x_loc       =    0;
   char        x_val       =    0;
   /*---(check mode)---------------------*/
   x_index  = MODE__by_abbr (a_abbr);
   if (x_index < 0)  {
      DEBUG_MODE   yLOG_snote   ("mode not found");
      return 0;
   }
   /*---(prepare)------------------------*/
   switch (a_target) {
   case 'p' :  x_loc = S_PREP ;  break;
   case 'i' :  x_loc = S_INIT ;  break;
   case 'n' :  x_loc = S_BASE ;  break;
   case 'r' :  x_loc = S_READY;  break;
   case 'o' :  x_loc = S_OPER ;  break;
   }
   /*---(look-up)------------------------*/
   DEBUG_MODE   yLOG_sint    (x_loc);
   x_val = g_modes [x_index].actual [x_loc];
   /*---(check)--------------------------*/
   DEBUG_MODE   yLOG_schar   (a_target);
   DEBUG_MODE   yLOG_schar   (x_val);
   if (x_val != a_target) {
      DEBUG_MODE   yLOG_snote   ("NOT READY");
      return 0;
   }
   DEBUG_MODE   yLOG_snote   ("ready");
   /*---(complete)-----------------------*/
   return 1;
}

char
STATUS_check_prep        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__check  (a_abbr, 'p');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS_check_needs        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   if (rc == 0)  rc = STATUS__check  (a_abbr, 'i');
   if (rc >  0)  rc = STATUS__check  (a_abbr, 'n');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS_operational         (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__check  (a_abbr, 'o');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        status filling                        ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_FILL_____o () { return; }

char
STATUS__filling         (char a_abbr, char a_target)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_loc       =    0;
   char        x_len       =    0;
   int         n           =    0;
   int         i           =    0;
   int         c           =    0;
   /*---(prepare)------------------------*/
   switch (a_target) {
   case 'p' :
      rc    = STATUS__check  (a_abbr, 'i');
      x_loc = S_PRE;
      x_len =  5;
      break;
   case 'n' :
      rc    = STATUS__check  (a_abbr, 'i');
      x_loc = S_NEEDS;
      x_len =  5;
      break;
   case 'd' :
      rc    = STATUS__check  (a_abbr, 'r');
      x_loc = S_DEPS;
      x_len = 10;
      break;
   }
   /*---(check ready)--------------------*/
   if (rc < 1)   return rc;
   /*---(search for tasks)---------------*/
   for (n = 0; n < s_nmode; ++n) {
      for (i = 0; i < x_len; ++i) {
         if (g_modes [n].expect [x_loc + i] == '-')       continue;
         if (g_modes [n].expect [x_loc + i] != a_abbr)    continue;
         g_modes [n].actual [x_loc + i] = a_abbr;
         ++c;
         DEBUG_MODE   yLOG_schar   (g_modes [n].abbr);
      }
   }
   DEBUG_MODE   yLOG_snote   ("complete");
   DEBUG_MODE   yLOG_sint    (c);
   /*---(complete)-----------------------*/
   return c;
}

char
STATUS__prep_fill       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__filling  (a_abbr, 'p');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS__need_fill       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__filling  (a_abbr, 'n');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS__deps_fill       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__filling  (a_abbr, 'd');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      config specific                         ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_READY____o () { return; }

char
STATUS__conf_verify     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =   -1;
   char        x_actual    [LEN_LABEL] = "";
   char        x_expect    [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   /*---(check ready)--------------------*/
   n = s_last;
   strlcpy (x_expect, g_modes [n].expect + S_CONF, 5);
   strlcpy (x_actual, g_modes [n].actual + S_CONF, 5);
   if (strcmp (x_expect, x_actual) == 0) {
      g_modes [n].actual [S_READY] = 'r';
      DEBUG_MODE   yLOG_snote   ("marked ready, COMPLETE");
   } else {
      DEBUG_MODE   yLOG_snote   ("config not complete");
   }
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        status updating                       ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_UPDATE___o () { return; }

char
STATUS__updating        (char a_target)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_loc       =    0;
   char        x_len       =    0;
   char        x_mark      =    0;
   int         n           =    0;
   char        x_actual    [50] = "";
   char        x_expect    [50] = "";
   int         c           =    0;
   int         t           =    0;
   /*---(prepare)------------------------*/
   switch (a_target) {
   case 'p' :
      x_loc  = S_PRE;
      x_len  =  5;
      x_mark = S_PREP;
      break;
   case 'n' :
      x_loc  = S_NEEDS;
      x_len  =  5;
      x_mark = S_BASE;
      break;
   case 'd' :
      x_loc  = S_DEPS;
      x_len  = 10;
      x_mark = S_INTEG;
      break;
   case 'o' :
      x_loc  = S_PRE;
      x_len  = 30;
      x_mark = S_OPER;
      break;
   }
   /*---(search for prep tasks)----------*/
   for (n = 0; n < s_nmode; ++n) {
      if (g_modes [n].actual [x_mark] == a_target) {
         ++t;
         continue;
      }
      strlcpy (x_expect, g_modes [n].expect + x_loc, x_len);
      strlcpy (x_actual, g_modes [n].actual + x_loc, x_len);
      if (strcmp (x_expect, x_actual) != 0) continue;
      g_modes [n].actual [x_mark] = a_target;
      ++c;
      ++t;
      DEBUG_MODE   yLOG_schar   (g_modes [n].abbr);
   }
   DEBUG_MODE   yLOG_snote   ("complete");
   DEBUG_MODE   yLOG_sint    (c);
   DEBUG_MODE   yLOG_sint    (t);
   /*---(complete)-----------------------*/
   return 0;
}

char
STATUS__prep_checkall   (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__updating  ('p');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS__need_checkall   (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__updating  ('n');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS__deps_checkall   (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__updating  ('d');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
STATUS__oper_checkall   (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = STATUS__updating  ('o');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      status setting                          ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_SETTING__o () { return; }

char
STATUS_init_set         (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(ready)--------------------------*/
   rc = STATUS_check_prep (a_abbr);
   --rce;  if (rc == 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update this status)-------------*/
   n = s_last;
   DEBUG_PROG   yLOG_note    ("mark as initialized");
   g_modes [n].actual [S_INIT ] = 'i';
   /*---(mark others lines)--------------*/
   STATUS__prep_fill      (a_abbr);
   STATUS__prep_checkall  ();
   STATUS__need_fill      (a_abbr);
   STATUS__need_checkall  ();
   STATUS__conf_verify    ();
   STATUS__deps_fill      (a_abbr);
   STATUS__deps_checkall  ();
   STATUS__oper_checkall  ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STATUS_conf_set         (char a_abbr, char a_step)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(ready)--------------------------*/
   rc = STATUS_check_needs (a_abbr);
   --rce;  if (rc == 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(status)-------------------------*/
   n = s_last;
   /*---(ready)--------------------------*/
   DEBUG_PROG   yLOG_char    ("a_step"    , a_step);
   g_modes [n].actual [S_CONF + a_step - '1'] = a_step;
   /*---(mark others lines)--------------*/
   STATUS__prep_checkall  ();
   STATUS__need_checkall  ();
   STATUS__conf_verify    ();
   STATUS__deps_fill      (a_abbr);
   STATUS__deps_checkall  ();
   STATUS__oper_checkall  ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      status overall                          ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_PROG_____o () { return; }

char
STATUS__purge           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_senter  (__FUNCTION__);
   /*---(count status)-------------------*/
   for (i = 0; i < s_nmode; ++i) {
      DEBUG_PROG   yLOG_schar   (g_modes [i].abbr);
      strlcpy (g_modes [i].actual, "----- - - ----- - ----- - ---------- - -", LEN_DESC);
      g_modes [i].count = 0;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
STATUS_init             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(count status)-------------------*/
   s_nmode = 0;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == 0  )  break;
      if (g_modes [i].abbr == '-')  break;
      ++s_nmode;
   }
   DEBUG_PROG   yLOG_value   ("s_nmode" , s_nmode);
   /*---(update)-------------------------*/
   STATUS__purge ();
   STATUS__prep_checkall ();
   STATUS_init_set       (FMOD_STATUS);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STATUS_wrap             (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(purge)--------------------------*/
   STATUS__purge ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
STATUS_dump             (FILE *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   char        t           [LEN_LABEL];
   int         c           =    0;
   int         M           =    0;
   int         F           =    0;
   int         S           =    0;
   int         U           =    0;
   int         x           =    0;
   int         x_cat       =   -1;
   /*---(list)---------------------------*/
   fprintf (a_file, "yVIKEYS, capability status/readiness reporting                                          (:dump status)\n");
   fprintf (a_file, "                   ---expected-----------------------------   ---actual-------------------------------\n");
   fprintf (a_file, "---mode---   a c   prep--- i needs-- conf--- deps-------- o   prep--- i needs-- conf--- deps-------- o\n");
   for (n = 0; n < s_nmode; ++n) {
      if (x_cat != g_modes [n].cat)  fprintf (a_file, "¦");
      if (strchr ("MF", g_modes [n].type) != NULL)  strlcpy (t, g_modes [n].terse, LEN_LABEL);
      else                                          sprintf (t, " %-9.9s", g_modes [n].terse);
      fprintf (a_file, "%-10.10s   %c %c   %s   %s¦", t, g_modes [n].abbr, g_modes [n].type, g_modes [n].expect, g_modes [n].actual);
      x_cat = g_modes [n].cat;
      ++c;
      switch (g_modes [n].type) {
      case 'F' : ++F;  break;
      case 'M' : ++M;  break;
      case 's' : ++S;  break;
      case 'u' : ++U;  break;
      case 'x' : ++x;  break;
      }
   }
   fprintf (a_file, "\n");
   fprintf (a_file, "---mode---   a c   prep--- i needs-- conf--- deps-------- o   prep--- i needs-- conf--- deps-------- o\n");
   fprintf (a_file, "                   ---expected-----------------------------   ---actual-------------------------------\n");
   fprintf (a_file, "status mode count %d (fund %d, major %d, sub %d, micro %d, extern %d)\n", c, F, M, S, U, x);
   fprintf (a_file, "\n");
   fprintf (a_file, "prep  = must be initialized before this mode can initialize\n");
   fprintf (a_file, "i     = initialized and ready to configure\n");
   fprintf (a_file, "needs = must be initialized before this mode can configure\n");
   fprintf (a_file, "conf  = externally called steps in the configuration process\n");
   fprintf (a_file, "deps  = must be operational before this mode can be\n");
   fprintf (a_file, "o     = operational and ready to use\n");
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      mode stack handling                     ----===*/
/*====================------------------------------------====================*/
static void  o___MODE_STACK______o () { return; }

char         /*--> prepare mode stack for use ------------[--------[--------]-*/
MODE_init          (char a_mode)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         j           =    0;
   char        n           =    0;
   char        t           [5] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (!STATUS_check_prep  (FMOD_MODE)) {
      DEBUG_PROG   yLOG_note    ("status is not ready for init");
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   strlcpy (s_majors, "", MAX_MODES);
   /*---(identify majors)----------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes  [i].type != MODE_MAJOR)    continue;
      sprintf (t, "%c", g_modes  [i].abbr);
      strlcat (s_majors, t, MAX_MODES);
   }
   /*---(clear transitions)--------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == 0)  break;
      strcpy (g_modes [i].allow, "");
   }
   /*---(load transitions)---------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (s_modechanges [i][0] == 0)  break;
      n = MODE__by_abbr (s_modechanges [i][0]);
      /*> printf ("%c %2d\n", s_modechanges [i][0], n);                               <*/
      for (j = 1; j < LEN_TERSE; ++j) {
         if (s_modechanges [i][j] == 0)  break;
         sprintf (t, "%c", s_modechanges [i][j]);
         strlcat (g_modes [n].allow, t, LEN_DESC);
      }
   }
   /*---(clear stack)--------------------*/
   for (i = 0; i < MAX_STACK; ++i) {
      s_mode_stack [i] = '-';
   }
   /*---(clear controls)-----------------*/
   s_mode_depth =  0;
   s_mode_curr = '-';
   /*> strlcpy (myVIKEYS.mode_text, "--", LEN_TERSE);                                 <*/
   /*---(custom functions)---------------*/
   /*> s_formatter = NULL;                                                            <* 
    *> s_uniter    = NULL;                                                            <* 
    *> s_paletter  = NULL;                                                            <*/
   /*---(update status)------------------*/
   STATUS_init_set   (FMOD_MODE);
   /*---(go to default mode)-------------*/
   if (a_mode == MODE_GOD)  MODE_enter (MODE_GOD);
   else                     MODE_enter (MODE_MAP);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yvikeys_mode__update    (void)
{
   if (strchr (s_majors, s_mode_curr) != NULL) {
      /*> sprintf (myVIKEYS.mode_text, "[%c ]" , s_mode_curr);                        <*/
   } else {
      /*> sprintf (myVIKEYS.mode_text, "[%c%c]", s_mode_stack [s_mode_depth - 2], s_mode_curr);   <*/
   }
   /*> yvikeys_view_modes      (myVIKEYS.mode_text);                                  <*/
   return 0;
}

char         /*--> add a mode to the stack ---------------[--------[--------]-*/
MODE_enter         (char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           = 0;
   char        x_mode      = '-';
   int         x_index     = -1;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(check for dup)------------------*/
   /*> if (s_mode_stack [s_mode_depth] == a_mode)  return 1;                            <*/
   /*---(validate mode)------------------*/
   DEBUG_PROG   yLOG_char    ("a_mode"    , a_mode);
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'    )  break;
      if (g_modes [i].abbr != a_mode )  continue;
      ++g_modes  [i].count;
      x_mode  = a_mode;
   }
   DEBUG_PROG   yLOG_char    ("x_mode"    , x_mode);
   --rce;  if (x_mode  == '-') {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check if allowed)---------------*/
   if (s_mode_depth > 0)  {
      DEBUG_PROG   yLOG_char    ("curr"      , s_mode_curr);
      for (i = 0; i < MAX_MODES; ++i) {
         if (g_modes [i].abbr == '-'    )      break;
         if (g_modes [i].abbr != s_mode_curr)  continue;
         x_index = i;
      }
      DEBUG_PROG   yLOG_value   ("x_index"   , x_index);
      DEBUG_PROG   yLOG_info    ("name"      , g_modes [x_index].terse);
      --rce;  if (x_index <   0 ) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_PROG   yLOG_info    ("allow"     , g_modes [x_index].allow);
      --rce;  if (strchr (g_modes [x_index].allow, a_mode) == NULL) {
         DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_PROG   yLOG_value   ("x_index"   , x_index);
   /*---(add mode)-----------------------*/
   --rce;  if (s_mode_depth >= MAX_STACK) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   s_mode_stack [s_mode_depth] = a_mode;
   DEBUG_PROG   yLOG_complex ("stack"     , "%2d %s", s_mode_depth, s_mode_stack);
   ++s_mode_depth;
   /*---(set global mode)----------------*/
   s_mode_curr = a_mode;
   DEBUG_PROG   yLOG_char    ("mode_curr" , s_mode_curr);
   yvikeys_mode__update ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MODE_exit          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(check stack)--------------------*/
   DEBUG_PROG   yLOG_value   ("depth"     , s_mode_depth);
   --rce;  if (s_mode_depth <= 1) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --s_mode_depth;
   s_mode_stack [s_mode_depth] = x_mode;
   DEBUG_PROG   yLOG_complex ("stack"     , "%2d %s", s_mode_depth, s_mode_stack);
   x_mode = s_mode_stack [s_mode_depth - 1];
   /*---(set global mode)----------------*/
   s_mode_curr = x_mode;
   DEBUG_PROG   yLOG_char    ("mode_curr" , s_mode_curr);
   yvikeys_mode__update ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MODE_curr          (void)
{
   return s_mode_curr;
}

char
yVIKEYS_mode       (void)
{
   return s_mode_curr;
}

char
MODE_prev          (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(check stack)--------------------*/
   --rce;  if (s_mode_depth <= 1)                return '-';
   /*---(grab previous)------------------*/
   x_mode = s_mode_stack [s_mode_depth - 2];
   if (strchr (s_majors, x_mode) != NULL)   return x_mode;
   /*---(go back one more)---------------*/
   --rce;  if (s_mode_depth <= 2)                return '-';
   x_mode = s_mode_stack [s_mode_depth - 3];
   /*---(complete)-----------------------*/
   return x_mode;
}

char
MODE_not           (char a_mode)
{
   if (a_mode != s_mode_stack [s_mode_depth - 1]) return -1;
   return 0;
}

char       /*----: list the current mode stack -------------------------------*/
MODE_status        (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   char        t           [10];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(walk the list)------------------*/
   sprintf (a_list, "modes (%d)", s_mode_depth);
   for (i = 0; i < 8; ++i) {
      sprintf (t, " %c", s_mode_stack [i]);
      strlcat (a_list, t, LEN_FULL);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char*
MODE_message       (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        x_major     = ' ';
   char        x_minor     = ' ';
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'   )   break;
      if (g_modes [i].abbr == s_mode_curr)  break;
   }
   /*> if (g_modes  [i].type == MODE_MAJOR)  {                                        <* 
    *>    x_major = s_mode_curr;                                                      <* 
    *> } else {                                                                       <* 
    *>    x_major = MODE_prev  ();                                                    <* 
    *>    x_minor = s_mode_curr;                                                      <* 
    *> }                                                                              <*/
   snprintf (s_message, LEN_RECD, "[%c%c] %-3.3s : %s¦", x_major, x_minor, g_modes [i].three, g_modes [i].mesg);
   snprintf (s_message, LEN_RECD, "%-3.3s  %s¦", g_modes [i].three, g_modes [i].mesg);
   return s_message;
}

char
yVIKEYS_mode_change  (char a_mode, char *a_allow, char *a_mesg)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           = 0;
   int         x_mode      = -1;
   /*---(find)---------------------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'   )   break;
      if (g_modes [i].abbr != a_mode)   continue;
      x_mode = i;
   }
   --rce;  if (x_mode < 0)  return rce;
   /*---(update)-------------------------*/
   if (a_allow  != NULL) {
      strlcpy (g_modes  [x_mode].allow, a_allow, 25);
   }
   if (a_mesg  != NULL) {
      strlcpy (g_modes  [x_mode].mesg , a_mesg , LEN_FULL);
   }
   /*---(complete)-----------------------*/
   return 0;
}


