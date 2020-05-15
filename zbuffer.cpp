#include "zbuffer.h"

namespace ecg {

    float getVal(const vec2 &vec2, const vec4 &v1, const vec4 &v2);

    void computeAlphaBetaGamma(const std::vector<ecg::vec4> &triangleVertices, vec2 pixel, float &alpha, float &beta,
                               float &gamma) {
        vec4 A, B, C;
        A = triangleVertices.at(0);
        B = triangleVertices.at(1);
        C = triangleVertices.at(2);

        alpha = getVal(pixel, B, C) / getVal(triangleVertices.at(0), B, C);
        beta = getVal(pixel, C, A) / getVal(triangleVertices.at(1), C, A);
        gamma = 1 - alpha - beta;
    }

    float getVal(const vec2 &vec2, const vec4 &v1, const vec4 &v2) {
        return (v1.y - v2.y) * vec2.x + (v2.x - v1.x) * vec2.y + v1.x * v2.y - v2.x * v1.y;
    }

    void findBoundingBox(const std::vector<ecg::vec4> &triangleVertices, std::vector<vec2> &boundingBox) {
        vec2 min, max;
        for (const ecg::vec4 &vec : triangleVertices) {
            if (vec.x >= max.x) {
                max.x = vec.x;
            }
            if (vec.x <= min.x || min.x == 0) {
                min.x = vec.x;
            }
            if (vec.y >= max.y) {
                max.y = vec.y;
            }
            if (vec.y <= min.y || min.y == 0) {
                min.y = vec.y;
            }
        }
        boundingBox.push_back(min);
        boundingBox.push_back(max);
    }

    void
    drawTriangleInZBuffer(std::vector<ecg::vec4> triangle, float depthBuffer[WINDOW_HEIGHT][WINDOW_WIDTH], float &zmin,
                          float &zmax) {
        std::vector<ecg::vec2> boundingBox;
        findBoundingBox(triangle, boundingBox);

        float alpha, beta, gamma;

        for (float x = boundingBox.at(0).x; x <= boundingBox.at(1).x; x++) {
            for (float y = boundingBox.at(0).y; y <= boundingBox.at(1).y; y++) {
                vec2 pixel(x, y);
                computeAlphaBetaGamma(triangle, pixel, alpha, beta, gamma);
                if (-0.15 <= alpha && alpha <= 1.15 && -0.15 <= beta && beta <= 1.15 && -0.15 <= gamma &&
                    gamma <= 1.15) {
                    float z = triangle.at(0).z * alpha + triangle.at(1).z * beta + triangle.at(2).z * gamma;
                    if (z > depthBuffer[(int) pixel.x][(int) pixel.y]) {
                        depthBuffer[(int) pixel.x][(int) pixel.y] = z;
                    }
                    zmax = std::min(z, zmax);
                    zmin = std::max(z, zmin);
                }
            }
        }
    }

}
