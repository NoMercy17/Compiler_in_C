#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "parser.h"


void print_error(char *error_type) {
    char *error = strdup(error_type); // Make a copy of the string
    printf("ERROR: %s\n", error);
    free(error);
    exit(1);
}

void print_tree(Node *node, int indent, char* indentifier) {
    if(node == NULL)
        return;
    for(int i=0; i< indent; i++) {
        printf(" ");
    }
    printf("%s -> ", indentifier);
    for(size_t i =0; (*node).value[i] != '\0'; i++) {
        printf("%c",(*node).value[i]);
    }
    printf("\n");
    print_tree((*node).left, indent +1, strdup("left"));
    print_tree((*node).right, indent +1, strdup("right"));
}

Node *init_node(Node *node, char *value, TokenType type) {
    node = (Node*)malloc(sizeof(Node));
    (*node).value = (char*)malloc(sizeof(char) * 2);
    (*node).value = value;
    (*node).type = type; 
    (*node).left = NULL;
    (*node).right = NULL;
    
    return node;
}

Node *parse_expression(Token **token_ptr) {
    Token *current = *token_ptr;
    
    // Parse first integer
    // if(current->type != INT) {
    //     print_error(strdup("Expected integer at start of expression"));
            //THINK NOT NEEDED
    // }
    
    Node *left_expr = (Node*)malloc(sizeof(Node));
    left_expr = init_node(left_expr, current->value, INT);
    current++;
    
    // Continue parsing operators and operands as long as we find them
    while((*current).type == OPERATOR) {
        // Create operator node
        Token *op_token = current;
        Node *op_node = (Node*)malloc(sizeof(Node));
        op_node = init_node(op_node, op_token->value, OPERATOR);
        current++;
        
        // Parse right operand
        if( (*current).type != INT) {
            print_error(strdup("Expected integer after operator"));
        }
        
        Node *right_expr = (Node*)malloc(sizeof(Node));
        right_expr = init_node(right_expr, current->value, INT);
        current++;
        
        // Link nodes - make operator the new root with left_expr as its left child
        // and right_expr as its right child
        (*op_node).left = left_expr;
        (*op_node).right = right_expr;
        
        // Update left_expr for the next iteration
        left_expr = op_node;
    }
    
    // Update the token pointer
    *token_ptr = current;
    return left_expr;
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
                            printf("current TOKeN: %s\n", (*current_token).value);
                            
                            // Parse expression
                            Node *expr_node = parse_expression(&current_token);
                            (*(*current).left).left = expr_node;
                            
                            printf("Next toKen : %s\n", (*current_token).value);
                            
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