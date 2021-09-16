/*============================---beg-of-source---============================*/
/*---(headers)---------------------------*/
#include    "yMODE.h"
#include    "yMODE_priv.h"



s_modechanges  [MAX_MODES][LEN_TERSE] = {
   /*    base              1               2               3               4               5               6               7               8               9               10       */
   /*12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234  12345678901234 */
   /*---(major mode-to-mode)---------*/
   { MODE_MAP      , UMOD_REPEAT   , MODE_GOD      , MODE_OMNI     , MODE_PROGRESS , MODE_SOURCE   , MODE_COMMAND  , MODE_SEARCH   , 0             , 0             , 0             },  /* all other modes */
   { MODE_GOD      , UMOD_REPEAT   , MODE_OMNI     , MODE_PROGRESS , MODE_SOURCE   , MODE_COMMAND  , MODE_SEARCH   , 0             , 0             , 0             , 0             },  /* all other modes */
   /*---(sub/umode/xmode)------------*/
   { MODE_MAP      , UMOD_VISUAL   , SMOD_MREG     , UMOD_MAP_UNDO , SMOD_BUFFER   , UMOD_MARK     , 0             , 0             , 0             , 0             , 0             },
   { MODE_MAP      , SMOD_MACRO    , XMOD_FORMAT   , XMOD_OBJECT   , SMOD_HINT     , SMOD_MENUS    , XMOD_PALETTE  , UMOD_SENDKEYS , 0             , 0             , 0             },
   { MODE_GOD      , SMOD_MACRO    , SMOD_MENUS    , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   { MODE_PROGRESS , UMOD_REPEAT   , MODE_COMMAND  , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   { MODE_OMNI     , UMOD_REPEAT   , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   /*---(source-related)-------------*/
   { MODE_SOURCE   , UMOD_REPEAT   , UMOD_SRC_INPT , UMOD_SRC_REPL , UMOD_SRC_UNDO , SMOD_SREG     , SMOD_MACRO    , UMOD_WANDER   , 0             , 0             , 0             },
   { MODE_COMMAND  , UMOD_REPEAT   , UMOD_SRC_INPT , UMOD_SRC_REPL , UMOD_SRC_UNDO , SMOD_SREG     , UMOD_HISTORY  , 0             , 0             , 0             , 0             },
   { MODE_SEARCH   , UMOD_REPEAT   , UMOD_SRC_INPT , UMOD_SRC_REPL , UMOD_SRC_UNDO , SMOD_SREG     , UMOD_HISTORY  , 0             , 0             , 0             , 0             },
   { SMOD_HINT     , UMOD_REPEAT   , UMOD_SRC_INPT , UMOD_SRC_REPL , UMOD_SRC_UNDO , 0             , 0             , 0             , 0             , 0             , 0             },
   { UMOD_SRC_INPT , UMOD_WANDER   , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   { UMOD_WANDER   , UMOD_REPEAT   , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   /*---(other)----------------------*/
   { XMOD_FORMAT   , XMOD_UNITS    , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
   /*---(done)-----------------------*/
   { 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             , 0             },
};

char
yMODE_allow_load        (void)
{
   int         n           =    0;
   int         i           =    0;
   int         j           =    0;
   char        t           [LEN_SHORT] = "";
   /*---(load transitions)---------------*/
   for (i = 0; i < MAX_MODES; ++i) {
      if (s_modechanges [i][0] == 0)  break;
      n = MODE__by_abbr (s_modechanges [i][0]);
      /*> printf ("%c %2d\n", s_modechanges [i][0], n);                               <*/
      for (j = 1; j < LEN_TERSE; ++j) {
         if (s_modechanges [i][j] == 0)  break;
         sprintf (t, "%c", s_modechanges [i][j]);
         strlcat (g_modes [n].allow, t, LEN_DESC);
      }
   }
   return 0;
}
