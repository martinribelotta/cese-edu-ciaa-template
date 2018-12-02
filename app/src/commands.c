#include <commands.h>

#include <stdio.h>
#include <sapi_usbms.h>
#include <sapi.h>
#include <ff.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(*(a)))

static usbms_t disk;

static int func_echo(int argc, const char * const *argv);
static int func_ls(int argc, const char * const *argv);
static int func_usb(int argc, const char * const *argv);
static int func_cat(int argc, const char * const *argv);
static int func_info(int argc, const char * const *argv);
extern int func_exec(int argc, const char * const *argv);

static ExecEntry_t cmdList[] = {
    { "echo", func_echo, "print echo of text" },
    { "ls", func_ls, "list directory" },
    { "cat", func_cat, "concat archives to stdout" },
    { "exec", func_exec, "load and execute elf" },
    { "usb", func_usb, "usb utility" },
    { "info", func_info, "system info" },
    { "help", func_info, "system info [same as info]" },
    { "?", func_info, "system info [same as info]" },
};

int executeCommand(int argc, const char * const *argv) {
    printf("\n");
    for (size_t i=0; i<ARRAY_SIZE(cmdList); i++)
        if (strcmp(cmdList[i].cmd, argv[0]) == 0) {
            int r = cmdList[i].func(argc, argv);
            fflush(stdout);
            fflush(stderr);
            return r;
        }
    printf("Unrecognized command %s\n", argv[0]);
    return -1;
}

static void f_printError(const char *s, FRESULT r) {
    printf("%s: %s\n", s, f_resultString(r));
}

static int func_echo(int argc, const char * const *argv) {
    for (int i=1; i<argc; i++)
        printf("%s ", argv[i]);
    printf("\n");
    return 0;
}

static int func_ls(int argc, const char * const *argv) {
    (void) argc;
    (void) argv;
    for(int i=1; i<argc; i++) {
        FILINFO info;
        DIR dir;
        if (f_opendir(&dir, argv[i]) == FR_OK) {
            printf("Directoriy %s contents:\n", argv[i]);
            while (f_readdir(&dir, &info) == FR_OK) {
                if (info.fname[0] == 0)
                    break;
                char type[] = "   ";
                if (info.fattrib & AM_RDO)
                    type[2] = 'R';
                if (info.fattrib & AM_HID)
                    type[1] = 'H';
                if (info.fattrib & AM_SYS)
                    type[0] = 'S';
                const char *isDir = (info.fattrib&AM_DIR)? "\033[94m" : "\033[32m";
                printf(" %s%s%-40s\033[39m %lld bytes\n", type, isDir, info.fname, info.fsize);
            }
        } else {
            printf("Cannot list %s\n", argv[i]);
        }
    }
    return 0;
}

static int func_usb(int argc, const char * const *argv) {
    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "init") == 0) {
            if (usbmsStatus() == USBMS_Status_StorageReadyMounted) {
                FRESULT r = f_mount(&disk.fatFs, usbmsDriveName(), 1);
                printf("Init: %s\n", f_resultString(r));
                continue;
            }
            if (!usbmsInit(&disk))
                printf("Error init disk\n");
            else {
                printf("Waiting for USB ready\n");
                while (usbmsStatus() != USBMS_Status_StorageReadyMounted) {
                    printf(".");
                    fflush(stdout);
                    delay(1000);
                    uint8_t c;
                    if (uartReadByte(UART_USB, &c)) {
                        if (c == 27)
                            break;
                    }
                }
            }
        } else if (strcmp(argv[i], "end") == 0) {
            f_unmount(usbmsDriveName());
        } else if (strcmp(argv[i], "help") == 0) {
            printf("usage: %s [init|end]\n", argv[0]);
        } else {
            printf("Unrecognized command %s\n", argv[i]);
        }
    }
    return 0;
}

static int func_cat(int argc, const char * const *argv) {
    for (int i=1; i<argc; i++) {
        FIL f;
        FRESULT r = f_open(&f, argv[i], FA_READ);
        if (r == FR_OK) {
            while (!f_eof(&f)) {
                static char buf[32];
                UINT readed;
                r = f_read(&f, buf, sizeof(buf), &readed);
                if (r != FR_OK) {
                    f_printError("Error reading file", r);
                    break;
                }
                fwrite(buf, sizeof(char), readed, stdout);
            }
            fflush(stdout);
        } else
            f_printError("Error opening file", r);
    }
    return 0;
}

static int func_info(int argc, const char * const *argv) {
    (void) argc;
    (void) argv;
    printf("Build on %s at %s\n"
           "  with compiler %s\n"
           "  for cortex-m%d\n\n"
           "List of commands:\n",
           __DATE__, __TIME__,
           __VERSION__,
           __CORTEX_M);
    for (size_t i=0; i<ARRAY_SIZE(cmdList); i++)
        printf("  %s %-20s\n", cmdList[i].cmd, cmdList[i].info);
    return 0;
}
