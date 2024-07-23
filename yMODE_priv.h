/*============================----beg-of-source---============================*/
#ifndef yMODE_priv
#define yMODE_priv yes


/*===[[ HEADER BEG ]]=========================================================*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*········· ··········· ´·····························´········································*/
#define     P_NAME      "yMODE"
/*········· ··········· ´·····························´········································*/
#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "us (user control)"
#define     P_SUBJECT   "mode tracking and control"
#define     P_PURPOSE   "coordinate the vikeys model interface"
/*········· ··········· ´·····························´········································*/
#define     P_NAMESAKE  "zeus-xenia (guest-friend)"
#define     P_PRONOUNCE "zoos zeen·eea"
#define     P_HERITAGE  "xenia is the ancient greek customary treatment of guests"
#define     P_BRIEFLY   "customs for guest-friends"
#define     P_IMAGERY   "zeus appears as a ordinary traveller to test a household"
#define     P_REASON    "yMODE is the coordination and treatment of library guests"
/*········· ··········· ´·····························´········································*/
#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT
/*········· ··········· ´·····························´········································*/
#define     P_HOMEDIR   "/home/system/yMODE.vi_mode_tracking"
#define     P_BASENAME  "yMODE"
#define     P_FULLNAME  "/usr/local/lib64/libyMODE"
#define     P_SUFFIX    "···"
#define     P_CONTENT   "···"
/*········· ··········· ´·····························´········································*/
#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       ()"
#define     P_COMPILER  "gcc 11.3.0"
/*········· ··········· ´·····························´········································*/
#define     P_DEPSTDC   "stdio,stdlib,string"
#define     P_DEPEXTEND "···"
#define     P_DEPCORE   "yURG,yLOG,ySTR"
#define     P_DEPVIKEYS "yVIHUB"
#define     P_DEPOTHER  "···"
#define     P_DEPSOLO   "···"
/*········· ··········· ´·····························´········································*/
#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2016-11"
/*········· ··········· ´·····························´········································*/
#define     P_VERMAJOR  "2.--, clean, improve, and expand"
#define     P_VERMINOR  "2.1-, converted to SSH access and continue"
#define     P_VERNUM    "2.1m"
#define     P_VERTXT    "update for new DEBUG_ macro calls"
/*········· ··········· ´·····························´········································*/
#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"
/*········· ··········· ´·····························´········································*/
/*--------- 12345678901 ´123456789-123456789-123456789-123456789-123456789-123456789-123456789-*/
/*                      ´·········1·········2·········3·········4·········5·········6·········7*/
/*===[[ HEADER END ]]=========================================================*/



/*---(ansi-c standard)-------------------*/
#include    <stdio.h>             /* clibc  standard input/output             */
#include    <stdlib.h>            /* clibc  standard general purpose          */
#include    <string.h>            /* clibc  standard string handling          */
/*---(custom core)-----------------------*/
#include    <yURG.h>              /* heatherly urgent processing              */
#include    <yLOG.h>              /* heatherly program logging                */
#include    <ySTR.h>              /* heatherly string processing              */
/*---(custom yvikeys)--------------------*/
#include    <yVIHUB.h>            /* yvikeys coordintation                    */
/*---(done)------------------------------*/




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
   cchar       desc        [LEN_HUND]; /* description of mode                 */
   cchar       mesg        [LEN_HUND];;/* informative message for display     */
   cchar       who         [LEN_LABEL]; /* which library provides service     */
   /*---(done)--------------*/
};
extern const tMODE_INFO  g_modes   [MAX_MODES];
extern int         g_nmode;
extern char        g_majors  [MAX_MODES];

extern char      (*g_handler [MAX_MODES]) (uchar a_major, uchar a_minor);
extern char      (*g_prepper [MAX_MODES]) (uchar a_major, uchar a_minor);
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

typedef    struct    cMY    tMY;
struct cMY {
   char        h_major;
   char      (*e_format)   (uchar a_type, uchar a_how, ushort u, ushort x, ushort y, ushort z);
   char      (*e_object)   (uchar a_type, uchar a_how, ushort u, ushort x, ushort y, ushort z);
   char      (*e_palette)  (uchar a_major, uchar a_minor);
};
extern tMY         myMODE;


#define     MAX_STACK   50
extern char    g_mode_stack    [MAX_STACK]; /* vi-like mode stack             */
extern int     g_mode_depth;                /* depth of current mode stack    */
extern char    g_mode_curr;                 /* current mode in stack          */
extern char    g_mode_exited;               /* last exited mode               */
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
char        yMODE_handle            (uchar a_key);
char        yMODE_hander_setup      (void);


/*===[[ yMACRO_control.c ]]===================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ymode__enter            (char a_force, char a_mode);
char        yMODE_enter             (char a_mode);
char        ymode_force             (char a_mode);
char        yMODE_exit              (void);
char        yMODE_curr              (void);
char        yMODE_prev              (void);
char        yMODE_not               (char a_mode);
char        yMODE_exited            (void);


/*===[[ yMACRO_rptg.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        ymode_update            (void);
char*       yMODE_text              (void);
char*       yMODE_message           (void);
char        yMODE_status            (char a_size, short a_wide, char a_list [LEN_FULL]);


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
char        yMODE_init_set          (char a_abbr, void *a_prepper, void *a_handler);
char        yMODE_conf_set          (char a_abbr, char a_step);
char        ymode_status_purge      (void);
char        ymode_status_init       (void);
/*> char        yMODE_statuses          (FILE *a_file);                               <*/

char        ymode_format_xmode      (uchar a_major, uchar a_minor);
char        ymode_units_xmode       (uchar a_major, uchar a_minor);
char        ymode_object_xmode      (uchar a_major, uchar a_minor);
char        ymode_palette_xmode     (uchar a_major, uchar a_minor);

char        ymode__cust_by_unit     (char a_abbr);
char        ymode__cust_by_format   (char a_abbr);



/*===[[ yMODE_test.c ]]=======================================================*/
/*---(program)--------------*/
char        ymode_unit__quiet       (void);
char        ymode_unit__loud        (void);
char        ymode_unit__end         (void);
/*---(handlers)-------------*/
char        yMODE_unit_reset        (void);
char        ymode_unit__log         (uchar a_mode, uchar a_key);
char        ymode_unit__stub        (uchar a_major, uchar a_minor);
char        ymode_unit__map         (uchar a_major, uchar a_minor);
char        ymode_unit__source      (uchar a_major, uchar a_minor);
char        ymode_unit__input       (uchar a_major, uchar a_minor);
char        ymode_unit__macro       (uchar a_major, uchar a_minor);
char        ymode_unit__prepare     (void);
char        ymode_unit__command     (uchar a_major, uchar a_minor);
char        yMODE_unit_handlers     (void);
/*---(accessor)-------------*/
char*       yMODE_unit              (char *a_question, int n);
/*---(done)-----------------*/


/*===[[ yMODE_libs.c ]]=======================================================*/
/*---(yKEYS)----------------*/
/*> char        yMODE_from_yKEYS        (void *f_init, void *f_quit, void *f_warning, void *f_check_rpt, void *f_repeat_umode, void *f_group_hmode, void *f_rpt_check);   <*/
/*> char        ymode_yKEYS_init        (void);                                       <*/
/*> char        ymode_yKEYS_quit        (void);                                       <*/
/*> char        ymode_yKEYS_warning     (void);                                       <*/
/*> char        ymode_yKEYS_check_rpt   (void);                                       <*/
/*> char        ymode_yKEYS_rpt_umode   (uchar a_major, uchar a_minor);               <*/
/*> char        ymode_yKEYS_grp_hmode   (uchar a_major, uchar a_minor);               <*/
/*> char        ymode_yKEYS_rpt_check   (uchar a_major, uchar a_minor, char a, char b, char c);   <*/
/*---(yMACRO)---------------*/
/*> char        yMODE_from_yMACRO       (void *f_hmode);                              <*/
/*> char        ymode_yMACRO_hmode      (uchar a_major, uchar a_minor);               <*/
/*---(yVIEW)----------------*/
/*> char        yMODE_from_yVIEW        (void *f_keys, void *f_modes);                <*/
/*> char        ymode_yVIEW_keys        (char *a_text);                               <*/
/*> char        ymode_yVIEW_modes       (char *a_text);                               <*/
/*---(done)-----------------*/



#endif

