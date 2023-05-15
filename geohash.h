#pragma once

#define left  -180.0
#define right  180.0
#define bottom -90.0
#define top     90.0

char base32(unsigned);

unsigned r_base32(char);

char *geohash(const double, double, unsigned const);

void lonfix(double *);

void r_geohash(char *, double *);

unsigned long geohash_region(double, double, double, double, unsigned, char ***);
