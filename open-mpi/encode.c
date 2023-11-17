#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening input file");
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long file_size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    char *buffer = malloc(file_size);
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(fp);
        return 1;
    }

    fread(buffer, file_size, 1, fp);
    fclose(fp);

    encrypt(buffer, file_size);

    FILE *out = fopen("encrypted.txt", "w");
    if (out == NULL) {
        perror("Error opening output file");
        free(buffer);
        return 1;
    }

    fwrite(buffer, file_size, 1, out);
    fclose(out);

    free(buffer);

    printf("File encrypted successfully!\n");

    return 0;
}
