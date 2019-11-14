#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  setpriority(9);
  printf(1, "(pid: %d) priority: %d\n", getpid(), getpriority());  
  sleep(6);
  printf(1, "(pid: %d) priority: %d\n", getpid(), getpriority());
  exit();
}
