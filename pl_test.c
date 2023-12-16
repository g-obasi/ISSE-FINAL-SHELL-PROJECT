////
//// pipline_test.c.
////
//
//#include <assert.h>
//#include "pipeline.h"
//#include <stddef.h>
//#include <stdio.h>
//#include <string.h>
//#include "tokenize.h"
//#include "execute.h"
//
//
//int main() {
//    // Test CMD_new and CMD_free
//    Command* cmd = CMD_new();
//    assert(cmd != NULL);
//    CMD_free(cmd);
//
//    // Test PLD_new and PLD_free
//    Pipeline* pl = PLD_new();
//    assert(pl != NULL);
//    PLD_free(pl);
//
//    // Test PLD_append
//    pl = PLD_new();
//    cmd = CMD_new();
//    Token test, test1, test2, test3;
//    test.value = "cat";
//    test.type = TOK_WORD;
//
////    pl->input_file = "file1.txt";
//
//    test1.type = TOK_PIPE;
//    test1.value = "|";
//
//    test2.type = TOK_WORD;
//    test2.value = "wc";
//
//    test3.type = TOK_WORD;
//    test3.value = "-l";
//
//    CL_append(cmd->arguments, test);
//    CL_append(cmd->arguments, test1);
//    CL_append(cmd->arguments, test2);
//    CL_append(cmd->arguments, test3);
//
//
//    PLD_append(pl, cmd);
//    assert(pl->head == cmd);
//
////    PLD_set_redirection(pl, TOK_LESSTHAN, "input.txt");
////    assert(strcmp(pl->input_file, "input.txt") == 0);
////    PLD_set_redirection(pl, TOK_GREATERTHAN, "output.txt");
////    assert(strcmp(pl->output_file, "output.txt") == 0);
//    PLD_print(pl);
//
//    execute_pipeline(pl);
//    PLD_free(pl);
//
//    // Test PLD_set_redirection
//    pl = PLD_new();
//    PLD_set_redirection(pl, TOK_LESSTHAN, "input.txt");
//    assert(strcmp(pl->input_file, "input.txt") == 0);
//    PLD_set_redirection(pl, TOK_GREATERTHAN, "output.txt");
//    assert(strcmp(pl->output_file, "output.txt") == 0);
////    PLD_print(pl);
//    PLD_free(pl);
//
//    printf("All tests passed!\n");
//    return 0;
//}


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

//    Pipeline* pl = PLD_new();
//    assert(pl != NULL);

    // Create a new command
    Command* cmd = CMD_new();
    assert(cmd != NULL);

    // Add an argument to the command
//    Token test;
//    test.value = "pwd";
//    test.type = TOK_WORD;
//    CL_append(cmd->arguments, test);

//    test1.value = "|";
//    test1.type = TOK_PIPE;
//    CL_append(cmd->arguments, test1);

//    PLD_set_redirection(pl, TOK_LESSTHAN, "file1.txt");
//    PLD_set_redirection(pl, TOK_GREATERTHAN, "file2.txt");
//    cmd = CMD_new();

//    test2.value = "sort";
//    test2.type = TOK_WORD;
//    CL_append(cmd->arguments, test2);

//    cmd = CMD_new();
//    test3.value = "-r";
//    test3.type = TOK_WORD;
//    CL_append(cmd->arguments, test3);
    // Add the command to the pipeline
//    PLD_append(pl, cmd);

    // Set the input file for the pipeline
//    PLD_set_redirection(pl, TOK_LESSTHAN, "file1.txt");

    // Create another command
//    cmd = CMD_new();
//    assert(cmd != NULL);

    // Add an argument to the command
//    Token test1;
//    test1.value = "wc";
//    test1.type = TOK_WORD;
//    CL_append(cmd->arguments, test1);

    // Add the command to the pipeline
//    PLD_append(pl, cmd);

//     Set the output file for the pipeline
//    PLD_set_redirection(pl, TOK_GREATERTHAN, "file2.txt");


    CList tokens = TOK_tokenize_input("pwd", errmsg, sizeof(errmsg));
    Pipeline* pl = Parse(tokens, errmsg, sizeof(errmsg));
    // Execute the pipeline
    PLD_print(pl);
    execute_pipeline(pl);

    // Free the pipeline
    PLD_free(pl);

    printf("All tests passed!\n");
    return 0;
}

