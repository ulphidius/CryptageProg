/*
 * function.h
 *
 *  Created on: 17 déc. 2017
 *      Author: louis
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

void launch_algo_cryptage();
void launch_algo_decryptage();
void matrice(char *);
char chiffrer(int);
FILE* createFile(char *);
void sortKey(int *);
int getCodeDecryptage(unsigned char, int *);

#endif /* FUNCTION_H_ */
