/*
 * function.c
 *
 *  Created on: 17 déc. 2017
 *      Author: louis
 */

#include "function.h"

static char arrayKey[4] = {0};

char chiffrer(int code) {
	int i;
	char result = 0;
	for (i = 3; i >= 0; i--) {
		if (code & 1) {
			// XOR de la ligne activé
			result ^= arrayKey[i];
		}
		code >>= 1;
	}
	return result;
}

// Récupération de la matrice de cryptage
void matrice(char *fileName){
	int i;
	int cpt1 = 0, cpt2 = 0;
	int ref = 41;
	char *refSt = "G4C=[";
	char input[MAX] = {0};
	FILE * key;

	key = fopen(fileName, "rb");

	if(key == NULL){
	    exit(EXIT_FAILURE);
	}

	if(fread(input, sizeof(char), 50, key) == -1){
	    exit(EXIT_FAILURE);
	}

	if(strlen(input) == ref && strstr(input, refSt) != NULL){
	    for(i = 0; i < 50; i++){
	        if(input[i] == '0' || input[i] == '1'){
	        	arrayKey[cpt1] *= 2;
	            if(input[i] == '1'){
	                arrayKey[cpt1] += 1;
	            }
	            cpt2++;
	            if(cpt2 == 8){
	                cpt1++;
	                cpt2 = 0;
	            }
	        }
	    }
	}else{
	    printf("Matrice non conforme");
	    exit(EXIT_FAILURE);
	}
	fclose(key);
}

FILE* createFile(char * fileName){
	FILE * openFile = NULL;
	printf("Creation du fichier : %s \n", fileName);
	openFile = fopen(fileName, "w+");
	if(openFile == NULL){
		printf("Creation du fichier echec ! \n");
		exit(EXIT_FAILURE);
	}

	return openFile;
}

// Récupération de la matrice identité
void sortKey(int * array){
	int i;
	for(i = 7; i >= 0; i--){
		if(((arrayKey[0] >> i) & 1) == 1
				&& ((arrayKey[1] >> i) & 1) == 0
				&& ((arrayKey[2] >> i) & 1) == 0
				&& ((arrayKey[3] >> i) & 1) == 0){
			array[0] = 7 - i;
		}
		if(((arrayKey[0] >> i) & 1) == 0
				&& ((arrayKey[1] >> i) & 1) == 1
				&& ((arrayKey[2] >> i) & 1) == 0
				&& ((arrayKey[3] >> i) & 1) == 0){
			array[1] = 7 - i;
		}
		if(((arrayKey[0] >> i) & 1) == 0
				&& ((arrayKey[1] >> i) & 1) == 0
				&& ((arrayKey[2] >> i) & 1) == 1
				&& ((arrayKey[3] >> i) & 1) == 0){
			array[2] = 7 - i;
		}
		if(((arrayKey[0] >> i) & 1) == 0
				&& ((arrayKey[1] >> i) & 1) == 0
				&& ((arrayKey[2] >> i) & 1) == 0
				&& ((arrayKey[3] >> i) & 1) == 1){
			array[3] = 7 - i;
		}
	}
}

// Récupération du message d'origine avec la matrice identité
int getCodeDecryptage(unsigned char c, int * codeKey){
	int i;
	int result = 0;
	for(i = 0; i < 4; i++) {

		// Déplacement de 1 dans le binaire
		result *= 2;
		result += (c >> (7 - codeKey[i])) & 1;
	}
	return result;
}
