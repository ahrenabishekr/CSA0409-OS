# Systems & Operating Concepts - Debugging & Optimization

---

## Q1. Deadlock Debugging (Code Trace)

### 1. Exact Condition Causing Deadlock
* **Hold and Wait:** Process $P_1$ holds resource $R_1$ while waiting for $R_2$; simultaneously, process $P_2$ holds resource $R_2$ while waiting for $R_1$.
* **Circular Wait (Lock Order Inversion):** A closed chain of dependencies exists ($P_1 \rightarrow R_2 \rightarrow P_2 \rightarrow R_1 \rightarrow P_1$).
* **Timing Trigger:** The `wait(100 ms)` delay creates an intentional window that guarantees both processes acquire their first lock before either attempts to request its second lock.

### 2. Optimization (Eliminating Deadlock)
* **Global Lock Ordering Protocol:** Enforce a strict hierarchical rule where all processes acquire resources in the **exact same order** (e.g., always acquire $R_1$ before acquiring $R_2$).
  * **Updated $P_2$ Logic:** Acquire $R_1$ $\rightarrow$ Acquire $R_2$ $\rightarrow$ Critical Section $\rightarrow$ Release $R_2$ $\rightarrow$ Release $R_1$.

---

## Q2. CPU Utilization Problem

### 1. Root Cause Analysis
* **High I/O Blocking / Frequent Yielding:** The continuous pattern of `ready → waiting → ready` indicates that processes are running for extremely short CPU bursts before hitting a blocking call (e.g., synchronous disk/network I/O or page faults) and relinquishing control.
* **Excessive Scheduling Overhead:** Because processes immediately go to the `waiting` state, the CPU spends a significant fraction of its time executing scheduler context switches rather than executing productive instructions.

### 2. System-Level Optimization
* **Adopt Non-Blocking / Asynchronous I/O:** Shift from synchronous blocking calls to asynchronous event-driven I/O models (e.g., `epoll`, `io_uring`). This keeps threads executing on the CPU instead of constantly transitioning to the `waiting` state.
* **Adjust Multiprogramming Level & I/O Batching:** Group small I/O operations into batched writes/reads to reduce the frequency of system calls and state transitions.

---

## Q3. Memory Bottleneck Debugging

### 1. Identification of Memory Issue
* **Total Memory Footprint Analysis:**
  $$\text{Total Used Memory} = 5\text{ MB (Code)} + 150\text{ MB (Heap)} + (25 \times 1\text{ MB Stack}) = 180\text{ MB}$$
  Since $180\text{ MB} \ll 2000\text{ MB (2 GB Available Physical RAM)}$, the bottleneck is **not** a lack of physical RAM.
* **Root Cause — Cache Line Bouncing & Non-Contiguous Memory Access:**
  Multiple threads are accessing shared, non-contiguous heap locations simultaneously. This leads to **poor spatial and temporal locality**, false sharing across CPU caches, and heavy Translation Lookaside Buffer (TLB) misses, triggering high page fault activity even inside a low overall memory footprint.

### 2. Two Optimizations
1. **Thread-Local Storage (TLS) / Custom Arena Allocator:** Use thread-local memory pools (e.g., `jemalloc` or `tcmalloc`) so each thread allocates memory sequentially within its own memory arena, drastically cutting down shared heap locks and page faults.
2. **Data Structure Alignment & Restructuring:** Reorganize data layouts from Array of Structures (AoS) to Structure of Arrays (SoA) and align heavily accessed variables to 64-byte cache lines to eliminate false sharing and optimize page fetching.

---

## Q4. Starvation Debugging

### 1. Root Cause Analysis
* **Strict Priority Inequity:** In a standard Multilevel Queue (MLQ) scheduler, lower-priority queues ($Q_3$) are served **only** when all higher-priority queues ($Q_1$ and $Q_2$) are completely empty.
* **Indefinite Postponement:** Constant arrival of interactive jobs in $Q_1$ (5 ms quantum) and system processes in $Q_2$ (10 ms quantum) starves $Q_3$. Because $Q_3$ uses First-Come, First-Served (FCFS), process $P_5$ remains trapped at the back of $Q_3$ and never receives CPU slice time.

### 2. Optimization for Fairness
* **Upgrade to Multilevel Feedback Queue (MLFQ) with Aging:** 
  * Introduce an **aging parameter**: tracking the wait time of processes in $Q_3$.
  * If a process like $P_5$ stays in $Q_3$ without execution for longer than a predefined threshold time $T_{\text{max}}$, dynamically promote it to $Q_2$ or $Q_1$ so it receives immediate execution.

---

## Q5. Context-Switch Overhead Debugging

### 1. Root Cause Analysis
* **Quantum Too Small Relative to Burst Time:** The CPU time quantum ($1\text{ ms}$) is smaller than the thread's average computation burst ($2\text{ ms}$). Every thread requires at least two full context switches just to complete one CPU burst.
* **High Interruption Rate:** The combination of forced time-slice preemption ($1\text{ ms}$) and frequent voluntary I/O blocks generates $20,000$ context switches/sec, consuming massive CPU cycles purely on saving and restoring thread registers and flushing CPU caches.

### 2. Two Optimizations
1. **Tune Time Quantum Value:** Increase the quantum from $1\text{ ms}$ to **$5\text{ ms} - 10\text{ ms}$**. This allows threads to complete their average $2\text{ ms}$ CPU bursts in a single run without forced preemption.
2. **Implement Thread Pooling with Async I/O:** Replace the excessive number of blocking threads with a fixed-size thread pool running non-blocking worker loops to eliminate unnecessary thread switching overhead.
