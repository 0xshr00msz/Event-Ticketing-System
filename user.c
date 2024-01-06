#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

// Global variable
const char *workingDir = ".";   // dot means current directory

typedef struct User{
    char name[50];
    int age;
    char sex;           
    char address[100];
    char mobileNum[12];         // 11 Digit number + 1 for null terminator
    char emailAddr[50];
} User;

void displayEvents(){
    DIR * dir;
    int i;
    struct dirent *entry;

    dir = opendir(workingDir);
    if(dir == NULL){
        perror("Unable to open directory!");;
        exit(EXIT_FAILURE);
    }

    printf("\nEvents that are accessible:\n\n", workingDir);
    i = 1;
    while((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG){
            const char *dot = strrchr(entry->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                size_t length = dot - entry->d_name;
                printf("%d\t%.*s\n", i, (int)length, entry->d_name);
                i++;
            }
        }
    }
    printf("0\tExit\n\n");
    closedir(dir);
}

int selectEvent(){
    int eventNum;
    printf("Select an event: ");
    scanf("%d", &eventNum);
    return eventNum;
}

void appendToFile(char *fileName, User *userDetails){
    FILE *file = fopen(fileName, "a+");
    if(file == NULL){
        printf("Unable to open file!");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "\n%s,%d,%c,%s,%s,%s", 
        userDetails->name,
        userDetails->age,
        userDetails->sex,
        userDetails->address,
        userDetails->mobileNum,
        userDetails->emailAddr
    );
}

void inputUserDetails(int choice){
    
    if(choice != 0){
        char fileName[50];
        DIR *dir = opendir(workingDir);
        struct dirent *entry;
        int i = 1;

        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(i == choice){
                    snprintf(fileName, sizeof(fileName), "%s/%s", workingDir, entry->d_name);
                    break;
                }
                i++;
            }
        }
        closedir(dir);

        User newUser;

        printf("Please enter your information:\n");
        printf("Name: ");
        scanf(" %[^\n]", newUser.name);
        printf("Age: ");
        scanf("%d", &newUser.age);
        printf("Sex (M/F): ");
        scanf(" %c", &newUser.sex);
        printf("Address: ");
        scanf(" %[^\n]", newUser.address);
        printf("Mobile Number: ");
        scanf("%s", newUser.mobileNum);
        printf("Email Address: ");
        scanf("%s", newUser.emailAddr);

        // Append user data to the selected file
        appendToFile(fileName, &newUser);
    }
    
}

void userMenu(){
    int flag = 1;
    int choice;

    displayEvents();
    choice = selectEvent();
    inputUserDetails(choice);
}

int main(){
    userMenu();
}