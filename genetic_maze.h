#define MOVE_LIMIT 25
#define MAZE_X 10
#define MAZE_Y 10

/* parameters to experiment with */
#define POPULATION_SIZE 500
#define BEST_CNT 7
#define REPRODUCTIVE 150
#define PENALTY_VAL 2
#define MUTATION_CHANCE 0.5f
#define FINAL_SCORE 3


typedef struct{
	int x;
	int y;
}point_t;

typedef struct{
	int score;
	int index;
}scores_t;



void print_maze();
void print_maze_path();

void player_reset();
void player_stay();
void player_move_up();
void player_move_down();
void player_move_left();
void player_move_right();
void fill();
int* sort(int* array, int size);
void sort_scores_t();
void cross();
int fitness(int chromo);
int score();
void move_player();
void mutate();
