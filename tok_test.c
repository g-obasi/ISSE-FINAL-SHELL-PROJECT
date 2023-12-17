/*
 * tok_test.c
 *
 * tests cases for tokenize.c
 *
 * Author: Greenfield Obasi
 */



#include <assert.h>
#include "tokenize.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>


void test_determine_token_type() {
    assert(determine_token_type("123") == TOK_WORD);
    assert(determine_token_type("\"hello\"") == TOK_QUOTED_WORD);
    assert(determine_token_type("<") == TOK_LESSTHAN);
    assert(determine_token_type(">") == TOK_GREATERTHAN);
    assert(determine_token_type("|") == TOK_PIPE);
//    assert(determine_token_type(" ") == TOK_END);
    printf("passed the determine_token_type==word test\n");
}


void test_TOK_tokenize_input() {
    char errmsg[100];
    CList tokens = TOK_tokenize_input("hello < | 123", errmsg, sizeof(errmsg));
//    TOK_print(tokens);
    assert(tokens != NULL);
    assert(TOK_next_type(tokens) == TOK_WORD);
    TOK_consume(tokens);
    assert(TOK_next_type(tokens) == TOK_LESSTHAN);
    TOK_consume(tokens);
    assert(TOK_next_type(tokens) == TOK_PIPE);
    TOK_consume(tokens);
    assert(TOK_next_type(tokens) == TOK_WORD);
    TOK_consume(tokens);
//    assert(TOK_next_type(tokens) == TOK_END);
}


int main() {
    test_determine_token_type();
    test_TOK_tokenize_input();

    while (1) {
        char input[100];
        char errmsg[100];

        printf("Enter input string to tokenize (or 'exit' to quit): ");
        fgets(input, 100, stdin);

        // If the user types 'exit', break out of the loop
        if (strcmp(input, "exit\n") == 0 || strcmp(input, "quit\n") == 0) {
            break;
        }

        CList tokens = TOK_tokenize_input(input, errmsg, 100);
        TOK_print(tokens);

        if (tokens == NULL) {
            printf("Error tokenizing input: %s\n", errmsg);
            return 1;
        }
    }

}
