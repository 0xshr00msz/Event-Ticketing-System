#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int saveEvents() {
    const char *fileName = "events/sampleDB.csv";

    // Create the directory if it doesn't exist
    // Open the CSV file for events in read mode
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Could not open file %s\n", fileName);
        return 1;
    }

    // Scan the CSV file for the event name, so scan for the first element for each line
    // Put them in an array to be made for each filename of the to-be-made csv
    char events[1000][1024]; // Array to hold events
    int numEvents = 0;

    char buffer[1024];
    // Read each line
    while (numEvents < 1000 && fgets(buffer, sizeof(buffer), file) != NULL) {
        // Get the first token (before the first comma)
        char *token = strtok(buffer, ",");
        if (token != NULL) {
            strncpy(events[numEvents], token, sizeof(events[numEvents]) - 1);
            events[numEvents][sizeof(events[numEvents]) - 1] = '\0'; // Ensure null-termination
            numEvents++;
        }
    }
    fclose(file);

    // using sprintf, create a filename for each event. Then from the filename of each event, create a csv file from the opened dir 
    // Create csv then put them in eventholder directory

    char filenames[1000][1024];

    for (int i = 0; i < numEvents; i++) {
        sprintf(filenames[i], "eventholder/%s.csv", events[i]);

        FILE *newFile = fopen(filenames[i], "w");

        if (newFile == NULL) {
            printf("Could not create file %s\n", filenames[i]);
            return 1;
        }

        // Write data to the new file here

        fclose(newFile);
    }

    return 0;
}

int main() {
    saveEvents();
}