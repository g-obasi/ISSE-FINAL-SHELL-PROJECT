/*
 * pipeline.h
 *
 * Definitions for a simple command pipeline
 *
 * Author:
 */

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "clist.h"


typedef struct Command {
    CList arguments; // List of command arguments
    struct Command* next; // Pointer to the next command
} Command;


typedef struct {
    Command* head; // Head pointer to a linked list of commands
    char* input_file; // Input file for the entire pipeline
    char* output_file; // Output file for the entire pipeline
} Pipeline;


Command* CMD_new();
void CMD_free(Command* cmd);
Pipeline* PLD_new();
void PLD_free(Pipeline* pl);
void PLD_append(Pipeline* pl, Command* cmd);
//void PLD_insert(Pipeline* pl, Command* cmd, int index);
Command* PLD_get(Pipeline* pl, int index);
int PLD_length(Pipeline* pl);
void PLD_remove(Pipeline* pl, int index);
void PLD_print(Pipeline* pl);

// Set the redirection for a command
void PLD_set_redirection(Pipeline* pl, TokenType redirection_type, const char *filename);


#endif /* PIPELINE_H_ */
