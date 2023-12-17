#include <assert.h>
#include <fcntl.h>
#include "pipeline.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "tokenize.h"
#include "execute.h"
#include "parse.h"

int main() {
    // Create a new pipeline
    char errmsg[1024];

    // Test cases
    char* test_cases[] = {
            "pwd",
            "ls | sort -r",
            "I love you!",
            "ls -l | grep .txt | wc -l",
            "find . -name '*.c' | xargs grep 'main' | wc -l",
            "echo 'Hello, World!' > test.txt; cat test.txt; rm test.txt",
            "ls -l | awk '{print $9}' | sort | uniq",
            "find . -type f -exec file {} \\; | grep text | cut -d':' -f1",
            "ps aux --sort=-%mem | head -n 5",
            "find / -type f -size +100M -exec ls -lh {} \\; | awk '{ print $NF \": \" $5 }'",
            "du -a . | sort -n -r | head -n 5",
            "echo 'Hello, World!' > test.txt", // Output redirection
            "cat < test.txt", // Input redirection
            "sort < test.txt > sorted.txt", // Input and output redirection
            "grep 'Hello' < test.txt > grep_results.txt", // Input and output redirection with pipe
            "rm test.txt sorted.txt grep_results.txt"
    };

    // Number of test cases
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_cases; i++) {
        CList tokens = TOK_tokenize_input(test_cases[i], errmsg, sizeof(errmsg));
        Pipeline* pl = Parse(tokens, errmsg, sizeof(errmsg));

        // Print the pipeline
        PLD_print(pl);
        printf("\n");
        // Free the pipeline
        PLD_free(pl);
    }

    return 0;
}
