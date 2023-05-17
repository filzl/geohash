#pragma once

/* Global includes */
#include <stdio.h> /* Error signaling */
#include <stdlib.h> /* Memory management */

/* Local includes */
#include "stringsort.h"

char fstrcmp(char* a, char* b){
	unsigned long i;
	for(i = 0; a[i] == b[i] && a[i] && b[i]; i++);

	if(a[i] > b[i])
		return 1;

	return 0;
}

void stringsort(char **array, unsigned long length){
	char **lef;
	unsigned long llen, rlen, i, j, k;
	
	if(length >= 2){
		/* Setting up the left part of the array */
		llen = length >> 1;
		stringsort(&(array[0]), llen);

		lef = (char**)malloc(llen * sizeof(char*));
		if(!lef){
			fprintf(stderr, "(stringsort) Fatal error: could not allocate temporary left array\n");
			exit(-1);
		}
		
		for(i = 0; i < llen; i++)
			lef[i] = array[i];

		/* Setting up the right part of the array */
		rlen = length - llen;
		
		stringsort(&(array[llen]), rlen);

		i = 0;
		j = llen;
		k = 0;

		while(i < llen && j < length){
			if(fstrcmp(lef[i], array[j])){
				array[k] = array[j++];
			}else{
				array[k] = lef[i++];
			}
			k++;
		}
		while(i < llen)
			array[k++] = lef[i++];
		while(j < length)
			array[k++] = array[j++];
			
		free(lef);
	}

	return;
}
