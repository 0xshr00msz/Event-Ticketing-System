#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

#define MAX_FILES 200

FILE *fp;                                           // Universal file pointer variable       
int x = 0;
const char *workingDir = "eventholder/";            // Directory for the events  
DIR *dir;                                           // Universal directory pointer variable

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
// int eventExists(FILE *fp, char *eventName);
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
// Control Functions for Admin
// Admin Menu for Admin section of the program - returns the value of choice of the admin user
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

// This function checks if an event already exists in the file that lists all available events
// int eventExists(FILE *fp, char* eventName) {
//     Admin admin;
//     char line[200]; // Buffer to hold values of each line
//     // Checks if the file exists
//     if(fp == NULL){
//         printf("Failed to open file\n");
//         return 0;
//     }
//     // Read each line from the file
//     while(fgets(line, sizeof(line), fp)){
//         line[strcspn(line, "\n")] = 0;                  // Remove new line character
//         sscanf(line, "%s", admin.eventName);            // Extract event name
//         // Compare event names
//         if(strcmp(admin.eventName, eventName) == 0){
//             printf("\nEvent already exists\n\n");
//             fclose(fp);
//             return 0;   // returns 0 if event is found
//         }
//     }
//     return 1;   // returns 1 if event is not found
// }

int eventExists(char* eventName) {
   struct dirent *entry;
   char *filenames[MAX_FILES];
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
    // Open or create the "Events.txt" file for appending and reading.
    FILE *eventsFile = fopen("Events.txt", "a+");
    if(eventsFile == NULL){
        printf("Failed to open file\n");
        return;
    }
    system("cls");
    // Loop to ensure a unique event name by checking against existing events in "Events.txt".
    do{
        printf("Enter Event Name: ");
        scanf(" %[^\n]", admin.eventName);
        clearInputBuffer();
    }while(!eventExists(admin.eventName));
    fclose(eventsFile);     // Closes the "Events.txt" file after checking.

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
    chdir(workingDir);      // Change the working directory to the "eventholder/" where the events are contained

    // Create a CSV file named after the event and open it for appending and reading.
    char fileName[256];
    sprintf(fileName, "%s.csv", admin.eventName);
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
    chdir("..");        // Go back to root directory
     // Open the "Events.txt" file for appending and writing.
    eventsFile = fopen("Events.txt", "a+");
    if(eventsFile == NULL){
        printf("Failed to open file\n");
        return;
    }
    fflush(eventsFile);
    writeCheck = fprintf(eventsFile, "%s\n", admin.eventName);
    fclose(eventsFile);

    // Write the event name to the "Events.txt" file.
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
    char line[250], eventName[100][100]; // Buffer to hold values of each line and array to store event names
    int countEvents = 0, choice, firstLine = 1; // Initialize variables for event count, user choice, and first line flag
    
    // Open the file containing event names and details
    fp = fopen("Events.txt", "r");
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }

    // Display the list of events
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
    chdir(workingDir);      // Go back to the root folder of the project

    printf("\t%d. Exit\nEnter Option: ", (countEvents + 1));
    scanf("%d", &choice);

    if(choice == (countEvents + 1)){
        return;
    }
    system("cls");

    // Display details of the selected event
    for(int x = 0; x < countEvents; x++){
        if(x == (choice - 1)){
            char fileName[256];
            sprintf(fileName, "%s.csv", eventName[x]);
            fp = fopen(fileName, "r");

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
    }
    chdir("..");
}

void deleteEvent(){
    Admin admin;
    char line[200]; // Buffer to hold each line
    char eventName[50]; // Name of the event to delete
    int eventNumber; // Number of the event to delete
    int found = 0; // Indicator whether the event was found
    int i = 0;

    system("cls"); 
    // Display all events with their numbers
    fp = fopen("Events.txt", "r");      // Open the file for reading
    if(fp == NULL){
        printf("Failed to open file\n");
        return;
    }
    chdir(workingDir);

    while(fgets(line, sizeof(line), fp)){
        // Extract event details from the line and display event number and name
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        printf("%d. %s\n", ++i, admin.eventName);
    }
    fclose(fp);

    // Ask the admin to enter either the name or the number of the event to delete
    printf("Enter the number of the event to delete: ");
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
                    if(i == eventNumber){
                        size_t length = dot - entry->d_name;
                        remove(entry->d_name);
                        break;
                    }
                    i++;
                }
            }
        }
        closedir(dir);
        chdir("..");
        // Open Events.txt file, create a temporary file, and copy non-deleted events to the temporary file
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
        // Delete the old file and rename the temporary file
        remove("Events.txt");
        rename("temp.txt", "Events.txt");
    }else{
        found = 0;
    }
    
    system("cls");
    if(found){
        printf("Event deleted successfully\n\n");
    }else{
        printf("Event not found\n\n");
    }
}

void editEvent() {
    Admin admin;
    char line[300]; // Buffer to hold each line
    char eventName[50]; // Name of the event to edit
    char newFileName[100];
    int eventNumber; // Number of the event to edit
    int found = 0; // Indicator whether the event was found
    int i, x, dateValidationResult, timeValidationResult;

    // Display all events with their numbers
    fp = fopen("Events.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return;
    }
    i = 0;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%[^,],%d/%d/%d,%d:%d,%d:%d", admin.eventName, admin.eventAddress, &admin.month, &admin.day, &admin.year, &admin.hour[0], &admin.min[0], &admin.hour[1], &admin.min[1]);
        printf("%d. %s\n", ++i, admin.eventName);
    }
    fclose(fp);

    // Ask the admin to enter either the name or the number of the event to edit
    printf("Enter the number of the event to edit: ");
    chdir(workingDir);
    if (scanf("%d", &eventNumber) == 1) {
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

    // Open the directory
    dir = opendir(workingDir); 
    if(dir == NULL){
        perror("Unable to open directory!");;
        exit(EXIT_FAILURE);
    }
    printf("\nEvents that are accessible:\n\n", workingDir);
    i = 1;
    // Loop through directory entries
    while((entry = readdir(dir)) != NULL){
        // This will check if the file has a '.csv' extension
        if(entry->d_type == DT_REG){
            const char *dot = strrchr(entry->d_name, '.');
            if(dot && strcmp(dot, ".csv") == 0){
                size_t length = dot - entry->d_name;
                // Print the file name without the extension
                printf("%d\t%.*s\n", i, (int)length, entry->d_name);
                i++;
            }
        }
    }
    printf("0\tExit\n\n");    // Display the option to exit
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
    printf("\nSuccessfully appended to file\n");
    fclose(file);

    // Generate a receipt file in the 'receipts' directory
    sprintf(fileName, "receipts/%s.txt", userDetails->name);
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