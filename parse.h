/*
 * parse.h
 * 
 * Simple description:
 * for parsing tokens into a pipline object building an abstract syntax tree
 *
 * Author: Greenfield Obasi
 */


#ifndef _PARSE_H_
#define _PARSE_H_

#include <stddef.h>
#include "pipeline.h"
#include "clist.h"

/*
 * Parses a list of tokens into a Pipeline object building an abstract syntax tree.
 *
 * Parameters:
 *   tokens     List of tokens remaining to be parsed
 *   errmsg     Return space for an error message, filled in in case of error
 *   errmsg_sz  The size of errmsg
 * 
 * Returns: The parsed Pipeline on success. If a parsing error is
 *   encountered, copies an error message into errmsg and returns
 *   NULL.
 */
Pipeline* Parse(CList tokens, char *errmsg, size_t errmsg_sz);

#endif /* _PARSE_H_ */
