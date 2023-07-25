#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
split(char *argv_arr[MAXARG], char *str, char delim) {
    char *start = str;
    char *end = str;
    int index = 0;

    if ( start == 0) {
        return 0;
    }

    while (*end != '\0') {
        if ( *end == delim) {
            if (start == end) {
                argv_arr[index] = 0;
            } else {
                argv_arr[index] = start;
            }

            *end = '\0';
            start = end + 1;
            index++;
        } 
        end++;
    }

    if ( start != end) {
        argv_arr[index] = start;
        index++;
    }
    // else {
    //     index--;
    // }
    return index;
    
}

int 
main(int argc, char* argv[]) {

    int i;
    // int j;
    int pid;
    int arrlen;
    char argv_buf[MAXARG];
    char *argv_arr[MAXARG];

    // char whitespace[] = " \t\r\n\v";
    if (argc < 2) {
        printf("usage: xargs <parm>\n");
        exit(0);
    }

    
    int n = read(0,argv_buf, MAXARG);
    if ( n < 0) {
        fprintf(2, "xargs: read arg error\n");
        exit(1);
    }

    // printf("arg:%s\n", argv_buf);
    // parse argv_buf

    arrlen = split(argv_arr, argv_buf, '\n');
    // printf("arrlen:%d\n", arrlen);
    for ( i = 0; i < argc; i++) {
        argv[i] = argv[i+1];
    }

    argc--;

    //argv debug
    // for ( i = 0; i < argc; i++) {
    //     printf("xargs argv[%d]:%s\n", i, argv[i]);
    // }

    //check it
    // printf("arrlen:%d\n", arrlen);
    // for ( i = 0; i < arrlen; i++) {
    //     printf("%s\n", argv_arr[i]);
    // }

    for ( i = 0; i < arrlen; i++) {
        pid = fork();
        if (pid == 0) {
            argv[argc] = argv_arr[i];
            argv[argc+1] = 0;
            exec(argv[0], argv);
            exit(0);
        }   else {
            wait(0);
        }

    }

    // for ( i = 0; i < argc; i++) {
    //     // printf("xargs argv[%d]:%s\n", i, argv[i]);
    // }
    exit(0);
}