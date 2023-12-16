//
// Created by gobasi on 12/14/23.
//

#ifndef PROJECT_EXECUTE_H
#define PROJECT_EXECUTE_H
#include "pipeline.h"



//#ifndef _EXECUTE_H_
//#define _EXECUTE_H_


// Function prototypes
void builtin_exit(int argc, char **argv);
void builtin_author(int out_fd, int argc, char **argv);
void builtin_cd(int argc, char **argv);
void builtin_pwd(int out_fd, int argc, char **argv);
int forkexec_external_cmd(int in_fd, int out_fd, int next_in_fd, char **argv);
void execute_pipeline(Pipeline* pl);

//#endif /* _EXECUTE_H_ */



#endif //PROJECT_EXECUTE_H
