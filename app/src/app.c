#include <sapi.h>
#include <stdio.h>
#include <ctype.h>

static char buffer[128];

static void readline(uartMap_t uart, char *s, size_t z) {
    bool_t eol = FALSE;
    size_t n = 0;
    *s = 0;
    while(!eol) {
        uint8_t c;
        if (uartReadByte(uart, &c)) {
            if (!iscntrl(c)) {
                s[n] = (char) c;
                uartWriteByte(uart, c);
                if (n < z) {
                    n++;
                    s[n] = 0;
                }
            }
            switch (c) {
            case '\r':
            case '\n':
                eol = TRUE;
                break;
            }
        } else
            sleepUntilNextInterrupt();
    }
}

static char *skipspaces(char *s, const char *end) {
    while (*s && isspace((int)*s)) {
        if (s >= end)
            return NULL;
        s++;
    }
    return s;
}

static char *nextspace(char *s, const char *end) {
    while (*s && !isspace((int)*s)) {
        if (s >= end)
            return NULL;
        s++;
    }
    return s;
}

static int parsearg(char *s, size_t max, const char **const argv, int maxargc) {
    int argc = 0;
    const char *end = s + max;
    while (*s) {
        s = skipspaces(s, end);
        if (!s || !*s)
            break;
        argv[argc] = s;
        argc++;
        s = nextspace(s, end);
        if (!s || !*s)
            break;
        *s++ = '\0';
        if (argc >= maxargc)
            break;
    }
    return argc;
}

static int exec(int argc, const char **argv) {
    // TODO
    printf("Try to exec %s with %d parameters:\n", argv[0], argc - 1);
    for (int i=1; i<argc; i++)
        printf("  %s\n", argv[i]);
    return 0;
}

static void repl() {
    static const char *argv[10];
    printf(">> ");
    fflush(stdout);
    readline(UART_USB, buffer, sizeof(buffer));
    int argc = parsearg(buffer, sizeof(buffer), argv, 10);
    puts("");
    if (argc)
         exec(argc, argv);
}

int main( void ) {
    boardConfig();
    puts("Welcome!!!");
    while( TRUE )
        repl();
}
