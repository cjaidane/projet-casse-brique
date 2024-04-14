#include "../include/brick.h"

Brick::Brick(int x, int y, int w, int h, int resistance) 
    : rect{x, y, w, h}, resistance(resistance), active(true) {}

void Brick::render(SDL_Renderer* renderer) const {
    if (!active) return;
    int colorIntensity = 255 * resistance / initialResistance; // Diminuer l'intensité de la couleur basée sur la résistance
    SDL_SetRenderDrawColor(renderer, 255, colorIntensity, colorIntensity, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Brick::checkCollision(const SDL_Rect& ballRect) {
    if (!active) return false;
    if (ballRect.x < rect.x + rect.w && ballRect.x + ballRect.w > rect.x &&
        ballRect.y < rect.y + rect.h && ballRect.y + ballRect.h > rect.y) {
        resistance--;
        if (resistance <= 0) {
            deactivate();
        }
        return true;
    }
    return false;
}

bool Brick::isActive() const {
    return active && resistance > 0;
}

void Brick::deactivate() {
    active = false;
}
