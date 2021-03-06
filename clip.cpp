#include "clip.h"
#include "mat3.h"
#include "transform.h"

namespace ecg {

    float getMaxX(std::vector<vec3> clipWindow) {
        return std::max(std::max(clipWindow.at(0).x, clipWindow.at(1).x),
                        std::max(clipWindow.at(2).x, clipWindow.at(3).x));
    }

    float getMaxY(std::vector<vec3> clipWindow) {
        return std::max(std::max(clipWindow.at(0).y, clipWindow.at(1).y),
                        std::max(clipWindow.at(2).y, clipWindow.at(3).y));
    }

    float getMinX(std::vector<vec3> clipWindow) {
        return std::min(std::min(clipWindow.at(0).x, clipWindow.at(1).x),
                        std::min(clipWindow.at(2).x, clipWindow.at(3).x));
    }

    float getMinY(std::vector<vec3> clipWindow) {
        return std::min(std::min(clipWindow.at(0).y, clipWindow.at(1).y),
                        std::min(clipWindow.at(2).y, clipWindow.at(3).y));
    }

    std::vector<int> computeCSCode(const std::vector<vec3> &clipWindow, const vec3 &p) {
        std::vector<int> code;

        code.push_back(p.y > getMaxY(clipWindow));
        code.push_back(p.y < getMinY(clipWindow));
        code.push_back(p.x > getMaxX(clipWindow));
        code.push_back(p.x < getMinX(clipWindow));

        return code;
    }

    bool simpleRejection(std::vector<int> cod1, std::vector<int> cod2) {
        for (int i = 0; i < cod1.size(); i++) {
            if (cod1.at(i) * cod2.at(i)) {
                return true;
            }
        }

        return false;
    }

    bool simpleAcceptance(std::vector<int> cod1, std::vector<int> cod2) {
        for (int i = 0; i < cod1.size(); i++) {
            if (cod1.at(i) || cod2.at(i)) {
                return false;
            }
        }

        return true;
    }

    bool isInsideClipWindow(std::vector<int> cod) {
        return !(cod.at(0) + cod.at(1) + cod.at(2) + cod.at(3));
    }

    void invertPoints(vec3 &p1, vec3 &p2) {
        vec3 temp = p1;
        p1 = p2;
        p2 = temp;
    }

    int lineClip_CohenSutherland(const std::vector<vec3> &clipWindow, vec3 &p1, vec3 &p2) {
        bool isFinished = false;

        while (!isFinished) {
            std::vector<int> cod1 = computeCSCode(clipWindow, p1);
            std::vector<int> cod2 = computeCSCode(clipWindow, p2);
            if (simpleRejection(cod1, cod2)) {
                return -1;
            }
            if (simpleAcceptance(cod1, cod2)) {
                isFinished = true;
                continue;
            }

            if (isInsideClipWindow(cod1)) {
                invertPoints(p1, p2);
                cod1 = computeCSCode(clipWindow, p1);
                cod2 = computeCSCode(clipWindow, p2);
            }

            if (cod1.at(0) && p1.y != p2.y) {
                p1.x = p1.x + (p2.x - p1.x) * (getMaxY(clipWindow) - p1.y) / (p2.y - p1.y);
                p1.y = getMaxY(clipWindow);
            } else if (cod1.at(1) && p1.y != p2.y) {
                p1.x = p1.x + (p2.x - p1.x) * (getMinY(clipWindow) - p1.y) / (p2.y - p1.y);
                p1.y = getMinY(clipWindow);
            } else if (cod1.at(2) && p1.x != p2.x) {
                p1.y = p1.y + (p2.y - p1.y) * (getMaxX(clipWindow) - p1.x) / (p2.x - p1.x);
                p1.x = getMaxX(clipWindow);
            } else if (cod1.at(3) && p1.x != p2.x) {
                p1.y = p1.y + (p2.y - p1.y) * (getMinX(clipWindow) - p1.x) / (p2.x - p1.x);
                p1.x = getMinX(clipWindow);
            }
        }
        return 0;
    }

    std::vector<vec3> getNormals(std::vector<vec3> clipWindow) {
        std::vector<vec3> normalized;
        normalized.reserve(4);

        mat3 rotationMatrix = rotate(90);

        vec3 edge;

        for (int i = 0; i < clipWindow.size(); i++) {
            edge = clipWindow[(i + 1) % clipWindow.size()] - clipWindow[i];
            normalized.push_back((rotationMatrix * edge).normalize());
        }

        return normalized;
    }

    int lineClip_CyrusBeck(std::vector<vec3> clipWindow, vec3 &p1, vec3 &p2) {
        std::vector<vec3> normals = getNormals(clipWindow);

        if (p1 == p2) {
            return -1;
        }

        float tE = 0, tL = 1.0;
        vec3 D = p2 - p1;

        for (int i = 0; i < clipWindow.size(); i++) {
            float product = dotProduct(normals[i], D);
            if (product == 0) {
                continue;
            }

            float t = -dotProduct(normals[i], (p1 - clipWindow[i])) / product;
            if (t < 0 || t > 1) {
                continue;
            }

            if (product < 0) {
                tE = std::max(tE, t);
            } else {
                tL = std::min(tL, t);
            }
        }

        if (tE > tL) {
            return -1;
        }

        vec3 tempP1 = p2 * tE + p1 * (1 - tE);
        p2 = p2 * tL + p1 * (1 - tL);
        p1 = tempP1;

        return 1;
    }
}

