#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
find(char* path, char* filename) {
    int fd;
    char *p;
    char *tmp_p;
    char buf[512];
    char tmp_buf[512];
    struct stat st;
    struct dirent de;
    
    memset(buf, 0, strlen(buf));
    
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
        // fprintf(2, "line30 path:%s\n", buf);
        case T_FILE:
            if (!strcmp(filename, de.name)) {
                printf("%s\n", filename);
                break;
            }
            
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
                // printf("de.name: %s\n", de.name);
                if (!strcmp(de.name, "." )) {
                    // printf(". done\n");
                    continue;
                } else if (!strcmp(de.name, "..") ) {
                    // printf(".. done\n");
                    continue;
                }
                // memmove(p, de.name, DIRSIZ);
                // p[DIRSIZ] = 0;
                // memcpy(p++, de.name, strlen(de.name));
                // printf("after memcpy, buf:%s\n", buf);
                memset(tmp_buf, 0, strlen(tmp_buf));
                // fprintf(2, "buf:%s\n", buf);
                memcpy(tmp_buf, buf, strlen(buf));
                tmp_p = tmp_buf + strlen(tmp_buf);

                // fprintf(2, "de.name:%s\n", de.name);
                memcpy(tmp_p, de.name, strlen(de.name));
                // fprintf(2, "tmp_buf:%s\n", tmp_buf);

                if (stat(tmp_buf, &st) < 0) {
                    // fprintf(2, "buf:%s\n", buf);
                    // fprintf(2, "tmp_buf:%s\n", tmp_buf);
                    fprintf(2, "find: can't stat file\n");
                    break;
                }

                if (st.type == T_DIR) {
                    // recurse 
                    
                    find(tmp_buf, filename);
                    // memset(tmp_buf, 0, strlen(tmp_buf));
                    // memcpy(tmp_buf, buf, strlen(buf));
                } else {
                    // cmp filename
                    if (!strcmp(filename, de.name)) {
                        // file has been find
                        printf("%s\n", tmp_buf);
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