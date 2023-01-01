/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"


char     yMODE_stub () { return 0; }



/*====================------------------------------------====================*/
/*===----                         yKEYS                                ----===*/
/*====================------------------------------------====================*/
static void  o___YKEYS___________o () { return; }

static char     (*s_ykeys_init)      (void)                           = yMODE_stub;
static char     (*s_ykeys_quit)      (void)                           = yMODE_stub;
static char     (*s_ykeys_warning)   (void)                           = yMODE_stub;
static char     (*s_ykeys_check_rpt) (void)                           = yMODE_stub;
static char     (*s_ykeys_rpt_umode) (uchar, uchar)                   = yMODE_stub;
static char     (*s_ykeys_grp_hmode) (uchar, uchar)                   = yMODE_stub;
static char     (*s_ykeys_rpt_check) (uchar, uchar, char, char, char) = yMODE_stub;

char
yMODE_from_yKEYS        (void *f_init, void *f_quit, void *f_warning, void *f_check_rpt, void *f_repeat_umode, void *f_group_hmode, void *f_rpt_check)
{
   s_ykeys_init      = f_init;
   s_ykeys_quit      = f_quit;
   s_ykeys_warning   = f_warning;
   s_ykeys_check_rpt = f_check_rpt;
   s_ykeys_rpt_umode = f_repeat_umode;
   s_ykeys_grp_hmode = f_group_hmode;
   s_ykeys_rpt_check = f_rpt_check;
   return 0;
}

char
ymode_yKEYS_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_init);
   if (s_ykeys_init != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_init ();
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yKEYS_quit        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_quit);
   if (s_ykeys_quit != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_quit ();
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yKEYS_warning     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_warning);
   if (s_ykeys_warning != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_warning ();
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yKEYS_check_rpt   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_check_rpt);
   if (s_ykeys_check_rpt != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_check_rpt ();
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yKEYS_rpt_umode   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_rpt_umode);
   if (s_ykeys_rpt_umode != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_rpt_umode (a_major, a_minor);
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yKEYS_grp_hmode   (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_grp_hmode);
   if (s_ykeys_grp_hmode != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_grp_hmode (a_major, a_minor);
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yKEYS_rpt_check   (uchar a_major, uchar a_minor, char a, char b, char c)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ykeys_rpt_check);
   if (s_ykeys_rpt_check != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ykeys_rpt_check (a_major, a_minor, a, b, c);
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                         yMACRO                               ----===*/
/*====================------------------------------------====================*/
static void  o___YMACRO__________o () { return; }

static char     (*s_ymacro_hmode)    (uchar, uchar)                   = yMODE_stub;

char
yMODE_from_yMACRO       (void *f_hmode)
{
   s_ymacro_hmode    = f_hmode;
   return 0;
}

char
ymode_yMACRO_hmode      (uchar a_major, uchar a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_ymacro_hmode);
   if (s_ymacro_hmode != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_ymacro_hmode (a_major, a_minor);
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                         yVIEW                                ----===*/
/*====================------------------------------------====================*/
static void  o___YVIEW___________o () { return; }

static char     (*s_yview_keys)      (uchar*)                         = yMODE_stub;
static char     (*s_yview_modes)     (uchar*)                         = yMODE_stub;

char
yMODE_from_yVIEW        (void *f_keys, void *f_modes)
{
   s_yview_keys      = f_keys;
   s_yview_modes     = f_modes;
   return 0;
}

char
ymode_yVIEW_keys        (char *a_text)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_yview_keys);
   if (s_yview_keys != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_yview_keys (a_text);
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}

char
ymode_yVIEW_modes       (char *a_text)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(call)---------------------------*/
   DEBUG_YMODE   yLOG_point   ("func"      , s_yview_modes);
   if (s_yview_modes != NULL) {
      DEBUG_YMODE   yLOG_note    ("calling");
      rc = s_yview_modes (a_text);
      DEBUG_YMODE   yLOG_value   ("rc"        , rc);
   } else {
      DEBUG_YMODE   yLOG_note    ("never configured");
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit(__FUNCTION__);
   return rc;
}


