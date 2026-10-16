#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#define MAXWORD 128

char words[MAXARG][MAXWORD];
char* args[MAXARG + 1];
int nfixed;
int nargs;

void usage(){
    fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
    exit(1);
}

void run(){
  args[nargs] = 0;
  int pid = fork();
  if(pid < 0){
    fprintf(2, "xargs: fork failed\n");
    exit(1);
  }
  if(pid == 0){
    exec(args[0], args);
    fprintf(2, "xargs: exec %s failed\n", args[0]);
    exit(1);
  }
  int status;
  wait(&status);
  if(status != 0) exit(1);
  nargs = nfixed;
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
    if(i >= argc) usage();

    nfixed = argc - i;
    nargs = nfixed;
    if(nargs >= MAXARG) usage();
    for(int j=0; j<nargs; j++) args[j] = argv[j + i];

    char c;
    int len = 0;
    int more;
    do {
        more = read(0, &c, 1) == 1;
        int sep = !more || c == ' ' || c == '\t' || c == '\n';

        if(!sep){
            if(len < MAXWORD - 1) words[nargs][len++] = c;
            continue;
        }

        if(len > 0){
            words[nargs][len] = 0;
            args[nargs] = words[nargs];
            nargs++;
            len = 0;
            if(n > 0 && nargs - nfixed == n) run();
            else if(nargs == MAXARG) run();
        }
        if(n == 0 && more && c == '\n' && nargs > nfixed) run();
    } while(more);

    if(nargs > nfixed) run();

    exit(0);
}