#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

char* words[MAXARG][128];

void usage(){
    fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
    exit(1);
}

int main(int argc, char* argv[]){
    int i = 1;
    int n = 0;
    if(argc > MAXARG) fprintf(2, "MAXARG is 32");

    if(strcmp(argv[1], "-n") == 0){
        for(char *c = argv[2]; *c; c++){
            if(*c < '0' || *c > '9') usage();
        }
        n = atoi(argv[2]);
        if(n < 1) usage();
        i = 3;
    }
    //if(i >= argc) usage();

    int args = argc - i;
    if(args >= MAXARG) usage();

    char c;
    read(0, &c, 1);
    printf("%d\n", c);
    read(0, &c, 1);
    printf("%d\n", c);
    read(0, &c, 1);
    printf("%d\n", c);

    exit(0);
}