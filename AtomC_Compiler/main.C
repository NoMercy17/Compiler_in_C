#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"


int main(int argc, char *argv[]){
    if(argc < 2){
        printf("ERROR: correct syntax: %s <filename.un>\n",argv[0]);
        exit(1);
    }

    FILE *file;
    file = fopen(argv[1],"r");

    if(!file){
        printf("ERROR opening the file!\n");
        exit(1);
    }

    Token *tokens = lexer(file);

    for (size_t i =0; tokens[i].type != END_OF_TOKENS; i++){
        print_token(tokens[i]);
    }

    Node *test = parser(tokens);


/*


    generate_code(test);

    FILE *assembly_file = fopen("generated.asm","r");
    if(!assembly_file){
        printf("ERRRRROOOORRR");
        exit(1);
    }
    
    system("./buildasm.sh");
    
*/
    return 0;
}