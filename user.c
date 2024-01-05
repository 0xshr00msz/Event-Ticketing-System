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
    printf("\n");
    closedir(dir);
}

void userMenu(){
    displayEvents();
}

int main(){
    displayEvents();
}