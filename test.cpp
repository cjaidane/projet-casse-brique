#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        return -1;
    }

    // Création de la fenêtre
    SDL_Window* fenetre = SDL_CreateWindow("Clement le bg",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!fenetre) {
        SDL_Log("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Boucle de rendu
    bool enExecution = true;
    while (enExecution) {
        // Gestion des événements
        SDL_Event evenement;
        while (SDL_PollEvent(&evenement) != 0) {
            if (evenement.type == SDL_QUIT) {
                enExecution = false;
            }
        }

        // Mise à jour de l'affichage
        // Vous pouvez ajouter du code de rendu ici

        SDL_Delay(10); // Ajout d'un léger délai pour éviter la surcharge CPU
    }

    // Libération des ressources
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    return 0;
}
