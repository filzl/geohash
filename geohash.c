#pragma once

/* Global Includes */
#include <stdio.h>	/* Needed for error signaling, still haven't gotten used to throwing exceptions, can/will change in the future */
#include <stdlib.h>	/* Needed for memory allocation/management */

/* Local includes */
#include "geohash.h"

char base32(unsigned n){
	n %= 32; /* This function is exclusively accessed by other functions, but you never know... */

	/* A bit of a braindead if chain, I didn't really come up with anything better */
	if(n < 10)
		return n + '0';

	n += 'b' - 10;

	if(n < 'i')
		return n;

	n++;

	if(n < 'l')
		return n;

	n++;

	if(n < 'o')
		return n;

	return n + 1;
}

unsigned r_base32(char c){
	/* Reverse functionality to the previous one, in this case it's even less likely it's gonna be called when illegal */
	if(c >= '0' && c <= '9')
		return c - '0';
	
	if(c > 'o')
		c--;

	if(c > 'l')
		c--;
	
	if(c > 'i')
		c--;

	return c - 'b' + 10;
}

void lonfix(double *lon){
	/* Fixes longitude coordinates that are out of bounds */
	while(*lon>right)
		*lon += left-right;
	
	while(*lon<left)
		*lon += right-left;
}

char *geohash(const double lat, double lon, unsigned const hashlen){
	double minlat, maxlat, minlon, maxlon, mid;
	char *hash;
	unsigned currbit;
	unsigned char tmphash;

	if(lat < bottom || lat > top){
		fprintf(stderr, "(geohash) Fatal error: latitude out of bounds.\n");
		exit(-2);
	}

	lonfix(&lon);

	minlat = bottom; maxlat = top; 
	minlon = left; maxlon = right; 

	hash = (char*)calloc(hashlen + 1, sizeof(char));

	if(!hash){
		fprintf(stderr, "(geohash) Fatal error: could not allocate hash string.\n");
		exit(-1);
	}

	currbit = 0;
	while((5 * hashlen) - currbit){ /* Exits on last bit */
		if(currbit % 5 == 0) 		/* Clears the temporary hash on the beginning of a new character */
			tmphash = 0;

		tmphash <<= 1;

		if(currbit % 2 == 0){		/* If in an even position, working with longitude... */
			mid = .5 * (minlon + maxlon);

			if(lon >= mid){
				minlon = mid;
				tmphash++;
			}
			else
				maxlon = mid;
		}
		else{						/* ...otherwise latitude */
			mid = .5 * (minlat + maxlat);
			
			if(lat >= mid){
				minlat = mid;
				tmphash++;
			}
			else
				maxlat = mid;
		}

		currbit++;

		if(currbit % 5 == 0)		/* Wanted to avoid a second loop */
			hash[(currbit / 5) - 1] = base32(tmphash);
	}

	return hash;
}

void r_geohash(char *hash, double *ret){
	/* Given a hash of any length, it returns the range of the region it represents through the given array, otherwise it just prints it out */
	/* Return format is as declared below, which is minimum (latitude, longitude) and then maximum (...) */
	double minlat, maxlat, minlon, maxlon, mid;
	unsigned char tmphash;
	unsigned i, j, hashlen = -1;

	while(hash[++hashlen]);

	minlat = bottom; maxlat = top;
	minlon = left; maxlon = right;
	
	for(i = 0; i < hashlen; i++){
		tmphash = r_base32(hash[i]);
		for(j = 0; j < 5; j++){
			if(!(j % 2)^(i % 2)){	/* XNOR to get the evenness of the bit you're on, same reason as above */
				mid = .5 * (minlon + maxlon);
				if((tmphash >> (4 - j)) % 2)	/* Compact way to get desired bit isolated, could have done in many other ways */
					minlon = mid;
				else
					maxlon = mid;
			}
			else{
				mid = .5 * (minlat + maxlat);
				if((tmphash >> (4 - j)) % 2)
					minlat = mid;
				else
					maxlat = mid;
			}
		}
	}
	if(ret){
		ret[0] = minlat;
		ret[1] = minlon;
		ret[2] = maxlat;
		ret[3] = maxlon;

		return;
	}

	printf("(%f, %f), (%f, %f)\n", minlat, minlon, maxlat, maxlon);
}

unsigned long geohash_region(double nwlat, double nwlon, double selat, double selon, unsigned hashlen, char ***hashlist){
	/* nw stands for North-West, se stands for South-East */
	/* Returns the number of hashes and gives back a 2-dimensional char array (passed through arguments) */
	double *tmpcoords, tmplat, tmplon;

	unsigned long i, memsize = 1, count = 0;

	/* Basic latitude checks */
	if(nwlat > top || nwlat < bottom){
		fprintf(stderr, "(geohash_region) Fatal error: top-left latitude is out of bounds.\n");
		exit(-2);
	}
	if(selat > top || selat < bottom){
		fprintf(stderr, "(geohash_region) Fatal error: bottom-right latitude is out of bounds.\n");
		exit(-2);
	}

	tmpcoords = (double*)malloc(4 * sizeof(double));
	if(!tmpcoords){
		fprintf(stderr, "(geohash_region) Fatal error: could not allocate temporary coordinates array.\n");
		exit(-1);
	}

	if(nwlat < selat){
		fprintf(stderr, "(geohash_region) Warning: latitudes inverted, fixing...\n");	/* Didn't want to exit when it's easy to handle it */
		tmpcoords[0] = nwlat;
		nwlat = selat;
		selat = tmpcoords[0];
	}

	lonfix(&nwlon);
	lonfix(&selon);

	*hashlist = (char**)malloc(memsize * sizeof(char*));
	if(!*hashlist){
		fprintf(stderr, "(geohash_region) Fatal error: could not allocate hash list array.\n");
		free(tmpcoords);
		exit(-1);
	}

	tmplat = selat;

	do{
		tmplon = nwlon;
		do{
			if(count == memsize){
				memsize <<= 1;
				*hashlist = realloc(*hashlist, memsize * sizeof(char*));
				if(!*hashlist){
					fprintf(stderr, "(geohash_region) Fatal error: could not allocate hash list array.\n");
					free(tmpcoords);
					exit(-1);
				}
			}
			(*hashlist)[count] = geohash(tmplat, tmplon, hashlen);
			if(!(*hashlist)[count]){
				fprintf(stderr, "(geohash_region) Fatal error: could not get temporary hash.\n");
				for(i = 0; i < count; i++)
					free((*hashlist)[i]);
				free(*hashlist);
				free(tmpcoords);
				exit(-3);
			}

			r_geohash((*hashlist)[count], tmpcoords);

			count++;

			tmplon = tmpcoords[3];	/* tmplon takes the value of the maximum longitude from the geohash region (which makes the next one move east)*/

			if(tmplon == 180.0)		/* Wrapping around the end of the longitudinal coordinates in case it's needed */
				tmplon = -180.0;
		/* Inner loop makes the region go east until the "southeast" point is longitudinally inside the temporary geohash region. */
		}while((selon >= nwlon) * (tmpcoords[3]<selon) + /* Normal case, "northwest" is actually west relative to "southeast" */
		(selon < nwlon) * (!(tmpcoords[3] >= nwlon) ^ (tmpcoords[3] >= selon)));	/* Edge case, fix for when wrapping around the -180/180 lon coordinates */
		
		tmplat = tmpcoords[2];		/* tmplat takes the value of the maximum latitude from the geohash region (which makes the next one move north)*/

	}while(tmpcoords[2] < nwlat);	/* Outerloop makes the whole region go north until the "northwest" point is latitudinally inside the geohash region */
	
	free(tmpcoords);

	*hashlist = (char**)realloc(*hashlist, count*sizeof(char**));	/* Freeing of excess memory (2x-ing every time is not necessarily spatially efficient) */

	return count;
}
