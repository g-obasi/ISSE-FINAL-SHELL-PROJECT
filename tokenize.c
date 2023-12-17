// tokenize.c

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glob.h>
#include <stddef.h>
//#include "clist.h"
#include "tokenize.h"
#include "token.h"

// Documented in .h file
const char *TT_to_str(TokenType tt)
{
    switch(tt) {
        case TOK_WORD:
            return "WORD";
        case TOK_QUOTED_WORD:
            return "QUOTED_WORD";
        case TOK_LESSTHAN:
            return "LESSTHAN";
        case TOK_GREATERTHAN:
            return "GREATERTHAN";
        case TOK_PIPE:
            return "PIPE";
        case TOK_END:
            return "END";
    }

    __builtin_unreachable();
}



TokenType determine_token_type(const char* token) {

if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
        char* endptr;
        strtod(token, &endptr);
        if (*endptr == '\0' || isalpha(token[0])) {
            return TOK_WORD; // Return TOK_NUMBER for a valid number
        }
    } else if (token[0] == '\"' || token[0] == '\'' ) {
        // Check for a valid defined token within quotes
        if (strlen(token) == 3 && strchr("<>|", token[1]) != NULL && token[2] == token[0]) {
            return (token[1] == '<') ? TOK_LESSTHAN : (token[1] == '>') ? TOK_GREATERTHAN : TOK_PIPE;
        } else {
            return TOK_QUOTED_WORD;
        }
    }  else if (strchr("<>|", token[0]) != NULL) {
        return (token[0] == '<') ? TOK_LESSTHAN : (token[0] == '>') ? TOK_GREATERTHAN : TOK_PIPE;
    }
    return TOK_WORD;
}


char* process_escape_sequences(const char* str) {
    char* processed_str = malloc(strlen(str) + 1);
    size_t i = 0, j = 0;

    while (str[i] != '\0') {
        if (str[i] == '\\') {
            // Look at the next character
            switch (str[i+1]) {
                case 'n': processed_str[j] = '\n'; i++; break;
                case 'r': processed_str[j] = '\r'; i++; break;
                case 't': processed_str[j] = '\t'; i++; break;
                case '\"': processed_str[j] = '\"'; i++; break;
                case '\\': processed_str[j] = '\\'; i++; break;
                case ' ': processed_str[j] = ' '; i++; break;
                case '|': processed_str[j] = '|'; i++; break;
                case '<': processed_str[j] = '<'; i++; break;
                case '>': processed_str[j] = '>'; i++; break;
                    // Handle any other character following a backslash
                default:
                    processed_str[j++] = '\\'; // Keep the backslash
                    processed_str[j] = str[i+1]; // Include the following character
                    i++;
                    break;
            }
        } else {
            processed_str[j] = str[i];
        }
        i++;
        j++;
    }

    processed_str[j] = '\0';
    return processed_str;
}


// Documented in the .h file
CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz) {
    CList tokens = CL_new();
    size_t input_length = strlen(input);
    size_t position = 0;

    while (position < input_length) {

        char currentChar = input[position];
        // Skip whitespace
        if (isspace(currentChar)) {
            position++;
            continue;
            // Handle special characters as separate tokens
        } else if (strchr("|><;", currentChar) != NULL) {
            if (position == 0 || position == input_length - 1 ||
                strchr("|><;", input[position - 1]) != NULL ||
                strchr("|><;", input[position + 1]) != NULL) {
                snprintf(errmsg, errmsg_sz, "Invalid use of special character '%c'", currentChar);
                return NULL;
            }
            Token newToken;
            newToken.type = determine_token_type(&currentChar);
            newToken.value = NULL;
            CL_insert(tokens, newToken, -1);
            position++;
            continue;
            // handle quoted string
        }else if (currentChar == '\"') {
            size_t quote_end = position + 1;
            while (quote_end < input_length && input[quote_end] != '\"') {
                // Skip escaped quotes
                if (input[quote_end] == '\\' && (quote_end + 1) < input_length && input[quote_end + 1] == '\"') {
                    quote_end += 2;
                } else {
                    quote_end++;
                }
            }

            // Extract the quoted string
            char* quoted_string = strndup(&input[position +1], quote_end - position - 1);
            char* processed_quoted_string = process_escape_sequences(quoted_string);
            free(quoted_string);

            // Create a new token with the processed quoted string
            Token newToken;
            newToken.type = determine_token_type(&currentChar);
            newToken.value = processed_quoted_string;
            CL_insert(tokens, newToken, -1);

            // Move the position to the end of the quoted string
            position = quote_end + 1;
            continue;
        }  else {
            // Handle non-quoted strings
            size_t word_end = position;
            while (word_end < input_length && !isspace(input[word_end]) &&
                   !strchr("|><;\"", input[word_end])) {
                if (input[word_end] == '\\' && (word_end + 1) < input_length) {
                    // Skip escaped characters
                    word_end += 2;
                } else {
                    word_end++;
                }
            }

                // Extract the word
                char* word = strndup(&input[position], word_end - position);
                char* processed_word = process_escape_sequences(word);
                free(word);

                // Determine the token type
                TokenType type = determine_token_type(processed_word);

            // Globbing
            glob_t glob_result;
            if (glob(processed_word, GLOB_TILDE_CHECK, NULL, &glob_result) == 0) {
                for (size_t i = 0; i < glob_result.gl_pathc; i++) {
                    Token globbed_token;
                    globbed_token.type = TOK_WORD;
                    globbed_token.value = strdup(glob_result.gl_pathv[i]);
                    CL_append(tokens, globbed_token);
                }
                globfree(&glob_result);
            } else {

                // Create a new token with the processed word and type
                Token newToken;
                newToken.type = type;
                newToken.value = processed_word;
                CL_insert(tokens, newToken, -1);
            }

            // Move the position to the end of the word or the special character
            position = word_end;
        }

        if (position < input_length && input[position] == '\"'){
            size_t quote_end = position + 1;
            while (quote_end < input_length && input[quote_end] != '\"') {
                // Skip escaped quotes
                if (input[quote_end] == '\\' && (quote_end + 1) < input_length && input[quote_end + 1] == '\"') {
                    quote_end += 2;
                } else {
                    quote_end++;
                }
            }

            // Extract the quoted string
            char* quoted_string = strndup(&input[position +1], quote_end - position - 1);
            char* processed_quoted_string = process_escape_sequences(quoted_string);
            free(quoted_string);

            // Create a new token with the processed quoted string
            Token newToken;
            newToken.type =  determine_token_type(&currentChar);
            newToken.value = processed_quoted_string;
            CL_insert(tokens, newToken, -1);

            // Move the position to the end of the quoted string
            position = quote_end + 1;
            continue;


        }

    }

    return tokens;
}


// Documented in .h file
TokenType TOK_next_type(CList tokens)
{
    if (CL_is_empty(tokens)) {
        return TOK_END;
    } else {
        return CL_nth(tokens,0).type;
    }
}


// Documented in .h file
Token TOK_next(CList tokens)
{
    if (CL_length(tokens) == 0) {
        Token endToken;
        endToken.type = TOK_END;
        return endToken;
    } else {
        return CL_nth(tokens,0);;
    }
}


// Documented in .h file
void TOK_consume(CList tokens)
{
    if (tokens == NULL) {
        return;

    } else {
        CL_remove(tokens,0);
    }
}


/*
 * helper function to print the tokens
 *
 * Parameters:
 *   position   The list of tokens
 *   element    The element to be printed
 *   data       Additional data to be passed in
 * Returns: None.
 */
void print_token(int position, CListElementType element, void *data) {
    Token current = element;
    if (current.type == TOK_WORD || current.type == TOK_QUOTED_WORD) {
        printf("TOK_%s(\"%s\")\n", TT_to_str(current.type), current.value);
    } else {
        printf("TOK_%s\n", TT_to_str(current.type));
    }
}


void TOK_print(CList tokens) {
    CL_foreach(tokens, print_token, NULL);
}

