#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// check the input string in argv if it is a legal numeric string
int
check(const char* s) {
    for (;*s != '\0' ;s++) {
        if (*s < '0' || *s > '9') {
            return -1;
        }
    }
    return 0;
}

int
main(int argc, char* argv[]) {
    int i;
    int sleep_time = 0;
    int flag = 0;

    if (argc <= 1) {
        fprintf(2, "sleep: missing operand\nTry 'sleep --help' for more information.\n");
        exit(1);
    }
    else {
        for (i = 1; i < argc; i++){
            if (check(argv[i]) != 0) {
                fprintf(2, "sleep: invalid time interval '%s'\n", argv[i]);
                flag = 1;
            } else {
                sleep_time += atoi(argv[i]);
            }
        }
        if (flag) {
            fprintf(2, "Try 'sleep --help' for more information.\n");
            exit(1);
        }
        sleep(sleep_time*10);
        exit(0);
    }
    exit(0);
}