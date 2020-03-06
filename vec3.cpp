//
// Created by hunor on 06/03/2020.
//

#include "vec3.h"

namespace egc {

    vec3 &vec3::operator=(const vec3 &srcVector) {
        this->x = srcVector.x;
        this->y = srcVector.y;
        this->z = srcVector.z;

        return *this;
    }

    vec3 vec3::operator+(const vec3 &srcVector) const {
        return vec3(this->x + srcVector.x, this->y + srcVector.y, this->z + srcVector.z);
    }

    vec3 &vec3::operator+=(const vec3 &srcVector) {
        this->x += srcVector.x;
        this->y += srcVector.y;
        this->z += srcVector.z;

        return *this;
    }

    vec3 vec3::operator*(float scalarValue) const {
        return vec3(this->x * scalarValue, this->y * scalarValue, this->z * scalarValue);
    }

    vec3 vec3::operator-(const vec3 &srcVector) const {
        return vec3(this->x - srcVector.x, this->y - srcVector.y, this->z - srcVector.z);
    }

    vec3 &vec3::operator-=(const vec3 &srcVector) {
        this->x -= srcVector.x;
        this->y -= srcVector.y;
        this->z -= srcVector.z;

        return *this;
    }

    vec3 vec3::operator-() const {
        return vec3(-this->x, -this->y, -this->z);
    }

    float vec3::length() const {
        return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    }

    vec3 &vec3::normalize() {
        float length = this->length();
        this->x /= length;
        this->y /= length;
        this->z /= length;

        return *this;
    }

    float dotProduct(const vec3 &v1, const vec3 &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    vec3 crossProduct(const vec3 &v1, const vec3 &v2) {
        float x = v1.y * v2.z - v1.z * v2.y;
        float y = v1.z * v2.x - v1.x * v2.z;
        float z = v1.x * v2.y - v1.y * v2.x;

        return vec3(x, y, z);
    }
}