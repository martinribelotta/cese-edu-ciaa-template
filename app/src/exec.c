#include <ff.h>
#include <loader.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

extern int open(const char *path, int mode, ...);

static struct {
    int (*open)(const char *path, int mode, ...);
    int (*close)(int fd);
    int (*write)(int fd, const void *data, size_t size);
    int (*read)(int fd, void *buf, size_t size);
    int (*printf)(const char *fmt, ...);
    int (*scanf)(const char *fmt, ...);
} const sysentries = { /* */
    open, /* */
    close, /* */
    write, /* */
    read, /* */
    printf, /* */
    scanf /* */
};

static const ELFSymbol_t exports[] = { { "syscalls", (void*) &sysentries } };
static const ELFEnv_t env = { exports, sizeof(exports) / sizeof(*exports) };

void *elf_alloc(size_t size, size_t align, ELFSecPerm_t perm) {
    (void) perm;
    return memalign(size, align);
}

void elf_free(void *ptr) {
    free(ptr);
}

void arch_jumpTo(entry_t entry) {
    (entry)();
}

int func_exec(int argc, const char * const *argv)
{
    if (argc > 1)
        return exec_elf(argv[1], &env);
    else
        printf("Usage: %s <program name>\n", argv[1]);
    return -1;
}
