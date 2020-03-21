#include "mat3.h"

namespace ecg {

    //methods used as GETTERS and SETTERS - to access the matrix elements
    float &mat3::at(int i, int j) {
        return matrixData[i + 3 * j];
    }

    const float &mat3::at(int i, int j) const {
        return matrixData[i + 3 * j];
    }

    mat3 &mat3::operator=(const mat3 &srcMatrix) {
        //usage example for the "at" getter/setter methods
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                //at(i,j) acts as a setter
                //srcMatrix.at(i, j) is used as a getter
                at(i, j) = srcMatrix.at(i, j);
            }
        }

        return (*this);
    }

    mat3 mat3::operator+(const mat3 &srcMatrix) const {
        mat3 m;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m.at(i, j) = at(i, j) + srcMatrix.at(i, j);
            }
        }

        return m;
    }

    mat3 mat3::operator*(float scalarValue) const {
        mat3 m;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m.at(i, j) = at(i, j) * scalarValue;
            }
        }
        return m;
    }

    mat3 mat3::operator*(const mat3 &srcMatrix) const {
        mat3 m;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m.at(i, j) = 0;
                for (int k = 0; k < 3; k++) {
                    m.at(i, j) += at(i, k) * srcMatrix.at(k, j);
                }
            }
        }

        return m;
    }

    vec3 mat3::operator*(const vec3 &srcVector) const {
        vec3 result;
        result.x = at(0, 0) * srcVector.x + at(0, 1) * srcVector.y + at(0, 2) * srcVector.z;
        result.y = at(1, 0) * srcVector.x + at(1, 1) * srcVector.y + at(1, 2) * srcVector.z;
        result.z = at(2, 0) * srcVector.x + at(2, 1) * srcVector.y + at(2, 2) * srcVector.z;
        return result;
    }

    float mat3::determinant() const {
        return at(0, 0) * at(1, 1) * at(2, 2) + at(1, 0) * at(2, 1) * at(0, 2) + at(2, 0) * at(1, 2) * at(0, 1) -
               at(0, 2) * at(1, 1) * at(2, 0) - at(2, 1) * at(1, 2) * at(0, 0) - at(1, 0) * at(0, 1) * at(2, 2);
    }

    mat3 mat3::transpose() const {
        mat3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.at(j, i) = at(i, j);
            }
        }
        return result;
    }

    float mat3::getMinorDeterminant(const mat3 &srcMatrix, int x, int y) const {
        int k = 0;
        float mat2[4];
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (i != x && j != y) {
                    mat2[k++] = srcMatrix.at(i, j);
                }
            }
        }
        return mat2[0] * mat2[3] - mat2[1] * mat2[2];
    }

    mat3 mat3::inverse() const {
        mat3 result;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result.at(i, j) = getMinorDeterminant(transpose(), i, j) / determinant();
                if ((i + j) % 2) {
                    result.at(i, j) *= -1;
                }
            }
        }
        return result;
    }
}