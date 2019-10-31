#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int pid;
  pid = getpid();
  int prio;
  prio = getpriority(pid);
  printf(1, "initial priority: %d\n", prio);
  prio = 3;
  setpriority(pid, prio);
  prio = getpriority(pid);
  printf(1, "final priority: %d\n", prio);
  exit();
}
