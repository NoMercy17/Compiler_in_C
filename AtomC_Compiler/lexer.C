#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#include "lexer.h"
// typedef enum{
//     BEGINNING,
//     INT,
//     KEYWORD,
//     SEPARATOR,
//     OPERATOR,
//     END_OF_TOKENS,
// }TokenType;

// typedef struct{
//     TokenType type;
//     char* value;
// }Token;

void print_token(Token token){
    printf("TOKEN VALUE: ");
    printf("'");
    for(int i=0; token.value[i] != '\0'; i++)
        printf("%c",token.value[i]);
    printf("' ");

    switch(token.type){
        case INT:
            printf("TOKEN TYPE: INT\n");
            break;
        case KEYWORD:
            printf("TOKEN TYPE: KEYWORD\n");
            break;        
        case SEPARATOR:
            printf("TOKEN TYPE: SEPARATOR\n");
            break;
        case OPERATOR:
            printf("TOKEN TYPE: OPERATOR\n");
            break;
        case BEGINNING:
            printf("BEGINNING\n");
            break;
        case END_OF_TOKENS:
            printf("END_OF_TOKENS\n");
            break;
    }
}


Token *generate_number(char *current, int *current_index){
    Token *token = (Token*)malloc(sizeof(Token));
    (*token).type = INT;
    char *value = (char*)malloc(sizeof(char) * 8);
    int value_index = 0;

    while(current[*current_index] != '\0' && isdigit(current[*current_index])){
        value[value_index] = current[*current_index];
        value_index++;
        (*current_index)++;
    }
    value[value_index] = '\0';
    (*token).value = value;

    return token;
}

Token *generate_keyword(char *current, int *current_index){
    Token *token = (Token*)malloc(sizeof(Token));
    char *keyword = (char*)malloc(sizeof(char) * 15); // bigger buffer for safety
    int keyword_index = 0;

    while(current[*current_index] != '\0' && isalpha(current[*current_index])){
        keyword[keyword_index] = current[*current_index];
        keyword_index++;
        (*current_index)++;
    }
    keyword[keyword_index] = '\0';

    if(strcmp(keyword,"exit") == 0){
        (*token).value = (char*)malloc(5); // "EXIT" + '\0'
        strcpy(token->value, "EXIT");
        (*token).type = KEYWORD;
    }
    free(keyword);
    return token;
}

Token *generate_separator_or_operator(char *current, int *current_index, TokenType type){
    Token *token = (Token*)malloc(sizeof(Token));
    (*token).value = (char*)malloc(sizeof(char) * 2);
    (*token).value[0] = current[*current_index];
    (*token).value[1] = '\0';
    (*token).type = type;

    return token;
}


size_t token_index;

Token *lexer(FILE *file){
    int length;
    char *current=0;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET); // we go back to the start after we got length
    
    current = (char*)malloc(sizeof(char) * (length + 1)); // +1 for null terminator    
    fread(current, 1, length, file);

    fclose(file); 

    current[length] = '\0'; 
    int current_index = 0;

    // tokens array
    Token *tokens = (Token*)malloc(sizeof(Token) * 100);
    token_index = 0;

    while(current[current_index] != '\0'){
        Token *token = (Token*)malloc(sizeof(Token));
        int advanced = 0;  // if we've advanced current_index
    
        if(current[current_index] == ';') {
            token = generate_separator_or_operator(current, &current_index, SEPARATOR);
            tokens[token_index] = *token;
            token_index++;
            advanced = 0;  
        } else if(current[current_index] == '(') {
            token = generate_separator_or_operator(current, &current_index, SEPARATOR);
            tokens[token_index] = *token;
            token_index++;
            advanced = 0;
        } else if(current[current_index] == ')') {
            token = generate_separator_or_operator(current, &current_index, SEPARATOR);
            tokens[token_index] = *token;
            token_index++;
            advanced = 0;
        } else if(isdigit(current[current_index])){
            token = generate_number(current, &current_index);
            tokens[token_index] = *token;
            token_index++;
            advanced = 1;  // generate_number does advance current_index
        } else if (current[current_index] == '+'){
            token = generate_separator_or_operator(current, &current_index, OPERATOR);
            tokens[token_index] = *token;
            token_index++;
            advanced = 0;
        } else if(isalpha(current[current_index])){
            token = generate_keyword(current, &current_index);
            tokens[token_index] = *token;
            token_index++;
            advanced = 1;  // generate_keyword does advance current_index
        }
        free(token);
        // we increment oonly if we didn t increment in the generate  functions
        if (!advanced) {
            current_index++;
        }
    }
    // END_OF_TOKENS marker
    tokens[token_index].type = END_OF_TOKENS;
    tokens[token_index].value = (char*)malloc(1);
    tokens[token_index].value[0] = '\0';
    return tokens;
}