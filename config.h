/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/XF86keysym.h>
static const unsigned int borderpx = 1; /* border pixel of windows */
static const int user_bh = 27; /* 0 means that dwm will calculate bar height, >=
                                  1 means dwm will user_bh as bar height */
static const int swallowfloating =
    0; /* 1 means swallow floating windows by default */
static const unsigned int snap = 1;   /* snap pixel */
static const unsigned int gappih = 6; /* horiz inner gap between windows */
static const unsigned int gappiv = 6; /* vert inner gap between windows */
static const unsigned int gappoh =
    6; /* horiz outer gap between windows and screen edge */
static const unsigned int gappov =
    6; /* vert outer gap between windows and screen edge */
static const char buttonbar[] = " ";
static int smartgaps =
    0; /* 1 means no outer gap when there is only one window */
static const int showbar = 1;     /* 0 means no bar */
static const int topbar = 1;      /* 0 means bottom bar */
static const int horizpadbar = 3; /* horizontal padding for statusbar */
static const int vertpadbar = 10; /* vertical padding for statusbar */
static const int vertpad = 0;     /* vertical padding of bar */
static const int sidepad = 0;     /* horizontal padding of bar */
static const unsigned int systraypinning =
    0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor
          X */
static const unsigned int systrayonleft =
    0; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst =
    1; /* 1: if pinning fails, display systray on the first monitor, False:
          display systray on the last monitor*/
static const unsigned int systrayiconsize = 20; /* systray icon size in px */
static const int showsystray = 1;               /* 0 means no systray */
static const char *fonts[] = {
    "Iosevka Nerd Font:size=12:style=Regular",
    "Noto Color Emoji:size=12:antialias=true:autohint=true",
    "Material Design Icons Desktop:size=11"};
static const char dmenufont[] = "Iosevka Nerd Font:size=12";
static const char col_back[] = "#121111";
static const char col_gray1[] = "#212126";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#dbdfdf";
static const char col_blue[] = "#808fbe";
static const char col_orange[] = "#eaac79";
static const char col_red[] = "#c15a5e";
static const char col_green[] = "#8fa176";
static const char col_cyan[] = "#8cb5af";
static const char col_yellow[] = "#d8b170";
static const char col_magenta[] = "#b183ba";

static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_back, col_gray2},
    [SchemeBtn] = {col_blue, col_gray1, col_gray2},
    [SchemeLt] = {col_gray4, col_back, col_gray2},
    [SchemeSel] = {col_gray4, col_blue, col_blue},
};
static const char *tagsel[][2] = {
    {col_green, col_back}, {col_red, col_back}, {col_yellow, col_back},
    {col_blue, col_back}, {col_magenta, col_back}, {col_cyan, col_back},
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x28", NULL};
const char *spcmd2[] = {"st", "-n", "spmpd", "-e", "ncmpcpp", NULL};
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm", spcmd1},
    {"spmpd", spcmd2},
};

static const StatusCmd statuscmds[] = {
    {"~/.local/bin/statusbar/power", 1},
    {"~/.local/bin/statusbar/wifi", 2},
    {"~/.local/bin/statusbar/calendar", 3},
    {"~/.local/bin/statusbar/battery", 4},
    {"~/.local/bin/statusbar/sound", 5},
    {"~/.local/bin/statusbar/mindash", 6},
    {"~/.local/bin/statusbar/mincalendar", 7},
    {"~/.local/bin/statusbar/notifications", 8},
};

static const char *statuscmd[] = {"/bin/bash", "-c", NULL, NULL};

/* tagging */
static char *tags[] = {"cmd", "www", "dev", "chat", "sys", "med"};
static char *alttags[] = {"[cmd]",  "[www]", "[dev]",
                          "[chat]", "[sys]", "[med]"};
static const unsigned int ulinepad =
    2; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke =
    2; /* thickness / height of the underline */
static const unsigned int ulinevoffset =
    0; /* how far above the bottom of the bar the line should appear */
static const int ulineall =
    0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal
       noswallow  monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"Firefox", NULL, NULL, 1 << 8, 0, 0, -1, -1},
    {"st-256color", NULL, NULL, 0, 0, 1, 0, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1}, /* xev */
    {NULL, "spterm", NULL, SPTAG(0), 1, 1, 0, -1},
    {NULL, "spmpd", NULL, SPTAG(1), 1, 1, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    1; /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen =
    1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT                                                           \
  1 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]", tile}, /* first entry is default */
    {"//", NULL}, /* no layout function means floating behavior */
    {"[@]", spiral},  {"[\\]", dwindle},
    {"[M]", monocle}, {"|M|", centeredmaster},
    {NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "/bin/sh", "-c", cmd, NULL }                       \
  }

/* commands */
static const char *btncmd[] = {"rofi", "-show", "drun", NULL};
static const Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_d, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_h, setcfact, {.f = +0.25}},
    {MODKEY | ShiftMask, XK_l, setcfact, {.f = -0.25}},
    {MODKEY | ShiftMask, XK_o, setcfact, {.f = 0.00}},
    {MODKEY, XK_Return, zoom, {0}},
    {MODKEY | Mod4Mask, XK_u, incrgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_u, incrgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_i, incrigaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_i, incrigaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_o, incrogaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_o, incrogaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_6, incrihgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_6, incrihgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_7, incrivgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_7, incrivgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_8, incrohgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_8, incrohgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_9, incrovgaps, {.i = +1}},
    {MODKEY | Mod4Mask | ShiftMask, XK_9, incrovgaps, {.i = -1}},
    {MODKEY | Mod4Mask, XK_0, togglegaps, {0}},
    {MODKEY | Mod4Mask | ShiftMask, XK_0, defaultgaps, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY | ShiftMask, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY | ControlMask, XK_comma, cyclelayout, {.i = -1}},
    {MODKEY | ControlMask, XK_period, cyclelayout, {.i = +1}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY | ShiftMask, XK_f, togglefullscr, {0}},
    {MODKEY, XK_0, view, {.ui = ~0}},
    {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_b, togglescratch, {.ui = 0}},
    {MODKEY | ShiftMask, XK_v, togglescratch, {.ui = 1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5)
            TAGKEYS(XK_7, 6){MODKEY | ShiftMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function argument */
    {ClkButton, 0, Button1, spawn, {.v = btncmd}},
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkStatusText, 0, Button1, spawn, {.v = statuscmd}},
    {ClkStatusText, 0, Button2, spawn, {.v = statuscmd}},
    {ClkStatusText, 0, Button3, spawn, {.v = statuscmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    {ClkTagBar, 0, Button1, view, {0}},
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
