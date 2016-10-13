#include "khaler.h"

int termcol() {

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_col;
}

int termrow() {

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_row;
}
