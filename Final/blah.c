void
scheduler(void)
{
  struct proc* rp;  
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Fixed priority scheduling:
      uint mn = 20;
      int idx = 0;
      struct proc *q;
      int plist[50];
      //plist[0] = 7;
      
      for(q = ptable.proc; q < &ptable.proc[NPROC]; q++){
	if(q->state == RUNNABLE && q->prio < mn){
	  mn = q->prio;
	}
      }

      for(q = ptable.proc; q< &ptable.proc[NPROC]; q++){
	if(q->state == RUNNABLE && q->prio ==  mn){
	  plist[idx] = q->pid;
	  //cprintf("%d ", plist[idx]);
	  idx++;
	}
      }

      //cprintf("\n");
      cprintf("proc idx: %d f\n", idx);
      struct rtcdate r;
      cmostimess(&r);
      int sch = (r.second%idx);
      //cprintf("%d: %d\n", sch, plist[sch]);
      int rpid = plist[sch];
      //cprintf("%d\n", rpid);
      
      for(q = ptable.proc; q< &ptable.proc[NPROC]; q++){
	if(q->state == RUNNABLE && q->pid == rpid){
	  rp = q;
	  p = rp;
	}
      }
      
      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;

      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);

  }
}
