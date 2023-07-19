#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int debug_p;

void 
Fork(int read_fd) {
    int p[2];
    pipe(p);
    int divisor;
    int num, n;
    if (getpid() >= 34) {
        wait(0);
    }
    int pid = fork();
    
    //debug
    printf("Fork: pid %d start\n", getpid());

    if (pid > 0) {
        close(p[0]);
        n = read(read_fd, &divisor, 4);
        if ( n < 0) {
            fprintf(2, "pid: %d <read error>\n", getpid());
            exit(1);
        } else if (n == 0) {
            close(read_fd);
            printf("pid %d exit\n", getpid());
            exit(0);
        }
        printf("pid %d get the prime %d to use divisor\n", getpid(), divisor);
        printf("Fork: func read return %d\n", n);
        while (n < 4) {
            n = read(read_fd, &num, 4);
            printf("pid %d read num %d", getpid(), num);
            if (n < 0) {
                fprintf(2, "pid: %d <read error>\n", getpid());
                exit(1);
            }
            if (num % divisor != 0) {
                int n = write(p[1], &num, 4);
                printf("pid%d write num %d", getpid(), num);
                if (n < 0) {
                    fprintf(2, "pid: %d <write error>\n", getpid());
                    exit(1);
                }
            }
        }
    } else {
        close(read_fd);
        close(p[1]);
        Fork(p[0]);
        debug_p = wait(0);
        printf("pid %d exit\n", debug_p);
        exit(0);
    }

    //debug
    printf("Fork: pid %d exit\n", getpid());
}

int
main() {
    int i;
    int p[2];

    pipe(p);
    printf("prime %d\n", 2);

    //debug
    printf("pid %d start\n", getpid());

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
        debug_p = wait(0);
        printf("wait the pid %d exit\n", debug_p);
    } else {
        close(p[1]);
        Fork(p[0]);
        debug_p = wait(0);
        printf("wait the pid %d exit\n", debug_p);
        close(p[0]);
        printf("pid %d exit", getpid());
        exit(0);
    }
    close(p[1]);

    //debug
    printf("pid %d exit\n", getpid());
    exit(0);
}
