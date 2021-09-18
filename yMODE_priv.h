/*============================----beg-of-source---============================*/
#ifndef yMODE_priv
#define yMODE_priv yes



/*===[[ BEG_HEADER ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "us (user control)"
#define     P_SUBJECT   "mode tracking and control
#define     P_PURPOSE   ""

#define     P_NAMESAKE  "zeus-xenia (guest-friend)"
#define     P_HERITAGE  ""
#define     P_IMAGERY   ""
#define     P_REASON    ""

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  "yMODE"
#define     P_FULLNAME  "/usr/local/lib64/libyMODE"
#define     P_SUFFIX    ""
#define     P_CONTENT   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
#define     P_DEPENDS   "none"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   ""

#define     P_VERMAJOR  "2.--, clean, improve, and expand"
#define     P_VERMINOR  "2.0-, separated into independent library"
#define     P_VERNUM    "2.0g"
#define     P_VERTXT    "decent start at the man (3) file"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*===[[ END_HEADER ]]=========================================================*/

/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom)----------------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */




/*===[[ STRUCTURE ]]==========================================================*/
#define     MAX_MODES   40
typedef  struct  cMODE_INFO  tMODE_INFO;
struct cMODE_INFO {
   /*---(desc)--------------*/
   cchar       abbr;                   /* single character abbreviation       */
   cchar       type;                   /* type of mode, major, sub, micro, .. */
   cchar       show;                   /* show a message line (y/n)           */
   cchar       three       [LEN_SHORT];/* very short name                     */
   cchar       terse       [LEN_TERSE];/* short name                          */
   cchar       cat;                    /* category for reporting              */
   /*---(status)------------*/
   cchar       expect      [LEN_DESC]; /* expected prep and setup             */
   /*---(message)-----------*/
   cchar       desc        [LEN_DESC]; /* description of mode                 */
   cchar       mesg        [LEN_HUND];;/* informative message for display     */
   /*---(done)--------------*/
};
extern const tMODE_INFO  g_modes   [MAX_MODES];
extern int         g_nmode;
extern char        g_majors  [MAX_MODES];

extern char       *g_allow   [MAX_MODES];
extern char        g_actual  [MAX_MODES] [LEN_DESC];
extern char       *g_mesg    [MAX_MODES];

/*    MODE_    major mode, used for most working time
 *    SMOD_    sub-mode, supports major mode(s), used for short-period
 *    UMOD_    micro-mode, very simple supporting capability, used very quickly
 *    XMOD_    external-mode, custom to applicaton supported
 *    FMOD_    fundamental, not a usable mode, but a building block
 */
#define       MODE_FUND     'F'
#define       MODE_MAJOR    'M'
#define       MODE_SUB      's'
#define       MODE_MICRO    'u'
#define       MODE_EXTERN   'x'
#define       MODE_NOT      '-'


#define     MAX_STACK   50
extern char    g_mode_stack    [MAX_STACK]; /* vi-like mode stack             */
extern int     g_mode_depth;                /* depth of current mode stack    */
extern char    g_mode_curr;                 /* current mode in stack          */
extern char    g_message       [LEN_RECD];
extern char    g_last;
extern char    g_text          [LEN_TERSE];


/*===[[ yMACRO_allow.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ymode_allow_purge       (void);
char        ymode_mesg_purge        (void);
char        ymode_allow_load        (void);
char        ymode_allow_single      (char a_mode);
char        yMODE_get_allow         (char a_abbr, char *a_allow);
char        yMODE_set_allow         (char a_abbr, char *a_allow);
char        yMODE_get_message       (char a_abbr, char *a_mesg);
char        yMODE_set_message       (char a_abbr, char *a_mesg);


/*===[[ yMACRO_base.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char*       yMODE_version           (void);
char        yMODE_init              (char a_mode);
char        yMODE_wrap              (void);
char        ymode__unit_quiet       (void);
char        ymode__unit_loud        (void);
char        ymode__unit_end         (void);
char*       ymode__unit             (char *a_question, int n);


/*===[[ yMACRO_control.c ]]===================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        yMODE_enter             (char a_mode);
char        yMODE_exit              (void);
char        yMODE_curr              (void);
char        yMODE_prev              (void);
char        yMODE_not               (char a_mode);


/*===[[ yMACRO_rptg.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ymode_update            (void);
char*       yMODE_text              (void);
char*       yMODE_message           (void);
char        yMODE_status            (char *a_list);


/*===[[ yMACRO_status.c ]]====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ymode__check            (char a_abbr, char a_target);
char        yMODE_check_prep        (char a_abbr);
char        yMODE_check_needs       (char a_abbr);
char        yMODE_operational       (char a_abbr);
char        ymode__filling          (char a_abbr, char a_target);
char        ymode__prep_fill        (char a_abbr);
char        ymode__need_fill        (char a_abbr);
char        ymode__deps_fill        (char a_abbr);
char        ymode__conf_verify      (void);
char        ymode__updating         (char a_target);
char        ymode__prep_checkall    (void);
char        ymode__need_checkall    (void);
char        ymode__deps_checkall    (void);
char        yMODE_init_set          (char a_abbr);
char        yMODE_conf_set          (char a_abbr, char a_step);
char        ymode_status_purge      (void);
char        ymode_status_init       (void);
/*> char        yMODE_statuses          (FILE *a_file);                               <*/


#endif

