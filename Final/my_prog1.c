#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  setpriority(2);
  printf(1, "(pid: %d) priority: %d\n", getpid(), getpriority());
  int i=0;int j=0;int sum=0;
  for(i=0;i<1000;i++){
    for(j=0;j<3000;j++)
      {
	sum = sum + j*j;
      }
  }
  printf(1,"sum: %d\n", sum);
  printf(1, "(pid: %d) priority: %d\n", getpid(), getpriority());
  exit();
}
