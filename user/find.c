#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char* path, char* filename) {
    int fd;
    char *p;
    char buf[512];
    struct stat st;
    struct dirent de;
    
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // get file type
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // don't use return use in switch case , use break and jug
    switch(st.type) {
        case T_FILE:
            if (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (!strcmp(filename, de.name)) {
                    printf("%s\n", filename);
                    break;
                }
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                fprintf(2, "find: path to long");
                break;
            }
            
            strcpy(buf, path);
            p = buf + strlen(path);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                // memmove(p, de.name, DIRSIZ);
                // p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    fprintf(2, "find: can't stat file\n");
                    continue;
                }

                if (st.type == T_DIR) {
                    // recurse 
                    find(buf, filename);
                } else {
                    // cmp filename
                    if ( !strcmp(filename, de.name)) {
                        // file has been find
                        printf("%s", buf);
                    }
                }
            }
    }
}

int
main(int argc, char* argv[]) {
    
    if (argc < 3) {
        printf("usage: find <path> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}