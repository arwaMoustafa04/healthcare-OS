#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_PATH 512
#define MAX_CMD 1024

// Validation: Checks if ID is 'P' followed by 4 digits
int valid_id(char *id){ 
    if ((strlen(id) != 5) || (id[0] != 'P'))
        return 0;
    for (int i = 1; i < 5; i++){
        if (!isdigit(id[i]))
            return 0;
    }
    return 1;
}

// Validation: Checks if name contains only letters and spaces
int valid_name(char *name){
    if (strlen(name) < 2) return 0;
    for (int i = 0; name[i]; i++){
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

void trim_newline(char *str){
    str[strcspn(str, "\n")] = 0;
}

// OS Initialization: Calls the Bash script
void init_os_filesystem(){
    printf("[SYSTEM] Initializing Healthcare OS Environment...\n");
    system("chmod +x setup.sh");
    if (system("./setup.sh") != 0){
        printf("[ERROR] Failed to initialize file system via Bash script.\n");
    }
}

void create_patient() {
    char id[10], name[50];
    char path[MAX_PATH], logpath[MAX_PATH + 20];
    FILE *fp;
    struct dirent *entry;

    printf("Enter Patient ID (Pxxxx): ");
    scanf("%9s", id);
    while (getchar() != '\n'); 

    char base_path[MAX_PATH];
    snprintf(base_path, sizeof(base_path), "%s/hospital_data/patients", getenv("HOME"));
    
    DIR *dir = opendir(base_path);
    if (dir != NULL){
        while ((entry = readdir(dir)) != NULL){
            // Check if any existing folder starts with the same ID
            if (strncmp(entry->d_name, id, 5) == 0){      // strncmp compares the first 5 characters
                printf("Error: Patient ID %s is already assigned to an existing record (%s).\n", id, entry->d_name);
                closedir(dir);
                return;
            }
        }
        closedir(dir);
    }

    printf("Enter Patient Name: ");
    fgets(name, sizeof(name), stdin);
    trim_newline(name);

    if (!valid_id(id) || !valid_name(name)) {
        printf("Error: Invalid ID format or Name.\n");
        return;
    }

    snprintf(path, sizeof(path), "%s/hospital_data/patients/%s_%s", getenv("HOME"), id, name);

    if (mkdir(path, 0700) == 0) {
        snprintf(logpath, sizeof(logpath), "%s/monitor.log", path);
        fp = fopen(logpath, "w");
        if (fp) {
            fprintf(fp, "[LOG START] Medical record created for: %s\n", name);
            fclose(fp);
        }
        printf("Patient record for [%s] created successfully.\n", id);
    } else {
        perror("System Error");
    }
}

void delete_patient(){
    char folder[100];
    char path[MAX_PATH], cmd[MAX_CMD];

    printf("Enter exact patient folder name (ID_Name): ");
    while (getchar() != '\n'); 
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    if (strchr(folder, '/') != NULL){
        printf("Invalid folder name.\n");
        return;
    }

    snprintf(path, sizeof(path), "%s/hospital_data/patients/%s", getenv("HOME"), folder);

    if(access(path, F_OK) != 0){
        printf("Error: Patient folder not found.\n");
        return;
    }

    snprintf(cmd, sizeof(cmd), "rm -rf \"%s\"", path);
    system(cmd);
    printf("Patient record deleted.\n");
}

void archive_patient(){
    char folder[100], cmd[MAX_CMD];
    char *home = getenv("HOME");

    printf("Enter exact patient folder name to archive: ");
    while (getchar() != '\n'); 
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    snprintf(cmd, sizeof(cmd), "mv \"%s/hospital_data/patients/%s\" \"%s/hospital_data/archive/\" 2>/dev/null", home, folder, home);

    if (system(cmd) == 0) {
        printf("Patient moved to secure archive.\n");
    } else {
        printf("Error: Archive failed. Check folder name.\n");
    }
}

void list_patients() {
    char path[MAX_PATH];
    struct dirent *entry;
    int count = 0;
    
    snprintf(path, sizeof(path), "%s/hospital_data/patients", getenv("HOME"));

    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("\n[ERROR] Patient directory not found. Please create a patient first.\n");
        return;
    }

    // Peek inside to see if there are actual patient folders
    while ((entry = readdir(dir)) != NULL) {
        // Skip the hidden "." and ".." directories
        if (entry->d_name[0] != '.') {
            count++;
        }
    }

    if (count == 0)
        printf("\n[INFO] The patient list is currently empty.\n");
    else{
        rewinddir(dir);    // Reset directory pointer to the start to list them properly
        printf("\n--- ACTIVE PATIENT RECORDS (%d) ---\n", count);
        while ((entry = readdir(dir)) != NULL){
            if (entry->d_name[0] != '.')
                printf(" - %s\n", entry->d_name);
        }
        printf("------------------------------\n");
    }
    closedir(dir);
}

int main() {
    int choice;
    init_os_filesystem();

    while (1) {
        printf("\n=== Healthcare OS: Patient Management ===\n");
        printf("1. Register New Patient\n");
        printf("2. Delete Record\n");
        printf("3. Archive Record\n");
        printf("4. View All Patients\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: create_patient(); break;
            case 2: delete_patient(); break;
            case 3: archive_patient(); break;
            case 4: list_patients(); break;
            case 5: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}