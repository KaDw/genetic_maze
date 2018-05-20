/*
	TODO
	- use qsort instead of bubble sort
	- add time measurement
	- test with one more stay state? (up, down, left, right, stay)
*/
#include "genetic_maze.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int (*func_ptr[])(point_t*) = {player_move_up, player_move_down, player_move_left, player_move_right};

int population[POPULATION_SIZE][MOVE_LIMIT];
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

void print_maze(point_t *player, point_t* finish_pos){
	for(int x = 0; x < MAZE_X; x++){
		for(int y = 0; y < MAZE_Y; y++){
			if(player->x == x && player->y == y){
				printf(" P ");
			}
			else if(finish_pos->x == x && finish_pos->y == y){
				printf(" F ");
			} else{
				printf(" %c ", maze[x][y]);
			}
		}
		printf("\r\n");
	}
	printf("\r\n");
}

void print_maze_path(scores_t scores[], point_t* finish_pos){
	point_t temp_player = {.x = 1, .y = 1};
	for(size_t i = 0; i < MOVE_LIMIT; i++){
		func_ptr[population[scores[0].index][i]](&temp_player); // move player according to best score path
		if(maze[temp_player.x][temp_player.y] != '#'){ // if not wall move player
			maze[temp_player.x][temp_player.y] = '^';
		}
	}

	print_maze(&temp_player, finish_pos);
}

void player_reset(point_t* player){
	player->x = 1;
	player->y = 1;
}

int player_move_up(point_t* player){
	if(maze[player->x - 1][player->y] == '#'){
		return PENALTY_VAL;
	}
	player->x -= 1;
	return 0;
}

int player_move_down(point_t* player){
	if(maze[player->x + 1][player->y] == '#'){
		return PENALTY_VAL;
	}
	player->x += 1;
	return 0;
}

int player_move_left(point_t* player){
	if(maze[player->x][player->y - 1] == '#'){
		return PENALTY_VAL;
	}
	player->y -= 1;
	return 0;
}

int player_move_right(point_t* player){
	if(maze[player->x][player->y + 1] == '#'){
		return PENALTY_VAL;
	}
	player->y += 1;
	return 0;
}

void fill(int array[][MOVE_LIMIT]){
	for(size_t x = 0; x < POPULATION_SIZE; x++){
		for(size_t y = 0; y < MOVE_LIMIT; y++){
			array[x][y] = rand() % DIRECTIONS_LEN;
		}
	}
}

void sort_scores(scores_t scores[]){
	for(size_t x = 0; x < POPULATION_SIZE - 1; x++){
		for(size_t i = 0; i < POPULATION_SIZE - x - 1; i++){
			if(scores[i].score > scores[i+1].score){
			    scores_t temp = scores[i];
			    scores[i] = scores[i+1];
			    scores[i+1] = temp;
			}
		}
	}
}

void cross(int best_spec[][MOVE_LIMIT], scores_t scores[]) {
	//copy to best_spec population
	for (size_t i = 0; i < BEST_CNT; i++) {
		memcpy(best_spec[i], population[scores[i].index], MOVE_LIMIT);
	}

	int firstParent = rand() % BEST_CNT;
	int secondParent = rand() % BEST_CNT;
	int crossOver = rand() % MOVE_LIMIT;

	for (size_t i = BEST_CNT + REPRODUCTIVE; i < POPULATION_SIZE; i++) {
		memcpy(population[i], best_spec[firstParent], crossOver);
		memcpy(population[i], best_spec[secondParent], (MOVE_LIMIT - crossOver));
	}

}

int fitness(int chromo, point_t *player, point_t *finish_pos){
	player_reset(player); // 
	int penalty = 0;
	for(size_t i = 0; i < MOVE_LIMIT; i++){
		penalty += func_ptr[population[chromo][i]](player);
	}

	int score = 2*abs(finish_pos->x - player->x) + 2*abs(finish_pos->y - player->y) + penalty; // fitness function
	return score;
}

int score(scores_t scores[], point_t *player, point_t *finish_pos){
	for(size_t i = 0; i < POPULATION_SIZE; i++){
		scores[i].score = fitness(i, player, finish_pos);
		scores[i].index = i;
	}
	sort_scores(scores);
	return scores[0].index;
}

void mutate(){
	for(size_t i = 0; i < POPULATION_SIZE-1; i++){
		if(MUTATION_CHANCE > (double)rand() / (double)RAND_MAX){ // random number (0,1)
			population[i][rand() % MOVE_LIMIT] = rand() % DIRECTIONS_LEN; // choose random element and mutate
		}
	}
}

int main(int argc, char *argv[]){
	int best_spec[BEST_CNT][MOVE_LIMIT] = {0};
	scores_t scores[POPULATION_SIZE] = {0};
	point_t player =  {.x = 1, .y = 1};
	point_t finish_pos = {.x = 8, .y = 8};
	char* ptr = NULL;
	long iter = 0; 
	time_t t;

	iter = strtol(argv[1], &ptr, 10);
	if(argc > 2 || iter < 1){
		printf("Usage: genetic_maze [iterations]\n");
		printf("eg genetic_maze 500\n");
		exit(0);	
	}

	srand((unsigned) time(&t));

	fill(population);
	printf("Start maze: \r\nP - player, F - finish\r\n");
	print_maze(&player, &finish_pos);
	// log best score in every iteration
	FILE *scores_file = fopen("scores.txt", "w");

	char write_buf[10];
	// main loop
	for(long i = 0; i < iter; i++){
		mutate();
		cross(best_spec, scores);
		score(scores, &player, &finish_pos);
		int buf_count = sprintf(write_buf, "%d ", scores[0].score);
		fwrite(write_buf, sizeof(char), buf_count, scores_file); // convert to ascii
		if(scores[0].score < FINAL_SCORE){
			print_maze_path(scores, &finish_pos);
			printf("Path found in %d generations\n", i);
			printf("Score: %d\n", scores[0].score);
			break;
		}
		if(i % 1000 == 0){
			printf("generation: %d\n", i);
		}
	}
	fclose(scores_file);
}