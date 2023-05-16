# Info
Basic program to find all the regions with a geohash of a given lenght intersected by a given area. Since it's just a demonstration, these geohashes simply get printed to the console.

# How to use
## Requirements
Since it's such a simple project I only make use of gcc, which can be downloaded [here](https://gcc.gnu.org/mirrors.html).

## Compiling
### Windows
- Open a command prompt window in the the project directory, then compile by typing `compile` and hit enter.
### Linux/macOS
- Open the terminal in the project directory, then compile by typing `bash compile.sh` and hit enter.

## Executing
### Windows
- Open a command prompt window in the project directory, then run by typing `main` followed by the arguments.
### Linux/macOS
- Open the terminal in the project directory, then run by typing `./main` followed by the arguments.

## Arguments
- If left blank the program will work on a default hash length of 6 and the region will be between Greenwich and the Tour Eiffel.
- If given 1 argument it will be used as the hash length, still maintaining the original points.
- If given 2 arguments they will be interpreted as a single coordinate pair, default hash length of 6.
- If given a 3rd argument it will be used as the hash length, first 2 interpreted as above.
- If given 4 arguments they will be interpreted as longitude and latitude coordinates, ordered as following:
    1. Northern latitude bound,
    2. Western longitude bound,
    3. Southern latitude bound,
    4. Eastern longitude bound.
- If given a 5th argument, it will be used as the hash length, first 4 interpreted as above.
- If given more than 5 arguments, everything after the 5th gets ignored.

## Examples
Given the argument `2`, the program will output `u0, u1`, which are the regions with geohash length of 2 intersected by the area containing the two default points.
Given the arguments `45.5 10.0 7` the program will output `u0p4ge1`, which is the region with geohash length of 7 containing the point of coordinates `45.5, 10.0`.
Given the arguments `-34.9 138.6 -37.8 145.0 3` the program will output `r16, r17, r1k, r1m, r1q, r1r, r1d, r1e, r1s, r1t, r1w, r1x, r1f, r1g, r1u, r1v, r1y, r1z`, which are the regions with geohash length of 3 intersected by the area delimitated by northwest point `-34.9, 138.6` and southeast point `-37.8, 145.0`. 
