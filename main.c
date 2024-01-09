#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

FILE *fp;
int x = 0;
const char *workingDir = "eventholder/";
DIR *dir;

// Structures for Admin
typedef struct Admin{
    char eventName[50];
    char eventAddress[100];
    int month, day, year;       // Format: MM/DD/YYYY
    int hour[2], min[2]; // Format : HH:MM
} Admin;

typedef struct User{
    char name[50];
    int age;
    char sex;           
    char address[100];
    char mobileNum[12];         // 11 Digit number + 1 for null terminator
    char emailAddr[50];
    char code[8];
} User;

// Control Functions for Admin
int adminMenu(){
    int choice;
    printf("Admin Menu:\n");
    printf("\t1. Create Event\n");
    printf("\t2. Delete Event\n");
    printf("\t3. View Event\n");
    printf("\t4. Update/Edit Event\n");
    printf("\t0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int eventExists(FILE *fp, char* eventName) {
    Admin admin;
    char line[200]; // Buffer to hold values of each line

    // fp = fopen("Events.txt", "r");
    if(fp == NULL){
        printf("Failed to open file\n");
        return 0;
    }

    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\n")] = 0;
        sscanf(line, "%s", admin.eventName);
        if(strcmp(admin.eventName, eventName) == 0){
            fclose(fp);
            printf("\nEvent already exists\n\n");
            return 0;
        }
    }
    fclose(fp);
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
    // opendir(workingDir);

    FILE *eventsFile = fopen("Events.txt", "a+");
    if(eventsFile == NULL){
        printf("Failed to open file\n");
        return;
    }
    system("cls");

    do{
        printf("Enter Event Name: ");
        scanf(" %[^\n]", admin.eventName);
        clearInputBuffer();
    }while(!eventExists(eventsFile, admin.eventName));
    fclose(eventsFile);

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

    chdir(workingDir);

    char fileName[256];
    sprintf(fileName, "%s.csv", admin.eventName);

    fp = fopen(fileName, "a+");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    fflush(fp);
    fprintf(fp, "Event Name,Event Address,Event Date,Start Time,End Time\n");
    int writeCheck = fprintf(fp, "%s,%s,%d/%d/%d,%d:%d,%d:%d\n", admin.eventName, admin.eventAddress, admin.month, admin.day, admin.year, admin.hour[0], admin.min[0], admin.hour[1], admin.min[1]);
    fprintf(fp, "\nName,Age,Sex,Address,Mobile Number, Email Address");

    if(writeCheck < 0){
        printf("Failed to write to file\n\n"); 
        return;
    }
    if(fclose(fp) != 0){
        printf("Failed to close %s\n", fileName);
        return;
    }

    chdir("..");

    eventsFile = fopen("Events.txt", "a+");
    if(eventsFile == NULL){
        printf("Failed to open file\n");
        return;
    }
    fflush(eventsFile);
    writeCheck = fprintf(eventsFile, "%s\n", admin.eventName);
    fclose(eventsFile);
    if (writeCheck < 0){
        printf("Failed to write to file\n\n");
        return;
    } else { 
        system("cls");
        printf("Event created successfully\n\n"); 
    }

}

void viewEvent(){
    Admin admin;
    char line[250], eventName[100][100]; // Buffer to hold values of each line
    int countEvents = 0, choice, firstLine = 1; // Number of events
    chdir(workingDir);

    fp = fopen("Events.txt", "r");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    system("cls");
    printf("Events:\n");
    while(fgets(line, sizeof(line), fp)){
        sscanf(line, "%[^,]", admin.eventName);
        admin.eventName[strcspn(admin.eventName, "\n")] = 0;
        printf("\t%d. %s", (countEvents + 1), admin.eventName);
        printf("\n");
        strcpy(eventName[countEvents], admin.eventName);
        countEvents++;
    }

    fclose(fp);

    printf("\t%d. Exit\nEnter Option: ", (countEvents + 1));
    scanf("%d", &choice);
    if(choice == (countEvents + 1)){
        return;
    }
    system("cls");

    for(int x = 0; x < countEvents; x++){
        if(x == (choice - 1)){
            char fileName[256];
            sprintf(fileName, "%s.csv", eventName[x]);
            fp = fopen(fileName, "r");
            while(fgets(line, sizeof(line), fp)){
                if(firstLine){
                    firstLine = 0;
                    continue;
                }
                else {
                    sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
                    printf("\nEvent Name: %s\n", admin.eventName);
                    printf("Event Address: %s\n", admin.eventAddress);
                    printf("Event Date: %d/%d/%d\n", admin.month, admin.day, admin.year);
                    printf("Event Start Time: %d:%d\n", admin.hour[0], admin.min[0]);
                    printf("Event End Time: %d:%d\n\n", admin.hour[1], admin.min[1]);
                    break;
                }
            }
            fclose(fp);
            break;
        }
    }
}

void deleteEvent(){
    Admin admin;
    char line[200]; // Buffer to hold each line
    char eventName[50]; // Name of the event to delete
    int eventNumber; // Number of the event to delete
    int found = 0; // Indicator whether the event was found
    int i = 0;

    chdir(workingDir);

    system("cls"); 
    // Display all events with their numbers
    fp = fopen("Events.txt", "r");
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

        DIR *dir;
        int i;
        struct dirent *entry;

        dir = opendir(".");
        if(dir == NULL){
            perror("Unable to open directory!");;
            exit(EXIT_FAILURE);
        }

        i = 1;
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(dot && strcmp(dot, ".csv") == 0){
                    size_t length = dot - entry->d_name;
                    // printf("%d\t%.*s\n", i, (int)length, entry->d_name);
                    remove(entry->d_name);
                    break;
                    i++;
                }
            }
        }
        closedir(dir);

        fp = fopen("Events.txt", "r");
        FILE *temp = fopen("temp.txt", "w");
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
        fp = fopen("Events.txt", "r");
        FILE *temp = fopen("temp.txt", "w");
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
    remove("Events.txt");
    rename("temp.txt", "Events.txt");
    system("cls");
    if(found){
        printf("Event deleted successfully\n\n");
    }else{
        printf("Event not found\n\n");
    }
}

void editEvent() {
    Admin admin;
    char line[200]; // Buffer to hold each line
    char eventName[50]; // Name of the event to edit
    int eventNumber; // Number of the event to edit
    int found = 0; // Indicator whether the event was found
    int i = 0;

    chdir(workingDir);
    dir = opendir(".");
    if(dir == NULL){
        perror("Unable to open directory!");;
        exit(EXIT_FAILURE);
    }

    // Display all events with their numbers
    fp = fopen("Events.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        printf("%d. %s\n", ++i, admin.eventName);
    }
    fclose(fp);

    // Ask the admin to enter either the name or the number of the event to edit
    printf("Enter the name or the number of the event to edit: ");

    if (scanf("%d", &eventNumber) == 1) {
        // The admin entered a number
        struct dirent *entry;
        i = 1;

        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(dot && strcmp(dot, ".csv") == 0){
                    size_t length = dot - entry->d_name;
                    // printf("%d\t%.*s\n", i, (int)length, entry->d_name);
                    char csvFileName[100];
                    sprintf(csvFileName, "%.*s.csv", (int)length, entry->d_name);

                    // Open the CSV file
                    FILE *csvFile = fopen(csvFileName, "r+");
                    if(csvFile == NULL){
                        perror("Unable to open .csv file!");
                        exit(EXIT_FAILURE);
                    }
                    fgets(line, sizeof(line), csvFile); // Skip the first line of csv
                    // Read the existing details from the .csv file
                    fgets(line, sizeof(line), csvFile);
                    sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
                    fclose(csvFile);

                    // Prompt the user for new details
                    printf("Enter new event details:\n");
                    printf("Event Name: ");
                    scanf(" %[^\n]", admin.eventName);
                    printf("Event Address: ");
                    scanf(" %[^\n]", admin.eventAddress);

                    // Move the file pointer to the beginning of the second line
                    fseek(csvFile, strlen(line) * -1, SEEK_CUR);

                    // Write the new details to the .csv file
                    fprintf(csvFile, "%s,%s,%d/%d/%d,%d:%d,%d:%d\n", admin.eventName, admin.eventAddress, admin.month, admin.day, admin.year, admin.hour[0], admin.min[0], admin.hour[1], admin.min[1]);
                    fclose(csvFile);
                    fclose(csvFile);
                    found = 1;
                    break;
                    // i++;
                }
            }
        }
        
        fp = fopen("Events.txt", "r");
        FILE *temp = fopen("temp.txt", "w");
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
    } else {
        // The admin entered a name
        fp = fopen("Events.txt", "r");

        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, eventName) != NULL) {
                found = 1;
                break;
            }
        }

        if (found) {
            // Modify the event details as needed
            // For example, you can prompt the admin to enter new details

            printf("Enter new event details:\n");
            printf("Event Name: ");
            scanf("%s", admin.eventName);
            printf("Event Address: ");
            scanf("%s", admin.eventAddress);

            // Update the line with the modified details
            fclose(fp);

            char csvFileName[100];
            sprintf(csvFileName, "%s.csv", admin.eventName);
            fp = fopen(csvFileName, "w");
            fprintf(fp, "%s,%s,%d/%d/%d,%d:%d,%d:%d\n", admin.eventName, admin.eventAddress, admin.month, admin.day, admin.year, admin.hour[0], admin.min[0], admin.hour[1], admin.min[1]);
        } else {
            printf("Event not found\n");
        }

        fclose(fp);
    }
    closedir(dir);
}

// Main Admin Function
void adminMain(){
    int choice;
    do{
        // system("cls");
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
            case 0:
                break;
            default:
                printf("Invalid Choice\n");
        }
    }while(choice != 0);
}

// Choose between Admin and User
int chooseMode(){
    int choice;
    printf("\nChoose Mode:\n");
    printf("\t1. Admin\n");
    printf("\t2. User\n");
    printf("\t0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

// Control Functions for User
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

// Main Runtime Function
int main(){
    int choice;
    // system("cls");
    do{
        choice = chooseMode();
        switch(choice){
            case 1:
                // Admin
                // system("cls");
                adminMain();
                break;
            case 2:
                // User
                system("cls");
                userMenu();
                break;
            case 0:
                // Exit
                return 0;
                break;
            default:
                printf("Invalid Choice\n");
        }
    }while(choice != 3);

    return 0;
}