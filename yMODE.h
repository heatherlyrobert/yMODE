/*============================----beg-of-source---============================*/
#ifndef yMODE
#define yMODE yes



/*===[[ CONSTANTS ]]==========================================================*/
/*---(foundation)------------------------*/
#define     FMOD_STATUS    '5'
#define     FMOD_FILE      'f'
#define     FMOD_MODE      '0'
#define     FMOD_VIEW      'V'
#define     MODE_NONE      '-'
/*---(major modes)-----------------------*/
#define     MODE_GOD       'G'
#define     MODE_OMNI      'O'
#define     MODE_PROGRESS  'P'
#define     MODE_MAP       'M'
#define     MODE_SOURCE    'S'
#define     MODE_COMMAND   ':'
#define     MODE_SEARCH    '/'
#define     MODES_ONELINE  "S:/;"
#define     MODES_EDITING  "S:/;triw"
/*---(sub-modes)-------------------------*/
#define     SMOD_ERROR     'e'    /* error reporting and actions              */
#define     SMOD_MREG      'R'    /* register actions                         */
#define     SMOD_SREG      't'    /* text register actions                    */
#define     SMOD_BUFFER    ','    /* selecting buffers                        */
#define     SMOD_HINT      ';'    /* hint labels                              */
#define     SMOD_TAGS      '+'    /* grouping tags                            */
#define     SMOD_MACRO     '@'    /* macro processing                         */
#define     SMOD_MENUS     'g'    /* show menu system (gui)                   */
#define     SMOD_FILTER    '!'    /* data filtering                           */
/*---(micro-modes)-----------------------*/
#define     UMOD_SRC_UNDO  'u'    /* incremental text change undo/redo        */
#define     UMOD_MAP_UNDO  'm'    /* incremental map change undo/redo         */
#define     UMOD_HISTORY   'H'    /* dislay command/search history            */
#define     UMOD_MARK      '\''   /* location and object marking              */
#define     UMOD_VISUAL    'v'    /* visual selection history                 */
#define     UMOD_REPEAT    '9'    /* accumulate multiplier                    */
#define     UMOD_SRC_REPL  'r'    /* replacing characters in source mode      */
#define     UMOD_SRC_INPT  'i'    /* direct input of text                     */
#define     UMOD_WANDER    'w'    /* formula creation by pointing             */
#define     UMOD_SENDKEYS  'k'    /* sending keys (but pacing them)           */
/*---(external-modes)--------------------*/
#define     XMOD_FORMAT    '$'    /* content formatting                       */
#define     XMOD_PALETTE   'p'    /* palette/coloration                       */
#define     XMOD_UNITS     'K'    /* content units for scaling                */
#define     XMOD_OBJECT    'o'    /* object formatting                        */
/*---(pseudo)----------------------------*/
#define     FILE_COLS      'X'    /* x-axis (cols)                            */
#define     FILE_ROWS      'Y'    /* y-axis (rows)                            */
#define     FILE_TABS      'Z'    /* z-axis (tabs)                            */
#define     FILE_DEPCEL    'd'
#define     FILE_FREECEL   'f'



#endif
/*============================----end-of-source---============================*/
