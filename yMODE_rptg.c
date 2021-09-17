/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"


char 
ymode_update            (void)
{
   if (strchr (g_majors, g_mode_curr) != NULL) {
      sprintf (g_text, "[%c ]" , g_mode_curr);
   } else {
      sprintf (g_text, "[%c%c]", g_mode_stack [g_mode_depth - 2], g_mode_curr);
   }
   return 0;
}

char* yMODE_text              (void) { return g_text; }


char       /*----: list the current mode stack -------------------------------*/
yMODE_status            (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   char        t           [10];
   /*---(defenses)-----------------------*/
   --rce;  if (a_list  == NULL)  return rce;
   /*---(walk the list)------------------*/
   sprintf (a_list, "modes (%d)", g_mode_depth);
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
   /*> if (g_modes  [i].type == MODE_MAJOR)  {                                        <* 
    *>    x_major = g_mode_curr;                                                      <* 
    *> } else {                                                                       <* 
    *>    x_major = MODE_prev  ();                                                    <* 
    *>    x_minor = g_mode_curr;                                                      <* 
    *> }                                                                              <*/
   snprintf (g_message, LEN_RECD, "[%c%c] %-3.3s : %s¦", x_major, x_minor, g_modes [i].three, g_modes [i].mesg);
   snprintf (g_message, LEN_RECD, "%-3.3s  %s¦", g_modes [i].three, g_modes [i].mesg);
   return g_message;
}



