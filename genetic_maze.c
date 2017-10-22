/*
	TODO
	- use qsort instead of bubble sort
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "genetic_maze.h"


int array[POPULATION_SIZE][MOVE_LIMIT];
int best_spec[BEST_CNT][MOVE_LIMIT];

point_t finish_pos;
point_t player;
int penalty;

scores_t scores[POPULATION_SIZE];

void (*func_ptr[5])() = {player_stay, player_move_up, player_move_down, player_move_left, player_move_right};


int maze[MAZE_X][MAZE_Y] = {	{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
								{'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', '#'},
								{'#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#'},
								{'#', ' ', '#', ' ', '#', '#', '#', ' ', ' ', '#'},
								{'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', '#'},
								{'#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', '#'},
								{'#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', '#'},
								{'#', '#', '#', ' ', '#', ' ', '#', '#', ' ', '#'},
								{'#', ' ', '#', ' ', ' ', '#', '#', '#', ' ', '#'},
								{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}};


void print_maze_path(){
	//int cnt = 0;
	char temp[MAZE_X][MAZE_Y] = {}; 
	memcpy(temp, maze, MAZE_X*MAZE_Y); 
	for(int i = 0; i < MOVE_LIMIT; i++){
		if(array[scores[0].index][i]){
			func_ptr[array[scores[0].index][i]](); // best score path
			maze[player.x][player.y] = '*';//(char)(i+97);
			//cnt++;
		}
	}

	for(int x = 0; x < MAZE_X; x++){
		for(int y = 0; y < MAZE_Y; y++){
			if(player.x == x && player.y == y)
				printf(" P ");
			else if(finish_pos.x == x && finish_pos.y == y)
				printf(" F ");
			else
				printf(" %c ", maze[x][y]);

		}
		printf("\r\n");
	}
	player_reset();
	memcpy(maze, temp, MAZE_X*MAZE_Y);
	printf("\r\n");
	//printf("Goal reached in %d moves\r\n", cnt);
}

void print_maze(){
	for(int x = 0; x < MAZE_X; x++){
		for(int y = 0; y < MAZE_Y; y++){
			if(player.x == x && player.y == y)
				printf(" P ");
			else if(finish_pos.x == x && finish_pos.y == y)
				printf(" F ");
			else
				printf(" %c ", maze[x][y]);

		}
		printf("\r\n");
	}
	printf("\r\n");
}

void player_reset(){
	player.x = 1;
	player.y = 1;
	penalty = 0;
}

void player_stay(){
	// add penalty?
	// penalty += 1;
	return;
}
void player_move_up(){
	if(maze[player.x-1][player.y] == '#'){
		penalty += PENALTY_VAL;
		return;
	}
	player.x -= 1;
}

void player_move_down(){
	if(maze[player.x+1][player.y] == '#'){
		penalty += PENALTY_VAL;
		return;
	}
	player.x += 1;
}

void player_move_left(){
	if(maze[player.x][player.y-1] == '#'){
		penalty += PENALTY_VAL;
		return;
	}
	player.y -= 1;
}

void player_move_right(){
	if(maze[player.x][player.y+1] == '#'){
		penalty += PENALTY_VAL;
		return;
	}
	player.y += 1;
}



void fill(){
	for(int x = 0; x < POPULATION_SIZE; x++){
		for(int y = 0; y < MOVE_LIMIT; y++){
			array[x][y] = rand() % 5;
		}
	}
}

// bubble sort
int* sort(int* array, int size){
	for(int x = 0; x < size - 1; x++){
		for(int i = 0; i < size - x - 1; i++){
			if(array[i] > array[i+1]){
				int c = array[i];
				array[i+1] = array[i];
				array[i] = c;
			}
		}
	}
	return array;
}

void sort_scores_t(){
	for(int x = 0; x < POPULATION_SIZE - 1; x++){
		for(int i = 0; i < POPULATION_SIZE - x - 1; i++){
			if(scores[i].score > scores[i+1].score){
			    scores_t temp = scores[i];
			    scores[i] = scores[i+1];
			    scores[i+1] = temp;
			}
		}
	}
}

void cross() {
	//copy to best_spec array
	for (int i = 0; i < BEST_CNT; i++) {
		memcpy(best_spec[i], array[scores[i].index], MOVE_LIMIT);
	}

	int firstParent = rand() % BEST_CNT;
	int secondParent = rand() % BEST_CNT;
	int crossOver = rand() % MOVE_LIMIT;

	for (int i = BEST_CNT + REPRODUCTIVE; i < POPULATION_SIZE; i++) {
		memcpy(array[i], best_spec[firstParent], crossOver);
		memcpy(array[i], best_spec[secondParent], (MOVE_LIMIT - crossOver));
	}

}

int fitness(int chromo){
	for(int i = 0; i < MOVE_LIMIT; i++){
		func_ptr[array[chromo][i]]();
	}

	int score = abs(finish_pos.x - player.x) + abs(finish_pos.y - player.y) + penalty;

	player_reset();
	return score;
}

int score(){
	for(int i = 0; i < POPULATION_SIZE; i++){
		scores[i].score = fitness(i);
		scores[i].index = i;
		//printf("%d %d\n", scores[i].score, scores[i].index);
	}
	sort_scores_t();
	return scores[0].index;
}
//

void move_player(){
	for(int i = 0; i < MOVE_LIMIT; i++){
		func_ptr[array[scores[0].index][i]]();
	}
}

void mutate(){
	for(int i = 0; i < POPULATION_SIZE-1; i++){
		if(MUTATION_CHANCE > (double)rand() / (double)RAND_MAX){ // random number (0,1)
			array[i][rand() % 30] = rand() % 5; // chhose random element and mutate
		}
	}
}

int main(){
	time_t t;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	player.x = 1;
	player.y = 1;
	finish_pos.x = 8;
	finish_pos.y = 8;
	fill();
	printf("Start maze: \r\n");
	print_maze();

	for(int i = 0; i < 50000; i++){
		mutate();
		cross();
		score();
		if(scores[0].score < FINAL_SCORE){
			print_maze_path();
			printf("Path found in %d generation\n", i);
			printf("Score: %d\n", scores[0].score);

			break;
		}
		if(i % 100 == 0){
			printf("generation: %d\n", i);
			//move_player(); // best
			//print_maze();
			//player_reset();
		}
	}
}