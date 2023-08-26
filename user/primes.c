#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int debug_p;

void 
Fork(int read_fd) {
    // printf("Fork: pid %d start\n", getpid());

    int p[2];
    pipe(p);
    // printf("pid:%d, p[0]:%d, p[1]:%d, read_fd:%d\n",getpid(), p[0], p[1], read_fd);
    
    int divisor;
    int num, n;

    if (getpid() == 35) {
        // wait(0);
        exit(0);
    }
    
    
    //debug
    
    int pid = fork();
    if (pid > 0) {
        close(p[0]);
        n = read(read_fd, &num, 4);
        if ( n < 0) {
            fprintf(2, "pid: %d <read error>\n", getpid());
            exit(1);
        }
        printf("primes %d\n", num);
        // printf("Fork read %d from read_fd:%d success\n", num, read_fd);
        if (n == 0) {
            // close(read_fd);
            // printf("primes %d\n", num);
            // printf("pid%d exit success\n", getpid());  
            exit(0);
        }

        divisor = num;
        // printf("pid %d get the prime %d to use divisor\n", getpid(), divisor);
        // printf("Fork: func read return %d\n", n);
        while ( n != 0) {
            n = read(read_fd, &num, 4);
            // printf("pid %d read num %d and read ret %d", getpid(), num, n);
            if (n < 0) {
                fprintf(2, "pid: %d <read error>\n", getpid());
                exit(1);
            }
            // printf("read num %d success\n", num);
            // printf("Fork read %d from read_fd:%d success\n", num, read_fd);
            // printf("read ret number: %d\n", n);
            if (num%divisor != 0) {
                int write_n = write(p[1], &num, 4);
                // printf("pid %d write num %d and write ret %d", getpid(), num, write_n);
                if (write_n < 0) {
                    fprintf(2, "pid: %d <write error>\n", getpid());
                    exit(1);
                }
            }
        }
        // close(read_fd);
        // printf("pid:%d close read_fd:%d success\n", getpid(), read_fd);
        wait(0);
    } else {
        // close(read_fd);
        close(p[1]);
        Fork(p[0]);

        // printf("pid:%d start wait\n", getpid());
        debug_p = wait(0);
        // printf("pid %d exit\n", getpid());
        exit(0);
    }

    //debug
    
    // printf("Fork: pid %d exit\n", getpid());
    
    exit(0);
}

int
main() {
    int i;
    int p[2]; 
    pipe(p);
    // printf("main, p[0]:%d, p[1]:%d\n", p[0], p[1]);
    //debug
    // printf("main pid %d start\n", getpid());
    
    int pid = fork();
    if (pid > 0) {
        close(p[0]);
        for (i = 2; i <= 35; i++) {
            if (i == 2) {
                // printf("primes %d\n", i);
            }
            if(i % 2 != 0) {
                int n = write(p[1], &i, 4);
                if (n <= 0) {
                    fprintf(2, "main write error\n");
                    exit(1);
                }
                // printf("main write %d to p[1]:%d success\n", i, p[1]);
            }
        }
        close(p[1]);
        debug_p = wait(0);
        close(p[1]);
        // printf("wait the pid %d exit\n", debug_p);
    } else {
        close(p[1]);
        Fork(p[0]);
        debug_p = wait(0);
        // printf("wait the pid %d exit\n", debug_p);
        close(p[0]);
        // printf("pid %d exit", getpid());
        exit(0);
    }
    close(p[1]);
    //debug
    // printf("main pid %d exit\n", getpid());
    exit(0);
}
