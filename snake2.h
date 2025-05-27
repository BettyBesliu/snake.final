#ifndef SNAKE2_H
#define SNAKE2_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define rows 20
#define cols 30
#define MAX_SIZE 256
#define foods 10
#define MAX_TEMP_WALLS 20
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define TILE_SIZE 20

struct SnakePart {
    int x, y;
};

struct Snake {
    int length;
    struct SnakePart part[MAX_SIZE];
};

struct Food {
    int x, y;
    int consumed;
    int type;
};

struct TempWall {
    int x;
    int y;
    int active;  // 1 = activ, 0 = inactiv
    int timer;   // câte frame-uri rămâne activ
};

struct Food food[foods];
struct Snake snake;
struct TempWall temp_walls[MAX_TEMP_WALLS];
int is_game_over;
int score;
int wall_timer_counter;
int fruits_eaten_recently;
int all_plus_and_dollar_consumed;
time_t last_fruit_time;
int high_score;

void clear_screen();
void build_board();
void print_board();
void draw_snake();
void move_snake(int deltaX, int deltaY);
void read_keyboard();
void draw_food();
void setup_food();
void setup_snake();
void check_self_collision();
void game_rules();
int is_position_on_snake(int x, int y);
void setup_temp_walls();
void spawn_temp_walls();
void update_temp_walls();
void load_high_score();
void save_high_score();

// SDL graphics functions

SDL_Window* window;
SDL_Renderer* renderer;
void afiseaza_mesaj_start(SDL_Renderer* renderer, TTF_Font* font, int screen_w, int screen_h);
void handle_events();
void draw_circle(SDL_Renderer *renderer, int cx, int cy, int radius);
void draw_dollar_sign(SDL_Renderer* renderer, int x, int y);
void draw_bomb(SDL_Renderer* renderer, int x, int y, int tile_size) ;
void draw_apple(SDL_Renderer* renderer, int x, int y);
void draw_score(SDL_Renderer* renderer, TTF_Font* font, int score, int high_score);
int init_graphics();
void draw_background_green();
void close_graphics();


#endif
