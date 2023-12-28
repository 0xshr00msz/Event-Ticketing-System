#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

// Structures for Admin
typedef struct Admin{
    char eventName[50];
    char eventAddress[50];
    int month, day, year;       // Format: MM/DD/YYYY
    // Time Variable
} Admin;

// Structure for User
typedef struct User{
    char name[50];
    int age;
    char sex;
    char address[100];
    char mobileNumber[12];   // 11 digit number + 1 for null terminator - 09xxxxxxxxx
    char emailAddress[50];
} User;

// Display Events listed inside the events directory
int displayEvents(){
    char path[50] = "events/";

    printf("=====================================================\n");
    printf("\nWelcome to XY Event Reservation and Ticketing System\n");
    printf("\n=====================================================\n");
    printf("Listed below are the events\n\n");

    DIR *dir = opendir(path);
    if(dir == NULL){
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    int i = 0;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0){
            if(entry->d_type == DT_DIR){
                printf("%d\t%s\n", i, entry->d_name);
            }
            i++;
        }
    }
    closedir(dir);
    return EXIT_SUCCESS;
}

int saveEvents(){
    Admin admin;
    User user;
    DIR *dir;
    struct dirent *entry;
    int i = 0;
    char filename[256];

    // Open the directory
    dir = opendir("/eventholder");
    if (dir == NULL) {
        // Handle error
        return;
    }

    sprintf(filename, "/eventholder/%s.csv", admin.eventName);

    // Open the CSV file in write mode
    FILE *file = fopen("/eventholder/admin.eventname", "w");
    if (file == NULL) {
        // Handle error
        closedir(dir);
        return;
    }

    // Write the header to the CSV file
    fprintf(file, "Index, %s\n", admin.eventName);

    // Read directory entries
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0){
            if(entry->d_type == DT_DIR){
                // Write the data to the CSV file
                fprintf(file, "%d,%s\n", i, entry->d_name);
            }
            i++;
        }
    }

    // Close the CSV file
    fclose(file);

    // Close the directory
    closedir(dir);

    return 0;
}

int main(){
    // Display Events listed inside the events directory
    displayEvents();
}

int main(){

    // Display Events listed inside the events directory
    displayEvents();

}
