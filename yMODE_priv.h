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

#define     P_NAMESAKE  ""
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
#define     P_VERNUM    "2.0a"
#define     P_VERTXT    "bare-bones, initial break-out of yVIKEYS code"

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
#define     MAX_MODES   100
typedef  struct  cMODE_INFO  tMODE_INFO;
struct cMODE_INFO {
   /*---(desc)--------------*/
   cchar       abbr;                   /* single character abbreviation       */
   cchar       type;                   /* type of mode, major, sub, micro, .. */
   cchar       show;                   /* show a message line (y/n)           */
   cchar       three       [LEN_SHORT];/* very short name                     */
   cchar       terse       [LEN_TERSE];/* short name                          */
   cchar       cat;                    /* category for reporting              */
   /*---(movement)----------*/
   char        allow       [LEN_DESC]; /* allowed mode transitions            */
   /*---(status)------------*/
   cchar       expect      [LEN_DESC]; /* expected prep and setup             */
   char        actual      [LEN_DESC]; /* actual prep and setup               */
   /*---(message)-----------*/
   cchar       desc        [LEN_DESC]; /* description of mode                 */
   char        mesg        [LEN_HUND];;/* informative message for display     */
   /*---(usage)-------------*/
   int         count;                  /* number of times used                */
   /*---(done)--------------*/
};
extern tMODE_INFO  g_modes [MAX_MODES];


#endif

