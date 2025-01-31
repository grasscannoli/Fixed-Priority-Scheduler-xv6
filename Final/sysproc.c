#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}




#define CMOS_PORT    0x70
#define CMOS_RETURN  0x71
#define CMOS_STATA   0x0a
#define CMOS_STATB   0x0b
#define CMOS_UIP    (1 << 7)        // RTC update in progress

#define SECS    0x00
#define MINS    0x02
#define HOURS   0x04
#define DAY     0x07
#define MONTH   0x08
#define YEAR    0x09

static uint
cmos_reads(uint reg)
{
  outb(CMOS_PORT,  reg);
  microdelay(200);

  return inb(CMOS_RETURN);
}



static void
fill_rtcdates(struct rtcdate *r)
{
  r->second = cmos_reads(SECS);
  r->minute = cmos_reads(MINS);
  r->hour   = cmos_reads(HOURS);
  r->day    = cmos_reads(DAY);
  r->month  = cmos_reads(MONTH);
  r->year   = cmos_reads(YEAR);
}


// qemu seems to use 24-hour GWT and the values are BCD encoded
void
cmostimes(struct rtcdate *r)
{
  struct rtcdate t1, t2;
  int sb, bcd;

  sb = cmos_reads(CMOS_STATB);

  bcd = (sb & (1 << 2)) == 0;

  // make sure CMOS doesn't modify time while we read it
  for(;;) {
    fill_rtcdates(&t1);
    if(cmos_reads(CMOS_STATA) & CMOS_UIP)
        continue;
    fill_rtcdates(&t2);
    if(memcmp(&t1, &t2, sizeof(t1)) == 0)
      break;
  }

  // convert
  if(bcd) {
#define    CONV(x)     (t1.x = ((t1.x >> 4) * 10) + (t1.x & 0xf))
    CONV(second);
    CONV(minute);
    CONV(hour  );
    CONV(day   );
    CONV(month );
    CONV(year  );
#undef     CONV
  }

  *r = t1;
  r->year += 2000;
}

int
sys_ssm(void)
{
  struct rtcdate r;
  int mm;
  cmostimes(&r);
  mm = ((r.second)+((r.minute)*60)+((r.hour)*60*60)+(5*3600)+(30*60))%86400;
  return mm;
}


int
sys_getpriority(void){
  int _pid;
  //if(argint(0, &_pid) < 0)
    //return -1;
  _pid = myproc()->pid;
  return gp(_pid);
}

int
sys_setpriority(void){
  int _pid;
  int _prio;
  if(argint(0, &_prio) < 0)
    return -1;
  //if(argint(1, &_prio) < 0)
    //return -1;
  _pid = myproc()->pid;
  return sp(_pid, _prio);
}

