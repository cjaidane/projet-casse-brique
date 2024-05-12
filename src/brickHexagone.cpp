#include "../include/briqueHexagone.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

SDL_Point hex_to_pixel(Layout layout, Hex h) {
    const Orientation& M = layout.orientation;
    double x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;
    double y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;
    SDL_Point point;
    point.x = x + layout.origin.x;
    point.y = y + layout.origin.y;
    return point;
}
SDL_FPoint hex_corner_offset(Layout layout, int corner) {
    SDL_Point size = layout.size;
    float angle = 2.0 * M_PI *
             (layout.orientation.start_angle + corner) / 6;
    return {size.x * cos(angle), size.y * sin(angle)};
}

std::vector<SDL_FPoint> polygon_corners(Layout layout, Hex h) {
    std::vector<SDL_FPoint> corners = {};
    SDL_Point center = hex_to_pixel(layout, h);
    for (int i = 0; i < 6; i++) {
        SDL_FPoint offset = hex_corner_offset(layout, i);
        corners.push_back({center.x + offset.x,
                                center.y + offset.y});
    }
    return corners;
}

/*Constructeur pour la classe Brick.
    Initialise une brique avec les dimensions et la résistance spécifiées.
    @param x La position horizontale de départ de la brique.
    @param y La position verticale de départ de la brique.
    @param w La largeur de la brique.
    @param h La hauteur de la brique.
    @param resistance La résistance initiale de la brique, indiquant combien de fois elle doit être touchée avant d'être détruite.
*/
BrickHexagone::BrickHexagone(int resistance, int q, int r, int s) 
    : resistance(resistance), active(true){
    this->corners = polygon_corners(Layout(layout_pointy, {30, 30}, {100, 100}), Hex(q, r, s));

}

/*
    Rend la brique à l'écran si elle est active.
    @param renderer Le renderer SDL utilisé pour dessiner la brique.

*/
void BrickHexagone::render( SDL_Renderer* renderer) const {
    if (!active) return;
     // Définir la couleur en fonction de la résistance
    SDL_Color colH;
    switch(resistance) {
        case 1:
            colH = {255, 105, 180, 255}; // Hot Pink pour les briques avec une résistance de 1.
            break;
        case 2:
            colH = {255, 165, 0, 255};   // Orange pour les briques avec une résistance de 2.
            break;
        case 3:
            colH = {34, 139, 34, 255};   // Forest Green pour les briques avec une résistance de 3.
            break;
        default:
            colH = {64, 224, 208, 255};  // Turquoise pour les briques avec une résistance supérieure.
            break;
    }
    // SDL_SetRenderDrawColor(renderer, 255, 255, 180, 255);  // Example: Hot Pink for visibility
    // SDL_RenderFillRect(renderer, &rect);
    // SDL_Point points[6];
    // for (int i = 0; i < 6; ++i) {
    //     points[i].x = centerX + size * cos(i * M_PI / 3);
    //     points[i].y = centerY + size * sin(i * M_PI / 3);
    // }

    // for (auto& corn : polygon_corners(Layout(layout_flat, {25, 25}, {100, 100}), Hex(0, 0, 0))) {
    //     SDL_RenderDrawLines(renderer, corn, corn +1);
    // }

    SDL_SetRenderDrawColor(renderer, colH.r ,colH.g, colH.b, colH.a);
    for (int i = 0; i < 6; i++) {
        SDL_RenderDrawLine(renderer, corners.at(i%6).x, corners.at(i%6).y, corners.at((i+1)%6).x, corners.at((i+1)%6).y);
    }

    // // Dessiner les bords de l'hexagone
    // for (int i = 0; i < 6; i++) {
    //     SDL_RenderDrawLine(renderer,
    //         corners[i%6].x, corners[i%6].y,
    //         corners[(i+1)%6].x, corners[(i+1)%6].y);
    // }

    SDL_Vertex vertex_1 = {corners.at(0), colH, {1, 1}};
    SDL_Vertex vertex_2 = {corners.at(1), colH, {1, 1}};
    SDL_Vertex vertex_3 = {corners.at(2), colH, {1, 1}};

    // Put them into array

    SDL_Vertex vertices[] = {
        vertex_1,
        vertex_2,
        vertex_3
    };

    SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);

    vertex_1 = {corners.at(3), colH, {1, 1}};
    vertex_2 = {corners.at(4), colH, {1, 1}};
    vertex_3 = {corners.at(5), colH, {1, 1}};

    SDL_Vertex vertices2[] = {
        vertex_1,
        vertex_2,
        vertex_3
    };

    SDL_RenderGeometry(renderer, nullptr, vertices2, 3, nullptr, 0);


    vertex_1 = {corners.at(0), colH, {1, 1}};
    vertex_2 = {corners.at(2), colH, {1, 1}};
    vertex_3 = {corners.at(3), colH, {1, 1}};

    SDL_Vertex vertices3[] = {
        vertex_1,
        vertex_2,
        vertex_3
    };

    SDL_RenderGeometry(renderer, nullptr, vertices3, 3, nullptr, 0);

    vertex_1 = {corners.at(0), colH, {1, 1}};
    vertex_2 = {corners.at(3), colH, {1, 1}};
    vertex_3 = {corners.at(5), colH, {1, 1}};

    SDL_Vertex vertices4[] = {
        vertex_1,
        vertex_2,
        vertex_3
    };

    SDL_RenderGeometry(renderer, nullptr, vertices4, 3, nullptr, 0);
    // SDL_SetRenderDrawColor(renderer, 20 ,20,20, 255);
    // SDL_RenderDrawPoint(renderer, corners.at(1).x, corners.at(1).y);
    // SDL_RenderFillRect(renderer, {corners.at(1).x, corners.at(1).y, corners.at(2).})
    // SDL_RenderFillRect(renderer, corners.data(), corners.size());
    // SDL_RenderGeometry(renderer, nullptr, /* Conrners */, 6, nullptr, 0);
    

    // Dessin de l'hexagone en reliant ses sommets
    // SDL_RenderDrawLines(renderer, corners, 6);
    // Relier le premier et le dernier sommet pour fermer l'hexagone
    // SDL_RenderDrawLine(renderer, points[5].x, points[5].y, points[0].x, points[0].y);
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
bool BrickHexagone::checkCollision(const SDL_Rect& ballRect) {
    if (!active) return false;  // Si la brique n'est pas active, ne vérifiez pas les collisions.
    // Créez un SDL_Rect pour la brique.
    // Utilisez SDL_HasIntersection pour vérifier si la balle intersecte la brique.
    for (int i = 0; i < 6; i++) {
        int xStart = corners.at(i).x;
        int yStart = corners.at(i).y;
        int xEnd = corners.at((i+1)%6).x;
        int yEnd = corners.at((i+1)%6).y;
        // SDL_IntersectRectAndLine(&ballRect, &xStart, &yStart, &xEnd, &yEnd)

        if (SDL_IntersectRectAndLine(&ballRect, &xStart, &yStart, &xEnd, &yEnd)) {
            if(resistance > 0){
                resistance--;  // Décrémentez la résistance de la brique.
            }
            if (resistance == 0) {
                active = false;  // Désactivez la brique si sa résistance atteint zéro.
            }
            return true;  // Retournez vrai car il y a eu une collision.
        }
    }

    return false;  // Retournez faux si aucune collision n'est détectée.
}


/*
    Retourne si la brique est toujours active.
    @return true si la brique est active et sa résistance est supérieure à zéro
*/
bool BrickHexagone::isActive() const {
    return active && resistance > 0;
}
