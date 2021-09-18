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




/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       yMODE_version           (void);
char        yMODE_init              (char a_mode);
char        yMODE_enter             (char a_mode);
char        yMODE_exit              (void);
char        yMODE_curr              (void);
char        yMODE_prev              (void);
char        yMODE_not               (char a_mode);

char        yMODE_get_allow         (char a_abbr, char *a_allow);
char        yMODE_set_allow         (char a_abbr, char *a_allow);
char        yMODE_get_message       (char a_abbr, char *a_mesg);
char        yMODE_set_message       (char a_abbr, char *a_mesg);

char        yMODE_enter             (char a_mode);
char        yMODE_exit              (void);
char        yMODE_curr              (void);
char        yMODE_prev              (void);
char        yMODE_not               (char a_mode);

char        yMODE_status            (char *a_list);
char*       yMODE_update            (void);
char*       yMODE_text              (void);
char*       yMODE_message           (void);

char        yMODE_check_prep        (char a_abbr);
char        yMODE_check_needs       (char a_abbr);
char        yMODE_operational       (char a_abbr);
char        yMODE_init_set          (char a_abbr);
char        yMODE_conf_set          (char a_abbr, char a_step);
char        yMODE_statuses          (void *a_file);


#endif
/*============================----end-of-source---============================*/
