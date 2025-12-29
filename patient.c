#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_PATH 512
#define MAX_FILE_PATH 540
#define MAX_CMD 600

int valid_id(char *id){
    if ((strlen(id) != 5) || (id[0] != 'P')) 
        return 0;
    for (int i = 1; i < 5; i++){
        if (!isdigit(id[i]))
            return 0;
    }
    return 1;
}

int valid_name(char *name){
    if (strlen(name) < 2)
        return 0;
    for(int i = 0; name[i]; i++){
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

void trim_newline(char *str){
    str[strcspn(str, "\n")] = 0;
}

void init_system(){
    struct stat st = {0};
    if (stat("Patients", &st) == -1){
        mkdir("Patients", 0700);
        printf("[SYSTEM] Main 'Patients' directory created.\n");
    }
}

void create_patient(){
    char id[10], name[50], age[5], condition[100];
    char patient_dir[MAX_PATH];
    char info_file[MAX_FILE_PATH]; 
    struct dirent *entry;

    printf("Enter Patient ID (Pxxxx): ");
    scanf("%9s", id);
    while (getchar() != '\n'); 

    DIR *dir = opendir("Patients");
    if(dir != NULL){
        while ((entry = readdir(dir)) != NULL){
            if (strncmp(entry->d_name, id, 5) == 0){
                printf("Error: ID %s already exists.\n", id);
                closedir(dir);
                return;
            }
        }
        closedir(dir);
    }

    printf("Enter Patient Name: ");
    fgets(name, sizeof(name), stdin);
    trim_newline(name);

    if(!valid_id(id) || !valid_name(name)){
        printf("Error: Invalid ID or Name format.\n");
        return;
    }

    printf("Enter Age: ");
    scanf("%4s", age);
    getchar();

    printf("Enter Condition: ");
    fgets(condition, sizeof(condition), stdin);
    trim_newline(condition);

    snprintf(patient_dir, sizeof(patient_dir), "Patients/%s_%s", id, name);

    if(mkdir(patient_dir, 0700) == 0){
        snprintf(info_file, sizeof(info_file), "%s/info.txt", patient_dir);
        FILE *fp = fopen(info_file, "w");
        if (fp){
            fprintf(fp, "ID: %s\nName: %s\nAge: %s\nCondition: %s\n", id, name, age, condition);
            fclose(fp);
            printf("Success: Record created.\n");
        }
    } 
    else
        perror("Error creating directory");
}

void view_patient(){
    char folder[100], filepath[MAX_FILE_PATH];
    char line[256];

    printf("Enter exact folder name to view (ID_Name): ");
    while (getchar() != '\n');
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    snprintf(filepath, sizeof(filepath), "Patients/%s/info.txt", folder);
    
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL){
        printf("Error: Could not find record for %s.\n", folder);
        return;
    }

    printf("\n--- Patient Medical Information ---\n");
    while (fgets(line, sizeof(line), fp)){
        printf("%s", line);
    }
    printf("-----------------------------------\n");
    fclose(fp);
}

void delete_patient(){
    char folder[100], path[MAX_PATH], cmd[MAX_CMD];
    printf("Enter folder name to delete (ID_Name): ");
    while (getchar() != '\n');
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    snprintf(path, sizeof(path), "Patients/%s", folder);
    
    if(access(path, F_OK) == 0){
        snprintf(cmd, sizeof(cmd), "rm -rf \"%s\"", path);
        system(cmd);
        printf("Patient record deleted.\n");
    } 
    else
        printf("Error: Folder not found.\n");
}

void list_patients(){
    struct dirent *entry;
    DIR *dir = opendir("Patients");
    int count = 0;

    if (dir == NULL)
        return;

    while ((entry = readdir(dir)) != NULL){
        if (entry->d_name[0] != '.') 
            count++;
    }

    if (count == 0)
        printf("\nThe patient list is currently empty.\n");
    else{
        rewinddir(dir);
        printf("\n--- REGISTERED PATIENT DIRECTORIES ---\n");
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') printf("[%s]\n", entry->d_name);
        }
    }
    closedir(dir);
}

int main(){
    int choice;
    init_system();

    while (1) {
        printf("\n=== Hospital Management System ===\n");
        printf("1. Create Patient Record\n");
        printf("2. View Patient Details\n");
        printf("3. Delete Patient Record\n");
        printf("4. List All Patients\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1){
            while (getchar() != '\n');
            continue;
        }

        switch (choice){
            case 1:
                create_patient(); 
                break;
            case 2:
                view_patient();
                break;
            case 3:
                delete_patient();
                break;
            case 4:
                list_patients(); 
                break;
            case 5: 
                exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}