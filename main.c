#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<direct.h>
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


int selectEvent(int index){
    int choice;
    printf("Select an event: ");
    scanf("%d", &choice);

    if(choice < 0 || choice > index){
        printf("\nInvalid choice. Please try again.\n");
        return selectEvent(index);
    }
    return choice;
}

int goToEvent(int choice, int index) {
    char buffer[1024];

    char *directory = "events/";
    _chdir(directory);

    DIR *dir;
    struct dirent *entry;

    if((dir = opendir(".")) != NULL){
        int i = 1;
        while ((entry = readdir(dir)) != NULL) {
            // Process each entry
            if (entry->d_type == DT_DIR) {
                // If the entry is a directory, change the working directory
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    if(i == choice){
                         printf("Changing working directory to: %s\n", entry->d_name);
                        _chdir(entry->d_name);
                        break;
                    }
                   i++;
                }
            }
        }
        closedir(dir);

        if (_getcwd(buffer, 1024) == NULL) {
            printf("Get current working directory failed!\n");
            return 1;
        }
        printf("\nCurrent directory: \n%s\n\n", buffer);
    } else {
        perror("Unable to open directory\n");
        return 1;
    }
}

void userMenu(){
    char path[25] = "events/";  // Path to events directory

    printf("=====================================================\n");
    printf("\nWelcome to XY Event Reservation and Ticketing System\n");
    printf("\n=====================================================\n");
    printf("Listed below are the events\n\n");

    DIR *dir = opendir(path);
    if(dir == NULL){
        printf("Error opening directory");
        exit(1);
    }

    struct dirent *entry;
    int index = 1;
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0){
            if(entry->d_type == DT_DIR){
                printf("%d\t%s\n", index, entry->d_name);
            }
            index++;
        }
    }
    printf("%d\tExit\n\n", 0);
    closedir(dir);

    int choice = selectEvent(index - 1);

    goToEvent(choice, index - 1);

}

int main(){
    userMenu();
}

