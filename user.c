#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

// Global variable
const char *workingDir = "eventholder/";
DIR * dir;   // dot means current directory

typedef struct User{
    char name[50];
    int age;
    char sex;           
    char address[100];
    char mobileNum[12];         // 11 Digit number + 1 for null terminator
    char emailAddr[50];
    char code[8];
} User;

void displayEvents(){
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

void generateCode(char *buffer) {
    srand(time(NULL));

    // Generate 3 random letters (A-Z)
    for (int i = 0; i < 3; i++) {
        buffer[i] = 'A' + rand() % 26;
    }

    // Generate 4 random numbers (0-9)
    for (int i = 3; i < 7; i++) {
        buffer[i] = '0' + rand() % 10;
    }

    // Null-terminate the string
    buffer[7] = '\0';
}

void appendToFile(char *fileName, User *userDetails){
    char eventName[256];
    strncpy(eventName, fileName + strlen("eventholder/"), sizeof(eventName));
    char *dot = strrchr(eventName, '.');
    if(dot != NULL){
        *dot = '\0';  // Remove the .csv extension
    }

    FILE *file = fopen(fileName, "a+");
    if(file == NULL){
        printf("Unable to open file!");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "\n%s,%d,%c,%s,%s,%s,%s", 
        userDetails->name,
        userDetails->age,
        userDetails->sex,
        userDetails->address,
        userDetails->mobileNum,
        userDetails->emailAddr,
        userDetails->code
    );
    fflush(file);
    printf("\nSuccessfully appended to file\n");
    fclose(file);

    sprintf(fileName, "receipts/%s.txt", userDetails->name);
    file = fopen(fileName, "w");

    fprintf(file, "=============================================================\n");
    fprintf(file, "Event Name: %s\n", eventName);  // Use the extracted event name
    fprintf(file, "Name: %s\n", userDetails->name);
    fprintf(file, "Age: %d\n", userDetails->age);
    fprintf(file, "Sex: %c\n", userDetails->sex);
    fprintf(file, "Address: %s\n", userDetails->address);
    fprintf(file, "Mobile Number: %s\n", userDetails->mobileNum);
    fprintf(file, "Email Address: %s\n", userDetails->emailAddr);
    fprintf(file, "Ticket Number: %s\n", userDetails->code);
    fprintf(file, "=============================================================\n");

    fclose(file);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int isValidName(char *name) {
    // Check if the name is not empty and only contains alphabetic characters.
    if (strlen(name) == 0) return 0;
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int isValidAge(int age) {
    // Check if the age is between 1 and 120.
    return age >= 1 && age <= 120;
}

int isValidSex(char sex) {
    // Check if the sex is 'M' or 'F' (case insensitive).
    return toupper(sex) == 'M' || toupper(sex) == 'F';
}

int isValidAddress(char *address) {
    // Check if the address is not empty and is less than 100 characters.
    return strlen(address) > 0 && strlen(address) < 100;
}

int isValidMobileNum(char *mobileNum) {
    // Check if the mobile number is 11 digits and starts with a valid prefix (e.g., '09').
    return strlen(mobileNum) == 11 && mobileNum[0] == '0' && mobileNum[1] == '9';
}

int isValidEmailAddr(char *emailAddr) {
    // Check if the email address contains '@' and '.'.
    char *atSign = strchr(emailAddr, '@');
    char *dot = strchr(emailAddr, '.');
    return atSign != NULL && dot != NULL;
}

void inputUserDetails(int choice){
    
    int reEnter = 1;

    if(choice != 0){
        char fileName[50];
        DIR *dir = opendir(workingDir);
        struct dirent *entry;
        int i = 1;

        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(i == choice){
                    snprintf(fileName, sizeof(fileName), "%s%s", workingDir, entry->d_name);
                    break;
                }
                i++;
            }
        }
        closedir(dir);

        User newUser;
        while (reEnter == 1)
        {
            printf("\nPlease enter your information:\n");
            int x = 0;
            do {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }                
                printf("Name: ");
                scanf(" %[^\n]", newUser.name);
                if (!isValidName(newUser.name)) {
                    printf("Invalid Input!\n");
                    x++;
                }
            } while (!isValidName(newUser.name));

            x = 0;
            printf("\33[2K");
            do {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }
                printf("Age: ");
                scanf("%d", &newUser.age);
                if (!isValidAge(newUser.age)) {
                    printf("Invalid Input!\n");
                    x++;
                }
            } while (!isValidAge(newUser.age));

            x = 0;
            printf("\33[2K");
            do {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }
                printf("Sex (M/F): ");
                scanf(" %c", &newUser.sex);
                clearInputBuffer();
                if (!isValidSex(newUser.sex)) {
                    printf("Invalid Input!\n");
                    x++;
                }
            } while (!isValidSex(newUser.sex));

            x = 0;
            printf("\33[2K");    
            do {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }
                printf("Address: ");
                scanf(" %[^\n]", newUser.address);
                if (!isValidAddress(newUser.address)) {
                    printf("Invalid Input!\n");
                    x++;
                }
            } while (!isValidAddress(newUser.address));

            x = 0;
            printf("\33[2K");    
            do {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }
                printf("Mobile Number: ");
                scanf("%s", newUser.mobileNum);
                if (!isValidMobileNum(newUser.mobileNum)) {
                    printf("Invalid Input!\n");
                    x++;
                }
            } while (!isValidMobileNum(newUser.mobileNum));

            x = 0;
            printf("\33[2K");    
            do {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }
                printf("Email Address: ");
                scanf("%s", newUser.emailAddr);
                if (!isValidEmailAddr(newUser.emailAddr)) {
                    printf("Invalid Input!\n");
                    x++;
                }
            } while (!isValidEmailAddr(newUser.emailAddr));
            generateCode(newUser.code);
  
            printf("\nDo you want to re-enter your information?:\n");
            printf("\t [1] Yes\n");
            printf("\t [0] No\n");
            x = 0;
            printf("\33[2K");
            do
            {
                if (x != 0){
                    printf("\033[2A\33[2K");
                }                
                printf("Enter your choice: ");
                scanf("%d", &reEnter);
                if (reEnter == 0){
                    appendToFile(fileName, &newUser);
                }
                else if (reEnter != 1 && reEnter != 0) {
                    printf("INVALID INPUT!\n");
                    x++;
                }
            } while (reEnter != 1 && reEnter != 0);          
        }
    }
}

int getUserChoice() {
    int again;
    int x = 0;
    printf("\nDo you want to continue adding a participant?:\n");
    printf("\t [1] Yes\n");
    printf("\t [0] No\n");
    do {
        if (x != 0){
            printf("\033[2A\33[2K");
        }
        printf("Enter your choice: ");
        scanf("%d", &again);
        if (again != 1 && again != 0) {
            printf("INVALID INPUT!\n");
            x++;
        }
    } while (again != 1 && again != 0);
    return again;
}

void userMenu(){
    int again = 1;
    int choice;
    do
    {
        displayEvents();
        choice = selectEvent();
        inputUserDetails(choice);
        again = getUserChoice();
        if (again == 1){
            system("cls");
        }
        if (again == 0){
            printf("\nThank you for using our system!\n");
        }
    } while (again == 1);
}

int main(){
    userMenu();
}