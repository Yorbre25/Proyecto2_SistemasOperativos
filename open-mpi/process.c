#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Library/commulib.h"

#define SHIFT 7
#define MAX_WORD_LEN 100
#define MAX_WORDS 10000

// A function to compare two words
int compare_words(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

// A function to find the most repeated word in a sorted array of words
char *find_most_repeated_word(char **words, int n) {
    int max_count = 0;
    char *max_word = NULL;
    int i = 0;
    while (i < n) {
        int count = 1;
        char *word = words[i];
        i++;
        while (i < n && strcmp(words[i], word) == 0) {
            count++;
            i++;
        }
        if (count > max_count) {
            max_count = count;
            max_word = word;
        }
    }
    return max_word;
}

// A function to split a string into words and store them in an array
int split_string(char *str, char **words) {
    int n = 0;
    char *token = strtok(str, " \n\t\r.,;:!?\"'()[]{}");
    while (token != NULL) {
        words[n] = token;
        n++;
        token = strtok(NULL, " \n\t\r.,;:!?\"'()[]{}");
    }
    return n;
}

// A function to convert a string to lowercase
void to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s filename.txt\n", argv[0]);
        }
        MPI_Finalize();
        return 0;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        if (rank == 0) {
            printf("Error: cannot open file %s\n", filename);
        }
        MPI_Finalize();
        return 0;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate chunk size for each process
    long chunk_size = (file_size + size - 1) / size; // Ensure that every process gets some data

    // Allocate buffer for each process
    char *buffer = (char *)malloc(chunk_size + 1);

    // Read the corresponding chunk of the file
    fseek(file, rank * chunk_size, SEEK_SET);
    long read_size = fread(buffer, 1, chunk_size, file);
    fclose(file);

    // Ensure null-terminated string
    buffer[chunk_size] = '\0';

    // Decrypt the buffer
    for (int i = 0; i < chunk_size; i++) {
        if (isupper(buffer[i])) {
            buffer[i] = ((buffer[i] - 'A' - SHIFT + 26) % 26) + 'A';
        } else if (islower(buffer[i])) {
            buffer[i] = ((buffer[i] - 'a' - SHIFT + 26) % 26) + 'a';
        }
        // Ignore non-alphabetic characters
    }

    // Gather decrypted data to the root (rank 0) process
    char *decrypted_buffer = NULL;
    if (rank == 0) {
        decrypted_buffer = (char *)malloc(file_size + 1);
    }

    MPI_Gather(buffer, read_size, MPI_CHAR, decrypted_buffer, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Write the decrypted content to a file by the root process
    if (rank == 0) {
        FILE *decrypted_file = fopen("decrypted.txt", "w");
        if (decrypted_file != NULL) {
            fwrite(decrypted_buffer, file_size, 1, decrypted_file);
            fclose(decrypted_file);
        } else {
            printf("Error: cannot create decrypted file 'decrypted.txt'\n");
        }

        free(decrypted_buffer);
    }

    free(buffer);

    // Now, use the decrypted file as input for the word counting program

    // Read the decrypted file content into a new buffer
    FILE *decrypted_fp = fopen("decrypted.txt", "r");
    if (decrypted_fp == NULL) {
        if (rank == 0) {
            printf("Error: cannot open decrypted file 'decrypted.txt'\n");
        }
        MPI_Finalize();
        return 0;
    }

    fseek(decrypted_fp, 0, SEEK_END);
    long decrypted_size = ftell(decrypted_fp);
    fseek(decrypted_fp, 0, SEEK_SET);
    decrypted_buffer = malloc(decrypted_size + 1);
    fread(decrypted_buffer, 1, decrypted_size, decrypted_fp);
    decrypted_buffer[decrypted_size] = '\0';
    fclose(decrypted_fp);

    // Split the buffer into words and store them in an array
    char **words = malloc(MAX_WORDS * sizeof(char *));
    int num_words = split_string(decrypted_buffer, words);

    // Convert all words to lowercase
    for (int i = 0; i < num_words; i++) {
        to_lower(words[i]);
    }

    // Sort the words using qsort
    qsort(words, num_words, sizeof(char *), compare_words);

    // Find the most repeated word in the sorted array
    char *most_repeated_word = find_most_repeated_word(words, num_words);

    // Write the result to a file named "lastCount.txt"
    if (rank == 0) {
        FILE *result_file = fopen("lastCount.txt", "w");
        if (result_file != NULL) {
            fprintf(result_file, "The most repeated word in the decrypted file is: %s\n", most_repeated_word);
            fclose(result_file);
            
            int serialPort = initSerialComm();
            sendCommand(serialPort, most_repeated_word);
        } else {
            printf("Error: cannot create result file 'lastCount.txt'\n");
        }
    }

    // Free the memory
    free(decrypted_buffer);
    free(words);

    MPI_Finalize();
    
    return 0;
}