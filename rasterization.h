#pragma once

#include "SDL2/SDL.h"
#include "vec2.h"
#include "vec4.h"
#include <vector>


namespace ecg {

    void computeAlphaBetaGamma(const std::vector<ecg::vec4> &triangleVertices, vec2 pixel, float &alpha, float &beta,
                               float &gamma);

    void rasterizeTriangle(SDL_Renderer *renderer, const std::vector<ecg::vec4> &triangleVertices,
                           const std::vector<ecg::vec4> &triangleColors);

}

