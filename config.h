/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_magenta[]     = "#ad69af";
static const char *colors[][3]      = {
  /*               fg         bg         border   */
  [SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
  [SchemeSel]  = { col_gray4, col_cyan,  col_magenta  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4"};

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class            instance    title        tags mask     iscentered     isfloating   monitor */
  { "Gimp",           NULL,       NULL,        0,            0,             1,           -1 },
  { "Firefox",        NULL,       NULL,        1 << 8,       0,             0,           -1 },
  { "Pamac-manager",  NULL,       NULL,        0,            1,             1,           -1 },
  { "Galculator",     NULL,       NULL,        0,            1,             1,           -1 },
  { NULL,             NULL,       "alsamixer", 0,            1,             1,           -1 },
  { "Nitrogen",       NULL,       NULL,        0,            1,             1,           -1 },
  { "Gcolor2",        NULL,       NULL,        0,            1,             1,           -1 },
  { "Oblogout",       NULL,       NULL,        0,            1,             1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "<>",       NULL },    /* no layout function means floating behavior */
  { "[M]",      monocle },
  { "TTT",      bstack },
  { "===",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/* static const char *dmenucmd[]           = { "dmenu_recency", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL }; */
static const char *dmenucmd[]           = { "rofi", "-show", "drun", NULL };
static const char *termcmd[]            = { "kitty", NULL };
static const char *musicplayercmd[]     = { "kitty", "-e", "tmux", "new-session", "-A", "-D", "-s", "music", "sh -c 'cmus'", NULL };
static const char *pomodorocmd[]        = { "kitty", "-e", "tmux", "new-session", "-A", "-D", "-s", "pomodoro", NULL };
static const char *editorcmd[]          = { "kitty", "-e", "nvim",NULL };
static const char *processviewercmd[]   = { "kitty", "-e", "htop", NULL };
static const char *browsercmd[]         = { "firefox", NULL };
static const char *filemanagercmd[]     = { "pcmanfm", NULL };
static const char *calccmd[]            = { "galculator", NULL };
static const char *pkgmanagercmd[]      = { "pamac-manager", NULL };

static const char *cmuspausecmd[]       = { "cmus-remote", "--pause", NULL };
static const char *cmusprevcmd[]        = { "cmus-remote", "--prev", NULL };
static const char *cmusnextcmd[]        = { "cmus-remote", "--next", NULL };
static const char *cmusvolupcmd[]       = { "cmus-remote", "--vol", "+10%", NULL };
static const char *cmusvoldowncmd[]     = { "cmus-remote", "--vol", "-10%", NULL };

static const char *printscreencmd[]     = { "i3-scrot", NULL };
static const char *printswindowcmd[]    = { "i3-scrot", "-w", NULL };
static const char *printsselectioncmd[] = { "sh", "-c", "sleep 0.2; i3-scrot -s", NULL };

static const char *exitcmd[]            = { "sh", "-c", "oblogout -c ~/my-configs/dwm/oblogout.conf", NULL };
static const char *lockcmd[]            = { "blurlock", NULL };

static const char *checkinternetcmd[]   = { "sh", "-c", "node ~/code-stuff/check-internet/index.js /usr/bin/chromium", NULL };

static const char *hidemousecmd[]       = { "xdotool", "mousemove", "1920", "1080", NULL };
static const char *showmousecmd[]       = { "xdotool", "mousemove", "960", "540", NULL };

static const char *mouse_move_up[]      = { "xdotool", "mousemove_relative", "--sync", "--", "0", "-25", NULL };
static const char *mouse_move_down[]    = { "xdotool", "mousemove_relative", "--sync", "--", "0", "25", NULL };
static const char *mouse_move_left[]    = { "xdotool", "mousemove_relative", "--sync", "--", "-25", "0", NULL };
static const char *mouse_move_right[]   = { "xdotool", "mousemove_relative", "--sync", "--", "25", "0", NULL };
static const char *mouse_click_right[]  = { "xdotool", "click", "--clearmodifiers", "3", NULL };
static const char *mouse_click_left[]   = { "xdotool", "click", "--clearmodifiers", "1", NULL };


#include "movestack.c"
static Key keys[] = {
  /* modifier                     key        function        argument */

  // Common applications
  { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
  { MODKEY,                       XK_F2,     spawn,          {.v = browsercmd } },
  { MODKEY,                       XK_F3,     spawn,          {.v = filemanagercmd } },
  { MODKEY,                       XK_F4,     spawn,          {.v = calccmd } },
  { MODKEY,                       XK_F12,    spawn,          {.v = musicplayercmd } },
  { MODKEY,                       XK_o,      spawn,          {.v = editorcmd } },
  { MODKEY,                       XK_i,      spawn,          {.v = pkgmanagercmd } },
  { MODKEY,                       XK_p,      spawn,          {.v = processviewercmd } },
  { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = pomodorocmd } },

  // Check internet connection
  { MODKEY,                       XK_c,      spawn,          {.v = checkinternetcmd } },

  // Run an application
  { MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },

  // Close an application
  { MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

  // Exit dwm
  { MODKEY,                       XK_Escape, spawn,          {.v = exitcmd } },
  { MODKEY,                       XK_9,      spawn,          {.v = lockcmd } },
  { MODKEY|ShiftMask,             XK_Escape, quit,           {0} },

  // Control cmus
  { 0,                            XK_F9,     spawn,          {.v = cmuspausecmd } },
  { 0,                            XK_F10,    spawn,          {.v = cmusprevcmd } },
  { 0,                            XK_F11,    spawn,          {.v = cmusnextcmd } },
  { MODKEY,                       XK_equal,  spawn,          {.v = cmusvolupcmd } },
  { MODKEY,                       XK_minus,  spawn,          {.v = cmusvoldowncmd } },

  // Screenshots
  { 0,                            XK_Print,  spawn,          {.v = printscreencmd } },
  { MODKEY,                       XK_Print,  spawn,          {.v = printswindowcmd } },
  { MODKEY|ShiftMask,             XK_Print,  spawn,          {.v = printsselectioncmd } },

  // Toggle bar
  { MODKEY,                       XK_b,      togglebar,      {0} },

  // Toggle between layouts
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // tile
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, // float
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, // monocle
  { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} }, // bstack
  { MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[4]} }, // bstack (h)
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY|ShiftMask,             XK_m,      zoom,           {0} },

  // Move focus in current tag
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_h,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_l,      focusstack,     {.i = -1 } },

  // Resize windows
  { MODKEY|Mod1Mask,              XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY|Mod1Mask,              XK_l,      setmfact,       {.f = +0.05} },

  // Move windows
  { MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
  { MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },


  { MODKEY,                       XK_x,      spawn,          {.v = hidemousecmd } },
  { MODKEY|ShiftMask,             XK_x,      spawn,          {.v = showmousecmd } },
  { MODKEY,                       XK_Up,     spawn,          {.v = mouse_move_up } },
  { MODKEY,                       XK_Down,   spawn,          {.v = mouse_move_down } },
  { MODKEY,                       XK_Left,   spawn,          {.v = mouse_move_left } },
  { MODKEY,                       XK_Right,  spawn,          {.v = mouse_move_right } },
  { MODKEY|ShiftMask,             XK_Left,   spawn,          {.v = mouse_click_left } },
  { MODKEY|ShiftMask,             XK_Right,  spawn,          {.v = mouse_click_right } },

  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  /* TAGKEYS(                        XK_5,                      4) */
  /* TAGKEYS(                        XK_6,                      5) */
  /* TAGKEYS(                        XK_7,                      6) */
  /* TAGKEYS(                        XK_8,                      7) */
  /* TAGKEYS(                        XK_9,                      8) */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

