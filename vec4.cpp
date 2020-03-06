//
// Created by hunor on 06/03/2020.
//

#include "vec4.h"

namespace egc {
    vec4 &vec4::operator=(const vec4 &srcVector) {
        this->x = srcVector.x;
        this->y = srcVector.y;
        this->z = srcVector.z;
        this->w = srcVector.w;

        return *this;
    }

    vec4 vec4::operator+(const vec4 &srcVector) const {
        return vec4(this->x + srcVector.x, this->y + srcVector.y, this->z + srcVector.z, this->w + srcVector.w);
    }

    vec4 &vec4::operator+=(const vec4 &srcVector) {
        this->x += srcVector.x;
        this->y += srcVector.y;
        this->z += srcVector.z;
        this->w += srcVector.w;

        return *this;
    }

    vec4 vec4::operator*(float scalarValue) const {
        return vec4(this->x * scalarValue, this->y * scalarValue, this->z * scalarValue, this->w * scalarValue);
    }

    vec4 vec4::operator-(const vec4 &srcVector) const {
        return vec4(this->x - srcVector.x, this->y - srcVector.y, this->z - srcVector.z, this->w - srcVector.w);
    }

    vec4 &vec4::operator-=(const vec4 &srcVector) {
        this->x -= srcVector.x;
        this->y -= srcVector.y;
        this->z -= srcVector.z;
        this->w -= srcVector.w;

        return *this;
    }

    vec4 vec4::operator-() const {
        return vec4(-this->x, -this->y, -this->z, -this->w);
    }

    float vec4::length() const {
        return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2));
    }

    vec4 &vec4::normalize() {
        float length = this->length();
        this->x /= length;
        this->y /= length;
        this->z /= length;
        this->w /= length;

        return *this;
    }
}
