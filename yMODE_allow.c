/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



char   s_modechanges  [MAX_MODES][LEN_TERSE] = {
   /*    base              1               2               3               4               5               6               7               8               9               10       */
   /*12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234 */
   /*---(major mode-to-mode)---------*/
   { MODE_MAP      , PMOD_REPEAT   , MODE_GOD      , MODE_OMNI     , MODE_PROGRESS , MODE_SOURCE   , MODE_COMMAND  , MODE_SEARCH   , 0             , 0             , 0             },  /* all other modes */
   { MODE_GOD      , PMOD_REPEAT   , MODE_OMNI     , MODE_PROGRESS , MODE_SOURCE   , MODE_COMMAND  , MODE_SEARCH   , 0             , 0             , 0             , 0             },  /* all other modes */
   /*---(sub/umode/xmode)------------*/
   { MODE_MAP      , UMOD_VISUAL   , SMOD_MREG     , UMOD_MUNDO    , UMOD_UNIVERSE , UMOD_MARK     , 0             , 0             , 0             , 0             , 0             },
   { MODE_MAP      , SMOD_MACRO    , XMOD_FORMAT   , XMOD_OBJECT   , SMOD_HINT     , SMOD_MENUS    , XMOD_PALETTE  , UMOD_SENDKEYS , XMOD_UNITS    , 0             , 0             },
   { MODE_GOD      , SMOD_MACRO    , SMOD_MENUS    , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   { MODE_PROGRESS , PMOD_REPEAT   , MODE_COMMAND  , SMOD_MENUS    , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   { MODE_OMNI     , PMOD_REPEAT   , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   /*---(source-related)-------------*/
   { MODE_SOURCE   , PMOD_REPEAT   , UMOD_INPUT    , UMOD_REPLACE  , UMOD_SUNDO    , SMOD_SREG     , SMOD_MACRO    , UMOD_WANDER   , 0             , 0             , 0             },
   { MODE_COMMAND  , PMOD_REPEAT   , UMOD_INPUT    , UMOD_REPLACE  , UMOD_SUNDO    , SMOD_SREG     , UMOD_HISTORY  , 0             , 0             , 0             , 0             },
   { MODE_SEARCH   , PMOD_REPEAT   , UMOD_INPUT    , UMOD_REPLACE  , UMOD_SUNDO    , SMOD_SREG     , UMOD_HISTORY  , 0             , 0             , 0             , 0             },
   { SMOD_HINT     , PMOD_REPEAT   , UMOD_INPUT    , UMOD_REPLACE  , UMOD_SUNDO    , 0             , 0             , 0             , 0             , 0             , 0             },
   { UMOD_INPUT    , UMOD_WANDER   , UMOD_HISTORY  , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   { UMOD_WANDER   , PMOD_REPEAT   , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   /*---(externnal)------------------*/
   { XMOD_FORMAT   , XMOD_UNITS    , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   /*---(done)-----------------------*/
   { 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
};

char
ymode_allow_purge       (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_allow [i] != NULL) {
         free (g_allow [i]);
         g_allow [i] = NULL;
      }
   }
   return 0;
}

char
ymode_mesg_purge        (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_mesg  [i] != g_modes [i].mesg) {
         free (g_mesg [i]);
         g_allow [i] = g_modes [i].mesg;
      }
   }
   return 0;
}

char
ymode_allow__loader     (char a_abbr)
{
   int         n           =    0;
   int         i           =    0;
   int         j           =    0;
   char        c           =  '-';
   char        d           =  '-';
   char        s           [LEN_DESC]  = "";
   char        t           [LEN_SHORT] = "";
   /*---(clear)--------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   DEBUG_YMODE   yLOG_char    ("a_move"    , a_abbr);
   if (a_abbr < 0) {
      DEBUG_YMODE   yLOG_note    ("purge all firstpurge");
      ymode_allow_purge ();
   }
   /*---(load transitions)---------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      /*---(skip empty lines)------------*/
      c = s_modechanges [i][0];
      DEBUG_YMODE   yLOG_char    ("c"         , c);
      if (c == 0)                      break;
      if (a_abbr >= 0 && c != a_abbr)  continue;
      /*---(find base)-------------------*/
      n = ymode_by_abbr (c);
      DEBUG_YMODE   yLOG_value   ("n"         , n);
      /*---(walk allowed)----------------*/
      for (j = 1; j < LEN_TERSE; ++j) {
         /*---(skip empty spaces)--------*/
         d = s_modechanges [i][j];
         DEBUG_YMODE   yLOG_char    ("d"         , d);
         if (d == 0)  break;
         /*---(save start)---------------*/
         strlcpy (s, ""         , LEN_DESC);
         if (g_allow [n] != NULL) {
            strlcpy (s, g_allow [n], LEN_DESC);
            free (g_allow [n]);
            g_allow [n] = NULL;
         }
         /*---(save combined)------------*/
         sprintf (t, "%c", d);
         strlcat (s          , t, LEN_DESC);
         g_allow [n] = strdup (s);
         DEBUG_YMODE   yLOG_info    ("g_allow"   , g_allow [n]);
         /*---(done)---------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char ymode_allow_load        (void)        { return ymode_allow__loader (-1    ); }
char ymode_allow_single      (char a_mode) { return ymode_allow__loader (a_mode); }

char
yMODE_get_allow         (char a_abbr, char *a_allow)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         n           = -1;
   /*---(defense)------------------------*/
   --rce;  if (a_allow == NULL)  return rce;
   /*---(find)---------------------------*/
   n = ymode_by_abbr (a_abbr);
   --rce;  if (n < 0)            return rce;
   /*---(update)-------------------------*/
   if (g_allow [n] == NULL)  strcpy  (a_allow, "");
   else                      strlcpy (a_allow, g_allow [n], LEN_DESC);
   /*---(complete)-----------------------*/
   return 0;
}

char
yMODE_set_allow         (char a_abbr, char *a_allow)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         n           = -1;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YMODE   yLOG_point   ("a_allow"   , a_allow);
   --rce;  if (a_allow == NULL) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMODE   yLOG_info    ("a_allow"   , a_allow);
   /*---(find)---------------------------*/
   DEBUG_YMODE   yLOG_char    ("a_abbr"    , a_abbr);
   n = ymode_by_abbr (a_abbr);
   --rce;  if (n < 0) {
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free existing)------------------*/
   DEBUG_YMODE   yLOG_point   ("g_allow"   , g_allow [n]);
   if (g_allow [n] != NULL) {
      DEBUG_YMODE   yLOG_note    ("exists, must free");
      free (g_allow [n]);
      g_allow [n] = NULL;
   }
   /*---(update)-------------------------*/
   if (strcmp (a_allow, "(def)") == 0) {
      DEBUG_YMODE   yLOG_note    ("set to default");
      ymode_allow_single (a_abbr);
   }
   else  {
      DEBUG_YMODE   yLOG_note    ("set directly");
      g_allow [n] = strdup (a_allow);
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yMODE_get_message       (char a_abbr, char *a_mesg)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         n           = -1;
   /*---(defense)------------------------*/
   --rce;  if (a_mesg  == NULL)  return rce;
   /*---(find)---------------------------*/
   n = ymode_by_abbr (a_abbr);
   --rce;  if (n < 0)            return rce;
   /*---(update)-------------------------*/
   if (g_allow [n] == NULL)  strcpy  (a_mesg , "");
   else                      strlcpy (a_mesg , g_mesg  [n], LEN_HUND);
   /*---(complete)-----------------------*/
   return 0;
}

char
yMODE_set_message       (char a_abbr, char *a_mesg)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         n           = -1;
   /*---(defense)------------------------*/
   --rce;  if (a_mesg  == NULL)  return rce;
   /*---(find)---------------------------*/
   n = ymode_by_abbr (a_abbr);
   --rce;  if (n < 0)  return rce;
   /*---(free existing)------------------*/
   if (g_mesg  [n] != g_modes [n].mesg) free (g_mesg [n]);
   /*---(update)-------------------------*/
   if (strcmp (a_mesg , "(def)") == 0)  g_mesg [n] = g_modes [n].mesg;
   else                                 g_mesg [n] = strdup (a_mesg);
   /*---(complete)-----------------------*/
   return 0;
}

