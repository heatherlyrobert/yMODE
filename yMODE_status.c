/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



#define       S_PRE        0
#define       S_PREP       6

#define       S_FUNC       8
#define       S_INIT      10

#define       S_NEEDS     12
#define       S_BASE      18

#define       S_CONF      20
#define       S_READY     26

#define       S_DEPS      28
#define       S_INTEG     39

#define       S_OPER      41



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
   char        x_exp       =    0;
   char        x_val       =    0;
   /*---(check mode)---------------------*/
   x_index  = ymode_by_abbr (a_abbr);
   if (x_index < 0)  {
      DEBUG_YMODE   yLOG_snote   ("mode not found");
      return 0;
   }
   /*---(prepare)------------------------*/
   switch (a_target) {
   case 'p' :  x_loc = S_PREP ;  break;
   case 'f' :  x_loc = S_FUNC ;  break;
   case 'i' :  x_loc = S_INIT ;  break;
   case 'n' :  x_loc = S_BASE ;  break;
   case 'r' :  x_loc = S_READY;  break;
   case 'd' :  x_loc = S_DEPS ;  break;
   case 'o' :  x_loc = S_OPER ;  break;
   }
   /*---(look-up)------------------------*/
   DEBUG_YMODE   yLOG_value   ("x_loc"     , x_loc);
   x_exp = g_modes  [x_index].expect [x_loc];
   x_val = g_actual [x_index] [x_loc];
   /*---(check)--------------------------*/
   DEBUG_YMODE   yLOG_char    ("a_target"  , a_target);
   DEBUG_YMODE   yLOG_char    ("a_val"     , x_val);
   if (x_exp != '-' && x_val != a_target) {
      DEBUG_YMODE   yLOG_note    ("NOT READY");
      return 0;
   }
   DEBUG_YMODE   yLOG_note    ("ready");
   /*---(complete)-----------------------*/
   return 1;
}

char
yMODE_check_prep        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   if (rc == 0)  rc = ymode__check  (a_abbr, 'p');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yMODE_check_needs       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   if (rc == 0)  rc = ymode__check  (a_abbr, 'i');
   if (rc >  0)  rc = ymode__check  (a_abbr, 'f');
   if (rc >  0)  rc = ymode__check  (a_abbr, 'n');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yMODE_operational       (char a_abbr)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__check  (a_abbr, 'o');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
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
         DEBUG_YMODE   yLOG_char    ("abbr"      , g_modes [n].abbr);
      }
   }
   DEBUG_YMODE   yLOG_note    ("complete");
   DEBUG_YMODE   yLOG_value   ("c"         , c);
   /*---(complete)-----------------------*/
   return c;
}

char
ymode__prep_fill        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__filling  (a_abbr, 'p');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode__need_fill        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__filling  (a_abbr, 'n');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode__deps_fill        (char a_abbr)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__filling  (a_abbr, 'd');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YMODE   yLOG_senter  (__FUNCTION__);
   /*---(check ready)--------------------*/
   n = g_last;
   strlcpy (x_expect, g_modes [n].expect + S_CONF, 5);
   strlcpy (x_actual, g_actual [n] + S_CONF, 5);
   if (strcmp (x_expect, x_actual) == 0) {
      g_actual [n] [S_READY] = 'r';
      DEBUG_YMODE   yLOG_snote   ("marked ready, COMPLETE");
   } else {
      DEBUG_YMODE   yLOG_snote   ("config not complete");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_sexit   (__FUNCTION__);
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
      x_len  = 40;
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
      DEBUG_YMODE   yLOG_char    ("abbr"      , g_modes [n].abbr);
   }
   DEBUG_YMODE   yLOG_note    ("complete");
   DEBUG_YMODE   yLOG_value   ("c"         , c);
   DEBUG_YMODE   yLOG_value   ("t"         , t);
   /*---(complete)-----------------------*/
   return 0;
}

char
ymode__prep_checkall    (void)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__updating  ('p');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode__need_checkall    (void)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__updating  ('n');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode__deps_checkall    (void)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__updating  ('d');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode__oper_checkall    (void)
{
   char        rc          =    0;
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   rc = ymode__updating  ('o');
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      status setting                          ----===*/
/*====================------------------------------------====================*/
static void  o___STATUS_SETTING__o () { return; }

char
yMODE_init_set          (char a_abbr, void *a_prepper, void *a_handler)
{
   /*---(design notes)-------------------*/
   /*
    *  allows re-initialization as is sometimes useful
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   DEBUG_YMODE   yLOG_char    ("a_abbr"    , a_abbr);
   /*---(ready)--------------------------*/
   rc = yMODE_check_prep (a_abbr);
   --rce;  if (rc == 0) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   n = g_last;
   /*---(check the handler)--------------*/
   --rce;  if (g_modes [n].expect [S_FUNC] != '-') {
      if (a_handler == NULL) {
         DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      g_handler [n]           = a_handler;
      g_prepper [n]           = a_prepper;
      g_actual  [n] [S_FUNC ] = 'f';
   }
   /*---(update this status)-------------*/
   DEBUG_YMODE   yLOG_note    ("mark as initialized");
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
   /*---(other updates)------------------*/
   /*> if (a_abbr == FMOD_FILE) {                                                                  <* 
    *>    yFILE_dump_add ("modes"     , "", "inventory of mode statuses"  , ymode_dump        );   <* 
    *> }                                                                                           <*/
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_conf_set          (char a_abbr, char a_step)
{
   /*---(design notes)-------------------*/
   /*
    *  allows re-configuration as is sometimes useful
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         n           =   -1;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   DEBUG_YMODE   yLOG_char    ("a_abbr"    , a_abbr);
   /*---(ready)--------------------------*/
   rc = yMODE_check_needs (a_abbr);
   --rce;  if (rc == 0) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_step == 0 || strchr (YSTR_NUMBER, a_step) == NULL) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   n = g_last;
   DEBUG_YMODE   yLOG_char    ("a_step"    , a_step);
   DEBUG_YMODE   yLOG_char    ("actual"    , g_actual [n]       [S_CONF + a_step - '1']);
   DEBUG_YMODE   yLOG_char    ("expect"    , g_modes [n].expect [S_CONF + a_step - '1']);
   --rce;  if (g_modes [n].expect [S_CONF + a_step - '1'] != a_step) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(ready)--------------------------*/
   g_actual [n] [S_CONF + a_step - '1'] = a_step;
   /*---(mark others lines)--------------*/
   ymode__prep_checkall   ();
   ymode__need_checkall   ();
   ymode__conf_verify     ();
   ymode__deps_fill       (a_abbr);
   ymode__deps_checkall   ();
   ymode__oper_checkall   ();
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
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
   DEBUG_YMODE   yLOG_senter  (__FUNCTION__);
   /*---(count status)-------------------*/
   for (i = 0; i < g_nmode; ++i) {
      DEBUG_YMODE   yLOG_schar   (g_modes [i].abbr);
      strlcpy (g_actual [i], "----- - - - ----- - ----- - ---------- - -", LEN_DESC);
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
ymode_status_init       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(count status)-------------------*/
   g_nmode = 0;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == 0  )  break;
      if (g_modes [i].abbr == '-')  break;
      ++g_nmode;
   }
   DEBUG_YMODE   yLOG_value   ("g_nmode" , g_nmode);
   /*---(update)-------------------------*/
   ymode_status_purge   ();
   ymode__prep_checkall ();
   yMODE_init_set       (FMOD_STATUS, NULL, NULL);
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}
