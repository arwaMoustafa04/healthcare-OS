#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t patient_pid = -1;

void log_event(const char *message) {
    char cmd[300];
    snprintf(cmd, sizeof(cmd), "cd .. && echo \"[%s] %s\" >> ./logs/patients.log", "$(date '+%Y-%m-%d %H:%M:%S')", message);
    system(cmd);
}

void start_process(){
    if(patient_pid > 0){
        printf("Patient monitoring already running\n");
        return;
    }

    patient_pid = fork();

    if(patient_pid == 0){
        setpgid(0,0);

        while(1){
            printf("Monitoring patient vitals...\n");
            sleep(1);
        }
    }
    else if(patient_pid > 0){
        printf("Patient monitoring started (PID: %d)\n", patient_pid);
        log_event("Patient monitoring process started");
    }
    else{
        perror("fork failed");
    }
}

void stop_process(){
    if(patient_pid <=0){
        printf("No patient monitoring process to stop\n");
        return;
    }

    kill(patient_pid, SIGSTOP);
    printf("patient monitoring paused\n");
    log_event("Patient monitoring process paused (SIGSTOP)");
}

void resume_process(){
    if(patient_pid <= 0){
        printf("No patient monitoring to resume\n");
        return;
    }

    kill(patient_pid, SIGCONT);
    printf("Patient monitoring resumed\n");
    log_event("Patient monitoring process resumed (SIGCONT)");
}

void kill_process(){
    if(patient_pid <= 0){
        printf("No patient monitoring process to kill\n");
        return;
    }

    kill(patient_pid, SIGTERM);
    waitpid(patient_pid, NULL, 0);
    log_event("Patient monitoring process terminated (SIGTERM)");

    printf("Patient monitoring stopped and cleaned up\n");
    patient_pid = -1;
}

void wait_patient() {
    if (patient_pid <= 0) {
        printf("No patient process to wait for\n");
        return;
    }

    int status;
    pid_t result = waitpid(patient_pid, &status, 0);

    if (result == -1) {
        perror("waitpid failed");
        return;
    }

    if (WIFEXITED(status)) {
        printf("Patient process exited normally with status %d\n", WEXITSTATUS(status));
         log_event("Patient process exited normally");
    } 
    else if (WIFSIGNALED(status)) {
        printf("Patient process terminated by signal %d\n", WTERMSIG(status));
        log_event("Patient process terminated by signal");
    }

    patient_pid = -1; 
}


int main() {
    char input[100];

    system("clear"); 
    printf("Welcome to HealthcareOS\n");
    printf("Type 'help' for a list of commands\n");

    while (1) {

        printf("\nHealthcareOS> ");

        // 2. Read Input
        if (fgets(input, 100, stdin) == NULL) {
            break; // Handle exit on Ctrl+D
        }

        // Remove the newline character (\n) at the end of input
        input[strcspn(input, "\n")] = 0;

        // 3. Input Validation & Command Handling
        if (strcmp(input, "shutdown") == 0){
            printf("System shutting down...\n");
            log_event("HealthcareOS shutting down");
            if (patient_pid > 0) {
                kill_process();
            }
            break;
        } 
        else if (strcmp(input, "system_status") == 0) {
            system("./system_status.sh");
        } 
        else if (strncmp(input, "patient_find", 12) == 0) {
            // We need to pass the argument (name) to the script
            // Construct the full command: ./commands/patient_find.sh <name>
            char command[150];
            
            // Safe way to format the string
            // Input is "patient_find John", we want just "John" (part after space)
            char *name = input + 13; 
            
            if (strlen(name) > 0) {
                sprintf(command, "./patient_find.sh \"%s\"", name);
                system(command);
            } else {
                printf("Error: Name required. Usage: patient_find <name>\n");
            }
        }
        else if(strcmp(input, "start_process") == 0){
            start_process();
        }
        else if(strcmp(input, "stop_process") == 0){
            stop_process();
        } 
        else if(strcmp(input, "resume_process") == 0){
            resume_process();
        } 
        else if(strcmp(input, "kill_process") == 0){
            kill_process();
        } 
        else if(strcmp(input, "clear") == 0){
            system("clear");
        } 
        else if(strcmp(input, "memory_map") == 0){
            system("gcc memory.c -o mem");
            system("./mem");
        }
        else if(strcmp(input, "disk_check") == 0){
            printf("--- Checking Database Storage Capacity ---\n");
            system("df -h"); // -h makes it human-readable
        }
        else if (strncmp(input, "check_deps", 10) == 0) {
        char *filename = input + 11; // Skip "check_deps " to get the file path

        char command[256];
        // We construct the command: ./memory_manager.sh deps <filename>
        snprintf(command, sizeof(command), "./memory_manager.sh %s", filename);
        system(command);
        }
        else if (strcmp(input, "vitals_mem") == 0) {
        printf("--- Monitoring System Vitals (RAM) ---\n");
        system("vmstat 3 2"); // Runs vmstat every 3 seconds, 2 times
        }
        else if (strcmp(input, "patient_wait") == 0) {
            wait_patient();
        }
        else if (strcmp(input, "help") == 0) {
            printf("Available Commands:\n");
            printf("patient_find <name> : Search for a patient\n");
            printf("system_status : Check OS health\n");
            printf("start_process : Start patient monitoring\n");
            printf("stop_process : Pause monitoring\n");
            printf("resume_process : Resume monitoring\n");
            printf("kill_process : Stop monitoring\n");
            printf("patient_wait : Wait for patient process to finish\n");
            printf("memory_map : Inspect memory segments (Text, Data, Heap, Stack)\n");
            printf("disk_check : Check hospital database storage\n");
            printf("check_deps <file> : Check process dependencies\n");
            printf("vitals_mem : Monitor system memory health\n");
            printf("clear : Clear screen\n");
            printf("shutdown : Exit the OS\n");
        }
        else {
            printf("Unknown command: %s\n", input);
        }
    }

    return 0;
}
