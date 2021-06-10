#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>

#define DIE(dpy, ...) {           \
    fprintf(stderr, __VA_ARGS__); \
    if (dpy)                      \
        XCloseDisplay(dpy);       \
    exit(-1);                     \
}

/* This only works for Latin-1.  I'd apologize, but as far as I can tell, it's
 * X that needs to be apologizing.  I mean... look at this. */
int lookup_keycode(Display *dpy, char c, bool *shift) {
    KeySym ks;
    char cs[2];

    if (isupper(c))
        *shift = true;

    if (isalnum(c)) {
        cs[0] = c;
        cs[1] = '\0';
        ks = XStringToKeysym(cs);
    } else if (c == '`') {
        ks = XStringToKeysym("grave");
    } else if (c == '~') {
        ks = XStringToKeysym("asciitilde");
        *shift = true;
    } else if (c == '!') {
        ks = XStringToKeysym("exclam");
        *shift = true;
    } else if (c == '@') {
        ks = XStringToKeysym("at");
        *shift = true;
    } else if (c == '#') {
        ks = XStringToKeysym("numbersign");
        *shift = true;
    } else if (c == '$') {
        ks = XStringToKeysym("dollar");
        *shift = true;
    } else if (c == '%') {
        ks = XStringToKeysym("percent");
        *shift = true;
    } else if (c == '^') {
        ks = XStringToKeysym("asciicircum");
        *shift = true;
    } else if (c == '&') {
        ks = XStringToKeysym("ampersand");
        *shift = true;
    } else if (c == '*') {
        ks = XStringToKeysym("asterisk");
        *shift = true;
    } else if (c == '(') {
        ks = XStringToKeysym("parenleft");
        *shift = true;
    } else if (c == ')') {
        ks = XStringToKeysym("parenright");
        *shift = true;
    } else if (c == '-') {
        ks = XStringToKeysym("minus");
    } else if (c == '_') {
        ks = XStringToKeysym("underscore");
        *shift = true;
    } else if (c == '=') {
        ks = XStringToKeysym("equal");
    } else if (c == '+') {
        ks = XStringToKeysym("plus");
        *shift = true;
    } else if (c == '[') {
        ks = XStringToKeysym("bracketleft");
    } else if (c == '{') {
        ks = XStringToKeysym("braceleft");
        *shift = true;
    } else if (c == ']') {
        ks = XStringToKeysym("bracketright");
    } else if (c == '}') {
        ks = XStringToKeysym("braceright");
        *shift = true;
    } else if (c == '\\') {
        ks = XStringToKeysym("backslash");
    } else if (c == '|') {
        ks = XStringToKeysym("bar");
        *shift = true;
    } else if (c == ';') {
        ks = XStringToKeysym("semicolon");
    } else if (c == ':') {
        ks = XStringToKeysym("colon");
        *shift = true;
    } else if (c == '\'') {
        ks = XStringToKeysym("apostrophe");
    } else if (c == '"') {
        ks = XStringToKeysym("quotedbl");
        *shift = true;
    } else if (c == ',') {
        ks = XStringToKeysym("comma");
    } else if (c == '<') {
        /* No shift here; don't know why */
        ks = XStringToKeysym("less");
    } else if (c == '.') {
        ks = XStringToKeysym("period");
    } else if (c == '>') {
        ks = XStringToKeysym("greater");
        *shift = true;
    } else if (c == '/') {
        ks = XStringToKeysym("slash");
    } else if (c == '?') {
        ks = XStringToKeysym("question");
        *shift = true;
    } else {
        DIE(dpy, "No keysym for %c\n", c);
    }

    return XKeysymToKeycode(dpy, ks);
}

int main() {
    char c;
    int shift_kc, kc;
    Display *dpy = NULL;
    KeySym shift_ks;
    ssize_t ret;
    const int delay = 1; /* DON'T TOUCH - SEE BELOW */

    dpy = XOpenDisplay(":0");
    if (!dpy)
        dpy = XOpenDisplay(":1");
    if (!dpy)
        DIE(dpy, "Failed to open display\n");

    shift_ks = XStringToKeysym("Shift_L");
    if (!shift_ks)
        DIE(dpy, "No keysym for shift\n");
    shift_kc = XKeysymToKeycode(dpy, shift_ks);

    for (ret = read(0, &c, 1); ret > 0; ret = read(0, &c, 1)) {
        bool shift = false;

        if (c == '\n')
            break;

        kc = lookup_keycode(dpy, c, &shift);

        XTestGrabControl(dpy, true);

        if (shift)
            XTestFakeKeyEvent(dpy, shift_kc, true, delay);

        XTestFakeKeyEvent(dpy, kc, true, delay);
        XTestFakeKeyEvent(dpy, kc, false, delay);

        if (shift)
            XTestFakeKeyEvent(dpy, shift_kc, false, delay);

        /* The combination of delay=1 and XPending/XFlush "seems to work".  With
         * delay=0, no amount of calls or sleeps I can come up with prevents
         * input from processing out-of-order in some cases. */
        /* XPending(dpy); */
        XFlush(dpy);

        XTestGrabControl(dpy, false);
    }

    XCloseDisplay(dpy);
    exit(0);
}
