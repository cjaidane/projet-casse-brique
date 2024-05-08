#include "../include/brick.h"


/*Constructeur pour la classe Brick.
    Initialise une brique avec les dimensions et la résistance spécifiées.
    @param x La position horizontale de départ de la brique.
    @param y La position verticale de départ de la brique.
    @param w La largeur de la brique.
    @param h La hauteur de la brique.
    @param resistance La résistance initiale de la brique, indiquant combien de fois elle doit être touchée avant d'être détruite.
*/
Brick::Brick(int x, int y, int w, int h, int resistance) 
    : rect{x, y, w, h}, resistance(resistance), active(true) {}


/*
    Rend la brique à l'écran si elle est active.
    @param renderer Le renderer SDL utilisé pour dessiner la brique.

*/
void Brick::render(SDL_Renderer* renderer) const {
    if (!active) return;
    int colorIntensity = 255 * resistance / 5; // Diminuer l'intensité de la couleur basée sur la résistance
    SDL_SetRenderDrawColor(renderer, 255, colorIntensity, colorIntensity, 255);
    SDL_RenderFillRect(renderer, &rect);
}


// /*
//     Vérifie si la brique entre en collision avec la balle et
//     décrémente la résistance si c'est le cas.

//     @param ballRect Le rectangle représentant la balle pour vérifier la collision.
// */
// bool Brick::checkCollision(const SDL_Rect& ballRect) {
//     if (!active) return false;
//     if (ballRect.x < rect.x + rect.w && ballRect.x + ballRect.w > rect.x &&
//         ballRect.y < rect.y + rect.h && ballRect.y + ballRect.h > rect.y) {
//         resistance--;
//         if (resistance <= 0) {
//             active = false;
//         }
//         return true;
//     }
//     return false;
// }



/*
    Vérifie si la brique entre en collision avec la balle et
    décrémente la résistance si c'est le cas.

    @param ballRect Le rectangle représentant la balle pour vérifier la collision.
*/
bool Brick::checkCollision(const SDL_Rect& ballRect) {
    if (!active) return false;  // Si la brique n'est pas active, ne vérifiez pas les collisions.

    // Créez un SDL_Rect pour la brique.
    SDL_Rect brickRect = {rect.x, rect.y, rect.w, rect.h};

    // Utilisez SDL_HasIntersection pour vérifier si la balle intersecte la brique.
    if (SDL_HasIntersection(&ballRect, &brickRect)) {
        resistance--;  // Décrémentez la résistance de la brique.
        if (resistance <= 0) {
            active = false;  // Désactivez la brique si sa résistance atteint zéro.
        }
        return true;  // Retournez vrai car il y a eu une collision.
    }

    return false;  // Retournez faux si aucune collision n'est détectée.
}


/*
    Retourne si la brique est toujours active.
    @return true si la brique est active et sa résistance est supérieure à zéro
*/
bool Brick::isActive() const {
    return active && resistance > 0;
}


