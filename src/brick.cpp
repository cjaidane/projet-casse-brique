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
    if (!active) return;  // Ne rien faire si la brique n'est pas active.

    // Choisissez une couleur basée sur la résistance de la brique.
    switch(resistance) {
        case 1:
            SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255); // Hot Pink pour les briques avec une résistance de 1.
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);   // Orange pour les briques avec une résistance de 2.
            break;
        case 3:
            SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);   // Forest Green pour les briques avec une résistance de 3.
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 64, 224, 208, 255);  // Turquoise pour les briques avec une résistance supérieure à 3.
            break;
    }

    // Dessinez la brique.
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

    SDL_Rect brickRect = {rect.x, rect.y, rect.w, rect.h};
    if (SDL_HasIntersection(&ballRect, &brickRect)) {
        if (resistance > 0) {
            resistance--;  // Décrémentez la résistance de la brique.
        }
        if (resistance == 0) {
            active = false;  // Désactivez la brique si sa résistance atteint zéro.
        }
        return true;  // Retournez vrai pour indiquer une collision.
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