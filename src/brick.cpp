#include "../include/brick.h"

Brick::Brick(int x, int y, int w, int h) : rect{x, y, w, h}, active(true) {}

void Brick::render(SDL_Renderer* renderer) const {
    if (!active) return;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge pour les briques visibles
    SDL_RenderFillRect(renderer, &rect);
}

bool Brick::checkCollision(const SDL_Rect& ballRect) {
    if (!active) return false;
    if (ballRect.x < rect.x + rect.w && ballRect.x + ballRect.w > rect.x &&
        ballRect.y < rect.y + rect.h && ballRect.y + ballRect.h > rect.y) {
        deactivate();
        return true;
    }
    return false;
}

bool Brick::isActive() const {
    return active;
}

void Brick::deactivate() {
    active = false;
}
