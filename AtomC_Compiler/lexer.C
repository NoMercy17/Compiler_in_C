#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"

size_t line_number = 1;

void print_token(Token token){
    printf("TOKEN VALUE: ");
    printf("'");
    for(int i=0; token.value[i] != '\0'; i++)
        printf("%c",token.value[i]);
    printf("' ");
    printf("line number: %llu \n", token.line_num);

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
        case IDENTIFIER:
            printf("TOKEN TYPE: IDENTIFIER\n");
            break;
        case STRING:
            printf("TOKEN TYPE: STRING\n");
            break;
        case COMP:
            printf("TOKEN TYPE COMP\n");
            break;
        case BEGINNING:
            printf("BEGINNING\n");
            break;
        case END_OF_TOKENS:
            printf("END_OF_TOKENS\n");
            break;
    }
}

Token *generate_string_token(char *current, int *current_index){
    Token *token = (Token*)malloc(sizeof(Token));
    char *current_string_value = (char*)malloc(sizeof(char)*100);
    int string_value_index = 0;
    *current_index +=1; // we go to the actual value inside the "

    while(current[*current_index] != '"'){
        current_string_value[string_value_index] = current[*current_index];
        string_value_index++;
        (*current_index)++;
    }
    current_string_value[string_value_index] = '\0';
    (*token).type = STRING;
    (*token).value = current_string_value;
    return token;

}

Token *generate_number(char *current, int *current_index){
    Token *token = (Token*)malloc(sizeof(Token));
    (*token).line_num = (size_t)malloc(sizeof(size_t)); //memory managment 
    (*token).line_num = line_number;
    (*token).type = INT;

    char *value = (char*)malloc(sizeof(char) * 8); // memory managment
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

Token *generate_keyword_or_identifier(char *current, int *current_index){
    Token *token = (Token*)malloc(sizeof(Token));
    char *keyword = (char*)malloc(sizeof(char) * 15); // a bit bigger safety
    (*token).line_num = (size_t)malloc(sizeof(size_t));
    (*token).line_num = line_number;
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
    }else if(strcmp(keyword,"int") == 0){
        (*token).value = (char*)malloc(4); 
        strcpy(token->value, "INT");
        (*token).type = KEYWORD;
    }else if(strcmp(keyword,"if") == 0){
        (*token).value = (char*)malloc(3); 
        strcpy(token->value, "IF");
        (*token).type = KEYWORD;
    }else if(strcmp(keyword,"while") == 0){
        (*token).value = (char*)malloc(6); 
        strcpy(token->value, "WHILE");
        (*token).type = KEYWORD;
    }else if(strcmp(keyword,"write") == 0){
        (*token).value = (char*)malloc(6); 
        strcpy(token->value, "WRITE");
        (*token).type = KEYWORD;
    }else if(strcmp(keyword,"less") == 0){
        (*token).value = (char*)malloc(5); 
        strcpy(token->value, "LESS");
        (*token).type = COMP;
    }else if(strcmp(keyword,"greater") == 0){
        (*token).value = (char*)malloc(8); 
        strcpy(token->value, "GREATER");
        (*token).type = COMP;
    }else if(strcmp(keyword,"eq") == 0){
        (*token).value = (char*)malloc(3);
        strcpy(token->value, "EQ");
        (*token).type = COMP;
    }else if(strcmp(keyword,"neq") == 0){
        (*token).value = (char*)malloc(4); 
        strcpy(token->value, "NEQ");
        (*token).type = COMP;
    }else{
        (*token).value = (char*)malloc(strlen(keyword) +1); 
        strcpy( (*token).value, keyword);
        (*token).type = IDENTIFIER;
    }

    free(keyword);
    return token;
}

Token *generate_separator_or_operator(char *current, int *current_index, TokenType type){
    Token *token = (Token*)malloc(sizeof(Token));
    (*token).value = (char*)malloc(sizeof(char) * 2);
    (*token).value[0] = current[*current_index];
    (*token).value[1] = '\0';
    (*token).line_num = (size_t)malloc(sizeof(size_t)); 
    (*token).line_num = line_number;    
    (*token).type = type;

    return token;
}

size_t tokens_index = 0;

Token *lexer(FILE *file){
    int length;
    char *current = 0;
  
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
  
    current = (char*)malloc(sizeof(char) * length);
    fread(current, 1, length, file);
  
    fclose(file);
  
    current[length] = '\0';
    int current_index = 0;
  
    int number_of_tokens = 12;
    int tokens_size = 0;
    Token *tokens = (Token*)malloc(sizeof(Token) * number_of_tokens);
    tokens_index = 0;
  
    while(current[current_index] != '\0'){
      Token *token = (Token*)malloc(sizeof(Token));
      int advanced = 0;  // if we've advanced current_index

      tokens_size++;
      if(tokens_size > number_of_tokens){
        number_of_tokens *= 1.5;
        tokens = (Token*)realloc(tokens, sizeof(Token) * number_of_tokens);
      }
      if(current[current_index] == ';'){
        token = generate_separator_or_operator(current, &current_index, SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == ','){
        token = generate_separator_or_operator(current, &current_index, SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '('){
        token = generate_separator_or_operator(current, &current_index, SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == ')'){
        token = generate_separator_or_operator(current, &current_index, SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '{'){
        token = generate_separator_or_operator(current, &current_index, SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '}'){
        token = generate_separator_or_operator(current, &current_index, SEPARATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '='){
        token = generate_separator_or_operator(current, &current_index, OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '+'){
        token = generate_separator_or_operator(current, &current_index, OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '-'){
        token = generate_separator_or_operator(current, &current_index, OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '*'){
        token = generate_separator_or_operator(current, &current_index, OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '/'){
        token = generate_separator_or_operator(current, &current_index, OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '%'){
        token = generate_separator_or_operator(current, &current_index, OPERATOR);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      } else if(current[current_index] == '"'){
        token = generate_string_token(current, &current_index);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 0;  
      }else if(isdigit(current[current_index])){
        token = generate_number(current, &current_index); 
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 1;  // generate_keyword does advance current_index
      } else if(isalpha(current[current_index])){
        token = generate_keyword_or_identifier(current, &current_index);
        tokens[tokens_index] = *token;
        tokens_index++;
        advanced = 1;  
      } else if(current[current_index] == '\n'){
            line_number += 1; 
        }
        free(token);
        // we increment oonly if we didn t increment in the generate  functions
        if (!advanced) {
            current_index++;
        }
    }
    tokens[tokens_index].type = END_OF_TOKENS;
    tokens[tokens_index].value = (char*)malloc(1);
    tokens[tokens_index].value[0] = '\0';

    return tokens;
  }