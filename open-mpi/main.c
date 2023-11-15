#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define SHIFT 7

void decrypt(char *buffer, int size) {
  for (int i = 0; i < size; i++) {
    if (isupper(buffer[i])) {
      buffer[i] = ((buffer[i] - 'A' - SHIFT + 26) % 26) + 'A';
    } else if (islower(buffer[i])) {
      buffer[i] = ((buffer[i] - 'a' - SHIFT + 26) % 26) + 'a';
    }
    // Ignore non-alphabetic characters
  }
}

int main(int argc, char** argv) {

  MPI_Init(&argc, &argv);
  
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char *filename = argv[1];
  
  FILE *fp = fopen(filename, "r");
  fseek(fp, 0L, SEEK_END);
  long file_size = ftell(fp);

  int chunk_size = file_size / size;
  
  fseek(fp, rank * chunk_size, SEEK_SET);

  char *buffer = malloc(chunk_size);
  fread(buffer, chunk_size, 1, fp);
  fclose(fp);

  decrypt(buffer, chunk_size);

  char *full_data;
  if (rank == 0) {
    full_data = malloc(file_size);
  }

  MPI_Gather(buffer, chunk_size, MPI_CHAR, full_data, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    FILE *out = fopen("decrypted.txt", "w");
    fwrite(full_data, file_size, 1, out);
    fclose(out);
  }

  free(buffer);
  if (rank == 0) {
    free(full_data);
  }

  MPI_Finalize();

  return 0;
}