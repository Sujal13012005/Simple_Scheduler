#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

typedef struct {
    char* command;
    pid_t pid;
    int priority;
    double execution_time;
    double wait_time;
} HistoryEntry;

void (histent* history, char* command, pid_t pid, int priority, int index);

void pfhist(histent* his, int hind);

struct Queue* q1;
struct Queue* q2;
struct Queue* q3;
struct Queue* q4;


// Struct to hold queue entry data
typedef struct QueueNodeData {
    pid_t pid;
    char *command;
    int state;
    int priority;
    double execution_time;
} QueueNodeData;

// Define the node structure for a linked list queue
typedef struct QueueNode {
    QueueNodeData data;
    struct QueueNode *nextNode;
} QueueNode;

// Queue structure with pointers to the front and rear nodes
typedef struct Queue {
    QueueNode *firstNode;
    QueueNode *lastNode;
} Queue;

// Function to initialize a new QueueNode with given values
QueueNode *createNewNode(int processID, char *cmd, int priorityLevel, double execTime) {
    // Allocate and verify memory for the new node
    QueueNode *newNode = malloc(sizeof(QueueNode));
    if (!newNode) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the node data and link to the next node as NULL
    newNode->data.pid = processID;
    newNode->data.command = cmd;
    newNode->data.priority = priorityLevel;
    newNode->data.execution_time = execTime;
    newNode->nextNode = NULL;
    
    return newNode;
}


    // Initialize the data part of the node
    node->data.pid = pid;
    node->data.command = command;
    node->data.state = 0;  // Default state as before
    node->data.priority = priority;
    node->data.execution_time = execution_time;

    // Initialize the next pointer as NULL
    node->next = NULL;

    return node;
}


// A utility function to create an empty queue with the new structure
Queue* initializeQueue() {
    // Allocate memory for the Queue structure
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    
    if (queue == NULL) { 
        perror("malloc");
        exit(EXIT_FAILURE); 
    }

    // Set front and rear pointers to NULL, indicating an empty queue
    queue->front = NULL;
    queue->rear = NULL;

    return queue;
}


// Add a process to the end of the queue
void addProcessToQueue(Queue* queue, int processID, char* cmd, int prio, double execTime) {
    // Allocate memory and initialize a new node
    QNode* newProcessNode = initializeNode(processID, cmd, prio, execTime);

    if (!newProcessNode) {
        fprintf(stderr, "Error: Unable to allocate memory for process node.\n");
        return;
    }

    // Handle case for an empty queue
    if (!queue->front) {
        queue->front = newProcessNode;
        queue->rear = newProcessNode;
    } else {
        // Link the new node to the end and update the rear pointer
        queue->rear->next = newProcessNode;
        queue->rear = newProcessNode;
    }
}


// Removes and returns the first node from the queue
QNode* removeFirstNode(Queue* queue) {
    // Check if queue is empty
    if (!queue->front) {
        fprintf(stderr, "Error: Attempt to dequeue from an empty queue.\n");
        return NULL;
    }

    // Capture the front node and shift the front pointer
    QNode* firstNode = queue->front;
    queue->front = firstNode->next;

    // Update rear pointer if the queue becomes empty after removal
    if (!queue->front) {
        queue->rear = NULL;
    }

    return firstNode;
}


// Function to determine if the queue has no elements
bool isQueueEmpty(const Queue* queue) {
    return queue->front == NULL;
}

// Function to display all nodes in the queue
void displayQueueElements(const Queue* queue);

typedef struct {
    sem_t lock;
    Queue primaryQueue;
    Queue secondaryQueue;
    Queue tertiaryQueue;
    Queue quaternaryQueue;
} SharedMemory;


int checkforpriority(char* input);

void signal_handler(int signum);

// Reads user input from the console
char* acquireInputFromUser();

// Executes a specific command
void executeProcess(char* fullCommand);

// Outputs the elements in multiple queues sequentially
void displayQueues(const Queue queues[], size_t queueCount);

int findSmallest(int firstVal, int secondVal);

// Manages process scheduling with details and event tracking
void scheduleProcesses(int* activeProcesses, int cpuCores, double timeQuantum, Queue* primaryQueue, Queue* backupQueue, int* activeQueueNumber, histent eventsLog[], int logIdx);

// Configures the initial waiting time parameters in the event log
void initializeWaitParameters(histent eventsLog[], int logIdx, int priorityLevel, double timeQuantum, int previousProcessCount);

// Calculates and updates waiting times based on the queue and time quantum
void updateWaitTimes(Queue* queue, double timeQuantum, int cpuCores, histent eventsLog[], int logIdx);

int countProcesses_Low = 0;
int countProcesses_Medium = 0;
int countProcesses_High = 0;
int countProcesses_Critical = 0;

// Prepares the scheduler configuration and launches the scheduling mechanism
void setupAndExecuteScheduler(Queue queues[], size_t queueCount, double timeQuantum, int cpuCores, histent eventsLog[], int logIdx);

int main(int argc, char *argv[]) {
    // Define and allocate the required queues
    Queue *queues[4];
    for (int i = 0; i < 4; i++) {
        queues[i] = createNewQueue();
    }

    // Verify argument count and display usage information if incorrect
    if (argc != 3) {
        fprintf(stderr, "Incorrect usage.\nExpected format: %s <Number of CPUs> <Time Slice>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse and validate command-line arguments
    int cpuCount = strtol(argv[1], NULL, 10);
    double timeSlice = strtod(argv[2], NULL);

    if (cpuCount <= 0 || timeSlice <= 0) {
        fprintf(stderr, "Invalid arguments: CPU count and time slice must be positive.\n");
        return EXIT_FAILURE;
    }

    // Set up signal handling for termination
    signal(SIGINT, handleSignalInterrupt);

    // Shared memory setup
    int sharedMemoryDescriptor;
    shm_t* sharedMemorySegment;

    // The remaining initialization code would follow here
    // including shared memory allocation and other setups
    ...

    // Function to initialize shared memory
int initializeSharedMemory(shm_t** sharedMemory) {
    // Open a new shared memory object
    int sharedMemoryDescriptor = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    if (sharedMemoryDescriptor < 0) {
        fprintf(stderr, "Error: Unable to open shared memory segment.\n");
        return EXIT_FAILURE;
    }

    // Define the size of the shared memory segment
    if (ftruncate(sharedMemoryDescriptor, sizeof(shm_t)) == -1) {
        perror("Error setting size of shared memory segment");
        close(sharedMemoryDescriptor);
        return EXIT_FAILURE;
    }

    // Map the shared memory object into the process's address space
    *sharedMemory = (shm_t*) mmap(NULL, sizeof(shm_t), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryDescriptor, 0);
    if (*sharedMemory == MAP_FAILED) {
        perror("Error mapping shared memory");
        close(sharedMemoryDescriptor);
        return EXIT_FAILURE;
    }

    return sharedMemoryDescriptor; // Return the descriptor for further use
}

// Call the function in the main
int main(int argc, char *argv[]) {
    shm_t* sharedMemoryPtr;
    int shm_fd = initializeSharedMemory(&sharedMemoryPtr);
    if (shm_fd < 0) {
        return EXIT_FAILURE; // Early exit on failure
    }

    // Additional code would follow here to utilize the shared memory
}


void initializeSemaphore(sem_t* semaphore) {
    if (sem_init(semaphore, 1, 1) == -1) {
        perror("Failed to initialize semaphore");
        exit(EXIT_FAILURE);
    }
}

void createAndStoreQueues(shm_t* sharedMemory, struct Queue** queues) {
    for (int i = 0; i < 4; i++) {
        queues[i] = createQueue();
        memcpy(&((sharedMemory->q1) + i), queues[i], sizeof(struct Queue));  // Storing each queue in shared memory
    }
}

void handleUserInput(shm_t* sharedMemory, struct Queue** queues, double timeSlice, int numCpus, histent history[], int* historyIndex) {
    char* userCommand;

    while (true) {
        userCommand = userinput();
        
        if (strcmp(userCommand, "exit") == 0) {
            free(userCommand);
            break;
        }

        if (strcmp(userCommand, "run") == 0) {
            sem_wait(&sharedMemory->mutex);
            while (anyQueueNotEmpty(queues)) {
                prepAndLaunch_schduler(queues[0], queues[1], queues[2], queues[3], timeSlice, numCpus, history, *historyIndex);
            }
            sem_post(&sharedMemory->mutex);
        } else if (strncmp(userCommand, "submit ", 7) != 0) {
            printf("Wrong Input Method!\n");
        } else {
            processSubmission(userCommand, queues, history, historyIndex);
        }

        free(userCommand); // Freeing memory for user input
    }
}

bool anyQueueNotEmpty(struct Queue** queues) {
    for (int i = 0; i < 4; i++) {
        if (!isEmpty(queues[i])) {
            return true;
        }
    }
    return false;
}

void processSubmission(char* input, struct Queue** queues, histent history[], int* historyIndex) {
    int priority = 1; // Default priority
    char* commandToRun = extractCommand(input, &priority);
    
    pid_t processId = fork();
    
    if (processId == 0) { // Child process
        launch(commandToRun);
        exit(0);
    } else if (processId > 0) { // Parent process
        handleParentProcess(processId, commandToRun, priority, queues, history, historyIndex);
    } else {
        perror("Forking error");
    }
}

char* extractCommand(char* input, int* priority) {
    int commandStart = 7; // After "submit "
    char* command;
    
    if (input[strlen(input) - 1] >= '1' && input[strlen(input) - 1] <= '4') {
        *priority = input[strlen(input) - 1] - '0'; // Last character indicates priority
        int length = strlen(input) - 8; // Length of command
        command = (char*)malloc(length + 1);
        strncpy(command, input + commandStart, length);
        command[length] = '\0';
    } else {
        command = strdup(input + commandStart); // No priority specified
    }

    return command;
}

void handleParentProcess(pid_t processId, char* command, int priority, struct Queue** queues, histent history[], int* historyIndex) {
    kill(processId, SIGSTOP); // Stop the process initially
    addToHistory(history, command, processId, priority, historyIndex);
    
    switch (priority) {
        case 1:
            enQueue(queues[0], processId, command, priority, 0);
            numProcess1++;
            break;
        case 2:
            enQueue(queues[1], processId, command, priority, 0);
            numProcess2++;
            break;
        case 3:
            enQueue(queues[2], processId, command, priority, 0);
            numProcess3++;
            break;
        default:
            enQueue(queues[3], processId, command, priority, 0);
            numProcess4++;
            break;
    }
}

int main(int argc, char *argv[]) {
    // Initialize the semaphore
    initializeSemaphore(&shm->mutex);
    
    struct Queue* queues[4];
    createAndStoreQueues(shm, queues);
    
    int historySize = 100;
    histent history[historySize];
    int historyIndex = 0;
    
    handleUserInput(shm, queues, TSLICE, NCPU, history, &historyIndex);

    pfhist(history, historyIndex);
}

    void cleanupSharedResources(shm_t* sharedMemory, int sharedMemFd) {
    munmap(sharedMemory, sizeof(shm_t));
    close(sharedMemFd);
    shm_unlink("/my_shm");
    sem_destroy(&sharedMemory->mutex);
}

int minimum(int a, int b) {
    return (a < b) ? a : b;
}

int hasPriority(char* command) {
    int commandLength = strlen(command);
    return (command[commandLength - 2] == ' ' && isdigit(command[commandLength - 1]));
}

void logHistoryEntry(histent* history, const char* command, pid_t processId, int priority, int index) {
    // Allocate memory for the command string and set the history entry
    history[index].command = (char*)malloc(strlen(command) + 1); // Allocate memory
    if (history[index].command != NULL) {
        strcpy(history[index].command, command); // Copy command string
    }
    history[index].pid = processId; // Set PID
    history[index].priority = priority; // Set priority
}

void displayHistory(HistoryEntry *history, int totalEntries) {
    // Print header for command history output
    printf("\nDisplaying Command History:\n(Command, PID, Execution Time, Waiting Time)\n");
    printf("*****************************************\n");

    // Loop through each history entry
    int entryIndex = 0;
    while (entryIndex < totalEntries) {
        HistoryEntry currentEntry = history[entryIndex];
        
        // Display the entry details
        printf("%d: %s      %u      %.3f ms     %.3f ms\n",
               entryIndex + 1, currentEntry.command, currentEntry.pid, 
               currentEntry.execution_time, currentEntry.wait_time);

        // Free the allocated memory for command after use
        free(currentEntry.command);

        // Move to the next entry
        entryIndex++;
    }

    printf("*****************************************\n");
}


void setInitialWaitTimes(HistoryEntry history[], int totalEntries, int targetPriority, double sliceDuration, int precedingProcessCount) {
    // Set wait times for entries with the matching priority level
    int entryIdx = 0;
    while (entryIdx < totalEntries) {
        HistoryEntry *entry = &history[entryIdx];

        // Check if entry has the specified priority
        if (entry->priority == targetPriority) {
            entry->wait_time = precedingProcessCount * sliceDuration;  // Assign computed wait time
        }

        // Proceed to the next entry
        entryIdx++;
    }
}


void calculateWaitTimes(struct Queue *queue, double sliceDuration, int numCPUs, HistoryEntry history[], int historySize) {
    // Start at the front of the queue and initialize position tracker
    int queuePosition = 0;
    QNode *currentNode = queue->front;

    // Iterate through the queue nodes
    while (currentNode != NULL) {
        int entryIdx = 0;
        
        // Search for a matching PID in the history
        while (entryIdx < historySize) {
            if (currentNode->pid == history[entryIdx].pid) {
                // Update the wait time based on position and CPU count
                history[entryIdx].wait_time += (queuePosition / (double)numCPUs) * sliceDuration;
                break;  // Exit loop once the correct PID is found
            }
            entryIdx++;
        }

        // Move to the next queue node and increment position
        currentNode = currentNode->next;
        queuePosition++;
    }
}


char* getUserInput() {
    int bufferSize = 100;
    char* commandBuffer = malloc(sizeof(char) * bufferSize);
    
    if (commandBuffer == NULL) {
        fprintf(stderr, "Memory allocation failed. Try again.\n");
        commandBuffer[0] = '\0';  // Safeguard
        return commandBuffer;
    }

    printf("SimpleShell$ ");
    fgets(commandBuffer, bufferSize, stdin);
    commandBuffer[strcspn(commandBuffer, "\n")] = '\0'; // Remove newline

    return commandBuffer;
}

void displayQueue(struct Queue* queue) {
    QNode* currentNode = queue->front;
    printf("Queue %d Elements (PID, Command, Priority): ", currentNode->priority);
    while (currentNode != NULL) {
        printf("(%d, %s) ", currentNode->pid, currentNode->command);
        currentNode = currentNode->next;
    }
    printf("\n");
}

void displayAllQueues(struct Queue *queueArray[]) {
    printf("\n*****************************************\n");

    // Initialize index for the while loop
    int index = 0;
    while (index < 4) {
        if (!isEmpty(queueArray[index])) {
            displayQueue(queueArray[index]);
            printf("\n");
        }
        index++; // Increment the index
    }

    printf("*****************************************\n\n");
}


void executeCommand(char *cmd) {
    // Attempt to execute the given command
    int execStatus = execlp(cmd, cmd, NULL);
    if (execStatus == -1) {
        fprintf(stderr, "Error: Command not found or not executable: %s\n", cmd);
        perror("Execution failed (execlp)");
    }
}

void handleSignal(int sigNumber) {
    // Handle Ctrl+C signal
    printf("\nDetected Ctrl+C usage.\n");
    
    // Check if any queues contain processes and exit if true
    if (!isEmpty(q1) || !isEmpty(q2) || !isEmpty(q3) || !isEmpty(q4)) {
        exit(EXIT_SUCCESS);
    }
}


void processScheduler(int *totalProcesses, int cpuLimit, double quantumTime, struct Queue *currentQueue, struct Queue *followingQueue, int *followingQueueCount, HistoryEntry recordHistory[], int recordCount) {
    // Loop to manage tasks in the current queue
    while (!isEmpty(currentQueue)) {
        int activeProcesses = minimum((*totalProcesses), cpuLimit);

        // Process each task within the current cycle
        for (int taskIndex = 0; taskIndex < activeProcesses; taskIndex++) {
            QNode *taskNode = deQueue(currentQueue);
            pid_t taskPID = fork();

            if (taskPID == 0) {
                // Execution begins in child process
                printf("Child process %u initiated\n", taskNode->pid);
                kill(taskNode->pid, SIGCONT);
                exit(EXIT_SUCCESS);
            } else if (processID > 0) {
                usleep(timeQuantum * 1000); // Convert time slice to milliseconds and sleep
                kill(node->pid, SIGSTOP);
                node->execution_time += timeQuantum;

                int waitStatus;
                int waitCheck = waitpid(node->pid, &waitStatus, WNOHANG);
                if (waitCheck == 0) {
                    // If process is not yet complete, handle priority and re-queueing
                    if (node->priority < 4) {
                        node->priority++;
                        enQueue(nextQueue, node->pid, node->command, node->priority, node->execution_time);
                        (*procCount)--;
                        (*nextQueueProcCount)++;
                    } else {
                        enQueue(curQueue, node->pid, node->command, node->priority, node->execution_time);
                    }
                } else {
                    // Process completed, update history
                    (*procCount)--;
                    int histIdx = 0;
                    while (histIdx < histSize) {
                        if (history[histIdx].pid == node->pid) {
                            history[histIdx].execution_time = node->execution_time;
                            break;
                        }
                        histIdx++;
                    }
                }
            } else {
                perror("Fork error occurred");
            }
        }

        // If queue still contains tasks, display its contents
        if (!isEmpty(curQueue)) {
            printf("\n");
            displayQueue(curQueue);
            printf("\n");
        }
    }
}

void prepareAndLaunchScheduler(struct Queue *allQueues[], double sliceDuration, int numCPUs, HistoryEntry historyRecords[], int historySize) {
    // Display number of processes in each queue level
    printf("Queue 1 process count: %d\n", numProcess1);
    printf("Queue 2 process count: %d\n", numProcess2);
    printf("Queue 3 process count: %d\n", numProcess3);
    printf("Queue 4 process count: %d\n", numProcess4);

    // Initialize wait times for each priority level
    int initialWait1 = 0;
    int initialWait2 = numProcess1 / numCPUs;
    int initialWait3 = (numProcess2 + numProcess1) / numCPUs;
    int initialWait4 = (numProcess3 + numProcess2 + numProcess1) / numCPUs;

    // Call wait time setting function for each priority
    setInitialWaitTimes(historyRecords, historySize, 1, sliceDuration, initialWait1);
    setInitialWaitTimes(historyRecords, historySize, 2, sliceDuration, initialWait2);
    setInitialWaitTimes(historyRecords, historySize, 3, sliceDuration, initialWait3);
    setInitialWaitTimes(historyRecords, historySize, 4, sliceDuration, initialWait4);
}

    displayAllQueues(queues);
    calculateWaitTimes(queues[0], timeSlice, cpuCount, history, count);
    processScheduler(&numProcess1, cpuCount, timeSlice, queues[0], queues[1], &numProcess2, history, count);

    displayAllQueues(queues);
    calculateWaitTimes(queues[1], timeSlice, cpuCount, history, count);
    processScheduler(&numProcess2, cpuCount, timeSlice, queues[1], queues[2], &numProcess3, history, count);

    displayAllQueues(queues);
    calculateWaitTimes(queues[2], timeSlice, cpuCount, history, count);
    processScheduler(&numProcess3, cpuCount, timeSlice, queues[2], queues[3], &numProcess4, history, count);

    displayAllQueues(queues);
    calculateWaitTimes(queues[3], timeSlice, cpuCount, history, count);
    processScheduler(&numProcess4, cpuCount, timeSlice, queues[3], queues[0], &numProcess1, history, count);
    
    displayHistory(history, count);
    exit(0);
}
