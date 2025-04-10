#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(){

    FILE *file = fopen("C:\\Users\\Antonio\\OneDrive\\Desktop\\ProiecCT\\for_random.txt", "a");

    if(!file){
        printf("Error opening the file!\n");
        exit(1);
    }
    int file_length = 0;

    fseek(file, 0, SEEK_END);
    file_length = ftell(file);
    fseek(file, 0 , SEEK_SET);

    char *buffer = (char*) malloc(file_length + 1);

    if(!buffer){
        fclose(file);
        return 0 ;
    }

    fread(buffer, 1, file_length, file);
    fputs("NEW LINE NOOB!\n", file);

    //fclose(file);

    buffer[file_length] = '\0';

    printf("%s",buffer);

    return 0;
}   
