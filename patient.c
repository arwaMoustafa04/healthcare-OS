#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// I used a large buffer so the string doesn't get cut off
#define BUFFER_SIZE 2048 

// Function to check if ID is valid
int is_id_valid(char id[]){
    // Must be 5 characters long and start with P
    if (strlen(id) != 5 || id[0] != 'P'){
        return 0;
    }
    // Check if the rest are numbers
    for (int i = 1; i < 5; i++){
        if (!isdigit(id[i])) return 0;
    }
    return 1;
}

// Function to remove the extra space at the end of fgets
void remove_newline(char str[]){
    str[strcspn(str, "\n")] = 0;
}

void add_new_patient(){
    char id[10], name[50], age[10], cond[100], my_cmd[BUFFER_SIZE];

    printf("Enter Patient ID (Pxxxx): ");
    scanf("%s", id);
    getchar(); // clear the buffer

    if (is_id_valid(id) == 0){
        printf("Error: Invalid ID format. Use P followed by 4 digits.\n");
        return;
    }

    printf("Enter Name: ");
    fgets(name, 50, stdin);
    remove_newline(name);

    printf("Enter Age: ");
    scanf("%s", age);
    getchar();

    printf("Enter Primary Condition: ");
    fgets(cond, 100, stdin);
    remove_newline(cond);

    // This part builds the command to run the bash script
    sprintf(my_cmd, "./manage_patient.sh create %s \"%s\" %s \"%s\"", id, name, age, cond);
    system(my_cmd);
}

void view_record(){
    char folder[100], my_cmd[BUFFER_SIZE];
    printf("Enter the exact folder name to view (ID_Name): ");
    getchar();
    fgets(folder, 100, stdin);
    remove_newline(folder);

    sprintf(my_cmd, "./manage_patient.sh view \"%s\"", folder);
    system(my_cmd);
}

void delete_record(){
    char folder[100], my_cmd[BUFFER_SIZE];
    printf("Enter folder name to delete (ID_Name): ");
    getchar();
    fgets(folder, 100, stdin);
    remove_newline(folder);

    sprintf(my_cmd, "./manage_patient.sh delete \"%s\"", folder);
    system(my_cmd);
}

void show_all(){
    system("./manage_patient.sh list");
}

int main(){
    int user_choice;
 
    // Makes sure the bash file is ready to run
    system("chmod +x manage_patient.sh");

    while (1){
        printf("\n=== Healthcare OS: Patient Management ===\n");
        printf("1. Register New Patient\n");
        printf("2. View Patient Medical Info\n");
        printf("3. Delete Patient Record\n");
        printf("4. List All Patient Folders\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if(scanf("%d", &user_choice) != 1){
            while (getchar() != '\n'); 
            continue;
        }

        switch (user_choice){
            case 1:
                add_new_patient();
                break;
            case 2:
                view_record();
                break;
            case 3: 
                delete_record();
                break;
            case 4:
                show_all(); 
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