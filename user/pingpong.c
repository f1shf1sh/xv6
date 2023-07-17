#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[]) {
    // p[0] is input p[1] is output
    char byte[1] = "*";
    char recv[1];
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    int pid = fork();
    if (pid == 0) {
        //child
        close(p1[1]);
        close(p2[0]);
        int n = read(p1[0], recv, 1);
        if (n < 0){
            fprintf(2, "child read error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        n = write(p2[1], byte, 1);
        if (n < 0) {
            fprintf(2, "child write error\n");
            exit(1);
        }
        exit(0);
    } else {
        // parent
        close(p1[0]);
        close(p2[1]);
        int n = write(p1[1], byte, 1);
        if (n < 0) {
            fprintf(2, "parent write error\n");
            exit(1);
        }
        wait(0);
        n = read(p2[0], recv, 1);
        if (n < 0) {
            fprintf(2, "parent read error\n");
            exit(1);
        }
        printf("%d: recvived pong\n", getpid());
    }
    exit(0);
}