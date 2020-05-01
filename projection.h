//
//  projection.h
//  Lab8_TODO
//
//  Copyright © 2016 CGIS. All rights reserved.
//

#pragma once

#include "SDL2/SDL.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "camera.h"
#include <vector>


namespace ecg {
    mat4 defineViewTransformMatrix(int startX, int startY, int width, int height);

    mat4 defineCameraMatrix(Camera camera);

    mat4 definePerspectiveProjectionMatrix(float fov, float aspect, float zNear, float zFar);

    void perspectiveDivide(vec4 &inputVertex);

    bool clipPointInHomogeneousCoordinate(const ecg::vec4 &vertex);

    bool clipTriangleInHomegeneousCoordinates(const std::vector<ecg::vec4> &triangle);

    ecg::vec3 findNormalVectorToTriangle(const std::vector<ecg::vec4> &triangle);

    ecg::vec4 findCenterPointOfTriangle(const std::vector<ecg::vec4> &triangle);

    bool isTriangleVisible(const std::vector<ecg::vec4> &triangle, const ecg::vec3 &normalVector);

    void displayNormalVectors(ecg::vec3 &normalVector, ecg::vec4 &triangleCenter, SDL_Renderer *renderer,
                              ecg::mat4 viewTransformMatrix, ecg::mat4 perspectiveMatrix);
}
