# Info
Basic program to find all the regions with a geohash of a given lenght intersected by a given area.

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
- If given one single argument, it will be used as the hash length.
- If given 4 arguments, they will be interpreted as longitude and latitude coordinates, ordered as following:
    1. Top coordinate
    2. Left coordinate
    3. Bottom coordinate
    4. Right coordinate
- If given 5 or more arguments, the 5th will be used as the hash length, while the first four remain as above.
