/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"


char 
ymode_update            (void)
{
   DEBUG_YMODE   yLOG_senter  (__FUNCTION__);
   DEBUG_YMODE   yLOG_schar   (g_mode_curr);
   if (strchr (g_majors, g_mode_curr) != NULL) {
      DEBUG_YMODE   yLOG_snote   ("major");
      sprintf (g_text, " %c·" , g_mode_curr);
   } else if (g_mode_depth >= 2) {
      DEBUG_YMODE   yLOG_snote   ("other");
      sprintf (g_text, " %c%c", g_mode_stack [g_mode_depth - 2], g_mode_curr);
   } else {
      DEBUG_YMODE   yLOG_snote   ("early");
      sprintf (g_text, " %c·" , g_mode_curr);
   }
   DEBUG_YMODE   yLOG_snote   (g_text);
   DEBUG_YMODE   yLOG_sexit   (__FUNCTION__);
   yVIHUB_yVIEW_modes (g_text); /* push update */
   return 0;
}

char* yMODE_update            (void) { ymode_update ();  return g_text; }
char* yMODE_text              (void) { return g_text; }


char       /*----: list the current mode stack -------------------------------*/
yMODE_status            (char a_size, short a_wide, char a_list [LEN_FULL])
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   int         n           =   0;
   char        t           [LEN_DESC] = "";
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(walk the list)------------------*/
   sprintf (a_list, " modes (%d)", g_mode_depth);
   for (i = 0; i < 8; ++i) {
      sprintf (t, " %c", g_mode_stack [i]);
      strlcat (a_list, t, LEN_FULL);
   }
   /*---(current yVIEW mode)-------------*/
   sprintf (t, "  %-3.3s", g_text);
   strlcat (a_list, t, LEN_FULL);
   sprintf (t, "  %c", g_mode_exited);
   strlcat (a_list, t, LEN_FULL);
   /*---(name)---------------------------*/
   n  = ymode_by_abbr (yMODE_curr ());
   sprintf (t, "  %-8.8s", g_modes [n].terse);
   strlcat (a_list, t, LEN_FULL);
   /*---(allow)--------------------------*/
   if (g_allow [n] == NULL || strcmp (g_allow [n], "") == 0) {
      sprintf (t, "  allows none");
   } else {
      sprintf (t, "  allows å%sæ", g_allow [n]);
   }
   strlcat (a_list, t, LEN_FULL);
   /*---(suffix)-------------------------*/
   strlcat (a_list, " Ï", LEN_FULL);
   /*---(complete)-----------------------*/
   return 0;
}

char*
yMODE_message           (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        x_major     = ' ';
   char        x_minor     = ' ';
   for (i = 0; i < MAX_MODES; ++i) {
      if (g_modes [i].abbr == '-'   )   break;
      if (g_modes [i].abbr == g_mode_curr)  break;
   }
   snprintf (g_message, LEN_RECD, " %-3.3s  %s¦", g_modes [i].three, g_modes [i].mesg);
   return g_message;
}

char
yMODE_statuses          (void *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   char        t           [LEN_LABEL];
   int         c           =    0;
   int         M           =    0;
   int         F           =    0;
   int         S           =    0;
   int         U           =    0;
   int         x           =    0;
   int         O           =    0;
   int         l           =    0;
   int         x_cat       =   -1;
   FILE       *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   --rce;  if (a_file == NULL) {
      return rce;
      DEBUG_YMODE   yLOG_exitr   (__FUNCTION__, rce);
   }
   /*---(list)---------------------------*/
   x_file = (FILE *) a_file;
   fprintf (x_file, "## yMODE, capability status/readiness reporting\n");
   fprintf (x_file, "##\n");
   fprintf (x_file, "#@ x-parse   åÏ-··Ï--··Ï---------··Ï··Ï··Ï··Ï---------··Ï------·Ï-----·Ï------·Ï------·Ï-----------·Ï··Ï------·Ï-----·Ï------·Ï------·Ï-----------·Ï··Ï-------------------------------------------------æ\n");
   fprintf (x_file, "#@ titles    åno··tla··mode········t··c··s··source······prep····init···needs···conf····deps·········o··prep····init···needs···conf····deps·········o··description·······································æ\n");
   fprintf (x_file, "##\n");
   fprintf (x_file, "##---mode----------------------------------  ---expected-------------------------------------  ---actual---------------------------------------                                                    \n");
   fprintf (x_file, "##  a  ---  name------  t  c  s  source----  åprep--- init--- needs-- conf--- deps-------- oæ  åprep--- init--- needs-- conf--- deps-------- oæ  ---description------------------------------------\n");
   for (n = 0; n < g_nmode; ++n) {
      /*---(category breaks)-------------*/
      if (x_cat != g_modes [n].cat)  fprintf (x_file, "\n");
      /*---(detail line)-----------------*/
      fprintf (x_file, "%2d  "                 , ++c);
      fprintf (x_file, "%c  %-3.3s  %-10.10s  ", g_modes [n].abbr , g_modes [n].three, g_modes [n].terse);
      fprintf (x_file, "%c  %d  %c  %-10.10s  ", g_modes [n].type , g_modes [n].cat  , g_modes [n].show , g_modes [n].who  );
      fprintf (x_file, "å%sæ  å%sæ  "          , g_modes [n].expect, g_actual[n]);
      fprintf (x_file, "%-50.50s\n"            , g_modes [n].desc);
      /*---(statistics)------------------*/
      switch (g_modes [n].type) {
      case 'F' : ++F;  break;
      case 'M' : ++M;  break;
      case 's' : ++S;  break;
      case 'u' : ++U;  break;
      case 'x' : ++x;  break;
      }
      l = strlen (g_actual [n]);
      if (g_actual [n] [l - 1] == 'o')  ++O;
      /*---(save)------------------------*/
      x_cat = g_modes [n].cat;
      /*---(done)------------------------*/
   }
   fprintf (x_file, "\n");
   fprintf (x_file, "##  a  ---  name------  t  c  s  source----  åprep--- init--- needs-- conf--- deps-------- oæ  åprep--- init--- needs-- conf--- deps-------- oæ  ---description------------------------------------\n");
   fprintf (x_file, "##---mode----------------------------------  ---expected-------------------------------------  ---actual---------------------------------------                                                    \n");
   fprintf (x_file, "##\n");
   fprintf (x_file, "## status mode count %d (fund %d, major %d, sub %d, micro %d, extern %d) and %d operational\n", c, F, M, S, U, x, O);
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char*
yMODE_actual            (char a_abbr)
{
   int n = ymode_by_abbr (a_abbr);
   if (n < 0)  return "not found";
   return g_actual [n];
}

char
yMODE_debug_status      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_YMODE   yLOG_enter   (__FUNCTION__);
   /*---(clear)--------------------------*/
   for (n = 0; n < g_nmode; ++n) {
      if (n == 0) {
         DEBUG_YMODE  yLOG_value   ("CAT"       , g_modes [n].cat);
      } else if (g_modes [n].cat != g_modes [n - 1].cat)  {
         DEBUG_YMODE  yLOG_value   ("CAT"       , g_modes [n].cat);
      }
      DEBUG_YMODE  yLOG_complex (g_modes [n].terse , "%c %c %-3.3s %-10.10s %-10.10s å%sæ  å%sæ", g_modes [n].abbr  , g_modes [n].type, g_modes [n].three , g_modes [n].terse, g_modes [n].who   , g_modes [n].expect, g_actual[n]);
   }
   /*---(complete)-----------------------*/
   DEBUG_YMODE   yLOG_exit    (__FUNCTION__);
   return 0;
}



