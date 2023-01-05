/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char           unit_answer [LEN_FULL];
int            s_xpos      = 0;
int            s_ypos      = 0;

char       /*----: set up program urgents/debugging --------------------------*/
ymode_unit__quiet       (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMODE_unit" };
   /*> yURG_logger   (x_narg, x_args);                                                <*/
   /*> yURG_urgs     (x_narg, x_args);                                                <*/
   yMODE_init (MODE_MAP);
   yMODE_init_after ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
ymode_unit__loud        (void)
{
   int         x_narg       = 1;
   char       *x_args [20]  = {"yMODE_unit" };
   yURG_logger   (x_narg, x_args);
   yURG_urgs     (x_narg, x_args);
   yURG_name  ("kitchen"      , YURG_ON);
   yURG_name  ("yvihub"       , YURG_ON);
   yURG_name  ("ymode"        , YURG_ON);
   yURG_name  ("ystr"         , YURG_ON);
   DEBUG_YMODE  yLOG_info     ("yMODE"     , yMODE_version   ());
   yMODE_init (MODE_MAP);
   yMODE_init_after ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
ymode_unit__end         (void)
{
   DEBUG_YMODE  yLOG_enter   (__FUNCTION__);
   yMODE_wrap ();
   DEBUG_YMODE  yLOG_exit    (__FUNCTION__);
   yLOGS_end    ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     unit testing handlers                    ----===*/
/*====================------------------------------------====================*/
static void  o___HANDLERS________o () { return; }

static char  s_keys [LEN_RECD] = "";
static char  s_mode [LEN_RECD] = "";

char
yMODE_unit_reset        (void)
{
   strlcpy (s_keys, "", LEN_RECD);
   strlcpy (s_mode, "", LEN_RECD);
   s_xpos = s_ypos = 0;
   return 0;
}

char
ymode_unit__log         (uchar a_mode, uchar a_key)
{
   char        t           [LEN_TERSE] = "";
   sprintf (t, "%c", a_mode);
   strlcat (s_mode, t, LEN_RECD);
   sprintf (t, "%c", chrvisible (a_key));
   strlcat (s_keys, t, LEN_RECD);
   return 0;
}

char yMODE_unit_stub      (uchar a_major, uchar a_minor) { return 0; }

char
ymode_unit__map          (uchar a_major, uchar a_minor)
{
   char        rc          =     0;
   ymode_unit__log ('M', a_minor);
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   DEBUG_YMODE   yLOG_char    ("a_minor"   , a_minor);
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
               rc = yVIHUB_yMACRO_hmode (a_major, a_minor);
               break;
   }
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode_unit__source       (uchar a_major, uchar a_minor)
{
   char        rc          =     0;
   ymode_unit__log ('S', a_minor);
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
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
   case '@'  : case 'q'    : case 'Q'      :
      rc = yVIHUB_yMACRO_hmode (a_major, a_minor);
      break;
   }
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
ymode_unit__input        (uchar a_major, uchar a_minor)
{
   ymode_unit__log ('i', a_minor);
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case '¥'  : case G_KEY_ESCAPE :
      yMODE_exit  ();
      break;
   case '¦'  : case G_KEY_RETURN :
      yMODE_exit  ();
      yMODE_exit  ();
      break;
   }
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

static  char s_command [LEN_HUND] = "";

char
ymode_unit__prepare     (void)
{
   strlcpy (s_command, ":", LEN_HUND);
   return 0;
}

char
ymode_unit__command      (uchar a_major, uchar a_minor)
{
   char        t           [LEN_SHORT] = "";
   ymode_unit__log (':', a_minor);
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   switch (a_minor) {
   case '¦'  : case G_KEY_RETURN :
      if      (strcmp (":q"     , s_command) == 0)  yVIHUB_yKEYS_quit ();
      else if (strcmp (":qa"    , s_command) == 0)  yVIHUB_yKEYS_quit ();
      else if (strcmp (":wqa"   , s_command) == 0)  yVIHUB_yKEYS_quit ();
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
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
ymode_unit__macro        (uchar a_major, uchar a_minor)
{
   ymode_unit__log ('@', a_minor);
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   yMODE_exit  ();
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}



char
yMODE_unit_handlers     (void)
{
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(reset)---------------------------*/
   yMODE_unit_reset ();
   /*---(init/yvihub)---------------------*/
   yMODE_init_set   (FMOD_KEYS    , NULL, NULL);
   yMODE_yvihub_set (FMOD_KEYS    );
   yMODE_init_set   (FMOD_FILE    , NULL, NULL);
   yMODE_yvihub_set (FMOD_FILE    );
   yMODE_init_set   (MODE_COMMAND , ymode_unit__prepare, ymode_unit__command);
   yMODE_yvihub_set (MODE_COMMAND );
   yMODE_init_set   (FMOD_VIEW    , NULL, NULL);
   yMODE_yvihub_set (FMOD_VIEW    );
   yMODE_init_set   (MODE_MAP     , NULL, ymode_unit__map);
   yMODE_yvihub_set (MODE_MAP     );
   yMODE_init_set   (MODE_SOURCE  , NULL, ymode_unit__source);
   yMODE_yvihub_set (MODE_SOURCE  );
   yMODE_init_set   (UMOD_INPUT   , NULL, ymode_unit__input);
   yMODE_init_set   (SMOD_MACRO   , NULL, ymode_unit__macro);
   yMODE_yvihub_set (SMOD_MACRO   );
   yMODE_init_set   (UMOD_SUNDO   , NULL, yMODE_unit_stub);
   yMODE_init_set   (SMOD_SREG    , NULL, yMODE_unit_stub);
   yMODE_init_set   (PMOD_REPEAT  , NULL, yMODE_unit_stub);
   /*---(after)---------------------------*/
   yMODE_after_set  (FMOD_KEYS    );
   yMODE_after_set  (FMOD_FILE    );
   yMODE_after_set  (MODE_COMMAND );
   yMODE_after_set  (FMOD_VIEW    );
   yMODE_after_set  (MODE_MAP     );
   yMODE_after_set  (MODE_SOURCE  );
   yMODE_after_set  (SMOD_MACRO   );
   /*---(sonf)----------------------------*/
   yMODE_conf_set   (FMOD_FILE    , '1');
   yMODE_conf_set   (MODE_MAP     , '1');
   yMODE_conf_set   (MODE_SOURCE  , '1');
   yMODE_conf_set   (SMOD_MACRO   , '1');
   /*---(done)----------------------------*/
   DEBUG_PROG   yLOG_info    ("mode"      , yMODE_actual (FMOD_MODE));
   DEBUG_PROG   yLOG_info    ("keys"      , yMODE_actual (FMOD_KEYS));
   DEBUG_PROG   yLOG_info    ("status"    , yMODE_actual (FMOD_STATUS));
   DEBUG_PROG   yLOG_info    ("view"      , yMODE_actual (FMOD_VIEW));
   DEBUG_PROG   yLOG_info    ("file"      , yMODE_actual (FMOD_FILE));
   DEBUG_PROG   yLOG_info    ("map"       , yMODE_actual (MODE_MAP));
   DEBUG_PROG   yLOG_info    ("comand"    , yMODE_actual (MODE_COMMAND));
   DEBUG_PROG   yLOG_info    ("source"    , yMODE_actual (MODE_SOURCE));
   DEBUG_PROG   yLOG_info    ("input"     , yMODE_actual (UMOD_INPUT));
   DEBUG_PROG   yLOG_info    ("macro"     , yMODE_actual (SMOD_MACRO));
   DEBUG_PROG   yLOG_info    ("repeat"    , yMODE_actual (PMOD_REPEAT));
   yVIHUB_yKEYS_init ();
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     unit testing accessor                    ----===*/
/*====================------------------------------------====================*/
static void  o___ACCESSOR________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.420.111.11]*/ /*-[01.0000.00#.Z]-*/ /*-[--.---.---.--]-*/
yMODE_unit              (char *a_question, int n)
{
   /*---(locals)-----------+-----------+-*/
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_FULL]  = "";
   char        t           [LEN_FULL]  = "";
   /*---(preprare)-----------------------*/
   strcpy  (unit_answer, "MODE unit        : question not understood");
   /*---(selection)----------------------*/
   if (n > 35)  n = ymode_by_abbr (n);
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
