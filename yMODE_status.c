/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



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
/*===----                      status checking                         ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_CHECK____o () { return; }

char
ymode__check            (char a_abbr, char a_target)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_index     =    0;
   char        x_loc       =    0;
   char        x_val       =    0;
   /*---(check mode)---------------------*/
   x_index  = ymode_by_abbr (a_abbr);
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
   x_val = g_actual [x_index] [x_loc];
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
yMODE_check_prep        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__check  (a_abbr, 'p');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
yMODE_check_needs       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   if (rc == 0)  rc = ymode__check  (a_abbr, 'i');
   if (rc >  0)  rc = ymode__check  (a_abbr, 'n');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
yMODE_operational       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__check  (a_abbr, 'o');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        status filling                        ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_FILL_____o () { return; }

char
ymode__filling          (char a_abbr, char a_target)
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
      rc    = ymode__check  (a_abbr, 'i');
      x_loc = S_PRE;
      x_len =  5;
      break;
   case 'n' :
      rc    = ymode__check  (a_abbr, 'i');
      x_loc = S_NEEDS;
      x_len =  5;
      break;
   case 'd' :
      rc    = ymode__check  (a_abbr, 'r');
      x_loc = S_DEPS;
      x_len = 10;
      break;
   }
   /*---(check ready)--------------------*/
   if (rc < 1)   return rc;
   /*---(search for tasks)---------------*/
   for (n = 0; n < g_nmode; ++n) {
      for (i = 0; i < x_len; ++i) {
         if (g_modes [n].expect [x_loc + i] == '-')       continue;
         if (g_modes [n].expect [x_loc + i] != a_abbr)    continue;
         g_actual [n] [x_loc + i] = a_abbr;
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
ymode__prep_fill        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__filling  (a_abbr, 'p');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
ymode__need_fill        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__filling  (a_abbr, 'n');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
ymode__deps_fill        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__filling  (a_abbr, 'd');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      config specific                         ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_READY____o () { return; }

char
ymode__conf_verify      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =   -1;
   char        x_actual    [LEN_LABEL] = "";
   char        x_expect    [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   /*---(check ready)--------------------*/
   n = g_last;
   strlcpy (x_expect, g_modes [n].expect + S_CONF, 5);
   strlcpy (x_actual, g_actual [n] + S_CONF, 5);
   if (strcmp (x_expect, x_actual) == 0) {
      g_actual [n] [S_READY] = 'r';
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
ymode__updating         (char a_target)
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
   for (n = 0; n < g_nmode; ++n) {
      if (g_actual [n] [x_mark] == a_target) {
         ++t;
         continue;
      }
      strlcpy (x_expect, g_modes [n].expect + x_loc, x_len);
      strlcpy (x_actual, g_actual [n] + x_loc, x_len);
      if (strcmp (x_expect, x_actual) != 0) continue;
      g_actual [n] [x_mark] = a_target;
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
ymode__prep_checkall    (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__updating  ('p');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
ymode__need_checkall    (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__updating  ('n');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
ymode__deps_checkall    (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__updating  ('d');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
ymode__oper_checkall    (void)
{
   char        rc          =    0;
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   rc = ymode__updating  ('o');
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      status setting                          ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_SETTING__o () { return; }

char
yMODE_init_set          (char a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   /*---(ready)--------------------------*/
   rc = yMODE_check_prep (a_abbr);
   --rce;  if (rc == 0) {
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update this status)-------------*/
   n = g_last;
   DEBUG_MODE   yLOG_note    ("mark as initialized");
   g_actual [n] [S_INIT ] = 'i';
   /*---(mark others lines)--------------*/
   ymode__prep_fill       (a_abbr);
   ymode__prep_checkall   ();
   ymode__need_fill       (a_abbr);
   ymode__need_checkall   ();
   ymode__conf_verify     ();
   ymode__deps_fill       (a_abbr);
   ymode__deps_checkall   ();
   ymode__oper_checkall   ();
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_conf_set          (char a_abbr, char a_step)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   /*---(ready)--------------------------*/
   rc = yMODE_check_needs (a_abbr);
   --rce;  if (rc == 0) {
      DEBUG_MODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(status)-------------------------*/
   n = g_last;
   /*---(ready)--------------------------*/
   DEBUG_MODE   yLOG_char    ("a_step"    , a_step);
   g_actual [n] [S_CONF + a_step - '1'] = a_step;
   /*---(mark others lines)--------------*/
   ymode__prep_checkall   ();
   ymode__need_checkall   ();
   ymode__conf_verify     ();
   ymode__deps_fill       (a_abbr);
   ymode__deps_checkall   ();
   ymode__oper_checkall   ();
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      status overall                          ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_PROG_____o () { return; }

char
ymode_status_purge      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   /*---(count status)-------------------*/
   for (i = 0; i < g_nmode; ++i) {
      DEBUG_MODE   yLOG_schar   (g_modes [i].abbr);
      strlcpy (g_actual [i], "----- - - ----- - ----- - ---------- - -", LEN_DESC);
   }
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ymode_status_init       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_MODE   yLOG_enter   (__FUNCTION__);
   /*---(count status)-------------------*/
   g_nmode = 0;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == 0  )  break;
      if (g_modes [i].abbr == '-')  break;
      ++g_nmode;
   }
   DEBUG_MODE   yLOG_value   ("g_nmode" , g_nmode);
   /*---(update)-------------------------*/
   ymode_status_purge   ();
   ymode__prep_checkall ();
   yMODE_init_set       (FMOD_STATUS);
   /*---(complete)-----------------------*/
   DEBUG_MODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_statuses          (FILE *a_file)
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
   for (n = 0; n < g_nmode; ++n) {
      if (x_cat != g_modes [n].cat)  fprintf (a_file, "¦");
      if (strchr ("MF", g_modes [n].type) != NULL)  strlcpy (t, g_modes [n].terse, LEN_LABEL);
      else                                          sprintf (t, " %-9.9s", g_modes [n].terse);
      fprintf (a_file, "%-10.10s   %c %c   %s   %s¦", t, g_modes [n].abbr, g_modes [n].type, g_modes [n].expect, g_actual [n]);
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
