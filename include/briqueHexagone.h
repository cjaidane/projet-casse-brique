#ifndef BRICKHEXAGONE_H
#define BRICKHEXAGONE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <vector>
#include <iostream>
//
// template <typename Number, int w>
// struct _Hex { // Both storage types, both constructors
//     union {
//         const Number v[3];
//         struct { const Number q, r, s; };
//     };
//
//     _Hex(Number q_, Number r_): v{q_, r_, -q_ - r_} {}
//     _Hex(Number q_, Number r_, Number s_): v{q_, r_, s_} {}
// };
// typedef _Hex<int, 1> Hex;
// typedef _Hex<int, 0> HexDifference;
// typedef _Hex<double, 1> FractionalHex;
// typedef _Hex<double, 0> FractionalHexDifference;

struct Hex
{
    const int q;
    const int r;
    const int s;
    Hex(int q_, int r_, int s_): q(q_), r(r_), s(s_) {
        if (q + r + s != 0) throw "q + r + s must be 0";
    }
};

struct Orientation {
    const double f0, f1, f2, f3;
    const double b0, b1, b2, b3;
    const double start_angle; // in multiples of 60°
    Orientation(double f0_, double f1_, double f2_, double f3_,
                double b0_, double b1_, double b2_, double b3_,
                double start_angle_)
    : f0(f0_), f1(f1_), f2(f2_), f3(f3_),
      b0(b0_), b1(b1_), b2(b2_), b3(b3_),
      start_angle(start_angle_) {}
};

const Orientation layout_pointy
  = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
                sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
                0.5);
const Orientation layout_flat
  = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0),
                2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0,
                0.0);

struct Layout {
    const Orientation orientation;
    const SDL_Point size;
    const SDL_Point origin;
    Layout(Orientation orientation_, SDL_Point size_, SDL_Point origin_)
    : orientation(orientation_), size(size_), origin(origin_) {}
};


class BrickHexagone {
public:
    BrickHexagone(int resistance, int q, int r, int s);
    void render(SDL_Renderer* renderer) const;
    bool checkCollision(const SDL_Rect& ballRect);
    bool isActive() const;
  

private:
    // SDL_Rect rect;
    bool active; // Ajout de l'état actif qui sert à savoir si elles sont encore actives
    int resistance; // Ajout de la résistance qui indique combien de fois elles doivent être touchées avant d'être détruites
    std::vector<SDL_FPoint> corners;
    
};

#endif // BRICKHEXAGONE_H

