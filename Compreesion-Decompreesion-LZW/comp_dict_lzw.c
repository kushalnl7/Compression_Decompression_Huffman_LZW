#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp_dict_lzw.h"

int leftover = 0;
int leftoverBits;

enum {
    emptyPrefix = -1 // empty prefix for ASCII characters
};


enum {
    dictionarySize = 4095, 
    codeLength = 12, 
    maxValue = dictionarySize - 1
};


void comp_dict_Init(){
	int i;
    	struct comp_DictNode *node;
	for (i = 0; i < 256; i++) { 
        	node = (struct comp_DictNode *)malloc(sizeof(struct comp_DictNode));
        	node->prefix = emptyPrefix;
        	node->character = i;
        	comp_appendNode(node);
        }       
}
void comp_appendNode(struct comp_DictNode *node){
	if (dictionary != NULL) tail->next = node;
        else dictionary = node;
        tail = node;
        node->next = NULL;
}
void comp_dict_Destroy(){
	while (dictionary != NULL) {
        	dictionary = dictionary->next; 
        }
}
int comp_dict_Lookup(int prefix, int character){
	struct comp_DictNode *node;
        for (node = dictionary; node != NULL; node = node->next) { // ...traverse forward
        	if (node->prefix == prefix && node->character == character) return node->value;
        }
        return emptyPrefix;	
}
void comp_dict_Add(int prefix, int character, int value){
	struct comp_DictNode *node;
        node = (struct comp_DictNode *)malloc(sizeof(struct comp_DictNode));
    	node->value = value;
    	node->prefix = prefix;
    	node->character = character;
    	//printf("\n(%i) = (%i) + (%i)\n", node->value, node->prefix, node->character);
    	comp_appendNode(node);
}
void comp_writeBinary(FILE * output, int code){
	if (leftover > 0) {
	        int previousCode = (leftoverBits << 4) + (code >> 8);
        
        	fputc(previousCode, output);
        	fputc(code, output);
        
       		leftover = 0; // no leftover now
    	} 
    	else {
        	leftoverBits = code & 0xF; // save leftover, the last 00001111
        	leftover = 1;
        	
        	fputc(code >> 4, output);
	}
}


void compress(FILE *inputFile, FILE *outputFile){
	   
        int prefix = getc(inputFile);
    	if (prefix == EOF) {
        	return;
    	}
    	int character;

	int nextCode;
        int index;
    
   
    	nextCode = 256; 
    	comp_dict_Init();
    
    
    	while ((character = getc(inputFile)) != (unsigned)EOF) { // ch = read a character;
        
        
        	if ((index = comp_dict_Lookup(prefix, character)) != -1) prefix = index; // prefix = prefix+character
        	else { 
        	    	comp_writeBinary(outputFile, prefix);
            
            // add prefix+character to dictionary
        	    	if (nextCode < dictionarySize) comp_dict_Add(prefix, character, nextCode++);
            
            // prefix = character
        	    	prefix = character; //... output the last string after adding the new one
        	}
    	}
    // encode s to output file
    	comp_writeBinary(outputFile, prefix); // output the last code
    
    	if (leftover > 0) fputc(leftoverBits << 4, outputFile);
    
    // free the dictionary here
    	comp_dict_Destroy();
}

