#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;
int x = 0;

// Structures for Admin
typedef struct Admin{
    char eventName[50];
    char eventAddress[100];
    int month, day, year;       // Format: MM/DD/YYYY
    int hour[2], min[2]; // Format : HH:MM
} Admin;

// Control Functions for Admin
int adminMenu(){
    int choice;
    printf("Admin Menu:\n");
    printf("\t1. Create Event\n");
    printf("\t2. Delete Event\n");
    printf("\t3. View Event\n");
    printf("\t4. Update/Edit Event\n");
    printf("\t5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int eventExists(FILE *fp, char* eventName) {
    Admin admin;
    char line[200]; // Buffer to hold values of each line
    if(fp == NULL){
        printf("Failed to open file\n");
        return 0;
    }

    while(fgets(line, sizeof(line), fp)){
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        if(strcmp(admin.eventName, eventName) == 0){
            fclose(fp);
            printf("Event already exists\n");
            return 0;
        }
    }
    return 1;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int validateDate(int month, int day, int year) {
    x++;
    if(month < 1 || month > 12) return -1;
    if(day < 1 || day > 31) return -2;
    if(year < 1900 || year > 2100) return -3;
    return 1;
}

int validateTime(int hour, int min) {
    x++;
    if(hour < 0 || hour > 23) return -1;
    if(min < 0 || min > 59) return -2;
    return 1;
}

void createEvent(){
    Admin admin;
    int dateValidationResult;
    int timeValidationResult;

    fp = fopen("gen_info.csv", "a+");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }

    do {
        printf("Enter Event Name: ");
        scanf(" %[^\n]", admin.eventName);
        clearInputBuffer();
    } while(!eventExists(fp, admin.eventName));

    fclose(fp);

    printf("Enter Event Address: ");
    scanf(" %[^\n]", admin.eventAddress);

    x = 0;
    do {
        if (x != 0){
            printf("\033[2A\33[2K");
        }
        printf("Enter Event Date (MM/DD/YYYY): ");
        scanf("%d/%d/%d", &admin.month, &admin.day, &admin.year);
        clearInputBuffer();
        dateValidationResult = validateDate(admin.month, admin.day, admin.year);
        if(dateValidationResult == -1) printf("Invalid month. Please try again.\n");
        else if(dateValidationResult == -2) printf("Invalid day. Please try again.\n");
        else if(dateValidationResult == -3) printf("Invalid year. Please try again.\n");
    } while(dateValidationResult <= 0);

    x = 0;
    printf("\33[2K");
    do {
        if (x != 0){
            printf("\033[2A\33[2K");
        }
        printf("Enter Event Start Time (HH:MM): ");
        scanf("%d:%d", &admin.hour[0], &admin.min[0]);
        clearInputBuffer();
        timeValidationResult = validateTime(admin.hour[0], admin.min[0]);
        if(timeValidationResult == -1) printf("Invalid hour. Please try again.\n");
        else if(timeValidationResult == -2) printf("Invalid minute. Please try again.\n");
    } while(timeValidationResult <= 0);

    x = 0;
    printf("\33[2K");
    do {
        if (x != 0){
            printf("\033[2A\33[2K");
        }
        printf("Enter Event End Time (HH:MM): ");
        scanf("%d:%d", &admin.hour[1], &admin.min[1]);
        clearInputBuffer();
        timeValidationResult = validateTime(admin.hour[1], admin.min[1]);
        if(timeValidationResult == -1) printf("Invalid hour. Please try again.\n");
        else if(timeValidationResult == -2) printf("Invalid minute. Please try again.\n");
    } while(timeValidationResult <= 0);

    char fileName[256];
    sprintf(fileName, "%s.csv", admin.eventName);

    fp = fopen(fileName, "a+");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    fflush(fp);
    int writeCheck = fprintf(fp, "%s,%s,%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, admin.month, admin.day, admin.year, admin.hour[0], admin.min[0], admin.hour[1], admin.min[1]);
    if(writeCheck < 0){
        printf("Failed to write to file\n\n"); 
        return;
    }
    fclose(fp);

    fp = fopen("gen_info.csv", "a+");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    fflush(fp);
    writeCheck = fprintf(fp, "%s,%s\n", admin.eventName, admin.eventName);
    if (writeCheck < 0){
        printf("Failed to write to file\n\n");
        return;
    } else { printf("Event created successfully\n\n"); }

    fclose(fp);
}

void viewEvent(){
    Admin admin;
    char line[200]; // Buffer to hold values of each line

    fp = fopen("gen_info.csv", "r");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }

    while(fgets(line, sizeof(line), fp)){
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        printf("Event Name: %s\n", admin.eventName);
        printf("Event Address: %s\n", admin.eventAddress);
        printf("Event Date: %02d/%02d/%d\n", admin.month, admin.day, admin.year);
        printf("Event Start Time: %02d:%02d\n", admin.hour[0], admin.min[0]);
        printf("Event End Time: %02d:%02d\n", admin.hour[1], admin.min[1]);
        printf("\n");
    }

    fclose(fp);
}

void deleteEvent(){
    Admin admin;
    char line[200]; // Buffer to hold each line
    char eventName[50]; // Name of the event to delete
    int eventNumber; // Number of the event to delete
    int found = 0; // Indicator whether the event was found
    int i = 0;

    // Display all events with their numbers
    fp = fopen("gen_info.csv", "r");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    while(fgets(line, sizeof(line), fp)){
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        printf("%d. %s\n", ++i, admin.eventName);
    }
    fclose(fp);

    // Ask the admin to enter either the name or the number of the event to delete
    printf("Enter the name or the number of the event to delete: ");
    if(scanf("%d", &eventNumber) == 1){
        // The admin entered a number
        fp = fopen("gen_info.csv", "r");
        FILE *temp = fopen("temp.csv", "w");
        i = 0;
        while(fgets(line, sizeof(line), fp)){
            if(++i == eventNumber){
                found = 1;
            }else{
                fprintf(temp, "%s", line);
            }
        }
        fclose(fp);
        fclose(temp);
    }else{
        // The admin entered a name
        scanf("%s", eventName);
        fp = fopen("gen_info.csv", "r");
        FILE *temp = fopen("temp.csv", "w");
        while(fgets(line, sizeof(line), fp)){
            sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
            if(strcmp(admin.eventName, eventName) == 0){
                found = 1;
            }else{
                fprintf(temp, "%s", line);
            }
        }
        fclose(fp);
        fclose(temp);
    }

    // Delete the old file and rename the temporary file
    remove("gen_info.csv");
    rename("temp.csv", "gen_info.csv");

    if(found){
        printf("Event deleted successfully\n");
    }else{
        printf("Event not found\n");
    }
}

void editEvent(){
    Admin admin;
    char line[200]; // Buffer to hold each line
    char eventName[50]; // Name of the event to edit
    int eventNumber; // Number of the event to edit
    int found = 0; // Indicator whether the event was found
    int i = 0;

    // Display all events with their numbers
    fp = fopen("gen_info.csv", "r");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    while(fgets(line, sizeof(line), fp)){
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        printf("%d. %s\n", ++i, admin.eventName);
    }
    fclose(fp);

    // Ask the admin to enter either the name or the number of the event to edit
    printf("Enter the name or the number of the event to edit: ");
    if(scanf("%d", &eventNumber) == 1){
        // The admin entered a number
        fp = fopen("gen_info.csv", "r");
        FILE *temp = fopen("temp.csv", "w");
        i = 0;
        while(fgets(line, sizeof(line), fp)){
        }
    }else{
        // The admin entered a name
        scanf("%s", eventName);
        while(fgets(line, sizeof(line), fp)){
            if (strcmp(line, eventName) == 0){
                found = 1;
                break;
            }
            
        }
    }
}

// Main Admin Function
void adminMain(){
    int choice;
    do{
        choice = adminMenu();
        switch(choice){
            case 1:
                // Create Event
                createEvent();
                break;
            case 2:
                // Delete Event
                deleteEvent();
                break;
            case 3:
                // View Event
                viewEvent();
                break;
            case 4:
                // Edit Event
                editEvent();
                break;
            case 5:
                break;
            default:
                printf("Invalid Choice\n");
        }
    }while(choice != 5);
}

// Choose between Admin and User
int chooseMode(){
    int choice;
    printf("Choose Mode:\n");
    printf("\t1. Admin\n");
    printf("\t2. User\n");
    printf("\t3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

// Main Runtime Function
int main(){
    int choice;
    do{
        choice = chooseMode();
        switch(choice){
            case 1:
                // Admin
                adminMain();
                break;
            case 2:
                // User
                break;
            case 3:
                // Exit
                return 0;
                break;
            default:
                printf("Invalid Choice\n");
        }
    }while(choice != 3);

    return 0;
}
