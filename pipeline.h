/*
 * pipeline.h
 *
 * Function and structure definitions for a simple command pipeline
 *
 * Author: Greenfield Obasi
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


/*
 * Definition: Creates a new Command structure.
 *
 * Parameters: None.
 *
 * Returns: A pointer to the newly created Command structure.
 */
Command* CMD_new();

/*
 * Definition: Frees the memory allocated for a Command structure.
 *
 * Parameters:
 *   cmd    a pointer to the Command structure to be freed.
 *
 * Returns: None.
 */
void CMD_free(Command* cmd);

/*
 * Definition: Creates a new Pipeline structure.
 *
 * Parameters: None.
 *
 * Returns: A pointer to the newly created Pipeline structure.
 */
Pipeline* PLD_new();

/*
 * Definition: Frees the memory allocated for a Pipeline structure.
 *
 * Parameters:
 *   pl    a pointer to the Pipeline structure to be freed.
 *
 * Returns: None.
 */
void PLD_free(Pipeline* pl);

/*
 * Definition: Appends a Command structure to the end of a Pipeline structure.
 *
 * Parameters:
 *   pl    a pointer to the Pipeline structure.
 *   cmd   a pointer to the Command structure to be appended.
 *
 * Returns: None.
 */
void PLD_append(Pipeline* pl, Command* cmd);

/*
 * Definition: Retrieves a Command structure from a Pipeline structure at a specified index.
 *
 * Parameters:
 *   pl    a pointer to the Pipeline structure.
 *   index the index of the Command structure to be retrieved.
 *
 * Returns: A pointer to the Command structure at the specified index.
 */
Command* PLD_get(Pipeline* pl, int index);

/*
 * Definition: Returns the length of a Pipeline structure.
 *
 * Parameters:
 *   pl    a pointer to the Pipeline structure.
 *
 * Returns: The number of Command structures in the Pipeline structure.
 */
int PLD_length(Pipeline* pl);

/*
 * Definition: Removes a Command structure from a Pipeline structure at a specified index.
 *
 * Parameters:
 *   pl    a pointer to the Pipeline structure.
 *   index the index of the Command structure to be removed.
 *
 * Returns: None.
 */
void PLD_remove(Pipeline* pl, int index);

/*
 * Definition: Prints the contents of a Pipeline structure.
 *
 * Parameters:
 *   pl    a pointer to the Pipeline structure.
 *
 * Returns: None.
 */
void PLD_print(Pipeline* pl);

/*
 * Definition: Sets the redirection for a command in a Pipeline structure.
 *
 * Parameters:
 *   pl                a pointer to the Pipeline structure.
 *   redirection_type  the type of redirection (input or output).
 *   filename          the name of the file for redirection.
 *
 * Returns: None.
 */
void PLD_set_redirection(Pipeline* pl, TokenType redirection_type, const char *filename);


#endif /* PIPELINE_H_ */
