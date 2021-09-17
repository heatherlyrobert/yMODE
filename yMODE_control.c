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
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_schar   (a_abbr);
   DEBUG_MODE   yLOG_schar   (x_abbr);
   /*---(short-cut)----------------------*/
   if (a_abbr == x_abbr) {
      DEBUG_MODE   yLOG_snote   ("fast");
      DEBUG_MODE   yLOG_sint    (n);
      DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
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
   g_last = n;
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return n;
}



/*====================------------------------------------====================*/
/*===----                      mode stack handling                     ----===*/
/*====================------------------------------------====================*/
static void  o___MODE_STACK______o () { return; }

char         /*--> add a mode to the stack ---------------[--------[--------]-*/
yMODE_enter             (char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           = 0;
   char        x_mode      = '-';
   int         x_index     = -1;
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   /*---(check for dup)------------------*/
   /*> if (g_mode_stack [g_mode_depth] == a_mode)  return 1;                            <*/
   /*---(validate mode)------------------*/
   DEBUG_MODE   yLOG_char    ("a_mode"    , a_mode);
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'    )  break;
      if (g_modes [i].abbr != a_mode )  continue;
      x_mode  = a_mode;
   }
   DEBUG_MODE   yLOG_char    ("x_mode"    , x_mode);
   --rce;  if (x_mode  == '-') {
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check if allowed)---------------*/
   if (g_mode_depth > 0)  {
      DEBUG_MODE   yLOG_char    ("curr"      , g_mode_curr);
      for (i = 0; i < MAX_MODES; ++i) {
         if (g_modes [i].abbr == '-'    )      break;
         if (g_modes [i].abbr != g_mode_curr)  continue;
         x_index = i;
      }
      DEBUG_MODE   yLOG_value   ("x_index"   , x_index);
      DEBUG_MODE   yLOG_info    ("name"      , g_modes [x_index].terse);
      --rce;  if (x_index <   0 ) {
         DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_MODE   yLOG_info    ("allow"     , g_allow [x_index]);
      --rce;  if (g_allow [x_index] == NULL || strchr (g_allow [x_index], a_mode) == NULL) {
         DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   DEBUG_MODE   yLOG_value   ("x_index"   , x_index);
   /*---(add mode)-----------------------*/
   --rce;  if (g_mode_depth >= MAX_STACK) {
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   g_mode_stack [g_mode_depth] = a_mode;
   DEBUG_MODE   yLOG_complex ("stack"     , "%2d %s", g_mode_depth, g_mode_stack);
   ++g_mode_depth;
   /*---(set global mode)----------------*/
   g_mode_curr = a_mode;
   DEBUG_MODE   yLOG_char    ("mode_curr" , g_mode_curr);
   ymode_update ();
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_exit              (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        x_mode      = '-';
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   /*---(check stack)--------------------*/
   DEBUG_MODE   yLOG_value   ("depth"     , g_mode_depth);
   --rce;  if (g_mode_depth <= 1) {
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --g_mode_depth;
   g_mode_stack [g_mode_depth] = x_mode;
   DEBUG_MODE   yLOG_complex ("stack"     , "%2d %s", g_mode_depth, g_mode_stack);
   x_mode = g_mode_stack [g_mode_depth - 1];
   /*---(set global mode)----------------*/
   g_mode_curr = x_mode;
   DEBUG_MODE   yLOG_char    ("mode_curr" , g_mode_curr);
   ymode_update ();
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
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


