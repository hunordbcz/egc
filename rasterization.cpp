#include "rasterization.h"

namespace ecg {

    float BC(const vec2 &vec2, const std::vector<ecg::vec4> &triangleVertices);

    float AC(const vec2 &vec2, const std::vector<ecg::vec4> &triangleVertices);

    void computeAlphaBetaGamma(const std::vector<ecg::vec4> &triangleVertices, vec2 pixel, float &alpha, float &beta,
                               float &gamma) {
        alpha = BC(pixel, triangleVertices) / BC(triangleVertices.at(0), triangleVertices);
        beta = AC(pixel, triangleVertices) / AC(triangleVertices.at(1), triangleVertices);
        gamma = 1 - alpha - beta;
    }

    float AC(const vec2 &vec2, const std::vector<ecg::vec4> &triangleVertices) {
        vec4 A, C;
        A = triangleVertices.at(0);
        C = triangleVertices.at(2);
        return (C.y - A.y) * vec2.x + (A.x - C.x) * vec2.y + C.x * A.y - A.x * C.y;
    }

    float BC(const vec2 &vec2, const std::vector<ecg::vec4> &triangleVertices) {
        vec4 B, C;
        B = triangleVertices.at(1);
        C = triangleVertices.at(2);
        return (B.y - C.y) * vec2.x + (C.x - B.x) * vec2.y + B.x * C.y - C.x * B.y;
    }

    void findBoundingBox(const std::vector<ecg::vec4> &triangleVertices, std::vector<vec2> &boundingBox);

    vec4 getColor(const std::vector<ecg::vec4> &triangleColors, float alpha, float beta, float gamma);

    void rasterizeTriangle(SDL_Renderer *renderer, const std::vector<ecg::vec4> &triangleVertices,
                           const std::vector<ecg::vec4> &triangleColors) {
        std::vector<ecg::vec2> boundingBox;
        findBoundingBox(triangleVertices, boundingBox);
        float alpha, beta, gamma;

        for (float x = boundingBox.at(0).x; x <= boundingBox.at(1).x; x++) {
            for (float y = boundingBox.at(0).y; y <= boundingBox.at(1).y; y++) {
                vec2 pixel(x, y);
                computeAlphaBetaGamma(triangleVertices, pixel, alpha, beta, gamma);

                if (0 < alpha && alpha < 1 && 0 < beta && beta < 1 && 0 < gamma && gamma < 1) {
                    vec4 color = getColor(triangleColors, alpha, beta, gamma);

                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
        }
    }

    vec4 getColor(const std::vector<ecg::vec4> &triangleColors, float alpha, float beta, float gamma) {
        vec4 color;
        color.x = alpha * triangleColors.at(0).x + beta * triangleColors.at(1).x +
                  gamma * triangleColors.at(2).x;
        color.y = alpha * triangleColors.at(0).y + beta * triangleColors.at(1).y +
                  gamma * triangleColors.at(2).y;
        color.z = alpha * triangleColors.at(0).z + beta * triangleColors.at(1).z +
                  gamma * triangleColors.at(2).z;
        color.w = alpha * triangleColors.at(0).w + beta * triangleColors.at(1).w +
                  gamma * triangleColors.at(2).w;
        return color;
    }

    void findBoundingBox(const std::vector<ecg::vec4> &triangleVertices, std::vector<vec2> &boundingBox) {
        vec2 min, max;
        for (const ecg::vec4 &vec : triangleVertices) {
            if (vec.x > max.x) {
                max.x = vec.x;
            }
            if (vec.x < min.x || min.x == 0) {
                min.x = vec.x;
            }
            if (vec.y > max.y) {
                max.y = vec.y;
            }
            if (vec.y < min.y || min.y == 0) {
                min.y = vec.y;
            }
        }
        boundingBox.push_back(min);
        boundingBox.push_back(max);
    }

}