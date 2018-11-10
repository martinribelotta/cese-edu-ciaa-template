#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *cmd;
    int (*func)(int argc, const char * const *argv);
    const char *info;
} ExecEntry_t;

int executeCommand(int argc, const char * const *argv);

#ifdef __cplusplus
}
#endif

#endif /* _COMMANDS_H_ */
