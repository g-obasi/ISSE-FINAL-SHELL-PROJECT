/*
* token.h
*
* Enum containing all possible tokens
*
* Author:
*/

#ifndef TOKEN_H_
#define TOKEN_H_


/*
 * TokenType: Enum containing all possible tokens
 */
typedef enum {
    TOK_WORD,
    TOK_QUOTED_WORD,
    TOK_LESSTHAN,
    TOK_GREATERTHAN,
    TOK_PIPE,
    TOK_END
} TokenType;



typedef struct {
    TokenType type;
    char* value; // For TOK_WORD and TOK_QUOTED_WORD, represents the actual word or quoted word
} Token;


//typedef CommandStruct Command;

#endif /* TOKEN_H_ */
