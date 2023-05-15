#include "geohash.c"

int main(int argc, char **args){
	unsigned long hashcount, i;
	unsigned hashlen;
	char **hashlist;
	double north, west, south, east;

	/* Greenwich */
	north = 51.5;
	west = 0;

	/* Tour Eiffel */
	south = 48.858;
	east = 2.294;
	
	hashlen = 6;
		
	if(argc>4){
		north = strtod(args[1], NULL);
		west = strtod(args[2], NULL);
		south = strtod(args[3], NULL);
		east = strtod(args[4], NULL);
	}else if(argc>1)
		hashlen = atoi(args[1]);

	if(argc>5)
		hashlen = atoi(args[5]);
		

	hashcount = geohash_region(north, west, south, east, hashlen, &hashlist);

	printf("(");
	for(i = 0; i < hashcount; i++){
		if(i)
			printf(", ");
		printf("%s", hashlist[i]);
		free(hashlist[i]);
	}
	printf(")\n");
	
	free(hashlist);

	return 0;
}
