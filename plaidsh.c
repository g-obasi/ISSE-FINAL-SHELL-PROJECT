#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "pipeline.h"
#include "tokenize.h" // Include tokenize header if separate
#include "parse.h"   // Include parser header if separate
#include "execute.h" // Include executor header if separate


int main() {
    char *input;
    Pipeline *pipeline;   // Changed from Pipeline pipeline

    printf("Welcome to Plaid Shell Created by -\tGreenfield Obasi at CMU Africa\n");

    while (1) {
        input = readline("#? ");
        if (!input)
            break;

        add_history(input); // Add input to history

        // Tokenize the input
        char errmsg[256]; // Buffer for error messages
        CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));
//        TOK_print(tokens);

        // Parse tokens into a pipeline
        pipeline  = Parse(tokens,errmsg,sizeof(errmsg));
//        PLD_print(pipeline);

        // Execute the pipeline
        execute_pipeline(pipeline);

        // Clean up
        free(input);
        CL_free(tokens); // Free the token list
        PLD_free(pipeline); // Free the pipeline
    }

    return 0;
}
