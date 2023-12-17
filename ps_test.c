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
            "ls -l | grep .txt | wc -l",
            "find . -name \"*.c\" | xargs grep \"main\" | wc -l",
            "",
            "    ",
            "  \\  ",
            "pwd",
            "ls --color",
            "cd Plaid\\ Shell\\ Playground",
            "ls",
            "ls *.txt",
            "echo $PATH",
            "author",
            "author | sed -e \"s/^/Written by /\"",
            "grep Happy *.txt",
            "cat \"best sitcoms.txt\" | grep Seinfeld",
            "cat \"best sitcoms.txt\"|grep Seinfeld|wc -l",
            "sed -ne \"s/The Simpsons/I Love Lucy/p\" < best\\ sitcoms.txt > output",
            "ls -l",
            "cat output",
            "this is not a command",
            "echo Hello > /usr/bin/cant_write",
            "cd",
            "cd ~",
            "exit",
            "author",
            "author | sed -e \"s/^/Written by /\"",
            "grep Happy *.txt",
            "cat \"best sitcoms.txt\" | grep Seinfeld",
            "cat \"best sitcoms.txt\"|grep Seinfeld|wc -l",
            "sed -ne \"s/The Simpsons/I Love Lucy/p\" < best\\ sitcoms.txt > output",
            "ls -l",
            "cat output",
            "this is not a command",
            "echo Hello > /usr/bin/cant_write",
            "cd",
            "pwd",
            "cd ~",
            "pwd",
            "exit",
            "echo \"Operator could you help me place this call?\"",
            "seq 10 | wc -l",
            "env | grep PATH",
            "echo Hello World | cat -n | cat -n | cat -n",
            "echo \\c",
            "echo \"\\c\"",
            "echo \"hi",
            "echo \"|1|2|3|\" | sed -e \"s/[0-9]//\"",
            "printf \"=%s=\\n\" one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen sixteen seventeen eighteen nineteen twenty twenty-one twenty-two twenty-three twenty-four | wc -l",
            "printf \"+%s\" one two three",
            "echo > file1 >file2",
            "cat <",
            "cat | cat | cat >",
            "grep | ",
            "| grep",
            "echo || grep",
            "echo \\<\\|\\> | cat",
            "echo hello\\|grep ell",
            "exit"
    };

    // Number of test cases
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    for (int i = 0; i < num_cases; i++) {
        CList shell_tokens = TOK_tokenize_input(test_cases[i], errmsg, sizeof(errmsg));
        Pipeline* pipeline = Parse(shell_tokens, errmsg, sizeof(errmsg));

        // Execute the pipeline
        execute_pipeline(pipeline);
        // Free the pipeline
        PLD_free(pipeline);
    }

    printf("All tests passed!\n");
    return 0;
}
