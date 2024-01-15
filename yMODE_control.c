/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



/*====================------------------------------------====================*/
/*===----                         shared functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SHARED__________o () { return; }

char
ymode_by_abbr           (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   static int  n           =   -1;
   static char x_abbr      =  '-';
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_senter  (__FUNCTION__);
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_schar   (a_abbr);
   DEBUG_YMODE   yLOG_schar   (x_abbr);
   /*---(short-cut)----------------------*/
   if (a_abbr == x_abbr) {
      DEBUG_YMODE   yLOG_snote   ("fast");
      DEBUG_YMODE   yLOG_sint    (n);
      DEBUG_YMODE   yLOG_sexit   (__FUNCTION__);
      return n;
   }
   /*---(lookup)-------------------------*/
   n = -1;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '·'    )  break;
      if (g_modes [i].abbr != a_abbr)   continue;
      n = i;
      DEBUG_YMODE   yLOG_snote   ("found");
      break;
   }
   DEBUG_YMODE   yLOG_sint    (n);
   if (n <  0) {
      DEBUG_YMODE   yLOG_snote   ("FAILED");
      DEBUG_YMODE   yLOG_sexitr  (__FUNCTION__, n);
      return n;
   }
   x_abbr = a_abbr;
   g_last = n;
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_sexit   (__FUNCTION__);
   return n;
}



/*====================------------------------------------====================*/
/*===----                      mode stack handling                     ----===*/
/*====================------------------------------------====================*/
static void  o___MODE_STACK______o () { return; }

char         /*--> add a mode to the stack ---------------[--------[--------]-*/
ymode__enter            (char a_force, char a_mode)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   int         n           =    0;
   char        x_mode      =  '-';
   int         x_index     =   -1;
   char      (*x_prepper) (void)       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(defaulting)---------------------*/
   /*> g_mode_exited = '-';                                                           <*/
   /*> DEBUG_YMODE   yLOG_char    ("exited"    , g_mode_exited);                      <*/
   /*---(check for dup)------------------*/
   if (g_mode_stack [g_mode_depth] == a_mode)  return 1;
   /*---(validate mode)------------------*/
   DEBUG_YMODE   yLOG_char    ("a_mode"    , a_mode);
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'    )  break;
      if (g_modes [i].abbr != a_mode )  continue;
      n       = i;
      x_mode  = a_mode;
   }
   DEBUG_YMODE   yLOG_char    ("x_mode"    , x_mode);
   --rce;  if (x_mode  == '-') {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMODE   yLOG_complex ("before"    , "%2d %s", g_mode_depth, g_mode_stack);
   /*---(check if allowed)---------------*/
   if (g_mode_depth > 0)  {
      DEBUG_YMODE   yLOG_char    ("curr"      , g_mode_curr);
      for (i = 0; i < MAX_MODES; ++i) {
         if (g_modes [i].abbr == '-'    )      break;
         if (g_modes [i].abbr != g_mode_curr)  continue;
         x_index = i;
      }
      DEBUG_YMODE   yLOG_value   ("x_index"   , x_index);
      DEBUG_YMODE   yLOG_info    ("name"      , g_modes [x_index].terse);
      --rce;  if (x_index <   0 ) {
         DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_YMODE   yLOG_info    ("allow"     , g_allow [x_index]);
      --rce;  if (g_allow [x_index] == NULL || strchr (g_allow [x_index], a_mode) == NULL) {
         DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_YMODE   yLOG_value   ("x_index"   , x_index);
   /*---(check if ready)-----------------*/
   --rce;  if (a_force != 'y' && !yMODE_operational (a_mode)) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(run prepper)--------------------*/
   x_prepper = g_prepper [n];
   DEBUG_YMODE   yLOG_point   ("x_prepper" , x_prepper);
   --rce;  if (x_prepper != NULL) {
      rc = x_prepper ();
      DEBUG_YMODE   yLOG_value   ("prepper"   , rc);
      if (rc < 0) {
         DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(add mode)-----------------------*/
   --rce;  if (g_mode_depth >= MAX_STACK) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_mode_stack [g_mode_depth] = a_mode;
   ++g_mode_depth;
   DEBUG_YMODE   yLOG_complex ("after"     , "%2d %s", g_mode_depth, g_mode_stack);
   /*---(set global mode)----------------*/
   g_mode_curr = a_mode;
   DEBUG_YMODE   yLOG_char    ("mode_curr" , g_mode_curr);
   ymode_update ();
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char  yMODE_enter   (char a_mode)  { return ymode__enter ('-', a_mode); }
char  ymode_force   (char a_mode)  { return ymode__enter ('y', a_mode); }

char
yMODE_exit              (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(check stack)--------------------*/
   DEBUG_YMODE   yLOG_value   ("depth"     , g_mode_depth);
   --rce;  if (g_mode_depth <= 1) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMODE   yLOG_complex ("before"    , "%2d %s", g_mode_depth, g_mode_stack);
   --g_mode_depth;
   g_mode_exited = g_mode_stack [g_mode_depth];
   DEBUG_YMODE   yLOG_char    ("exited"    , g_mode_exited);
   g_mode_stack [g_mode_depth] = x_mode;
   DEBUG_YMODE   yLOG_complex ("after"     , "%2d %s", g_mode_depth, g_mode_stack);
   x_mode = g_mode_stack [g_mode_depth - 1];
   /*---(set global mode)----------------*/
   g_mode_curr   = x_mode;
   DEBUG_YMODE   yLOG_char    ("mode_curr" , g_mode_curr);
   ymode_update ();
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_curr              (void)
{
   return g_mode_curr;
}

char
yMODE_prev              (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(check stack)--------------------*/
   --rce;  if (g_mode_depth <= 1)                return '-';
   /*---(grab previous)------------------*/
   x_mode = g_mode_stack [g_mode_depth - 2];
   if (strchr (g_majors, x_mode) != NULL)   return x_mode;
   /*---(go back one more)---------------*/
   --rce;  if (g_mode_depth <= 2)                return '-';
   x_mode = g_mode_stack [g_mode_depth - 3];
   /*---(complete)-----------------------*/
   return x_mode;
}

char
yMODE_not               (char a_mode)
{
   if (a_mode != g_mode_stack [g_mode_depth - 1]) return -1;
   return 0;
}

char
yMODE_exited            (void)
{
   return g_mode_exited;
}

char
yMODE_using_command     (void)
{
   int         i           =    0;
   for (i = 0; i < g_mode_depth; ++i) {
      if (g_mode_stack [i] == MODE_COMMAND)  return 1;
   }
   return 0;
}

char
yMODE_using_menus       (void)
{
   int         i           =    0;
   for (i = 0; i < g_mode_depth; ++i) {
      if (g_mode_stack [i] == SMOD_MENUS  )  return 1;
   }
   return 0;
}

