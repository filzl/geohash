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

	if(argc == 2)
		hashlen = atoi(args[1]);
	
	else if(argc >= 3){
		north = strtod(args[1], NULL);
		west = strtod(args[2], NULL);

		if(argc == 4)
			hashlen = atoi(args[3]);

		if(argc >= 5){
			south = strtod(args[3], NULL);
			east = strtod(args[4], NULL);
			if(argc >= 6)
				hashlen = atoi(args[5]);
		}else{
			south = strtod(args[1], NULL);
			east = strtod(args[2], NULL);
		}
	}

	hashcount = geohash_region(north, west, south, east, hashlen, &hashlist);

	for(i = 0; i < hashcount; i++){
		if(i)
			printf(", ");
		printf("%s", hashlist[i]);
		free(hashlist[i]);
	}

	printf("\n");
	
	free(hashlist);

	return 0;
}
