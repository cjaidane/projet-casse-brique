#include "../include/bonusmalus.h"
#include <iostream>

BonusMalus::BonusMalus(int x, bool bonus, int radius, Bonus bon, Malus mal, bool act) : radius(radius), velX(0), velY(5), bonus(bonus), active(true), activationBonus(false), used(false){
    center.x = x;
    center.y = 0;
    this->bon = bon;
    this->mal = mal;
    this->active = act;
}

BonusMalus::~BonusMalus() {
}


void BonusMalus::render(SDL_Renderer* renderer) {
    if (active) {
        if (bonus) {
            SDL_SetRenderDrawColor(renderer, 23, 207, 62, 255); // Vert
        }else {
            SDL_SetRenderDrawColor(renderer, 207, 23, 23, 255); //  Rouge
        }
        SDL_Rect ballRect = {center.x - radius, center.y - radius, 2 * radius, 2 * radius};
        SDL_RenderFillRect(renderer, &ballRect);
    }
}


void BonusMalus::update(int windowWidth, int windowHeight, SDL_Rect paddleRect) {

    // // Gère la perte de la balle
    // if (center.y - radius > windowHeight) {
    //     velX = velY = 0;
    // }

    // Met à jour la position de la balle en fonction de sa vitesse
    if (active) {

        center.x += velX;
        center.y += velY;
        if (center.y + radius >= paddleRect.y &&
            center.y + radius <= paddleRect.y + paddleRect.h &&
            center.x >= paddleRect.x &&
            center.x <= paddleRect.x + paddleRect.w) {
            //Appliquer bonus ou malus
            // std::cout << "Bonus" << std::endl;
            active = false;
            activationBonus = true;

        }
    }
}

bool BonusMalus::getActivationBonus(){
    return activationBonus;
}

void BonusMalus::setActivationBonus(bool act){
    this->activationBonus = act;
}

Bonus BonusMalus::getBonusType(){
    return bon;
}

Malus BonusMalus::getMalusType(){
    return mal;
}

void BonusMalus::setActive(bool act){
    this->active = act;
}

bool BonusMalus::getUsed(){
    return this->used;
}

void BonusMalus::setUsed(bool us){
    this->used = us;
}   
