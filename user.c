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
    DIR *dir;
    struct dirent *entry;
    char filename[256];


    // Open dir to read the csv of the Event
    dir = opendir("events/");
    if (dir == NULL) {
        // Handle error
        perror("Error opening directory");
        return EXIT_FAILURE;
    }
    const char *fileName = "eventholder/data.csv";

    // Open the CSV file for events in read mode
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Could not open file %s\n", fileName);
        return 1;
    }

    // Scan the CSV file for the event name, so scan for the first element for each line
    char buffer[1024];
    while (fscanf(file, "%1023[^,],", buffer) == 1) {
    printf("Read: %s\n", buffer);
}


    // Put them in an array to be made for each fileanme of the to-be-made csv
    // using sprintf, create a filename for each event
    // Create csv then put them in eventholder directory
    // Open each CSV file in write mode
    // Write all the user info in each event CSV file
    // Close each CSV file

    // Open the directory
    dir = opendir("eventholder/");
    if (dir == NULL) {
        // Handle error
        perror("Error opening directory");
        return EXIT_FAILURE;
    }
    
    sprintf(filename, "eventholder/%s.csv", "DenielDave");

    // Open the CSV file in write mode
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        // Handle error
        closedir(dir);
        return EXIT_FAILURE;
    }


    // Read directory entries
    int i = 0;
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

    return EXIT_SUCCESS;
}

int main(){
    // Display Events listed inside the events directory
    saveEvents();
    // displayEvents();
}


