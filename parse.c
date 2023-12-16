#include <stdio.h>
#include "parse.h"
#include "tokenize.h"
#include "pipeline.h"
#include "string.h"
#include <glob.h>
#include <fnmatch.h>


Pipeline* Parse(CList tokens, char *errmsg, size_t errmsg_sz) {
    Pipeline* pl = PLD_new();
    Command* cmd = CMD_new();
    TokenType next_token_type = TOK_next_type(tokens);
    while (next_token_type != TOK_END) {
        Token next_token = TOK_next(tokens);
        if (next_token.type == TOK_LESSTHAN) {
            TOK_consume(tokens);
            Token filename_token = TOK_next(tokens);
            PLD_set_redirection(pl, TOK_LESSTHAN, filename_token.value);
            TOK_consume(tokens);
        } else if (next_token.type == TOK_GREATERTHAN) {
            TOK_consume(tokens);
            Token filename_token = TOK_next(tokens);
            PLD_set_redirection(pl, TOK_GREATERTHAN, filename_token.value);
            TOK_consume(tokens);
        } else if (next_token.type == TOK_PIPE) {
            CL_append(cmd->arguments, next_token);
            TOK_consume(tokens);
        } else {
            CL_append(cmd->arguments, next_token);
            TOK_consume(tokens);
        }
        next_token_type = TOK_next_type(tokens);
    }
    PLD_append(pl, cmd);
//    PLD_print(pl);

    return pl;
}

