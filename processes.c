#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t patient_pid = -1;

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
}

void resume_process(){
    if(patient_pid <= 0){
        printf("No patient monitoring to resume\n");
        return;
    }

    kill(patient_pid, SIGCONT);
    printf("Patient monitoring resumed\n");
}

void kill_process(){
    if(patient_pid <= 0){
        printf("No patient monitoring process to kill\n");
        return;
    }

    kill(patient_pid, SIGTERM);
    waitpid(patient_pid, NULL, 0);

    printf("Patient monitoring stopped and cleaned up\n");
    patient_pid = -1;
}


int main(){


    return 0;
}