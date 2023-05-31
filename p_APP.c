#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_TOKENS 100

int tokenize(char *str, char *delim, char **tokens) {
    int i = 0;
    char *token = strtok(str, delim);
    while (token != NULL && i < MAX_TOKENS) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, delim);
    }
    return i;
}

int main() {
    char file_name[100];
    printf("Enter the input file name: ");
    scanf("%s", file_name);

    FILE *file = fopen(file_name, "r");  // Open the input file for reading
    if (file == NULL) {
        printf("Failed to open the input file.\n");
        return 1;
    }

    // Read the input text from the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *input_str = malloc(file_size + 1);
    fread(input_str, 1, file_size, file);
    input_str[file_size] = '\0';
    fclose(file);

    char *delim = " ,.!"; // delimiter characters
    char *tokens[MAX_TOKENS];
    int num_tokens = 0;

    // parallelize tokenization using OpenMP
    #pragma omp parallel shared(num_tokens, tokens, input_str)
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        int chunk_size = strlen(input_str) / nthreads;
        int start = tid * chunk_size;
        int end = (tid == nthreads - 1) ? strlen(input_str) : (tid + 1) * chunk_size;
        char local_str[chunk_size + 1];
        strncpy(local_str, input_str + start, end - start);
        local_str[end - start] = '\0';
        char *local_tokens[MAX_TOKENS];
        int local_num_tokens = tokenize(local_str, delim, local_tokens);

        // copy local tokens to global tokens array using critical section
        #pragma omp critical
        {
            for (int i = 0; i < local_num_tokens; i++) {
                tokens[num_tokens + i] = local_tokens[i];
            }
            num_tokens += local_num_tokens;
        }
    }

    // print out the tokens
    printf("Number of tokens: %d\n", num_tokens);
    for (int i = 0; i < num_tokens; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }

    // free dynamically allocated memory
    free(input_str);

    return 0;
}
