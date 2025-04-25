// Node *parser(Token *tokens) {
//     Token *current_token = &tokens[0];
//     Node *root = (Node*)malloc(sizeof(Node));
//     root = init_node(root, strdup("PROGRAM"), BEGINNING);

//     Node *current = root; // we start from root
//     while((*current_token).type != END_OF_TOKENS) {
//         if(current == NULL)
//             return 0;

//         switch((*current_token).type) {
//             case KEYWORD:
//                 printf("TOKEN PARSER: %s\n", (*current_token).value);
//                 if(strcmp((*current_token).value, "EXIT") == 0) {
//                     Node *exit_node = (Node*)malloc(sizeof(Node));
//                     exit_node = init_node(exit_node, (*current_token).value, KEYWORD);
//                     (*root).right = exit_node;
//                     current = exit_node;
//                     current_token++; // move to the next token
                    
//                     if((*current_token).type == END_OF_TOKENS) {
//                         print_error(strdup("INVALID Syntax on OPEN"));
//                         return root;
//                     }

//                     if(strcmp((*current_token).value, "(") == 0 && (*current_token).type == SEPARATOR) {
//                         Node *open_paren_node = (Node*) malloc(sizeof(Node));
//                         open_paren_node = init_node(open_paren_node, (*current_token).value, SEPARATOR);
//                         (*current).left = open_paren_node;
//                         current_token++;
                        
//                         if((*current_token).type == END_OF_TOKENS) {
//                             print_error(strdup("INVALID Syntax on INT"));
//                         }
                        
//                         if((*current_token).type == INT) {
//                             printf("current token: %s\n", (*current_token).value);
                            
//                             // Handle the expression - could be a single number or an operation
//                             Token *next_token = current_token + 1;
                            
//                             if(next_token->type == OPERATOR) {
//                                 // This is an operation like 1+2
//                                 Node *oper_node = (Node*)malloc(sizeof(Node));
//                                 oper_node = init_node(oper_node, next_token->value, OPERATOR);
                                
//                                 // Create left operand node
//                                 Node *left_expr_node = (Node*)malloc(sizeof(Node));
//                                 left_expr_node = init_node(left_expr_node, current_token->value, INT);
                                
//                                 // Create right operand node
//                                 Token *right_token = next_token + 1;
//                                 if(right_token->type != INT) {
//                                     print_error(strdup("Expected integer for right operand"));
//                                 }
                                
//                                 Node *right_expr_node = (Node*)malloc(sizeof(Node));
//                                 right_expr_node = init_node(right_expr_node, right_token->value, INT);
                                
//                                 // Connect the nodes
//                                 (*(*current).left).left = oper_node;  // Set the operator as a child of the open parenthesis
//                                 (*oper_node).left = left_expr_node;   // Attach the left operand to operator's left
//                                 (*oper_node).right = right_expr_node; // Attach the right operand to operator's right
                                
//                                 // Move to closing parenthesis token (should be after right operand)
//                                 current_token = right_token + 1;
//                             } else {
//                                 // This is a single number expression like (1)
//                                 Node *expr_node = (Node*)malloc(sizeof(Node));
//                                 expr_node = init_node(expr_node, current_token->value, INT);
//                                 (*(*current).left).left = expr_node;
                                
//                                 // Move to closing parenthesis token
//                                 current_token = next_token;
//                             }
                            
//                             printf("Next token (should be ')'): %s\n", (*current_token).value);
                            
//                             if((*current_token).type == END_OF_TOKENS) {
//                                 print_error(strdup("INVALID Syntax on CLOSE"));
//                             }
                            
//                             if(strcmp((*current_token).value, ")") == 0 && (*current_token).type == SEPARATOR) {
//                                 Node *close_paren_node = (Node*)malloc(sizeof(Node));
//                                 close_paren_node = init_node(close_paren_node, (*current_token).value, SEPARATOR);
//                                 (*(*current).left).right = close_paren_node;
//                                 current_token++;
                                
//                                 if((*current_token).type == END_OF_TOKENS) {
//                                     print_error(strdup("INVALID Syntax on SEMI"));
//                                 }
                                
//                                 if(strcmp((*current_token).value, ";") == 0 && (*current_token).type == SEPARATOR) {
//                                     Node *semicolon_node = (Node*)malloc(sizeof(Node));
//                                     semicolon_node = init_node(semicolon_node, (*current_token).value, SEPARATOR);
//                                     (*current).right = semicolon_node;
//                                     current_token++; 
//                                 } else {
//                                     print_error(strdup("INVALID Syntax on SEMI"));
//                                 }
//                             } else {
//                                 print_error(strdup("INVALID Syntax on CLOSE"));
//                             }
//                         } else {
//                             print_error(strdup("INVALID Syntax on INT"));
//                         }
//                     } else {
//                         print_error(strdup("INVALID Syntax on OPEN"));
//                     }
//                 } 
//                 break;
                
//             case OPERATOR:
//                 printf("OPERATOR: %s", (*current_token).value);
//                 current_token++; 
//                 break;
                
//             case SEPARATOR:
//                 printf("SEPARATOR: %s", (*current_token).value);
                
//                 current_token++; 
//                 break;
                
//             case INT:
//                 printf("INTEGER: %s", (*current_token).value);
//                 current_token++; 
//                 break;
                
//             case BEGINNING:
//                 current_token++;
//                 break;
                
//             case END_OF_TOKENS:
//                 break;
                
//             default:
//                 current_token++;
//                 break;
//         }
        
//         print_tree(root, 0, strdup("root"));

//     }
//     return root;

// } 