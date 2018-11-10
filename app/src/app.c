#include <sapi.h>
#include <stdio.h>
#include <microrl.h>
#include <ff.h>

#include <commands.h>

static microrl_t rl;

static void stdout_putstr(const char *s) {
    fprintf(stdout, "%s", s);
    fflush(stdout);
}

void distkTickHook(void *ptr) {
    (void) ptr;
    disk_timerproc();
}

int main( void ) {
    boardConfig();
    tickCallbackSet(distkTickHook, NULL);
    microrl_init(&rl, stdout_putstr);
    microrl_set_execute_callback(&rl, executeCommand);
    while(TRUE) {
        uint8_t c;
        if (uartReadByte(UART_USB, &c)) {
            microrl_insert_char(&rl, c);
        }
    }
}
