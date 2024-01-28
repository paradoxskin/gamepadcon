#define first
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct {
    unsigned int type;
    unsigned int code;
    unsigned int value;
    KeySym keysym;
} GP;

#include "config.h"

void loop();
void* listen(void*);

static int running;

int main() {
    int lock_fd = mkfifo(lock_file, 0666);
    if (lock_fd == -1) {
        lock_fd = open(lock_file, O_WRONLY);
        const char* close_signal = "233";
        write(lock_fd, close_signal, 3);
        close(lock_fd);
        return 0;
    }
    close(lock_fd);
    running = 1;
    pthread_t id;
    pthread_create(&id, NULL, listen, NULL);
    loop();
    return 0;
}

void* listen(void* arg) {
    int lock_fd = open(lock_file, O_RDONLY);
    char buffer[16];
    while (running) {
        if (read(lock_fd, buffer, sizeof(buffer)) > 0) running = 0;
    }
    return NULL;
}

void loop() {
    int fd;
    struct input_event ev;
    Display *display;
    KeyCode mod_keycode;
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        return;
    }
    mod_keycode = XKeysymToKeycode(display, XK_Super_L);
    while (running) {
        fd = open(gamepad_path, 0);
        if (fd < 0) {
            system("notify-send gp-con 'Failed to open device'");
            usleep(10000000);
            continue;
        }
        system("notify-send gp-con 'Connect device'");
        int last = 0;
        int now = 100;
        while (running) {
            read(fd, &ev, sizeof(struct input_event));
            now = ev.type + ev.code + ev.value;
            if (last == 0 && now == 0) break;
            for(int i=0; i < (sizeof gps / sizeof gps[0]); i++) {
                if (gps[i].type == ev.type && gps[i].code == ev.code && gps[i].value == ev.value) {
                    XTestFakeKeyEvent(display, mod_keycode, True, CurrentTime);
                    XTestFakeKeyEvent(display, XKeysymToKeycode(display, gps[i].keysym), True, CurrentTime);
                    XTestFakeKeyEvent(display, XKeysymToKeycode(display, gps[i].keysym), False, CurrentTime);
                    XTestFakeKeyEvent(display, mod_keycode, False, CurrentTime);
                    XFlush(display);
                    break;
                }
            }
            last = now;
        }
        close(fd);
    }
    unlink(lock_file);
    XCloseDisplay(display);
}
