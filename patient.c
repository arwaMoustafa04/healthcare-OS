#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CMD_BUFFER 2048 // I used a large buffer to ensure 'snprintf' never truncates

int valid_id(char *id){
    if ((strlen(id) != 5) || (id[0] != 'P'))
        return 0;
    for (int i = 1; i < 5; i++){
        if (!isdigit(id[i]))
            return 0;
    }
    return 1;
}

void trim_newline(char *str){
    str[strcspn(str, "\n")] = 0;
}

void create_patient(){
    char id[10], name[50], age[10], condition[100], cmd[CMD_BUFFER];

    printf("Enter Patient ID (Pxxxx): ");
    scanf("%9s", id);
    while (getchar() != '\n'); // clear buffer

    if (!valid_id(id)){
        printf("Error: Invalid ID format. Use P followed by 4 digits.\n");
        return;
    }

    printf("Enter Patient Name: ");
    fgets(name, sizeof(name), stdin);
    trim_newline(name);

    printf("Enter Age: ");
    scanf("%9s", age);
    getchar();

    printf("Enter Primary Condition: ");
    fgets(condition, sizeof(condition), stdin);
    trim_newline(condition);

    // Call the bash script: Pass variables as arguments
    snprintf(cmd, sizeof(cmd), "./manage_patient.sh create %s \"%s\" %s \"%s\"", id, name, age, condition);
    system(cmd);
}

void view_patient(){
    char folder[100], cmd[CMD_BUFFER];
    printf("Enter the exact folder name to view (ID_Name): ");
    while (getchar() != '\n');
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    snprintf(cmd, sizeof(cmd), "./manage_patient.sh view \"%s\"", folder);
    system(cmd);
}

void delete_patient(){
    char folder[100], cmd[CMD_BUFFER];
    printf("Enter folder name to delete (ID_Name): ");
    while (getchar() != '\n');
    fgets(folder, sizeof(folder), stdin);
    trim_newline(folder);

    if(strchr(folder, '/') != NULL){
        printf("Error: Invalid folder path.\n");
        return;
    }

    snprintf(cmd, sizeof(cmd), "./manage_patient.sh delete \"%s\"", folder);
    system(cmd);
}

void list_patients(){
    system("./manage_patient.sh list");
}

int main(){
    int choice;
 
    system("chmod +x manage_patient.sh");

    while (1){
        printf("\n=== Healthcare OS: Patient Management ===\n");
        printf("1. Register New Patient\n");
        printf("2. View Patient Medical Info\n");
        printf("3. Delete Patient Record\n");
        printf("4. List All Patient Folders\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if(scanf("%d", &choice) != 1){
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
                printf("OS shutting down... Done.\n"); 
                exit(0);
            default: 
                printf("Invalid choice. Please select 1-5.\n");
        }
    }
    return 0;
}