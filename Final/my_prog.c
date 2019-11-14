#include "types.h"
#include "stat.h"
#include "user.h"

/* priority reduces(value) on calling a print*/

int
main(int argc, char *argv[])
{
  setpriority(1);
  int k=fork();
  if(k==0){
    setpriority(9);
    printf(1, "(%d) before sleeping, prio : %d\n", getpid(), getpriority());
    setpriority(9);
    sleep(5);
    printf(1,"(%d) after wake-up, prio : %d\n",getpid(),getpriority());
    exit();
  }
  else{
    setpriority(1);
    printf(1, "(pid: %d) before computation, prio: %d\n", getpid(), getpriority());
    setpriority(1);
    int i=0;int j=0;int sum=0;int k;
    for(k=0;k<10;k++){
      for(i=0;i<1000;i++){
        for(j=0;j<3000;j++)
          {sum = sum + j*j;}
      }
      printf(1,"(%d) during computation, prio: %d\n",getpid(),getpriority());
    }
    sleep(1);
    printf(1,"(%d) after execution, prio: %d, sum: %d\n",getpid(),getpriority(),sum);
    wait();
    exit();
  }
}
