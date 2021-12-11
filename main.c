#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <dirent.h>
#include <stdbool.h>

pid_t get_process_pid(const char *process_name) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        fprintf(stderr, "[!] Unable to open \"/proc\" directory.\n");
        exit(1);
    }

    struct dirent *cd;
    while (cd = readdir(dir)) {
        pid_t pid = atoi(cd->d_name);
        if (pid == 0) continue;

        char buf[256];
        sprintf(buf, "/proc/%d/cmdline", pid);
        
        int fd = open(buf, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "[!] Unable to open %s file.\n", buf);
            exit(1);
        }
        if(read(fd, buf, sizeof(buf)) == 0) continue;

        const char *exe_name = strrchr(buf, '/');
        if (exe_name == NULL) continue;
        exe_name++; // Eat up '/'

        if (!strcmp(exe_name, process_name)) return pid;
    }

    closedir(dir);
    return (pid_t) -1;
}

bool read_memory(pid_t pid, void *addr, void *buf, size_t n_bytes) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = buf;
	local[0].iov_len = n_bytes;
	remote[0].iov_base = addr;
	remote[0].iov_len = n_bytes;

	if (process_vm_readv(pid, local, 1, remote, 1, 0) != n_bytes) {
        printf("[!] Unable to read target process's memory, check if you're running the cheat as root.\n");
        return false;
    }
    return true;
}

bool write_memory(pid_t pid, void *addr, void *buf, size_t n_bytes) {
	struct iovec local[1];
	struct iovec remote[1];

	local[0].iov_base = buf;
	local[0].iov_len = n_bytes;
	remote[0].iov_base = addr;
	remote[0].iov_len = n_bytes;

	if (process_vm_writev(pid, local, 1, remote, 1, 0) != n_bytes) {
        printf("[!] Unable to write target process's memory, check if you're running the cheat as root.\n");
        return false;
    }
    return true;
}

int main(int argc, const char **argv) {
    pid_t pid = get_process_pid("linux_64_client");
    if (pid == -1) {
        fprintf(stderr, "[!] Assault Cube's process not found.\n");
        exit(1);
    }
    printf("[*] Assault Cube's process found. (pid: %d)\n", pid);
	
    int value = 1337;
    write_memory(pid, (void*)0x007770D4, &value, sizeof(value));
    return 0;
}
