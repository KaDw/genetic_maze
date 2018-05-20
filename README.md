# genetic_maze
Maze solving using genetic algorithm in about 200 lines of C code

## Compilation
To compile you can use existing makefile or just type in cosnole 

`gcc -std=c11 -g -Wall -Wextra -o genetic_maze genetic_maze.c genetic_maze.h`

I'm using GCC 6.3.0

## Usage
Run:
```genetic_maze [number_of_iterations] ```
for example
```genetic_maze 4000```

## Parameters

There are a few defines in `genetic_maze.h` you can change:

* POPULATION_SIZE - size of population
* BEST_CNT - number of best specimens used for corssing, from this part of population we will create new specimens (after corssing). Also, we don't modify them
* REPRODUCTIVE - specimens good enough to leave them as they are
* PENALTY_VAL - penalty for hitting the wall 
* MUTATION_CHANCE - chance to modify random specimen 
* FINAL_SCORE - if score is lower than this value algorithm will stop

Population array will look like this:

```
[ BEST_CNT | REPRODUCTIVE | POPULATION_SIZE - BEST_CNT - REPRODUCTIVE ]   
  ^           ^              ^
  |           |               This part will be replace by new specimens created by corssing best specimens
  |            Specimens good enough to leave them as they are
   Best specimens used for crossing, we also dont modify them 
```

   