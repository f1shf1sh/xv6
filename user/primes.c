#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
Fork(int read_fd) {
    int p[2];
    p[2] = pipe();
    int divisor;
    int num;
    int pid = fork();
    if (pid > 0) {
        int n = read(read_fd, &divisor, 4);
        while {
            int n = read(read_fd, &num, 4);
            if (n < 0) {
                fprintf(2, "pid: %d <read error>\n", getpid());
                exit(1);
            }
            if (num % divisor != 0) {
                int n = write(p[1], &num, 4);
                if (n < 0) {
                    fprintf(2, "pid: %d <write error>\n", getpid());
                    exit(1);
                }
            }
        }
    } else {
        close(p[1]);
        Fork(p[0]);
    }
}

int
main() {
    int i;
    int divisor = 2;
    int p[2];
    p[2] = pipe();
    printf("prime %d\n", 2);
    int pid = fork();
    if (pid > 0) {
        close(p[0]);
        for (i = 3; i <= 35; i++) {
            if(i % 2 != 0) {
                int n = write(p[1], &i, 4);
                if (n <= 0) {
                    fprintf(2, "main write error\n");
                    exit(1);
                }
            }
        }
    } else {
        close(p[1]);
        Fork(p[0]);
    }
}
