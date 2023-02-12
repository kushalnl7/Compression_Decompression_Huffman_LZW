#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp_dict_lzw.h" 
#include "dcomp_dict_lzw.h"
FILE *inputFile;
FILE *outputFile;

int main(int argc, char** argv) { 
    
    if (argc > 2){
        if (strcmp(argv[1], "c") == 0) { 
            inputFile = fopen(argv[2], "r"); 
            outputFile = fopen(strcat(argv[2], ".lzw"), "w+b"); 
            
            if (outputFile == NULL || inputFile == NULL) {
                printf("Can't open files\n'"); return 0;
            }
            
            compress(inputFile, outputFile);
        } 
        else { // decompression
            inputFile = fopen(argv[2], "rb"); // binary read from the input file

            char temp[20]; int length = strlen(argv[2])-4;
            strncpy(temp, argv[2], length);
            temp[length] = '\0';
            outputFile = fopen(temp, "w"); // write to output file 
            
            if (outputFile == NULL || inputFile == NULL) {
                printf("Can't open files\n'"); return 0;
            }
           // printf("decompressing\n");
            decompress(inputFile, outputFile);
        }
        
        fclose(inputFile); fclose(outputFile); // close handles
    }
    return 0;
}
