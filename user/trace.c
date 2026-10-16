#include "kernel/syscall.h"
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 3 || argv[1][0] == 0){
    fprintf(2, "usage: trace <mask> <cmd> [args...]\n");
    exit(1);
  }
  for(char *p = argv[1]; *p; p++){
    if(*p < '0' || *p > '9'){
      fprintf(2, "usage: trace <mask> <cmd> [args...]\n");
      exit(1);
    }
  }

  if(trace(atoi(argv[1])) < 0){
    fprintf(2, "trace: trace failed\n");
    exit(1);
  }
  exec(argv[2], &argv[2]);    // become the command; argv[2..] are its args
  fprintf(2, "trace: exec %s failed\n", argv[2]);
  exit(1);
}