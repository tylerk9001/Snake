#include <stdlib.h>
#include <time.h>
#include "Console.h"

//  All extra credit features have been completed.

// Default frames per second.
#define FPS 10

// Define max amount of segments
#define MAX_SEGMENTS 100

// Define the dir options
#define UP     1
#define RIGHT  2
#define DOWN   3
#define LEFT   4


// TODO: define other struct types, e.g., struct Point, struct Snake

struct Point {
    // TODO: add fields
    int x, y;
};

struct Snake {
    struct Point segments[MAX_SEGMENTS];
    int num_segments;
    int dir;
    struct Point snake_head;
};

struct Scene {
    struct Snake snake;
    struct Point fruit;
    int score;
    int game_over;
};

// TODO: define functions to operate on your struct types

void scene_init(struct Scene *s);
void scene_render(const struct Scene *s);
int scene_update(struct Scene *s);
void scene_delay(struct Scene *s);

void snake_init(struct Snake *snake);
void snake_movement(struct Snake *snake);
void snake_dir(struct Snake *snake);
void new_position(struct Snake *snake);


int main(void) {
	// Important: do NOT modify the main function
	struct Scene scene;

	scene_init(&scene);

	int keep_going = 1;
	while (keep_going == 1) {
		scene_render(&scene);
		cons_update();
		scene_delay(&scene);
		keep_going = scene_update(&scene);
	}

	return 0;
}


void snake_init(struct Snake *snake){
    // Intialize the starting point of the snake head to a random point in the terminal window.
    snake->snake_head.x = rand() % 80;
    snake->snake_head.y = rand() % 23;
    
    // Initialize the starting amount of segments the snake has (8).
    snake->num_segments = 8;
    
    // Set the starting direction of the snake.
    snake->dir = RIGHT;
}


void snake_movement(struct Snake *snake){
    // Initialize/Movement of snake segments
    for (int i = 0; i < snake->num_segments; i++){
        if (snake->dir == UP){
            snake->segments[i].x = snake->snake_head.x;
            snake->segments[i].y = snake->snake_head.y + (i + 1);
        }
        if (snake->dir == RIGHT){
            snake->segments[i].x = snake->snake_head.x - (i + 1);
            snake->segments[i].y = snake->snake_head.y;
        }
        if (snake->dir == DOWN){
            snake->segments[i].x = snake->snake_head.x;
            snake->segments[i].y = snake->snake_head.y - (i + 1);
        }
        if (snake->dir == LEFT){
            snake->segments[i].x = snake->snake_head.x + (i + 1);
            snake->segments[i].y = snake->snake_head.y;
        }
    }
}


void snake_dir(struct Snake *snake, int key){
    
    // Change direction of the snake based on user input
    
    if (key == RIGHT_ARROW && snake->dir != LEFT){
        snake->dir = RIGHT;
    }
    if (key == UP_ARROW && snake->dir != DOWN){
        snake->dir = UP;
    }
    if (key == LEFT_ARROW && snake->dir != RIGHT){
        snake->dir = LEFT;
    }
    if (key == DOWN_ARROW && snake->dir != UP){
        snake->dir = DOWN;
    }
}


void new_position(struct Snake *snake){
    // Store temp values for the head and segments position
    struct Point snake_get_head = snake->snake_head;
    
    // Change head to the new position based on the temp position
    if (snake->dir == UP){
        snake->snake_head.y = snake_get_head.y - 1;
    }
    if (snake->dir == RIGHT){
        snake->snake_head.x = snake_get_head.x + 1;
    }
    if (snake->dir == DOWN){
        snake->snake_head.y = snake_get_head.y + 1;
    }
    if (snake->dir == LEFT){
        snake->snake_head.x = snake_get_head.x - 1;
    }
}


void scene_init(struct Scene *s) {
	srand(time(0));

	// TODO: add your code
    
    // Start the game as false (0)
    s->game_over = 0;
    
    // Intitalize the score (0)
    s->score = 0;
    
    // Intialize the starting point of the snake head to a random point in the terminal window.
    // Initialize the starting amount of segments the snake has (8).
    // Set the starting direction of the snake.
    snake_init(&s->snake);
    
    // Randomize the fruit location each time.
    // Fruit_ tester is testing to make sure that the fruit does not spawn where the snake is.
    int fruit_position = 1;
    
    while (fruit_position == 1){
        fruit_position = 0;
        s->fruit.x = rand() % 80;
        s->fruit.y = rand() % 23;
        
        // Checking for contact with the head
        if (s->fruit.x == s->snake.snake_head.x && s->fruit.y == s->snake.snake_head.y){
            fruit_position = 1;
        }
        
        // Checking for contact with segments    
        for (int i = 0; i < s->snake.num_segments; i++){
            if (s->fruit.x == s->snake.segments[i].x && s->fruit.y == s->snake.segments[i].y){
                fruit_position = 1;
            }
        }
    }
    
    
    // Initialize/Movement of snake segments
    snake_movement(&s->snake);
    
}

void scene_render(const struct Scene *s) {
	// TODO: add your code
    
    if (s->game_over == 0){
        // Clear the screen after each time
        cons_clear_screen();
        
        // Print the number of segements
        cons_move_cursor(23, 48);
        cons_change_color(GRAY, BLACK);
        cons_printw("Segments: ");
        cons_change_color(GRAY+INTENSE, BLACK);
        cons_printw("%.3i", s->snake.num_segments);
        cons_change_color(GRAY, BLACK);
        cons_printw("/100");
        
        // Print the score
        cons_move_cursor(23, 11);
        cons_change_color(GRAY, BLACK);
        cons_printw("Score: ");
        cons_change_color(GRAY+INTENSE, BLACK);
        cons_printw("%.6i", s->score);
        
        // Print the direction of the snake head
        cons_move_cursor(s->snake.snake_head.y, s->snake.snake_head.x);
        cons_change_color(CYAN+INTENSE, BLACK);
        if (s->snake.dir == UP){
            cons_printw("v");
        }
        else if (s->snake.dir == RIGHT){
            cons_printw("<");
        }
        else if (s->snake.dir == DOWN){
            cons_printw("^");
        }
        else if (s->snake.dir == LEFT){
            cons_printw(">");
        }
       
        // Print the fruit on the screen
        cons_move_cursor(s->fruit.y, s->fruit.x);
        cons_change_color(RED+INTENSE, BLACK);
        cons_printw("@");
        
        // Print the segements of the snakes body
        for (int i = 0; i < s->snake.num_segments; i++){
            cons_move_cursor(s->snake.segments[i].y, s->snake.segments[i].x);
            if (i % 2 != 0){
                cons_change_color(CYAN+INTENSE, BLACK);
            }
            else {
                cons_change_color(GREEN+INTENSE, BLACK);
            }
            cons_printw("*");
        }
    }

    
    // Print the game over screen
    // 0 = false 1 = true
    if (s->game_over == 1){
        cons_change_color(GREEN+INTENSE, GREEN+INTENSE);
        cons_move_cursor(11,30);
        cons_printw("                        ");
        cons_move_cursor(12,30);
        cons_printw("   ");
        cons_change_color(GRAY+INTENSE, BLACK);
        cons_printw("     GAME OVER    ");
        cons_change_color(GRAY+INTENSE, GREEN+INTENSE);
        cons_printw("   ");
        cons_move_cursor(13,30);
        cons_printw("                        ");
    }
    
}

int scene_update(struct Scene *s) {
	// This function should return 0 if the player presses 'q',
	// 1 otherwise.
    int key = cons_get_keypress();
    
	// TODO: add your code
    // Quit game if the q key is pressed
    if (key == 'q'){
        return 0;
    }
    
    // Change direction of the snake based on user input
    snake_dir(&s->snake, key);
   
    // Store temp values for the head and segments position
    struct Point snake_get_head = s->snake.snake_head;
    struct Point snake_get_segment[MAX_SEGMENTS];
    for (int i = 0; i < s->snake.num_segments; i++){
        snake_get_segment[i] = s->snake.segments[i];
    }
    
    // Change head to the new position based on the temp position
    new_position(&s->snake);
   
    // Change the body segments to new positions on temp position
    s->snake.segments[0] = snake_get_head;
    for (int i = 1; i < s->snake.num_segments; i++){
        s->snake.segments[i] = snake_get_segment[i-1];
    }
    
    // Check to see if the snake has come in contact with the piece of fruit
    if (s->snake.snake_head.x == s->fruit.x && s->snake.snake_head.y == s->fruit.y){
        // Fruit_ tester is testing to make sure that the fruit does not spawn where the snake is.
        int fruit_position = 1;
        
        while (fruit_position == 1){
            fruit_position = 0;
            s->fruit.x = rand() % 80;
            s->fruit.y = rand() % 23;
            
            // Checking for contact with the head
            if (s->fruit.x == s->snake.snake_head.x && s->fruit.y == s->snake.snake_head.y){
                fruit_position = 1;
            }
            
            // Checking for contact with segments
            for (int i = 0; i < s->snake.num_segments; i++){
                if (s->fruit.x == s->snake.segments[i].x && s->fruit.y == s->snake.segments[i].y){
                    fruit_position = 1;
                }
            }
        }
        
        // Add one to the snakes length each time
        if (s->snake.num_segments < 100){
            s->snake.num_segments++;
        }
        
        // Change the score based off of the size of the snakes segments
        s->score += s->snake.num_segments * 10;
    }
    
    
    // Check to see if the game is over
    // Checking to see if head touches the body of the snake
    for (int i = 0; i < s->snake.num_segments; i++){
        if(s->snake.snake_head.x == s->snake.segments[i].x && s->snake.snake_head.y == s->snake.segments[i].y){
            s->game_over = 1;
        }
    }
    
    // Checking if head touches boundaries
    if (s->snake.snake_head.x == -1 || s->snake.snake_head.y == -1 || s->snake.snake_head.x == 80 || s->snake.snake_head.y == 24){
        s->game_over = 1;
    }
    
    return 1;
}

void scene_delay(struct Scene *s) {
	// This function determines how many milliseconds the game is
	// paused between each frame of animation.
	// You can modify this if you want to vary the game speed.
	cons_sleep_ms(1000/FPS);
}

