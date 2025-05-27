#include "snake2.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
char board[cols * rows];
int is_game_over=0;
int score=0;
int wall_timer_counter = 0;
int fruits_eaten_recently = 0;
time_t last_fruit_time = 0;
int high_score = 0;
int all_plus_and_dollar_consumed = 1;
struct Snake snake;
struct Food food[foods];
struct TempWall temp_walls[MAX_TEMP_WALLS];


// Clear screen

void clear_screen()
{
    system("cls");
}

// Build walls with '#'

void build_board ()
{
    int i,j;
    for (i=0;i<rows;i++)
    {
        for (j=0;j<cols;j++)
        {
            if (i==0 || i==rows-1 || j==0 || j==cols-1)
            {
                board[i*cols+j]='#';
            }
            else
            {
                board[i*cols+j]=' ';
            }
        }
    }
}

// print board

void print_board ()
{
    int i,j;
    for (i=0;i<rows;i++)
    {
        for (j=0;j<cols;j++)
        {
            putchar(board[i*cols+j] );
        }
        putchar ('\n');
    }
}

//Draw snake (head-'@',body-'*')

/*void draw_snake(){
    int i;
    for (i=snake.length-1;i>0;i--)
    {
        board [snake.part[i].y*cols + snake.part[i].x] = '*';
    }
    board [snake.part[0].y*cols + snake.part[0].x] = '@';

}*/

void draw_snake() {
    // Snake's body - light blue
    SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
    for (int i = 1; i < snake.length; i++) {
        SDL_Rect rect = {  // create un rectangle
            snake.part[i].x * (WINDOW_WIDTH / cols),
            snake.part[i].y * (WINDOW_HEIGHT / rows),
            WINDOW_WIDTH / cols,
            WINDOW_HEIGHT / rows
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    // Snake's head - dark blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
    SDL_Rect headRect = {
        snake.part[0].x * (WINDOW_WIDTH / cols),
        snake.part[0].y * (WINDOW_HEIGHT / rows),
        WINDOW_WIDTH / cols,
        WINDOW_HEIGHT / rows
    };
    SDL_RenderFillRect(renderer, &headRect);

    // Eyes - small white square
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // alb

    // Eye size: approximately 1/4 of the head square size
    int eyeSizeX = (WINDOW_WIDTH / cols) / 4;
    int eyeSizeY = (WINDOW_HEIGHT / rows) / 4;

    // Eye position: at the top-left corner of the head
    SDL_Rect eyeRect = {
        headRect.x + eyeSizeX, // Slightly to the right of the edge
        headRect.y + eyeSizeY, // Slightly below the edge
        eyeSizeX,
        eyeSizeY
    };
    SDL_RenderFillRect(renderer, &eyeRect);
}


// The snake's body moves following its head

void move_snake(int deltaX,int deltaY)
{
    int i;

    for (i=snake.length-1;i>0;i--)
    {
        snake.part[i]=snake.part[i-1];
    }
    snake.part[0].x += deltaX;
    snake.part[0].y += deltaY;

    if (snake.length < MAX_SIZE) {
        snake.part[snake.length].x = -1;
        snake.part[snake.length].y = -1;
    }
}

// read keyboard as an input

void read_keyboard()
{
    int ch = getch(); // Read the pressed key
    switch (ch) {
        case 'w':
            move_snake(0, -1);
            break;
        case 'a':
            move_snake(-1, 0);
            break;
        case 's':
            move_snake(0, 1);
            break;
        case 'd':
            move_snake(1, 0);
            break;
    }
}

// Process SDL events

void handle_events() {
    SDL_Event event;  // // SDL event
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:  // // Window closing
                is_game_over = 1;
                break;
            case SDL_KEYDOWN:  // Key press
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        move_snake(0, -1);
                        break;
                    case SDLK_a:
                        move_snake(-1, 0);
                        break;
                    case SDLK_s:
                        move_snake(0, 1);
                        break;
                    case SDLK_d:
                        move_snake(1, 0);
                        break;
                    case SDLK_ESCAPE:
                        is_game_over = 1;
                        break;
                }
                break;

        }
    }
}
/*
void draw_food(){
    int i;
    for(i=0;i<foods;i++){
        if ( !food[i].consumed){
              if (food[i].type == 0) {
                board[food[i].y * cols + food[i].x] = '+'; // food that increases snake size
            } else if (food[i].type == 1) {
                board[food[i].y * cols + food[i].x] = '$'; // rare food (does not affect snake size)
            } else if (food[i].type == 2) {
                board[food[i].y * cols + food[i].x] = '-'; // food that reduces snake size
            }
        }
    }

}

*/

// Draw a circle for draw_food

void draw_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int dx, dy;
    for (dx = -radius; dx <= radius; dx++) { // Horizontal offset (decalaj) from the center of the circle
        for (dy = -radius; dy <= radius; dy++) {
            if (dx*dx + dy*dy <= radius*radius) { // Check if the point at offset (dx, dy) from the center is inside or on the circle's boundary.

                SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
            }
        }
    }
}
// Draw a $ for draw_food

void draw_dollar_sign(SDL_Renderer* renderer, int x, int y) {
    int cx = x * TILE_SIZE + TILE_SIZE / 2;
    int cy = y * TILE_SIZE + TILE_SIZE / 2;

    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // yellow

    // vertical line of the $ sign
    for (int i = -8; i <= 8; i++) {
        SDL_RenderDrawPoint(renderer, cx, cy + i);
    }

    // horizontal line of the $ sign
    for (int i = -5; i <= 5; i++) {
        SDL_RenderDrawPoint(renderer, cx + i, cy - 4);
    }

    // bottom horizontal line
    for (int i = -5; i <= 5; i++) {
        SDL_RenderDrawPoint(renderer, cx + i, cy + 4);
    }

    // Draw the curves
    SDL_RenderDrawPoint(renderer, cx - 4, cy - 6);
    SDL_RenderDrawPoint(renderer, cx - 3, cy - 7);
    SDL_RenderDrawPoint(renderer, cx - 2, cy - 8);

    SDL_RenderDrawPoint(renderer, cx + 4, cy + 6);
    SDL_RenderDrawPoint(renderer, cx + 3, cy + 7);
    SDL_RenderDrawPoint(renderer, cx + 2, cy + 8);
}
// Draw the bomb

void draw_bomb(SDL_Renderer* renderer, int x, int y, int tile_size) {
    int bomb_size = tile_size / 2;
    SDL_Rect bomb = {
        x * tile_size + tile_size / 4,
        y * tile_size + tile_size / 4,
        bomb_size,
        bomb_size
    };

   // Bomb body - black circle
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bomb);

    // Red fuse
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer,
        bomb.x + bomb_size / 2,
        bomb.y,
        bomb.x + bomb_size / 2,
        bomb.y - 5
    );
}
// Draw the apple

void draw_apple(SDL_Renderer* renderer, int x, int y) {
    int center_x = x * TILE_SIZE + TILE_SIZE / 2;
    int center_y = y * TILE_SIZE + TILE_SIZE / 2;
    int radius = TILE_SIZE / 2 - 2;

    // Set the apple color
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Draw the circle
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, center_x + dx, center_y + dy);
            }
        }
    }

    // Green stem

    SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
    SDL_RenderDrawLine(renderer, center_x, center_y - radius, center_x, center_y - radius - 4);
}

// Draw all types of food

void draw_food() {
    for (int i = 0; i < foods; i++) {
        if (!food[i].consumed) {
            if (food[i].type == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255 , 0, 255); // roșu pentru măr
                draw_apple(renderer, food[i].x, food[i].y); // draw apple
            } else if (food[i].type == 1) {
                draw_dollar_sign(renderer, food[i].x, food[i].y); // draw $
            } else if (food[i].type == 2) {
                draw_bomb(renderer, food[i].x, food[i].y, TILE_SIZE); // draw bomb
            }
        }
    }
}


// Generate random positions for each food item.

void setup_food(){
    for (int i = 0; i < foods; i++) {
        do {
            food[i].x = 1 + rand() % (cols - 2);
            food[i].y = 1 + rand() % (rows - 2);
        } while (is_position_on_snake(food[i].x, food[i].y));

        food[i].consumed = 0;
        food[i].type = rand() % 3; // 0, 1, sau 2
}


}

// Initialize the snake

void setup_snake(){
    snake.length = 1;
    snake.part[0].x = 1+rand()%(cols-2);  // Random position inside the grid.
    snake.part[0].y = 1+rand()%(rows-2);
}
// check if snake touches his body

void check_self_collision() {
    int i;
    for (i = 1; i < snake.length; i++) {
        if (snake.part[0].x == snake.part[i].x &&     // The snake's head and body has the same coordinates as another part of the body
            snake.part[0].y == snake.part[i].y) {
            is_game_over = 1;
            break;
        }
    }
}
// Control the game rules

void game_rules() {
    int i;
    time_t now = time(NULL);  // current time in sec
    all_plus_and_dollar_consumed = 1;
    for (i = 0; i < foods; i++) {
        if (!food[i].consumed) {        //if the food is on the board
            // check if the snake has eaten a food
            if (food[i].x == snake.part[0].x && food[i].y == snake.part[0].y) {
                food[i].consumed = 1;

                // Type of food
            if (food[i].type == 0) {  // "+"
                snake.length++;
                score += 1;
                printf("Snake grew! Length: %d | +1 point\n", snake.length);

                // Bonus check only for "+" and "$"
                if (difftime(now, last_fruit_time) <= 5) { // The difference in seconds between two moments in time
                    fruits_eaten_recently++;
                } else {
                    fruits_eaten_recently = 1;
                }
                last_fruit_time = now;

                }
                else if (food[i].type == 2) { // "-"
                    if (snake.length > 1) {
                            snake.length--;
                    }
                    score -= 1;
                    printf("Snake decreased! Length: %d | -1 point\n", snake.length);

                // Doesn't affect the bonus, so we disable it
                    fruits_eaten_recently = 0;
                    last_fruit_time = 0;

                } else if (food[i].type == 1) { // "$"
                    score += 5;
                    printf("Rare food eaten! +5 points\n");

                        // Bonus check only for "+" and "$"
                if (difftime(now, last_fruit_time) <= 5) {
                    fruits_eaten_recently++;
                } else {
                    fruits_eaten_recently = 1;
                }
                last_fruit_time = now;
                }

                // Apply bonus only if the last 3 were "+" or "$"
                if (fruits_eaten_recently == 3) {
                    score += 3;
                    printf("Combo bonus! 3 fruits in 5 seconds : +3 points!\n");
                    fruits_eaten_recently = 0;
                    last_fruit_time = 0;
                }
            }

            // check if there are any uneaten '+' or '$' foods
            if (food[i].type == 0 || food[i].type == 1) {
                all_plus_and_dollar_consumed = 0;
            }
        }
    }

    // collision with map edge
    if (snake.part[0].x == 0 || snake.part[0].x == cols - 1 ||
        snake.part[0].y == 0 || snake.part[0].y == rows - 1) {
        is_game_over = 1;
    }

    // collision with body
    check_self_collision();

    // Collision with temporary walls
    for (i = 0; i < MAX_TEMP_WALLS; i++) {
        if (temp_walls[i].active &&
            temp_walls[i].x == snake.part[0].x &&
            temp_walls[i].y == snake.part[0].y) {
            is_game_over = 1;
            printf("Snake hit a temporary wall!\n");
            return;
        }
    }

    // if all + and $ food has been consumed, we regenerate
    if (all_plus_and_dollar_consumed) {
        printf("All + and $ food consumed! Regenerating food...\n");
        setup_food();
    }
}


// if a certain position (x, y) is on the snake's body

int is_position_on_snake(int x, int y) {
    for (int i = 0; i < snake.length; i++) {
        if (snake.part[i].x == x && snake.part[i].y == y) {
            return 1;
        }
    }
    return 0;
}

//set temporary walls

void setup_temp_walls() {
    int i;
    for (i = 0; i < MAX_TEMP_WALLS; i++) {
        temp_walls[i].x = 0;  //set walls at x=0
        temp_walls[i].y = 0;  //set walls at y=0
        temp_walls[i].active = 0; //walls are not active
        temp_walls[i].timer = 0; // reset time
    }
}
// Generate random temporary walls on the map without overlapping the snake

void spawn_temp_walls() {
    int groups_to_spawn = 3 + rand() % 3; // Generate a random number between 3 and 5 — meaning between 3 and 5 wall groups will be created.

    for (int g = 0; g < groups_to_spawn; g++) {
        int length = 2 + rand() % 4; //  every group has between 2 and 5 walls
        int direction = rand() % 2;  // 0 = orizontal, 1 = vertical
        int tries = 0;
        int base_x, base_y;  //basic coordinates

        // Choose a random starting position, but make sure it is not on the snake| Try a maximum of 10 times.

        do {
            base_x = 1 + rand() % (cols - 2);
            base_y = 1 + rand() % (rows - 2);
            tries++;
        } while (is_position_on_snake(base_x, base_y) && tries < 10);

        // If the starting point is valid
        if (!is_position_on_snake(base_x, base_y)) {
            int added = 0;
            for (int i = 0; i < MAX_TEMP_WALLS && added < length; i++) {
                if (!temp_walls[i].active) {
                    int x = base_x + (direction == 0 ? added : 0);
                    int y = base_y + (direction == 1 ? added : 0);

                    // we check that it is inside the map and does not touch the snake
                    if (x > 0 && x < cols - 1 && y > 0 && y < rows - 1 && !is_position_on_snake(x, y)) {
                        temp_walls[i].x = x;
                        temp_walls[i].y = y;
                        temp_walls[i].active = 1;
                        temp_walls[i].timer = 30 + rand() % 20; // lasts 30-50 frames
                        added++;
                    }
                }
            }
        }
    }
}

// Update temporary walls

void update_temp_walls() {
    int block_width = WINDOW_WIDTH / cols;
    int block_height = WINDOW_HEIGHT / rows;

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (temp_walls[i].active) {
            temp_walls[i].timer--;
            if (temp_walls[i].timer <= 0) {
                temp_walls[i].active = 0;
            } else {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray
                SDL_Rect rect = {
                    temp_walls[i].x * block_width,
                    temp_walls[i].y * block_height,
                    block_width,
                    block_height
                };
                SDL_RenderFillRect(renderer, &rect);

                // Draw red X above the rectangle
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // rosu

                // Diagonal line from top-left to bottom-right
                SDL_RenderDrawLine(renderer,
                    rect.x, rect.y,
                    rect.x + rect.w, rect.y + rect.h);

                // Diagonal line from bottom-left to top-right
                SDL_RenderDrawLine(renderer,
                    rect.x, rect.y + rect.h,
                    rect.x + rect.w, rect.y);
            }
        }
    }

    wall_timer_counter++;
    if (wall_timer_counter >= 30) { // 30 frames
        wall_timer_counter = 0;
        spawn_temp_walls();
    }
}
// Load the high score from a file

void load_high_score() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &high_score);
        fclose(file);
    } else {
        high_score = 0; //if there is no file,start from 0
    }
}
// Save the high score to a file if the current score is higher

void save_high_score() {
    if (score > high_score) {
        FILE *file = fopen("highscore.txt", "w");
        if (file != NULL) {
            fprintf(file, "%d", score);
            fclose(file);
        }
        printf("New High Score: %d!\n", score);
    }
}

// Draw the score

void draw_score(SDL_Renderer* renderer, TTF_Font* font, int score, int high_score) {
    SDL_Color white = {255, 255, 255, 255};

    char text[64];

    // Draw the current score
    snprintf(text, sizeof(text), "Score: %d", score); // Write into the str buffer, not on the screen.
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int w, h;
    TTF_SizeText(font, text, &w, &h);
    SDL_Rect rect = {WINDOW_WIDTH / 2 - w / 2, 10, w, h}; // center up

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Draw high score
    snprintf(text, sizeof(text), "Best: %d", high_score);
    surface = TTF_RenderText_Solid(font, text, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    TTF_SizeText(font, text, &w, &h);
    rect.x = WINDOW_WIDTH / 2 - w / 2;
    rect.y = 10 + h + 5;  // below the current score
    rect.w = w;
    rect.h = h;

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
// Display a start message on the screen.

void afiseaza_mesaj_start(SDL_Renderer* renderer, TTF_Font* font, int screen_w, int screen_h) {
    SDL_Color alb = {255, 255, 255, 255};
    const char* linii[] = {
        "Marul: +1",
        "Dolarul: +5",
        "Bomba: -1",
        "",
        "Apasa SPACE pentru a incepe"
    };
    int nr_linii = sizeof(linii) / sizeof(linii[0]);

    SDL_Event e;
    int gata = 0;

    while (!gata) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                gata = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int total_inaltime = 0;
        int inaltimi[nr_linii];
        for (int i = 0; i < nr_linii; i++) {
            int w, h;
            TTF_SizeText(font, linii[i], &w, &h);
            inaltimi[i] = h;
            total_inaltime += h + 10;
        }
        total_inaltime -= 10;

        int y_start = (screen_h - total_inaltime) / 2;

        for (int i = 0; i < nr_linii; i++) {
            int w, h;
            TTF_SizeText(font, linii[i], &w, &h);

            SDL_Surface* suprafata = TTF_RenderText_Solid(font, linii[i], alb);
            SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, suprafata);

            SDL_Rect dreptunghi = { (screen_w - w) / 2, y_start, w, h };
            SDL_RenderCopy(renderer, textura, NULL, &dreptunghi);

            y_start += h + 10;

            SDL_FreeSurface(suprafata);
            SDL_DestroyTexture(textura);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

// Initialize the SDL window and engine.

int init_graphics() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Snake Game",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

// Draw the green background

void draw_background_green() {
    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // fundal verde
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // pereți gri

    int wall_thickness = 5;

    SDL_Rect top_wall = {0, 0, WINDOW_WIDTH, wall_thickness};
    SDL_RenderFillRect(renderer, &top_wall);

    SDL_Rect bottom_wall = {0, WINDOW_HEIGHT - wall_thickness, WINDOW_WIDTH, wall_thickness};
    SDL_RenderFillRect(renderer, &bottom_wall);

    SDL_Rect left_wall = {0, 0, wall_thickness, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &left_wall);

    SDL_Rect right_wall = {WINDOW_WIDTH - wall_thickness, 0, wall_thickness, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &right_wall);
}

// Free SDL resources and close the window.

void close_graphics() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();   // Dezinițializează SDL_ttf
    SDL_Quit();
}



