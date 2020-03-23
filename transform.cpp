#include "mat3.h"
#include "mat4.h"
#include "transform.h"
#include <cmath>

namespace ecg {

    //transformation matrices in 2D
    mat3 translate(const vec2 translateArray) {
        mat3 result;
        result.at(0, 2) = translateArray.x;
        result.at(1, 2) = translateArray.y;
        return result;
    }

    mat3 translate(float tx, float ty) {
        mat3 result;
        result.at(0, 2) = tx;
        result.at(1, 2) = ty;
        return result;
    }

    mat3 scale(const vec2 scaleArray) {
        mat3 result;
        result.at(0, 0) = scaleArray.x;
        result.at(1, 1) = scaleArray.y;
        return result;
    }

    mat3 scale(float sx, float sy) {
        mat3 result;
        result.at(0, 0) = sx;
        result.at(1, 1) = sy;
        return result;
    }

    mat3 rotate(float angle) {
        mat3 result;
        float radian = PI * angle / 180.0;
        result.at(0, 0) = cos(radian);
        result.at(0, 1) = -sin(radian);
        result.at(1, 0) = sin(radian);
        result.at(1, 1) = cos(radian);
        return result;
    }

    //transformation matrices in 3D
    mat4 translate(const vec3 translateArray) {
        mat4 result;
        result.at(0, 3) = translateArray.x;
        result.at(1, 3) = translateArray.y;
        result.at(2, 3) = translateArray.z;
        return result;
    }

    mat4 translate(float tx, float ty, float tz) {
        mat4 result;
        result.at(0, 3) = tx;
        result.at(1, 3) = ty;
        result.at(2, 3) = tz;
        return result;
    }

    mat4 scale(const vec3 scaleArray) {
        mat4 result;
        result.at(0, 0) = scaleArray.x;
        result.at(1, 1) = scaleArray.y;
        result.at(2, 2) = scaleArray.z;
        return result;
    }

    mat4 scale(float sx, float sy, float sz) {
        mat4 result;
        result.at(0, 0) = sx;
        result.at(1, 1) = sy;
        result.at(2, 2) = sz;
        return result;
    }

    mat4 rotateX(float angle) {
        mat4 result;
        float radian = PI * angle / 180.0;
        result.at(1, 1) = cos(radian);
        result.at(1, 2) = -sin(radian);
        result.at(2, 1) = sin(radian);
        result.at(2, 2) = cos(radian);
        return result;
    }

    mat4 rotateY(float angle) {
        mat4 result;
        float radian = PI * angle / 180.0;
        result.at(0, 0) = cos(radian);
        result.at(2, 0) = -sin(radian);
        result.at(0, 2) = sin(radian);
        result.at(2, 2) = cos(radian);
        return result;
    }

    mat4 rotateZ(float angle) {
        mat4 result;
        float radian = PI * angle / 180.0;
        result.at(0, 0) = cos(radian);
        result.at(0, 1) = -sin(radian);
        result.at(1, 0) = sin(radian);
        result.at(1, 1) = cos(radian);
        return result;
    }
}