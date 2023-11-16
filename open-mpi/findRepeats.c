#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <ctype.h>

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
    int rank, size;
    MPI_Init(&argc, &argv);
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

    // Read the file content into a buffer
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    // Split the buffer into words and store them in an array
    char **words = malloc(MAX_WORDS * sizeof(char *));
    int num_words = split_string(buffer, words);

    // Convert all words to lowercase
    for (int i = 0; i < num_words; i++) {
        to_lower(words[i]);
    }

    // Sort the words using qsort
    qsort(words, num_words, sizeof(char *), compare_words);

    // Find the most repeated word in the sorted array
    char *most_repeated_word = find_most_repeated_word(words, num_words);

    // Print the result
    if (rank == 0) {
        printf("The most repeated word in the file %s is: %s\n", filename, most_repeated_word);
    }

    // Free the memory
    free(buffer);
    free(words);

    MPI_Finalize();
    return 0;
}
