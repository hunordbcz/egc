//
// Created by hunor on 06/03/2020.
//

#include "vec2.h"

namespace ecg {
    vec2 &vec2::operator=(const vec2 &srcVector) {
        this->x = srcVector.x;
        this->y = srcVector.y;

        return *this;
    }

    vec2 vec2::operator+(const vec2 &srcVector) const {
        return vec2(this->x + srcVector.x, this->y + srcVector.y);
    }

    vec2 &vec2::operator+=(const vec2 &srcVector) {
        this->x += srcVector.x;
        this->y += srcVector.y;

        return *this;
    }

    vec2 vec2::operator*(float scalarValue) const {
        return vec2(this->x * scalarValue, this->y * scalarValue);
    }

    vec2 vec2::operator-(const vec2 &srcVector) const {
        return vec2(this->x - srcVector.x, this->y - srcVector.y);
    }

    vec2 &vec2::operator-=(const vec2 &srcVector) {
        this->x -= srcVector.x;
        this->y -= srcVector.y;

        return *this;
    }

    vec2 vec2::operator-() const {
        return vec2(-this->x, -this->y);
    }

    float vec2::length() const {
        return sqrt(pow(this->x, 2) + pow(this->y, 2));
    }

    vec2 &vec2::normalize() {
        float length = this->length();
        this->x /= length;
        this->y /= length;

        return *this;
    }

    float dotProduct(const vec2 &v1, const vec2 &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }
}