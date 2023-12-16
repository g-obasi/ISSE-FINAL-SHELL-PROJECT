/*
 * pipeline.c
 *
 * Implementation of a simple command pipeline
 *
 * Author:
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "pipeline.h"
#include "tokenize.h"


Command* CMD_new() {
    Command* cmd = (Command*) malloc(sizeof(Command));
    cmd->arguments = CL_new();
    cmd->next = NULL;
    return cmd;
}

// Free the memory occupied by a Command
void CMD_free(Command* cmd) {
    CL_free(cmd->arguments);
    free(cmd);
}


// Create a new Pipeline
Pipeline* PLD_new() {
    Pipeline* pl = (Pipeline*) malloc(sizeof(Pipeline));
    pl->head = NULL;
    pl->input_file = NULL;
    pl->output_file = NULL;
    return pl;
}


// Get the length of the pipeline
int PLD_length(Pipeline* pl) {
    int length = 0;
    Command* cmd = pl->head;
    while (cmd != NULL) {
        length++;
        cmd = cmd->next;
    }
    return length;
}


// Free the memory occupied by a Pipeline
void PLD_free(Pipeline* pl) {
    Command* cmd = pl->head;
    while (cmd != NULL) {
        Command* next = cmd->next;
        CMD_free(cmd);
        cmd = next;
    }
    free(pl->input_file);
    free(pl->output_file);
    free(pl);
}


// Remove a command at a specific position in the pipeline
void PLD_remove(Pipeline* pl, int index) {
    if (pl->head == NULL) {
        return;
    }
    // Special case for removing the head of the pipeline
    if (index == 0) {
        Command* next = pl->head->next;
        CMD_free(pl->head);
        pl->head = next;
        return;
    }
    // Find the command before the one we want to remove
    Command* cmd = PLD_get(pl, index - 1);
    if (cmd != NULL && cmd->next != NULL) {
        Command* next = cmd->next->next;
        CMD_free(cmd->next);
        cmd->next = next;
    }
}


// Get a command at a specific position in the pipeline
Command* PLD_get(Pipeline* pl, int index) {
    int i = 0;
    Command* cmd = pl->head;
    while (cmd != NULL && i < index) {
        cmd = cmd->next;
        i++;
    }
    // Return the command if it exists, otherwise return NULL
    return (i == index) ? cmd : NULL;
}

// Append a command to a pipeline
void PLD_append(Pipeline* pl, Command* cmd) {
    if (pl->head == NULL) {
        pl->head = cmd;
    } else {
        Command* current = pl->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = cmd;
    }
}


// Function to print a single argument for debugging purposes
void print_argument(int position, CListElementType element, void *data) {
    Token current = element;
    if (current.type == TOK_WORD || current.type == TOK_QUOTED_WORD  ) {
        printf("COMMAND_%s(\"%s\")\n", TT_to_str(current.type), current.value);
    } else {
        printf("COMMAND_%s\n", TT_to_str(current.type));
    }
}


// Print the pipeline for debugging purposes
void PLD_print(Pipeline* pl) {
    Command* cmd = pl->head;
    while (cmd != NULL) {
//        pl_print_command(cmd);
        printf("Command: ");
        CL_foreach(cmd->arguments,  print_argument, NULL);
        printf("\n");
        cmd = cmd->next;
    }
    printf("\nPipeline input: %s\nPipeline output: %s\n", pl->input_file, pl->output_file);

}


// Set the redirection for a pipeline
void PLD_set_redirection(Pipeline* pl, TokenType redirection_type, const char *filename) {
    if (redirection_type == TOK_LESSTHAN) {
        free(pl->input_file);
        pl->input_file = strdup(filename);
    } else if (redirection_type == TOK_GREATERTHAN) {
        free(pl->output_file);
        pl->output_file = strdup(filename);
    }
}