#include <stdio.h>
#include "parse.h"
#include "tokenize.h"
#include "pipeline.h"
#include "string.h"
#include <glob.h>
#include <fnmatch.h>
#include <assert.h>

// Documented in the .h file
Pipeline* Parse(CList tokens, char *errmsg, size_t errmsg_sz) {
//    assert(tokens);

    Pipeline* pl = PLD_new();
    Command* cmd = CMD_new();
    TokenType next_token_type = TOK_next_type(tokens);
    while (next_token_type != TOK_END) {
        Token next_token = TOK_next(tokens);
        if (next_token.type == TOK_LESSTHAN || next_token.type == TOK_GREATERTHAN) {
            if (PLD_get(pl, next_token.type) != NULL) {
                snprintf(errmsg, errmsg_sz, "Multiple redirection");
                return NULL;
            }
            TOK_consume(tokens);
            Token filename_token = TOK_next(tokens);
            if (filename_token.type != TOK_WORD) {
                snprintf(errmsg, errmsg_sz, "Expect filename after redirection");
                return NULL;
            }
            PLD_set_redirection(pl, next_token.type, filename_token.value);
            TOK_consume(tokens);
        } else if (next_token.type == TOK_PIPE) {
            if (CL_length(cmd->arguments) == 0) {
                snprintf(errmsg, errmsg_sz, "No command specified");
                return NULL;
            }
            CL_append(cmd->arguments, next_token);
            TOK_consume(tokens);
        } else {
            CL_append(cmd->arguments, next_token);
            TOK_consume(tokens);
        }
        next_token_type = TOK_next_type(tokens);
    }
    if (CL_length(cmd->arguments) == 0) {
        snprintf(errmsg, errmsg_sz, "No command specified");
        return NULL;
    }
    PLD_append(pl, cmd);

    return pl;
}



//Pipeline* Parse(CList tokens, char *errmsg, size_t errmsg_sz) {
//    Pipeline* pl = PLD_new();
//    Command* cmd = CMD_new();
//    TokenType next_token_type = TOK_next_type(tokens);
//    while (next_token_type != TOK_END) {
//        Token next_token = TOK_next(tokens);
//        if (next_token.type == TOK_LESSTHAN) {
//            TOK_consume(tokens);
//            Token filename_token = TOK_next(tokens);
//            PLD_set_redirection(pl, TOK_LESSTHAN, filename_token.value);
//            TOK_consume(tokens);
//        } else if (next_token.type == TOK_GREATERTHAN) {
//            TOK_consume(tokens);
//            Token filename_token = TOK_next(tokens);
//            PLD_set_redirection(pl, TOK_GREATERTHAN, filename_token.value);
//            TOK_consume(tokens);
//        } else if (next_token.type == TOK_PIPE) {
//            CL_append(cmd->arguments, next_token);
//            TOK_consume(tokens);
//        } else {
//            CL_append(cmd->arguments, next_token);
//            TOK_consume(tokens);
//        }
//        next_token_type = TOK_next_type(tokens);
//    }
//    PLD_append(pl, cmd);
//
//    return pl;
//}

