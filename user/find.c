#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char path[512];

int match(char* pattern, char* name){
  if(*pattern == 0) return *name == 0;
  if(*pattern == '*') return match(pattern + 1, name) || (*name && match(pattern, name + 1));
  return *name == *pattern && match(pattern + 1, name + 1);
}

void find(char* pattern){
    int dir = open(path, O_RDONLY);
    int dir_len = strlen(path);
    struct dirent de;
    struct stat s;
    char name[DIRSIZ + 1];

    while(read(dir, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0) continue;
        memmove(name, de.name, DIRSIZ);
        name[DIRSIZ + 1] = 0;

        if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
        if(dir_len + 1 + strlen(name) + 1 > sizeof(path)) continue;
        path[dir_len] = '/';
        strcpy(path + dir_len + 1, name);

        if(stat(path, &s) == 0){
            if(match(pattern, name)) printf("%s\n", path);
            if(s.type == T_DIR) find(pattern);
        }

        path[dir_len] = 0;
    }

    close(dir);
}

int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(2, "usage: find <dir> <pattern> [-type f|d]\n");
        exit(1);
    }

    int dir = open(argv[1], O_RDONLY);
    if(dir < 0){
        fprintf(2, "find: cannot open %s\n", argv[1]);
        exit(1);
    }
    close(dir);

    strcpy(path, argv[1]);
    find(argv[2]);
    exit(0);
}