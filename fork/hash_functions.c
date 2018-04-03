#include <stdio.h>
#include "hash.h"
#include <stdlib.h>
// Complete these two functions according to the assignment specifications

char* hash(FILE *input) {
	
    char *hash_val= (char*)malloc((sizeof(char)* (BLOCK_SIZE + 1)));
    // initialize the hash_val to '\0'
    int j = 0;
    for(j = 0; j < BLOCK_SIZE; j++){
    	hash_val[j] = '\0';
    }
    char loaded_value;
    int i = 0;
    while (fread(&loaded_value, 1, 1, input) == 1){
    	hash_val[i] = hash_val[i] ^ loaded_value;
    	i++;
    	if (i == BLOCK_SIZE){
    		i = 0;
    	}
    }
    return hash_val;
}
