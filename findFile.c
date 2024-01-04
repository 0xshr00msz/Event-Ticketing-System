#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int searchFile(const char *directory, const char *filename) {
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    if ((dir = opendir(directory)) == NULL) {
        perror("opendir");
        return -1;
    }

    // Iterate through the directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Check if the entry is a regular file
        if (entry->d_type == DT_REG) {
            // Compare the filename
            if (strcmp(entry->d_name, filename) == 0) {
                closedir(dir);
                return 1; // File found
            }
        }
    }

    closedir(dir);
    return 0; // File not found
}

int main() {
    const char *directory = "."; // Change this to the directory where you want to search
    const char *filename = "main.exe"; // Change this to the filename you want to search for

    int result = searchFile(directory, filename);

    if (result == 1) {
        printf("File found!\n");
    } else if (result == 0) {
        printf("File not found.\n");
    } else {
        printf("Error occurred while searching for the file.\n");
    }

    return 0;
}
