#ifndef first
#include "gamepadcon.c"
#endif
static const GP gps[] = {
    /*  type    code    value   keysym  */
    {   3,      1,      0,      XK_F4},
    {   3,      1,      255,    XK_F3},
    {   3,      0,      0,      XK_bracketleft},
    {   3,      0,      255,    XK_bracketright},
    {   1,      288,    1,      XK_s}, // X
    {   1,      289,    1,      XK_Tab}, // A
    {   1,      290,    1,      XK_q}, // B
    {   1,      291,    1,      XK_o}, // Y
    // {   1,      292,    1,      XK_Print}, // L
    // {   1,      293,    1,      XK_backslash}, // R
    {   1,      296,    1,      XK_Print}, // SELECT
    {   1,      297,    1,      XK_backslash}, // START
};

static char gamepad_path[] = "/dev/input/event14";
static char lock_file[] = "/tmp/gpc_lock";
