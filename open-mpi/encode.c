#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // Include ctype.h for isupper and islower functions

#define SHIFT 7

void encrypt(char *buffer, int size) {
    for (int i = 0; i < size; i++) {
        if (isupper(buffer[i])) {
            buffer[i] = ((buffer[i] - 'A' + SHIFT) % 26) + 'A';
        } else if (islower(buffer[i])) {
            buffer[i] = ((buffer[i] - 'a' + SHIFT) % 26) + 'a';
        }
        // Ignore non-alphabetic characters
    }
}

void displayMenu() {
    printf("1. Encrypt a file\n");
    printf("2. Add additional features (not implemented yet)\n");
    printf("0. Exit\n");
}

int main() {
    int choice;
    char filename[100]; // Assuming a maximum filename length of 100 characters

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the filename: ");
                scanf("%s", filename);

                FILE *fp = fopen(filename, "r");
                if (fp == NULL) {
                    perror("Error opening input file");
                    break;
                }

                fseek(fp, 0L, SEEK_END);
                long file_size = ftell(fp);

                fseek(fp, 0L, SEEK_SET);

                char *buffer = malloc(file_size);
                if (buffer == NULL) {
                    perror("Memory allocation error");
                    fclose(fp);
                    break;
                }

                fread(buffer, file_size, 1, fp);
                fclose(fp);

                encrypt(buffer, file_size);

                FILE *out = fopen("encrypted.txt", "w");
                if (out == NULL) {
                    perror("Error opening output file");
                    free(buffer);
                    break;
                }

                fwrite(buffer, file_size, 1, out);
                fclose(out);

                free(buffer);

                printf("File encrypted successfully!\n");
                break;

            case 2:
                // Add additional features here in the future
                printf("Additional features not implemented yet.\n");
                break;

            case 0:
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (choice != 0);

    return 0;
}
