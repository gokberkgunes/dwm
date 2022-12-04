static const unsigned int borderpx = 2;  // border pixel of windows
static       unsigned int gappx    = 12; // gap pixel between windows
static const unsigned int snap     = 0;  // snap pixel for floating windows
static const int swallowfloating   = 1;  // 1 means swallow floating windows
static const int showbar           = 1;  // 0 means no bar
static const int topbar            = 1;  // 0: bottom bar
static const char nor_bg_clr[]     = "#121212"; // bar color
static const char nor_fg_clr[]     = "#c5ebed";
static const char nor_edge_clr[]   = "#121212"; // non-selected edge
static const char sel_bg_clr[]     =  ENVCOLOR; // selected tab background
static const char sel_fg_clr[]     = "#929292";
static const char sel_edge_clr[]   =  ENVCOLOR;
static const char *colors[][3]     = {
	/*               fg          bg          edge */
	[SchemeNorm] = { nor_fg_clr, nor_bg_clr, nor_edge_clr },
	[SchemeSel]  = { sel_fg_clr, sel_bg_clr, sel_edge_clr },
};
static const char *fonts[]   = { "Gohu GohuFont:pixelsize=11",
                                 "Symbols Nerd Font:pixelsize=12" };


/* SCRATCHPAD DEFINITIONS */
#define sp_sh(cmd) .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL }
#define sp_s1(cmd1) .v = (const char*[]){ cmd1, NULL }

typedef struct {
	const char* class;
	const void* v;
} Sp;

const Sp spsignal = { .class = "Signal", sp_s1("signal-desktop") };
const Sp spkpass  = { .class = "KeePassXC", sp_s1("keepassxc") };
const Sp spst     = { .class = "spterm", sp_sh("st -t '' -c spterm") };
const Sp sptabbed = { .class = "tabbed" };
const Sp spsteam  = { .class = "Steam", sp_s1("steam") };
const Sp spmpv    = { .class = "mpv" };
const Sp spftube  = { .class = "FreeTube", sp_s1("freetube") };
const Sp spcalc   = { .class = "spcalc", sp_sh("st -c spcalc bc -lq") };
const Sp spmusc   = { .class = "spmusc", sp_sh("st -c spmusc ncmpcpp") };

static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7" };

static const Rule rules[] = {
	/* xprop(1):
	 *      WM_CLASS(STRING) = instance, class
	 *      WM_NAME(STRING) = title
	 *      syntax errors cause misbehavior, space after name etc.
	 */
	/* class              tag     flt ter nosw mon, flt-xywh  */
	//{"Gimp",       0, 0, 010000, 0,   0,   0, -1,                      },
	//{"xournalpp",  0, 0, 0,      1,   0,   0, -1,                      },
	{ "firefox",     0, 0, 1 << 2, 0,  0,  1,  -1,                       },
	{ "Steam",       0, 0, 0,      1,  0,  0,   0, 0,0,500,500           },
	{ "steam_app",   0, 0, 1 << 5, 0,  0,  0,   0,                       },
	{ "mpv",         0, 0, 0,      0,  0,  1,  -1,                       },
	{ "matplotlib",  0, 0, 0,      1,  0,  1,  -1,                       },
	{ "Slicer",      0, 0, 1 << 4, 1,  0,  1,  -1,                       },
	{ "FreeTube",    0, 0, 0,      1,  0,  1,  -1, 0,0,1000,1000         },
	{ "Signal",      0, 0, 0,      1,  0,  1,  -1, 0,0,800,800           },
	{ "KeePassXC",   0, 0, 0,      1,  1,  0,  -1, -1,-1,700,800         },
	{ "chatterino",  0, 0, 0,      0,  0,  0,  -1,                       },
	{ "spterm",      0, 0, 0,      1,  0,  0,  -1, 0,0,500,500           },
	{ "spcalc",      0, 0, 0,      1,  1,  0,  -1, 0,0,300,300           },
	{ "spmusc",      0, 0, 0,      1,  1,  0,  -1, 0,0,500,500           },
	{ "zoom",        0, 0, 0,      1,  0,  0,  -1,                       },
	{ "Inkscape",    0, 0, 0,      0,  0,  0,  -1,                       },

	/*title         tag mask float term  noswal mon */
	{ 0,     0, "Event Tester", 0,  0,   0,    1,    -1 },

	/* st is declared here cause tabbed takeovers the class */
	/*                 tag mask float term  noswal mon */
	{ 0, 0, "st",      000000,  0,    1,    0,     -1 },


//	     instance tag mask float  term swal  mon
//	{ 0, instance, 0,      000000,   0,     1,   0,    -1},
};

// layout(s)
static const float mfact     = 0.45; // factor of master area size [0.05..0.95]
static const int nmaster     = 1;    // number of clients in master area
static const int resizehints = 0;    // respect size hints in tiled resizals
static const int lockfullscreen = 1; // force focus on the fullscreen window

#include "layouts.c"
static const Layout layouts[] = {
	// symbol     arrange function
	{ "[]#", mastergrid  }, /* first entry is default */
	{ "###", gapfulgrid },
	{ "[M]", monocle     },
};

// key definitions
#define MOD Mod4Mask
#define SHIFT ShiftMask
#define CTRL ControlMask
#define ALT Mod1Mask

#define TAGKEYS(KEY,TAG) \
	{ MOD,       KEY, view,       {.ui = 1 << TAG} }, \
	{ MOD|SHIFT, KEY, tag,        {.ui = 1 << TAG} }, \
	{ MOD|CTRL,  KEY, toggleview, {.ui = 1 << TAG} }, \
	{ MOD|ALT,   KEY, toggletag,  {.ui = 1 << TAG} },

/* Helper function for spawning shell commands in the pre dwm-5.0 fashion
 * Usage: Pass a command as string into SH function, e.g. SH("echo "hi").
 * Avoid using this for spawning stuff since there is no need to call dash for
 * running programs like firefox, chromium and gimp.
 */
// #define SH(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define s1(cmd1)             { cmd1, NULL }
#define s2(cmd1, cmd2)       { cmd1, cmd2, NULL }
#define s3(cmd1, cmd2, cmd3) { cmd1, cmd2, cmd3, NULL }
#define sh(cmd)              { "/bin/sh", "-c", cmd, NULL }

/* Self declared commands, avoiding sh command is the best method for
 * spawning programs because it calls kernel directly.
 */
static char dmenumon[2] = "0"; // component of dmenucmd, manipulated in spawn()
static const char *dmenucmd[]   = s3("dmenu_run", "-m", dmenumon);
static const char *st[]         = s3("tabbed", "st", "-w");
static const char *dmenu[]      = s1("dmenu_run");
static const char *firefox[]    = s1("firefox");
static const char *chromium[]   = s1("chromium");
static const char *studio_sc[]  = s1("studio-sc");
static const char *sinkswap[]   = s1("sinkswapper");
static const char *alsatog[]    = s2("volume-bar", "tog");
static const char *alsadec[]    = s2("volume-bar", "dec");
static const char *alsainc[]    = s2("volume-bar", "inc");
static const char *mpcprev[]    = s2("mpc", "prev");
static const char *mpcnext[]    = s2("mpc", "next");
static const char *mpcdec[]     = s3("mpc", "volume", "-10");
static const char *mpcinc[]     = s3("mpc", "volume", "+10");
static const char *mpctog[]     = s2("mpc", "toggle");
static const char *screenshot[] = s1("screenshotter");
static const char *startrec[]   = s1("dmenurecord");
static const char *stoprec[]    = s2("dmenurecord", "kill");
static const char *powerman[]   = s1("sysact");
static const char *mpvvid[]     = sh("mpv $(xclip -o)");
// make a function to save pid, and when pressed again kill it since
//static const char *mpvsnd[]   = sh("mpv --vid=no $(xclip -o)");
// runs without a video... (if you run without st -e)
// This is temporary fix, empty terminal screen:
static const char *mpvaud[]     = sh("st -e mpv --vid=no $(xclip -o)");
static const char *glossinc[]   = { "/bin/sh", "-c", "dimness-bar +", NULL};
static const char *glossdec[]   = { "/bin/sh", "-c", "dimness-bar -", NULL};

/* KEYBINDINGS
 * Instead of using #include <X11/XF86keysym.h> preferring hex codes which
 * may be found /usr/include/X11/XF86keysym.h
 */
#define XK_AudioMute    0x1008FF12 // Mute sound from the system
#define XK_AudioLow     0x1008FF11 // Volume control down
#define XK_AudioInc     0x1008FF13 // Volume control up
#define XK_lbracket     0x005b     // U+005B LEFT SQUARE BRACKET
#define XK_backslash    0x005c     // U+005C REVERSE SOLIDUS
#define XK_rbracket     0x005d     // U+005D RIGHT SQUARE BRACKET


static const Key keys[] = {
	TAGKEYS( XK_1, 0)
	TAGKEYS( XK_2, 1)
	TAGKEYS( XK_3, 2)
	TAGKEYS( XK_4, 3)
	TAGKEYS( XK_5, 4)
	TAGKEYS( XK_6, 5)
	TAGKEYS( XK_7, 6)

/* Note that if keys are already mapped, xev shall show them unrecognizable.*/
//	  modifier      key             function        argument

//	{ MOD,          XK_Escape,      ,               {}                   },
	{ 0,            XK_Print,       spawn,          {.v = screenshot}    },
//	{ ShiftMask,    XK_Print,       spawn,          {.v = }              },
	{ MOD,          XK_Print,       spawn,          {.v = startrec}      },
//	{ 0,            XK_Scroll_Lock, spawn,          {.v = }              },
//	{ 0,            XK_Scroll_Lock, spawn,          {.v = }              },
	{ MOD,          XK_Delete,      spawn,          {.v = stoprec}       },
	{ MOD,          XK_Home,        spawn,          {.v = glossinc}      },
	{ MOD,          XK_End,         spawn,          {.v = glossdec}      },
	{ MOD,          XK_AudioMute,   spawn,          {.v = sinkswap}      },
	{ 0,            XK_AudioMute,   spawn,          {.v = alsatog}       },
	{ 0,            XK_AudioInc,    spawn,          {.v = alsainc}       },
	{ 0,            XK_AudioLow,    spawn,          {.v = alsadec}       },
	{ MOD,          XK_F1,          spawn,          {.v = studio_sc}     },
//	{ MOD,          XK_F2,          ,               {}                   },
//	{ MOD,          XK_F3,          ,               {}                   },
//	{ MOD,          XK_F4,          ,               {}                   },
//	{ MOD,          XK_F5,          ,               {}                   },
//	{ MOD,          XK_F6,          ,               {}                   },
//	{ MOD,          XK_F7,          ,               {}                   },
//	{ MOD,          XK_F8,          ,               {}                   },
//	{ MOD,          XK_F9,          ,               {}                   },
//	{ MOD,          XK_F10,         ,               {}                   },
//	{ MOD,          XK_F11,         ,               {}                   },
//	{ MOD,          XK_F12,         ,               {}                   },
//	{ MOD,          XK_grave,       ,               {}                   },
	{ MOD,          XK_q,           killclient,     {0}                  },
	{ MOD|SHIFT,    XK_q,           quit,           {0}                  },
	{ MOD|ALT,      XK_q,           quit,           {1}                  },
	{ MOD,          XK_w,           spawn,          {.v = firefox}       },
	{ MOD|ALT,      XK_w,           spawn,          {.v = chromium}      },
	{ MOD,          XK_e,           togglescratch,  {.v = &spsignal}     },
	{ MOD|ALT,      XK_e,           togglescratch,  {.v = &sptabbed}     },
	{ MOD,          XK_r,           togglescratch,  {.v = &spftube}      },
	{ MOD|SHIFT,    XK_r,           spawn,          {.v = mpvvid}        },
	{ MOD|ALT,      XK_r,           spawn,          {.v = mpvaud}        },
//	{ MOD|SHIFT,    XK_r,           spawn,          {.v = htop}          },
	{ MOD,          XK_t,           togglescratch,  {.v = &spmpv}        },
	{ MOD|ALT,      XK_t,           togglescratch,  {.v = &spsteam}      },
	{ MOD,          XK_y,           togglescratch,  {.v = &spst}         },
//	{ MOD|SHIFT,    XK_y,           ,               {}                   },
//	{ MOD,          XK_u,           togglescratch,  {.v = &splf}         },
//	{ MOD|SHIFT,    XK_u,           ,               {}                   },
	{ MOD,          XK_i,           togglescratch,  {.v = &spkpass}      },
	{ MOD|SHIFT,    XK_i,           togglescratch,  {.v = &spcalc}       },
	{ MOD,          XK_o,           incnmaster,     {.i = +1}            },
	{ MOD|SHIFT,    XK_o,           incnmaster,     {.i = -1}            },
	{ MOD,          XK_p,           spawn,          {.v = mpctog}        },
//	{ MOD|SHIFT,    XK_p,           spawn,          {0}                  },
	{ MOD,          XK_lbracket,    spawn,          {.v = mpcprev}       },
//	{ MOD|SHIFT,    XK_lbracket,    spawn,          {0}                  },
	{ MOD,          XK_rbracket,    spawn,          {.v = mpcnext}       },
//	{ MOD|SHIFT,    XK_rbracker,    spawn,          {0}                  },
//	{ MOD,          XK_backslash,   spawn,          {0}                  },
//	{ MOD|SHIFT,    XK_backslash,   spawn,          {0}                  },
//	{ MOD,          XK_a,           ,               {}                   },
//	{ MOD|SHIFT,    XK_a,           ,               {}                   },
	{ MOD,          XK_s,           togglesticky,   {0}                  },
//	{ MOD|SHIFT,    XK_s,           ,               {}                   },
	{ MOD,          XK_d,           spawn,          {.v = dmenu}         },
//	{ MOD|SHIFT,    XK_d,           ,               {}                   },
	{ MOD,          XK_f,           fullscreen,     {0}                  },
//	{ MOD|SHIFT,    XK_f,           cyclelayout,    {NULL}               },
//	{ MOD,          XK_g,           ,               {}                   },
//	{ MOD|SHIFT,    XK_g,           ,               {}                   },
	{ MOD,          XK_h,           setmfact,       {.f = -0.05}         },
//	{ MOD|SHIFT,    XK_h,           ,               {}                   },
	{ MOD|ALT,      XK_h,           focusmon,       {.i = +1}            },
	{ MOD,          XK_j,           focusstack,     {.i = +1}            },
//	{ MOD|SHIFT,    XK_j,           ,               {}                   },
	{ MOD|ALT,      XK_j,           tagmon,         {.i = +1}            },
	{ MOD,          XK_k,           focusstack,     {.i = -1}            },
//	{ MOD|SHIFT,    XK_k,           ,               {}                   },
//	{ MOD|ALT,      XK_j,           tagmon,         {.i + -1}            },
	{ MOD,          XK_l,           setmfact,       {.f = +0.05}         },
//	{ MOD|SHIFT,    XK_l,           ,               {}                   },
//	{ MOD|ALT,      XK_l,           focusmon,       {.i = -1}            },
//	{ MOD,          XK_semicolon,   ,               {}                   },
//	{ MOD|SHIFT,    XK_semicolon,   ,               {}                   },
//	{ MOD,          XK_apostrophe,  ,               {}                   },
//	{ MOD|SHIFT,    XK_apostrophe,  ,               {}                   },
//	{ MOD,          XK_z,           ,               {}                   },
//	{ MOD,          XK_x,           ,               {}                   },
	{ MOD,          XK_c,           cyclelayout,    {0}               },
//	{ MOD,          XK_v,           ,               {}                   },
	{ MOD,          XK_b,           togglebar,      {0}                  },
//	{ MOD,          XK_n,           ,               {}                   },
//	{ MOD|SHIFT,    XK_m,           cyclelayout,    {0}               },
	{ MOD,          XK_m,           togglescratch,  {.v = &spmusc}       },
//	{ MOD,          XK_comma,       focusmon,       {.i = -1}            },
//	{ MOD|SHIFT,    XK_comma,       tagmon,         {.i = -1}            },
	{ MOD,          XK_minus,       spawn,          {.v = mpcdec}        },
	{ MOD,          XK_Return,      spawn,          {.v = st }           },
//	{ MOD|SHIFT,    XK_Return,      ,               {0}                  },
	{ MOD,          XK_Tab,         view,           {0}                  },
	{ MOD,          XK_space,       zoom,           {0}                  },
	{ MOD|SHIFT,    XK_space,       togglefloating, {0}                  },
	{ MOD,          XK_0,           view,           {.ui = ~0}           },
	{ MOD|SHIFT,    XK_0,           tag,            {.ui = ~0}           },
//	{ MOD,          XK_period,      focusmon,        {.i = +1}           },
//	{ MOD|SHIFT,    XK_period,      tagmon,         {.i = +1}            },
	{ MOD,          XK_BackSpace,   spawn,          {.v = powerman}      },
	{ MOD,          XK_equal,       spawn,          {.v = mpcinc}        },
//	{ MOD,          XK_Left,        ,               {}                   },
//	{ MOD,          XK_Right,       ,               {}                   },
//	{ MOD,          XK_Page_Up,     ,               {}                   },
//	{ MOD,          XK_Page_Down,   ,               {}                   },
//	{ MOD,          XK_Insert,      ,               {}                   },
};

// button definitions
#define STATUSBAR "dwmblocks"
// click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkClientWin, or ClkRootWin
static const Button buttons[] = {
// 	  click          mask   button   function       argument
	{ ClkLtSymbol,   0,     Button1, cyclelayout,   {0}                },
	{ ClkStatusText, 0,     Button1, sigstatusbar,  {.i = 1}              },
	{ ClkStatusText, 0,     Button2, sigstatusbar,  {.i = 2}              },
	{ ClkStatusText, 0,     Button3, sigstatusbar,  {.i = 3}              },
	{ ClkStatusText, 0,     Button4, sigstatusbar,  {.i = 4}              },
	{ ClkStatusText, 0,     Button5, sigstatusbar,  {.i = 5}              },
	{ ClkStatusText, SHIFT, Button1, sigstatusbar,  {.i = 6}              },
	{ ClkClientWin,  MOD,   Button1, movemouse,     {0}                   },
	{ ClkClientWin,  MOD,   Button2, togglefloating,{0}                   },
	{ ClkClientWin,  MOD,   Button3, resizemouse,   {0}                   },
	{ ClkTagBar,     0,     Button1, view,          {0}                   },
	{ ClkTagBar,     0,     Button3, toggleview,    {0}                   },
	{ ClkTagBar,     MOD,   Button1, tag,           {0}                   },
	{ ClkTagBar,     MOD,   Button3, toggletag,     {0}                   },
};
