/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"


char 
ymode_update            (void)
{
   DEBUG_MODE   yLOG_senter  (__FUNCTION__);
   DEBUG_MODE   yLOG_schar   (g_mode_curr);
   if (strchr (g_majors, g_mode_curr) != NULL) {
      DEBUG_MODE   yLOG_snote   ("major");
      sprintf (g_text, " %c·" , g_mode_curr);
   } else if (g_mode_depth >= 2) {
      DEBUG_MODE   yLOG_snote   ("other");
      sprintf (g_text, " %c%c", g_mode_stack [g_mode_depth - 2], g_mode_curr);
   } else {
      DEBUG_MODE   yLOG_snote   ("early");
      sprintf (g_text, " %c·" , g_mode_curr);
   }
   DEBUG_MODE   yLOG_snote   (g_text);
   DEBUG_MODE   yLOG_sexit   (__FUNCTION__);
   yVIEW_modes (g_text); /* push update */
   return 0;
}

char* yMODE_update            (void) { ymode_update ();  return g_text; }
char* yMODE_text              (void) { return g_text; }


char       /*----: list the current mode stack -------------------------------*/
yMODE_status            (char a_size, short a_wide, char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   char        t           [10];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(walk the list)------------------*/
   sprintf (a_list, " modes (%d)", g_mode_depth);
   for (i = 0; i < 8; ++i) {
      sprintf (t, " %c", g_mode_stack [i]);
      strlcat (a_list, t, LEN_FULL);
   }
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
   int         n           =    0;
   char        t           [LEN_LABEL];
   int         c           =    0;
   int         M           =    0;
   int         F           =    0;
   int         S           =    0;
   int         U           =    0;
   int         x           =    0;
   int         x_cat       =   -1;
   FILE       *x_file      = NULL;
   /*---(list)---------------------------*/
   x_file = (FILE *) a_file;
   fprintf (a_file, "yVIKEYS, capability status/readiness reporting                                          (:dump status)\n");
   fprintf (a_file, "                   ---expected-------------------------------   ---actual---------------------------------\n");
   fprintf (a_file, "---mode---   a c   prep--- f i needs-- conf--- deps-------- o   prep--- f i needs-- conf--- deps-------- o\n");
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
   fprintf (a_file, "---mode---   a c   prep--- f i needs-- conf--- deps-------- o   prep--- f i needs-- conf--- deps-------- o\n");
   fprintf (a_file, "                   ---expected-------------------------------   ---actual---------------------------------\n");
   fprintf (a_file, "status mode count %d (fund %d, major %d, sub %d, micro %d, extern %d)\n", c, F, M, S, U, x);
   fprintf (a_file, "\n");
   fprintf (a_file, "prep  = must be initialized before this mode can initialize\n");
   fprintf (a_file, "f     = mode handler function pointer\n");
   fprintf (a_file, "i     = initialized and ready to configure\n");
   fprintf (a_file, "needs = must be initialized before this mode can configure\n");
   fprintf (a_file, "conf  = externally called steps in the configuration process\n");
   fprintf (a_file, "deps  = must be operational before this mode can be\n");
   fprintf (a_file, "o     = operational and ready to use\n");
   /*---(complete)-----------------------*/
   return 0;
}

char*
yMODE_actual            (char a_abbr)
{
   int n = ymode_by_abbr (a_abbr);
   if (n < 0)  return "not found";
   return g_actual [n];
}



