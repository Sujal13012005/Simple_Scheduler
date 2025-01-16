IMPLEMENTATION OF A SIMPLE SCHEDULER  
This \`SimpleScheduler\` code implements a basic process scheduling simulation in C, using a multi-queue structure with priority levels and a shared memory region for managing queues. The scheduler aims to handle process submission, command execution, and scheduling across different priority queues. The system is composed of a linked list-based queue structure, shared memory, semaphores for concurrency control, and various utility functions to manage process states, priorities, and command history.

\#\#\# Key Components

1\. \*\*Data Structures\*\*:  
   \- The main data structures include a \`Queue\` struct to represent each priority level and a \`QueueNode\` for each individual process. A \`HistoryEntry\` struct logs process information like command, process ID, priority, execution, and wait times.

   \- Each queue has a front and rear pointer for efficient insertion and removal of processes. When a process is added, it enters the rear, and when removed, it leaves from the front, following FIFO (first-in-first-out) behavior.

2\. \*\*Queue Operations\*\*:  
   \- The scheduler provides functions for creating nodes and initializing queues. Each node holds data for a single process, including PID, command, priority, and execution time. 

   \- The \`addProcessToQueue\` function enqueues a process at the rear of the specified priority queue, while \`removeFirstNode\` dequeues from the front. The \`displayQueueElements\` function iterates through nodes to display all queue processes, useful for debugging or user feedback.

3\. \*\*Shared Memory and Semaphores\*\*:  
   \- Shared memory is allocated and mapped for holding all four priority queues, allowing different processes to share the same memory space.  
   \- Semaphores are used for mutual exclusion, ensuring that when multiple processes access the shared memory (e.g., for queue updates), they don’t cause data inconsistency. Functions like \`initializeSemaphore\` and \`initializeSharedMemory\` handle the setup of these components.

4\. \*\*User Command Handling\*\*:  
   \- Users can submit commands with an optional priority level. Commands are processed with \`processSubmission\`, which forks a child process to execute the command. In the parent process, the command and metadata are added to the appropriate queue based on priority.  
   \- The code includes mechanisms to support basic command validation and command history logging, which are beneficial for tracking and debugging process execution.

5\. \*\*Process Scheduling\*\*:  
   \- The main scheduling function, \`scheduleProcesses\`, dequeues processes from the highest priority queue that contains processes, executing them for a defined time quantum. After each cycle, if a process isn’t complete, it’s re-enqueued at a lower priority level. Completed processes are removed and logged.  
   \- The function manages time slicing with \`usleep\`, and \`SIGSTOP\` signals are used to suspend processes temporarily. A re-queueing mechanism pushes incomplete processes to lower priority queues, following a round-robin approach to optimize CPU allocation.

6\. \*\*Cleanup and Exit\*\*:  
   \- On receiving a signal (e.g., \`SIGINT\`), the scheduler releases shared memory and semaphore resources. The \`cleanupSharedResources\` function ensures safe deallocation, preventing memory leaks.

\#\#\# Execution Flow

1\. The program begins with parsing command-line arguments for CPU count and time slice, essential parameters for time-slicing in the scheduling function.  
2\. The main function initializes shared memory, semaphore, and the four priority queues. A loop then continuously reads user input, allowing users to submit new commands, run the scheduler, or exit.  
3\. Upon command submission, a child process is forked, executing the command while the parent process manages the scheduling and re-queueing of active processes according to their priority.  
4\. The scheduler manages queues based on priority. Processes in higher-priority queues are executed first, and if they exceed the time slice, they’re moved down to the next queue.

\#\#\# Error Handling and Concurrency

Error handling includes checks for memory allocation, shared memory access, and command execution failures, ensuring robust execution under various scenarios.

Gitbub:

https://github.com/utk-2-004/Osassignment3

Contributions:  
Utkarsh: Constructtheschedulerportion,signalhandler,launch,initilaisewaitime,calling waittime,preandlaunchshcedular, Create a shared memory in scheduler ,priority.  
Sujal Chaudhary:  
 createqueue,dequeue,printqueue,command line argument,addtohistory,printhistory,documentation,error check.

