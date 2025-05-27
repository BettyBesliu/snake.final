#include "snake2.h"


int main(int argc, char *argv[]) {
    if (!init_graphics()) {
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("C:/Users/juliu/OneDrive/Documents/Code Blocks/sdl3/bin/Debug/fonts/AncizarSans-Black.ttf", 20);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        TTF_Quit();
        close_graphics();
        return 1;
    }

    // Află dimensiunea ferestrei (folosește variabilele tale de lățime/înălțime)
    int screen_w = 640;  // pune lățimea corectă dacă o ai definită altundeva
    int screen_h = 480;  // pune înălțimea corectă

    // Afișează mesajul de start și așteaptă să apeși SPACE
    afiseaza_mesaj_start(renderer, font, screen_w, screen_h);

    load_high_score();
    srand(time(0));
    setup_snake();
    setup_food();
    setup_temp_walls();

    while (!is_game_over) {
        draw_background_green();

        // Desenează restul elementelor
        draw_food();
        draw_snake();
        update_temp_walls();
        draw_score(renderer, font, score, high_score);

        SDL_RenderPresent(renderer);

        handle_events();
        game_rules();

        SDL_Delay(100);
    }

    printf("Game Over, Final score: %d\n", snake.length);
    save_high_score();
    printf("Saved High Score: %d\n", score);

    TTF_CloseFont(font);
    TTF_Quit();
    close_graphics();

    return 0;
}
