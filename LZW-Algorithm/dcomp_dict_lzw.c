#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "dcomp_dict_lzw.h"

enum {
        dictionarySize = 4095, // maximum number of entries defined for the dictionary (2^12 = 4096)
    	codeLength = 12, 
    	maxValue = dictionarySize - 1
};


int leftoverd = 0;
	int leftoverBitsd;



void dcomp_dict_ArrayAdd(int prefix, int character, int value) {
        dictionaryArray[value].prefix = prefix;
        dictionaryArray[value].character = character;
}

int dcomp_dict_ArrayPrefix(int value) {
        return dictionaryArray[value].prefix;
}

int dcomp_dict_ArrayCharacter(int value) {
        return dictionaryArray[value].character;
}

int readBinary(FILE * input) {
	
        int code = fgetc(input);    
    	if (code == EOF) return 0;

    	if (leftoverd > 0) {
    		code = (leftoverBitsd << 8) + code;
        
        	leftoverd = 0;
    	} 
    	else {
        	int nextCode = fgetc(input);
        
       		leftoverBitsd = nextCode & 0xF; // save leftover, the last 00001111
        	leftoverd = 1;
        
        	code = (code << 4) + (nextCode >> 4);
    	}
    	return code;
}

void decompress(FILE * inputFile, FILE * outputFile) {
	
    
        int previousCode; int currentCode;
        int nextCode = 256; 

    	int firstChar;
    
    
    	previousCode = readBinary(inputFile);
    	if (previousCode == 0) {
   	        return;
    	}
    	fputc(previousCode, outputFile);
	//printf("fputc(previousCode, outputFile);\n");    
   
   	while ((currentCode = readBinary(inputFile)) > 0) { 
    
        	if (currentCode >= nextCode) {
        		//printf("Inside if\n");
                fputc(firstChar = decode(previousCode, outputFile), outputFile); 
            
        } 
        else firstChar = decode(currentCode, outputFile); 
        
        
        if (nextCode < dictionarySize) dcomp_dict_ArrayAdd(previousCode, firstChar, nextCode++);
        
      
        previousCode = currentCode;
    	}
    	//printf("Out of While\n");
  
}

int decode(int code, FILE * outputFile) {
    	int character; int temp;

    	if (code > 255) { // decode
        	character = dcomp_dict_ArrayCharacter(code);
        	temp = decode(dcomp_dict_ArrayPrefix(code), outputFile); // recursion
    	} 	
    	else {
        	character = code; // ASCII
        	temp = code;
    	}
    	fputc(character, outputFile);
    
    	return temp;
}
