# Final Project jtw6-dada3

## [Video](https://drive.google.com/file/d/1ZRWRF6l9ZnkPYXckeGQIRglWIXSMcrXr/view?usp=sharing)  (earphones highly recommended)
## Code locations
The code required to process the input files is located in "readFromFile.cpp" and "readFromFile.h". 
The Graph implementation, Depth first Search, and Djikstra's algorithm, as well as the class defintions are stored in "classes/classes.h" and "classes/classes.cpp"
The image processing functions are stored in "main.cpp"
The provided data can be found in the folder "data"

## Instructions for running executable

Running the main executable is easy.
Simply execute 'make' in the project directory to run with default values which are as follows:
|Djikstra's source|Djikstra's destination|Routes File    |Airports File    |
|-----------------|----------------------|---------------|-----------------|
|JFK              |LAX                   |data/routes.dat|data/airports.dat|

You can run the executable with different data with the following command:

'./main [source] [destination] [routes file] [airports file]'

Where the source and destination are IATA codes. A useful resource to look up these codes is available [here](https://www.iata.org/en/publications/directories/code-search/)

If you wish to specify new route and airport data, don't forget to input the Djikstra's source and destinaiton values so as to preserve the ordering.

This will generate two files
'airports.png' and '[source]_[destination].png'

The former shows all airports which have routes, and the latter shows the shortest path between the source and destination.
Also, the executable will print the depth first traversal to the terminal followed by the distance of the shortest path.

### Instructions for running test cases

All of our test cases were created using Catch
run 'make test'
execute './test'
if  you wish to run only one test case, 
execute './test -s "[test_name]"'
