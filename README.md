# Fixed-Priority-Scheduler-xv6
### Setting up:
1. Must have QEMU emulator installed on linux.
2. Clone the repo and run ```make qemu``` and it should open a QEMU terminal that simulates the OS with the required scheduler.

### The Theory Behind Implementation:
This modificaiton of the OS Scheduler prioritizes based on how each process behaves, with respect to the time slice allotted to it.

A process can behave in two ways:
1] IO Intensive: The process frequently forfeits its time slice, in order to perform some IO operation, which takes a much longer time to perform relative to CPU timescales. In this case, the process must be given a bump in its priority.

2] CPU Intensive: The process usually completes its allotted time slice (after being schedule to run). It heavily depends on CPU calculation. This must lower its priority.

We measure the behaviour by timing the process' when it is scheduled for a time slice. If this time crosses a threshold, we would classify it as a CPU Intensive process, otherwise an IO intensive process.

In order to avoid starvation, we periodically bump priorities of CPU intensive processes as well.
