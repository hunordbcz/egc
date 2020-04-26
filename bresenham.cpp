#include "bresenham.h"
#include <cmath>

void setIncrements(int &d, int &inc1, int &inc2, int &dx, int &dy, bool invert);

void plot_points(int x, int y, BresenhamCircle circle, SDL_Renderer *renderer);

//Detects in which octant a line is situated
int getOctant(BresenhamLine line) {

    int deltaY = (line.endY - line.startY);
    int deltaX = (line.endX - line.startX);

    //Slope
    float m = (float) abs(deltaY) / (float) abs(deltaX);

    if ((m <= 1) && (deltaX > 0) && (deltaY <= 0)) { return 1; }
    if ((m > 1) && (deltaX > 0) && (deltaY <= 0)) { return 2; }
    if ((m >= 1) && (deltaX < 0) && (deltaY <= 0)) { return 3; }
    if ((m < 1) && (deltaX < 0) && (deltaY <= 0)) { return 4; }
    if ((m < 1) && (deltaX <= 0) && (deltaY >= 0)) { return 5; }
    if ((m >= 1) && (deltaX <= 0) && (deltaY >= 0)) { return 6; }
    if ((m >= 1) && (deltaX >= 0) && (deltaY >= 0)) { return 7; }
    if ((m < 1) && (deltaX >= 0) && (deltaY >= 0)) { return 8; }

    return 0;
}

//Draws a new raster line using Bresenham's algorithm
void BresenhamDraw(BresenhamLine line, SDL_Renderer *renderer) {
    int currentX = line.startX,
            currentY = line.startY,
            endX = line.endX,
            endY = line.endY,
            dx = abs(currentX - endX),
            dy = abs(currentY - endY),
            d = 0, increment1 = 0, increment2 = 0;

    switch (getOctant(line)) {
        case 1:
            setIncrements(d, increment1, increment2, dx, dy, false);

            while (currentX < endX) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentX++;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentY--;
                }
            }
            break;
        case 2:
            setIncrements(d, increment1, increment2, dx, dy, true);

            while (currentY > endY) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentY--;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentX++;
                }
            }
            break;
        case 3:
            setIncrements(d, increment1, increment2, dx, dy, true);

            while (currentY > endY) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentY--;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentX--;
                }
            }
            break;
        case 4:
            setIncrements(d, increment1, increment2, dx, dy, false);

            while (currentX > endX) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentX--;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentY--;
                }
            }
            break;
        case 5:
            setIncrements(d, increment1, increment2, dx, dy, false);

            while (currentX > endX) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentX--;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentY++;
                }
            }
            break;
        case 6:
            setIncrements(d, increment1, increment2, dx, dy, true);

            while (currentY < endY) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentY++;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentX--;
                }
            }
            break;
        case 7:
            setIncrements(d, increment1, increment2, dx, dy, true);

            while (currentY < endY) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentY++;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentX++;
                }
            }
            break;
        case 8:
            setIncrements(d, increment1, increment2, dx, dy, false);

            while (currentX < endX) {
                SDL_RenderDrawPoint(renderer, currentX, currentY);
                currentX++;

                if (d < 0) {
                    d += increment1;
                } else {
                    d += increment2;
                    currentY++;
                }
            }
            break;
        default:
            break;
    }
}

void setIncrements(int &d, int &inc1, int &inc2, int &dx, int &dy, bool invert) {
    if (invert) {
        inc1 = 2 * dx;
        inc2 = 2 * (dx - dy);
        d = inc1 - dy;
    } else {
        inc1 = 2 * dy;
        inc2 = 2 * (dy - dx);
        d = inc1 - dx;
    }
}

//Draws a new raster circle using Bresenham's algorithm
void BresenhamDraw(BresenhamCircle circle, SDL_Renderer *renderer) {
    int currentY = circle.radius;
    double decisionVar = (double) 1 / 4 - circle.radius;

    for (int currentX = 0; currentX < ceil(circle.radius / sqrt(2)); currentX++) {
        plot_points(currentX, currentY, circle, renderer);

        if ((decisionVar += 2 * currentX + 1) > 0) {
            decisionVar += 2 - 2 * currentY;
            currentY--;
        }
    }
}

void plot_points(int x, int y, BresenhamCircle circle, SDL_Renderer *renderer) {
    int op[] = {-1, 1};
    for (int i : op) {
        for (int j : op) {
            SDL_RenderDrawPoint(renderer, circle.centerX + (x * i), circle.centerY + (y * j));
            SDL_RenderDrawPoint(renderer, circle.centerX + (y * i), circle.centerY + (x * j));
        }
    }
}