#include "zbuffer.h"

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

    void findBoundingBox(const std::vector<ecg::vec4> &triangleVertices, std::vector<vec3> &boundingBox) {
        vec3 min, max;
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
            if (vec.z >= max.z) {
                max.z = vec.z;
            }
            if (vec.z <= min.z || min.z == 0) {
                min.z = vec.z;
            }
        }
        boundingBox.push_back(min);
        boundingBox.push_back(max);
    }

    void
    drawTriangleInZBuffer(std::vector<ecg::vec4> triangle, float depthBuffer[WINDOW_HEIGHT][WINDOW_WIDTH], float &zmin,
                          float &zmax) {

        //TO DO  -  implement the "drawing" of the triangle inside the depth buffer
        //The buffer has the same dimension as the screen - use it to fill in not the pixel color of each pixel in the triangel - but the pixel depth (Z component)
        //So you are going to fill in the Z of each pixel inside the triangle - use the algorithm from last week to access the inside of the triangle

        //While you compute the z for each pixel, you can also determine the zmin and zmax values ===> from all the points of the rabbit
        //zmin and zmax are the minimum and maximum Z values FROM ALL THE VERICES OF THE RABBIT

        std::vector<ecg::vec3> boundingBox;
        findBoundingBox(triangle, boundingBox);

        float alpha, beta, gamma;

        for (float x = boundingBox.at(0).x; x <= boundingBox.at(1).x; x++) {
            for (float y = boundingBox.at(0).y; y <= boundingBox.at(1).y; y++) {
                vec2 pixel(x, y);
                computeAlphaBetaGamma(triangle, pixel, alpha, beta, gamma);
                if (0 <= alpha && alpha <= 1 && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1) {
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
