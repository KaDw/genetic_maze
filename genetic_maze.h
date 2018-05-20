#define MOVE_LIMIT 20
#define MAZE_X 10
#define MAZE_Y 10
#define DIRECTIONS_LEN 4

/* parameters to experiment with */
#define POPULATION_SIZE 100
#define BEST_CNT 2
#define REPRODUCTIVE 30
#define PENALTY_VAL 1
#define MUTATION_CHANCE 0.5f
#define FINAL_SCORE 6

typedef struct{
	int x;
	int y;
}point_t;

typedef struct{
	int score;
	int index;
}scores_t;

int player_move_up(point_t* player);
int player_move_down(point_t* player);
int player_move_left(point_t* player);
int player_move_right(point_t* player);
