#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

#define MAX_FILES 200
#define MAX_LINE 100
#define MAX_COL 100

FILE *fp;                                           // Universal file pointer variable       
int x = 0;
const char *workingDir = "eventholder/";            // Directory for the events  
DIR *dir;                                           // Universal directory pointer variable
char *filenames[MAX_FILES];

// Structures for Admin
typedef struct Admin{
    char eventName[50];
    char eventAddress[100];
    int month, day, year;       // Format: MM/DD/YYYY
    int hour[2], min[2];        // Format : HH:MM
} Admin;

// Structures of Users
typedef struct User{
    char name[50];
    int age;                    // Age of the User
    char sex;                   // Sex of the User
    char address[100];          // Address of the User
    char mobileNum[12];         // 11 Digit number + 1 for null terminator
    char emailAddr[50];         // User Email Address
    char code[8];               // Receipt Code
} User;

// FUNCTION PROTOTYPES
int chooseMode();
// Function Prototypes - ADMIN
void adminMain();
int adminMenu();
int eventExists(char *eventName);
void clearInputBuffer();
int validateDate(int month, int day, int year);
int validateTime(int hour, int min);
void createEvent();
void viewEvent();
void deleteEvent();
void editEvent();
// Function Prototypes - USER
void userMenu();
void displayEvents();
int selectEvent();
void generateCode(char *buffer);
void appendToFile(char *fileName, User *userDetails);
int isValidName(char *name);
int isValidAge(int age);
int isValidSex(char sex);
int isValidAddress(char *address);
int isvalidEmailAddr(char *emailAddr);
void inputUserDetails(int choice);
int getUserChoice();
int ticketfilter();
void filterArray(char arr[MAX_FILES][MAX_LINE], int size, char *filter);
// Main Runtime Function
int main(){
    int choice;
    // system("cls");
    do{
        // Get user choice through chooseMode() function
        choice = chooseMode();
        switch(choice){
            case 1:
                // Admin
                system("cls");
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

// Choose between Admin and User
int chooseMode(){
    int choice;
    char password[50];
    char correctPassword[] = "admin"; // Replace with your actual password

    system("cls");
    printf("Choose Mode:\n");
    printf("\t[1] Admin\n");
    printf("\t[2] User\n");
    printf("\t[0] Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    if(choice == 1) {
        printf("Enter password: ");
        scanf("%s", password);
        if(strcmp(password, correctPassword) != 0) {
            printf("Incorrect password. Please try again.\n");
            return -1;
        }
    }
    return choice;
}
// Control Functions for Admin
// Admin Menu for Admin section of the program - returns the value of choice of the admin user
int adminMenu() {
    int choice;
    printf("Admin Menu:\n");
    printf("\t[1] Create Event\n");
    printf("\t[2] Delete Event\n");
    printf("\t[3] View Event\n");
    printf("\t[4] Update/Edit Event\n");
    printf("\t[5] Ticket Filter\n");
    printf("\t[0] Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

int eventExists(char* eventName) {
   struct dirent *entry;
   
   int numFiles = 0;

   dir = opendir(workingDir);
   if(dir == NULL){
        perror("Unable to open folder");
        return 1;
    }

    // 2. Read all the file name of the .csv files
    while((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG){
            const char *dot = strrchr(entry->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                // 3. Store them in an array
                size_t length = dot - entry->d_name;
                char *filenameWithoutExtension = malloc(length + 1);
                strncpy(filenameWithoutExtension, entry->d_name, length);
                filenameWithoutExtension[length] = '\0'; // Null-terminate the string
                filenames[numFiles] = filenameWithoutExtension;
                numFiles++;
                if(numFiles >= MAX_FILES){
                    printf("Too many files!\n");
                    break;
                }
            }
        }
    }
    closedir(dir);

    // 4. Compare to user input
    for(int i = 0; i < numFiles; i++){
        if(strcmp(filenames[i], eventName) == 0){
            printf("File found!\n");
            return 0;
        }
    }
    // Free the memory allocated for the file names
    for(int i = 0; i < numFiles; i++){
        free(filenames[i]);
    }
    return 1;
}

// This function clears the input buffer by reading and discarding characters until a newline or EOF is encountered.
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
// Validates the given month, day, and year values. Returns 1 if valid,
// -1 for an invalid month, -2 for an invalid day, and -3 for an invalid year.
int validateDate(int month, int day, int year) {
    x++;
    if(month < 1 || month > 12) return -1;
    if(day < 1 || day > 31) return -2;
    if(year < 1900 || year > 2100) return -3;
    return 1;
}
// Validates the given hour and minute values. Returns 1 if valid, -1 for invalid hour, and -2 for invalid minute.
int validateTime(int hour, int min) {
    x++;
    if(hour < 0 || hour > 23) return -1;
    if(min < 0 || min > 59) return -2;
    return 1;
}
// This function is for admin users who wants to create an event
void createEvent(){
    Admin admin;
    int dateValidationResult;
    int timeValidationResult;

    dir = opendir(workingDir);
    if(dir == NULL){
        perror("Unable to open folder");
        exit(1);
    }
    system("cls");
    // Loop to ensure a unique event name by checking against existing events in "Events.txt".
    printf("Creating an Event:\n");
    do{
        printf("Enter Event Name: ");
        scanf(" %[^\n]", admin.eventName);
        clearInputBuffer();
    }while(!eventExists(admin.eventName));

    printf("Enter Event Address: ");
    scanf(" %[^\n]", admin.eventAddress);

    // Input and validate event date using a loop.
    // Display error messages for invalid month, day, or year.
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

    // Input and validate event start time using a loop.
    // Display error messages for invalid hour or minute.
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

    // Input and validate event end time using a loop.
    // Display error messages for invalid hour or minute.
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

    // Create a CSV file named after the event and open it for appending and reading.
    char fileName[256];
    sprintf(fileName, "%s.csv", admin.eventName);
    chdir(workingDir);
    fp = fopen(fileName, "a+");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    fflush(fp);

    // Write header and event details to the CSV file.
    fprintf(fp, "Event Name,Event Address,Event Date,Start Time,End Time\n");
    int writeCheck = fprintf(fp, "%s,%s,%d/%d/%d,%d:%d,%d:%d\n", admin.eventName, admin.eventAddress, admin.month, admin.day, admin.year, admin.hour[0], admin.min[0], admin.hour[1], admin.min[1]);
    fprintf(fp, "\nName,Age,Sex,Address,Mobile Number, Email Address");

    // Check if writing to the file was successful.
    if(writeCheck < 0){
        printf("Failed to write to file\n\n"); 
        return;
    }
    // Close the CSV file.
    if(fclose(fp) != 0){
        printf("Failed to close %s\n", fileName);
        return;
    }
    // Write the event name to the "Events.txt" file.
    if (writeCheck < 0){
        printf("Failed to write to file\n\n");
        return;
    } else { 
        system("cls");
        printf("Event created successfully\n\n"); 
    }
    chdir("..");
}

void viewEvent(){
    Admin admin;
    char line[250], eventName[100][100]; // Buffer to hold values of each line and array to store event names
    int countEvents = 0, choice, firstLine = 1; // Initialize variables for event count, user choice, and first line flag
    // Open the file containing event names and details
    int i=0;
    struct dirent *entry;

    // Open the directory
    system("cls"); 

    // Open the directory
    dir = opendir(workingDir);
    if(dir == NULL){
        perror("Unable to open directory!");
        return;
    }

    printf("\nEvents that are accessible:\n\n");
    while((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG){
            const char *dot = strrchr(entry->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                size_t length = dot - entry->d_name;
                printf("%d\t%.*s\n", i + 1, (int)length, entry->d_name);
                i++;
            }
        }
    }
    printf("\t[0] Exit\n");
    closedir(dir);
    printf("Enter Option: ");
    scanf("%d", &choice);

    if(choice == 0){
        return;
    }
    system("cls");

    // Open the directory again to find the selected event
    dir = opendir(workingDir);
    if(dir == NULL){
        perror("Unable to open directory!");
        return;
    }

    i = 1;
    while((entry = readdir(dir)) != NULL){
        if(entry->d_type == DT_REG){
            const char *dot = strrchr(entry->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                if(i == choice){
                    char fileName[256];
                    snprintf(fileName, sizeof(fileName), "%s/%s", workingDir, entry->d_name);
                    fp = fopen(fileName, "r");
                    if(fp == NULL){
                        printf("Failed to open file\n");
                        return;
                    }

                    // Read and display event details from the CSV file
                    while(fgets(line, sizeof(line), fp)){
                        if(firstLine){
                            firstLine = 0;
                            continue;   // Skip the first line (header) of the CSV file
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
                    break;  // Exit the loop after displaying details for the selected event
                }
                i++;
            }
        }
    }
    closedir(dir);
}

void deleteEvent(){
    Admin admin;
    char line[200]; // Buffer to hold each line
    char eventName[50]; // Name of the event to delete
    int eventNumber; // Number of the event to delete
    int found = 0; // Indicator whether the event was found
    int i = 0;

    struct dirent *entry;

    // Open the directory
    dir = opendir(workingDir);
        if(dir == NULL){
            perror("Unable to open directory!");
            return;
        }
        system("cls"); 
        printf("Events that are accessible:\n");
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(dot && strcmp(dot, ".csv") == 0){
                    size_t length = dot - entry->d_name;
                    printf("\t[%d] %.*s\n", i + 1, (int)length, entry->d_name);
                    i++;
                }
            }
        }
        printf("\t[0] Exit\n");
        closedir(dir);

        printf("Enter the number of the event to delete: ");
        if(scanf("%d", &eventNumber) != 1){
            printf("Invalid input!\n");
            return;
        }
        if(eventNumber == 0){
            return;
        }
        dir = opendir(workingDir);
        if(dir == NULL){
            perror("Unable to open directory!");
            return;
        }

        i = 1;
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(dot && strcmp(dot, ".csv") == 0){
                    if(i == eventNumber){
                        size_t length = dot - entry->d_name;
                        char filePath[1024];
                        snprintf(filePath, sizeof(filePath), "%s/%s", workingDir, entry->d_name);
                        if(remove(filePath) == 0){
                            system("cls");
                            printf("\nEvent deleted successfully!\n\n");
                            return;
                        } else {
                            perror("Error deleting file");
                        }
                        break;
                    }
                    i++;
                }
            }
        }
    closedir(dir);

    system("cls");
    printf("Event not found\n\n");
}

void editEvent() {
    Admin admin;
    char line[300]; // Buffer to hold each line
    char eventName[50]; // Name of the event to edit
    char newFileName[100];
    int eventNumber; // Number of the event to edit
    int found = 0; // Indicator whether the event was found
    int i, x, dateValidationResult, timeValidationResult;
    struct dirent *entryx;
    int b = 0;

    system("cls");
    dir = opendir(workingDir);
    printf("Events that are accessible:\n");
    while((entryx = readdir(dir)) != NULL){
        if(entryx->d_type == DT_REG){
            const char *dot = strrchr(entryx->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                size_t length = dot - entryx->d_name;
                printf("\t[%d] %.*s\n", b + 1, (int)length, entryx->d_name);
                b++;
            }
        }
    }
    printf("\t[0] Exit\n");

    // Ask the admin to enter either the name or the number of the event to edit
    printf("Enter the number of the event to edit: ");
    chdir(workingDir);
    if (scanf("%d", &eventNumber) == 1) {
        if (eventNumber == 0) {
            return;
        }
        // The admin entered a number
        struct dirent *entry;
        i = 1;
        dir = opendir(".");
        if(dir == NULL){
            perror("Unable to open directory!");;
            exit(EXIT_FAILURE);
        }

        // Traverse to directory and find the files that matches the file chosen
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(dot && strcmp(dot, ".csv") == 0){
                    size_t length = dot - entry->d_name;
                    // printf("%d\t%.*s\n", i, (int)length, entry->d_name);
                    char csvFileName[100];
                    sprintf(csvFileName, "%.*s.csv", (int)length, entry->d_name);

                    // Open the CSV file as read
                    FILE *csvFile = fopen(csvFileName, "r");
                    if(csvFile == NULL){
                        perror("Unable to open .csv file!");
                        exit(EXIT_FAILURE);
                    }
                    FILE *temp = fopen("temp.csv", "w");
                    if(temp == NULL){
                        perror("Unable to open temp.csv file!");
                        exit(EXIT_FAILURE);
                    }
                    i = 0;
                    while(fgets(line, sizeof(line), csvFile)){
                        if (i == 1) {
                            // Replace the 2nd row with input details
                            sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
                            printf("Enter new details for the event:\n");
                            printf("Event Name: ");
                            scanf(" %[^\n]", admin.eventName);
                            printf("Event Address: ");
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
                            fprintf(temp, "%s,%s,%d/%d/%d,%d:%d,%d:%d\n", admin.eventName, admin.eventAddress, admin.month, admin.day, admin.year, admin.hour[0], admin.min[0], admin.hour[1], admin.min[1]);
                        } else {
                            // Copy other lines unchanged
                            fprintf(temp, "%s", line);
                        }
                        i++;
                    }
                    fclose(csvFile);
                    fclose(temp);

                    remove(csvFileName);
                    snprintf(newFileName, sizeof(newFileName), "%s.csv", admin.eventName);
                    rename("temp.csv", newFileName);
                    found = 1;
                    break;
                    // i++;
                }
            }
        }
    } else {
        // The admin entered a name
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

            printf("Event details updated successfully!\n");

            printf("\n");
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
            case 5:
                // Edit Event
                ticketfilter();
                break;
            case 0:
                break;
            default:
                printf("Invalid Choice\n");
        }
    }while(choice != 0);
}

void filterArray(char arr[MAX_FILES][MAX_LINE], int size, char *filter) {
    int found = 0;
    for(int i = 0; i < size; i++) {
        char temp[MAX_LINE];
        // Remove newline character from the line
        if(arr[i][strlen(arr[i]) - 1] == '\n') {
            arr[i][strlen(arr[i]) - 1] = '\0';
        }
        strcpy(temp, arr[i]);
        char *token = strtok(temp, ",");
        int colCount = 0;
        char *columns[7];
        while(token != NULL) {
            columns[colCount] = token;
            colCount++;
            token = strtok(NULL, ",");
        }
        if(colCount == 7 && strcmp(columns[6], filter) == 0) {
            found = 1;
            printf("\033[H\033[J");
            printf("\nParticipant details:\n");
            printf("Name: %s\n", columns[0]);
            printf("Age: %s\n", columns[1]);
            printf("Gender: %s\n", columns[2]);
            printf("Address: %s\n", columns[3]);
            printf("Phone: %s\n", columns[4]);
            printf("Email: %s\n", columns[5]);
            printf("ID: %s\n", columns[6]);
        }
    }
    if(!found) {
        printf("\nNo participant found with the given ticket code.\n");
    }
}

int ticketfilter() {
    DIR *d;
    struct dirent *dir;
    char filenames[MAX_FILES][MAX_COL];
    char data[MAX_FILES][MAX_LINE];
    char filter[MAX_COL];
    int i = 0, choice;

    d = opendir("eventholder");
    system("cls");
    printf("Select an event that you want to filter based on their ticked code:\n");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(dir->d_type == DT_REG) {
                char filenameCopy[MAX_LINE];
                strcpy(filenameCopy, dir->d_name);
                char *token = strtok(filenameCopy, ".");
                printf("\t[%d] %s\n", i+1, token);
                strcpy(filenames[i], dir->d_name);
                i++;
            }
        }
        printf("\t[0] Exit\n");
        closedir(d);
    }

    printf("Choose an event: ");
    scanf("%d", &choice);
    if (choice == 0) {
        return 0;
    }
    choice--; // Adjust for 0-indexing

    char path[128] = "eventholder/";
    strcat(path, filenames[choice]);

    int dataCount = 0;
    FILE *file = fopen(path, "r");
    if(file != NULL) {
        char line[MAX_LINE];
        int lineCount = 0;
        while(fgets(line, sizeof(line), file)) {
            lineCount++;
            if(lineCount >= 5) {
                strcpy(data[dataCount], line);
                dataCount++;
            }
        }
        fclose(file);
    }

    int continueFiltering;
    do {
        printf("Enter ticket code: ");
        scanf("%s", filter);
        filterArray(data, dataCount, filter);
        do {
            printf("\nDo you want to continue filtering? (1/0): ");
            scanf("%d", &continueFiltering);
            clearInputBuffer();
            if(continueFiltering != 1 && continueFiltering != 0) {
                printf("Invalid input. Please enter 1 or 0.\n");
            }
        } while(continueFiltering != 1 && continueFiltering != 0);

        if (continueFiltering == 1)
        {
            printf("\033[H\033[J");
        }
        
    } while(continueFiltering == 1);
    return 0;
}




// Control Functions for User
// User Menu
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
// Function to display events
void displayEvents(){
    int i;
    struct dirent *entry;

    system("cls");
    // Open the directory
    dir = opendir(workingDir); 
    if(dir == NULL){
        perror("Unable to open directory!");;
        exit(EXIT_FAILURE);
    }
    printf("Events that are accessible:\n", workingDir);
    i = 1;
    // Loop through directory entries
    while((entry = readdir(dir)) != NULL){
        // This will check if the file has a '.csv' extension
        if(entry->d_type == DT_REG){
            const char *dot = strrchr(entry->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                size_t length = dot - entry->d_name;
                // Print the file name without the extension
                printf("\t[%d] %.*s\n", i, (int)length, entry->d_name);
                i++;
            }
        }
    }
    printf("\t[0] Exit\n");    // Display the option to exit
    closedir(dir);

}
// This function returns the value of the chosen event
int selectEvent(){
    int eventNum;
    printf("Select an event: ");
    scanf("%d", &eventNum);
    return eventNum;
}
// Function to generate code for user's receipt
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

    // Open the csv file for appending
    FILE *file = fopen(fileName, "a+");
    if(file == NULL){
        printf("Unable to open file!");
        exit(EXIT_FAILURE);
    }

    // Append user details to the CSV File
    fprintf(file, "\n%s,%d,%c,%s,%s,%s,%s", 
        userDetails->name,
        userDetails->age,
        userDetails->sex,
        userDetails->address,
        userDetails->mobileNum,
        userDetails->emailAddr,
        userDetails->code
    );
    // Flush the file buffer to ensure immediate write
    fflush(file);
    printf("\nParticipant is successfully registered.\n");
    fclose(file);

    // Generate a receipt file in the 'receipts' directory
    sprintf(fileName, "receipts/%s_Event-%s.txt", userDetails->name, eventName);
    file = fopen(fileName, "w");

    // Write the user and event details to the receipt file
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

// This function checks if the given name is valid
int isValidName(char *name) {
    // Checks if the name is not empty
    if (strlen(name) == 0) return 0;
    // Check if the name only contains alphabetic characters or space.
    for (int i = 0; name[i]; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;   // Name is valid
}

// This function checks if the given age is valid
int isValidAge(int age) {
    // Check if the age is between 1 and 120.
    return age >= 1 && age <= 120;
}

// Sex checker function
int isValidSex(char sex) {
    // Check if the sex is 'M' or 'F' (case insensitive).
    return toupper(sex) == 'M' || toupper(sex) == 'F';
}

// Address checker function
int isValidAddress(char *address) {
    // Check if the address is not empty and is less than 100 characters.
    return strlen(address) > 0 && strlen(address) < 100;
}

// Checks if the mobile entered is valid
int isValidMobileNum(char *mobileNum) {
    // Check if the mobile number is 11 digits and starts with a valid prefix (e.g., '09').
    return strlen(mobileNum) == 11 && mobileNum[0] == '0' && mobileNum[1] == '9';
}

// Checks if the email entered is valid
int isValidEmailAddr(char *emailAddr) {
    // Check if the email address contains '@' and '.'.
    char *atSign = strchr(emailAddr, '@');
    char *dot = strchr(emailAddr, '.');
    return atSign != NULL && dot != NULL;
}

// This function is where the user will input their details
void inputUserDetails(int choice){
    int reEnter = 1;

    if(choice != 0){
        char fileName[50];                  // Buffer to store the selected event file name
        DIR *dir = opendir(workingDir);     // Directory structure pointer
        struct dirent *entry;
        int i = 1;                          // Counter for event index

         // Loop through directory entries to find the selected event file
        while((entry = readdir(dir)) != NULL){
            if(entry->d_type == DT_REG){
                const char *dot = strrchr(entry->d_name, '.');
                if(i == choice){
                    snprintf(fileName, sizeof(fileName), "%s%s", workingDir, entry->d_name);
                    break;      // Stop when chosen event is found
                }
                i++;
            }
        }
        closedir(dir);
        system("cls");
        User newUser;
        while (reEnter == 1)
        {
            printf("Please enter your information:\n");
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

// Function for getting the  choice of the user when adding participant
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