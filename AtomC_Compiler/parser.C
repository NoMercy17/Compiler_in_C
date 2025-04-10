#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"

// typedef struct Node{
//     char *value;
//     TokenType type;
//     struct Node *left;
//     struct Node *right;
// }Node;

void print_error(char *error_type) {
    char *error = strdup(error_type); // Make a copy of the string
    printf("ERROR: %s\n", error);
    free(error);
    exit(1);
}

void print_tree(Node *node, int indent, char* indentifier){
    if(node == NULL)
        return;
    for(int i=0; i< indent; i++){
        printf(" ");
    }
    printf("%s -> ", indentifier);
    for(size_t i =0; (*node).value[i] != '\0'; i++){
        printf("%c",(*node).value[i]);
    }
    printf("\n");
    print_tree((*node).left, indent +1, strdup("left"));
    print_tree((*node).right, indent +1, strdup("right"));
}

Node *init_node(Node *node, char *value, TokenType type){
    node = (Node*)malloc(sizeof(Node));
    (*node).value = (char*)malloc(sizeof(char) * 2);
    (*node).value = value;
    (*node).type = type; 
    (*node).left = NULL;
    (*node).right = NULL;
    
    return node;
} 


void generate_operation_nodes(Token *current_token ,Node *current_node){
    // we go until we get a non int or operator
    while((*current_token).type == INT || (*current_token).type == OPERATOR){
        if((*current_token).type == INT){
            Node *expre_node = (Node*)malloc(sizeof(Node));
            expre_node = init_node(expre_node, (*current_token).value, INT);
            (*(*(*current_node).left).left).left = expre_node;
        }
        if((*current_token).type == OPERATOR){
            Node *oper_node = (Node*)malloc(sizeof(Node));
            oper_node = init_node(oper_node, (*current_token).value, OPERATOR);
            (*(*current_node).left).left = oper_node;
        }   
        current_token++;
    }
}


Node *parser(Token *tokens) {
    Token *current_token = &tokens[0];
    Node *root = (Node*)malloc(sizeof(Node));
    root = init_node(root, strdup("PROGRAM"), BEGINNING);

    Node *current = root; // we start from root
    while((*current_token).type != END_OF_TOKENS) {
        if(current == NULL)
            return 0;

        switch((*current_token).type) {
            case KEYWORD:
                printf("TOKEN PARSER: %s\n", (*current_token).value);
                if(strcmp((*current_token).value, "EXIT") == 0) {
                    Node *exit_node = (Node*)malloc(sizeof(Node));
                    exit_node = init_node(exit_node, (*current_token).value, KEYWORD);
                    (*root).right = exit_node;
                    current = exit_node;
                    current_token++; // move to the next token
                    
                    if((*current_token).type == END_OF_TOKENS) {
                        print_error(strdup("INVALID Syntax on OPEN"));
                        return root; // Return to avoid infinite loop on error
                    }

                    if(strcmp((*current_token).value, "(") == 0 && (*current_token).type == SEPARATOR) {
                        Node *open_paren_node = (Node*) malloc(sizeof(Node));
                        open_paren_node = init_node(open_paren_node, (*current_token).value, SEPARATOR);
                        (*current).left = open_paren_node;
                        current_token++;
                        
                        if((*current_token).type == END_OF_TOKENS) {
                            print_error(strdup("INVALID Syntax on INT"));
                        }
                        
                        if((*current_token).type == INT) {
                            current_token++;
                            if((*current_token).type == OPERATOR) {   
                                printf("SLALLAD\n");
                                generate_operation_nodes(current_token, current);   
                            } else {
                                current_token--;
                                Node *expre_node = (Node*)malloc(sizeof(Node));
                                expre_node = init_node(expre_node, (*current_token).value, INT);
                                (*(*current).left).left = expre_node;
                                current_token++;
                            }
                            current_token++;
                            current_token++;
                            printf("current token: %s\n", (*current_token).value);
                            if((*current_token).type == END_OF_TOKENS) {
                                print_error(strdup("INVALID Syntax on CLOSE"));
                            }
                            
                            if(strcmp((*current_token).value, ")") == 0 && (*current_token).type == SEPARATOR) {
                                Node *close_paren_node = (Node*)malloc(sizeof(Node));
                                close_paren_node = init_node(close_paren_node, (*current_token).value, SEPARATOR);
                                (*(*current).left).right = close_paren_node;
                                current_token++;
                                
                                if((*current_token).type == END_OF_TOKENS) {
                                    print_error(strdup("INVALID Syntax on SEMI"));
                                }
                                
                                if(strcmp((*current_token).value, ";") == 0 && (*current_token).type == SEPARATOR) {
                                    Node *semicolon_node = (Node*)malloc(sizeof(Node));
                                    semicolon_node = init_node(semicolon_node, (*current_token).value, SEPARATOR);
                                    (*current).right = semicolon_node;
                                    current_token++; 
                                } else {
                                    print_error(strdup("INVALID Syntax on SEMI"));
                                }
                            } else {
                                print_error(strdup("INVALID Syntax on CLOSE"));
                            }
                        } else {
                            print_error(strdup("INVALID Syntax on INT"));
                        }
                    } else {
                        print_error(strdup("INVALID Syntax on OPEN"));
                    }
                } 
                break;
                
            case OPERATOR:
                printf("OPERATOR: %s", (*current_token).value);
                current_token++; 
                break;
                
            case SEPARATOR:
                printf("SEPARATOR: %s", (*current_token).value);
                
                current_token++; 
                break;
                
            case INT:
                printf("INTEGER: %s", (*current_token).value);
                current_token++; 
                break;
                
            case BEGINNING:
                current_token++;
                break;
                
            case END_OF_TOKENS:
                break;
                
            default:
                current_token++;
                break;
        }
        
        print_tree(root, 0, strdup("root"));

    }
    return root;

} 






// void generate_operation_nodes(Token *current_token ,Node *current_node){
//     Node *oper_node = (Node*)malloc(sizeof(Node));
//     oper_node = init_node(oper_node, (*current_token).value, OPERATOR);
//     (*(*current_node).left).left = oper_node;
//     while((*current_token).type == INT || (*current_token).type == OPERATOR){
//         if((*current_token).type == INT){
//             Node *expre_node = (Node*)malloc(sizeof(Node));
//             expre_node = init_node(expre_node, (*current_token).value, INT);
//             (*oper_node).left = expre_node;
//             current_token++;
//             printf("Current token value: %s",(*current_token).value); // should've printed ')'
//         }
//         if( (*current_token).type != INT || current_token != NULL){
//             printf("SYNTAX ERROR\n");
//             exit(1);
//         }
//         Node *second_expre_node = (Node*)malloc(sizeof(Node));
//         second_expre_node = init_node(second_expre_node, (*current_token).value, INT);
//         (*oper_node).right = second_expre_node;
//     }
    
//     if( (*current_token).type == OPERATOR){
//         //
//     }
//     current_token++;

// }