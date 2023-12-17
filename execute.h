/*
 * execute.h
 *
 * Contains definition of execute_pipeline and its helper functions
 *
 * Author: Greenfield Obasi
 */

#ifndef PROJECT_EXECUTE_H
#define PROJECT_EXECUTE_H
#include "pipeline.h"


/*
 * Definition: This function exits the shell.
 *
 * Parameters:
 *   argc    an integer that represents the number of arguments.
 *   argv    a pointer to an array of strings that contains the arguments.
 *
 * Returns:
 *   None
 *
 */
void builtin_exit(int argc, char **argv);

/*
 * Definition: This function prints the author's name.
 *
 * Parameters:
 *   out_fd  an integer that represents the file descriptor of the output file.
 *   argc    an integer that represents the number of arguments.
 *   argv    a pointer to an array of strings that contains the arguments.
 *
 * Returns:
 *   None
 *
 */
void builtin_author(int out_fd, int argc, char **argv);

/*
 * Definition: This function changes the current working directory.
 *
 * Parameters:
 *   argc    an integer that represents the number of arguments.
 *   argv    a pointer to an array of strings that contains the arguments.
 *
 * Returns:
 *   None
 *
 */
void builtin_cd(int argc, char **argv);

/*
 * Definition: This function prints the current working directory.
 *
 * Parameters:
 *   out_fd  an integer that represents the file descriptor of the output file.
 *   argc    an integer that represents the number of arguments.
 *   argv    a pointer to an array of strings that contains the arguments.
 *
 * Returns:
 *   None
 *
 */
void builtin_pwd(int out_fd, int argc, char **argv);

/*
 * Definition: This function executes an external command.
 *
 * Parameters:
 *   in_fd       an integer that represents the file descriptor of the input file.
 *   out_fd      an integer that represents the file descriptor of the output file.
 *   next_in_fd  an integer that represents the file descriptor of the next input file.
 *   argv        a pointer to an array of strings that contains the arguments.
 *
 * Returns:
 *   An integer that represents the status of the execution.
 *
 */
int forkexec_external_cmd(int in_fd, int out_fd, int next_in_fd, char **argv);

/*
 * Definition: This function executes a pipeline.
 *
 * Parameters:
 *   pl  a pointer to a Pipeline struct that contains the pipeline to execute.
 *
 * Returns:
 *   None
 *
 */
void execute_pipeline(Pipeline* pl);

#endif //PROJECT_EXECUTE_H
