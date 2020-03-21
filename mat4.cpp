#include "mat4.h"
#include "mat3.h"

namespace ecg {
    //methods used as GETTERS and SETTERS -> to access the matrix elements
    float &mat4::at(int i, int j) {
        return matrixData[i + 4 * j];
    }

    const float &mat4::at(int i, int j) const {
        return matrixData[i + 4 * j];
    }

    mat4 &mat4::operator=(const mat4 &srcMatrix) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                at(i, j) = srcMatrix.at(i, j);
            }
        }

        return *this;
    }

    mat4 mat4::operator+(const mat4 &srcMatrix) const {
        mat4 result;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.at(i, j) = at(i, j) + srcMatrix.at(i, j);
            }
        }
        return result;
    }

    mat4 mat4::operator*(float scalarValue) const {
        mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.at(i, j) = at(i, j) * scalarValue;
            }
        }
        return result;
    }

    mat4 mat4::operator*(const mat4 &srcMatrix) const {
        mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.at(i, j) = 0;
                for (int k = 0; k < 4; k++) {
                    result.at(i, j) += this->at(i, k) * srcMatrix.at(k, j);
                }
            }
        }

        return result;
    }

    vec4 mat4::operator*(const vec4 &srcVector) const {
        vec4 result;
        result.x =
                at(0, 0) * srcVector.x + at(0, 1) * srcVector.y + at(0, 2) * srcVector.z + at(0, 3) * srcVector.w;
        result.y =
                at(1, 0) * srcVector.x + at(1, 1) * srcVector.y + at(1, 2) * srcVector.z + at(1, 3) * srcVector.w;
        result.z =
                at(2, 0) * srcVector.x + at(2, 1) * srcVector.y + at(2, 2) * srcVector.z + at(2, 3) * srcVector.w;
        result.w =
                at(3, 0) * srcVector.x + at(3, 1) * srcVector.y + at(3, 2) * srcVector.z + at(3, 3) * srcVector.w;
        return result;
    }

    mat4 mat4::transpose() const {

        mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.at(j, i) = at(i, j);
            }
        }
        return result;
    }

    float mat4::determinant() const {
        return getMinor(*this, 0, 0).determinant() - getMinor(*this, 0, 1).determinant() +
               getMinor(*this, 0, 2).determinant() - getMinor(*this, 0, 3).determinant();
    }

    mat3 mat4::getMinor(const mat4 &srcMatrix, int x, int y) const {
        mat3 result;
        int k = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (i != x && j != y) {
                    result.at(k / 3, k % 3) = srcMatrix.at(i, j);
                    k++;
                }
            }
        }
        return result;
    }

    mat4 mat4::inverse() const {
        mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                mat3 minorMat(getMinor(transpose(), i, j));
                result.at(i, j) = minorMat.determinant() / determinant();
                if ((i + j) % 2) {
                    result.at(i, j) *= -1;
                }
            }
        }
        return result;
    }
}