#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

pid_t get_process_pid(const char *process_name) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        printf("Unable to open \"/proc\" directory.\n");
        exit(1);
    }

    struct dirent *cd;
    while (cd = readdir(dir)) {
        pid_t pid = atoi(cd->d_name);
        if (pid == 0) continue;

        char buf[256];
        sprintf(buf, "/proc/%d/cmdline", pid);

        int fd = open(buf, O_RDONLY);
        if (!read(fd, buf, sizeof(buf))) continue;

        const char *exe_name = (strrchr(buf, '/') + 1);
        if (exe_name == (const char *)1) continue;

        if (!strcmp(exe_name, process_name)) {
            return pid;
        }
    }

    closedir(dir);
    return (pid_t) -1;
}

void read_memory(pid_t pid, void *addr, void *buf, size_t n_bytes) {
}

void write_memory(pid_t pid, void *addr, void *buf, size_t n_bytes) {
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Usage: %s <process name>\n", argv[0]);
        exit(1);
    }

    pid_t pid = get_process_pid(argv[1]);
    if (pid == -1) {
        printf("Unable to find \"%s\".\n", argv[1]);
    } else {
        printf("%s's pid: %d\n", argv[1], pid);
    }
}
