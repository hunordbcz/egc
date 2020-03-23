//
//  main.cpp
//  Lab2
//
//  Copyright © 2016 CGIS. All rights reserved.
//

#include <iostream>
#include "testMatrix.h"
#include "testTransform.h"
#include "testVector.h"
#include "SDL2/SDL.h"
#include "vec3.h"
#include "mat3.h"
#include "math.h"
#include "algorithm"
#include "transform.h"

using namespace ecg;

SDL_Renderer *windowRenderer;

//define window dimensions
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

SDL_Window *window = NULL;
SDL_Event currentEvent;

SDL_Rect rectangleCoordinates = {100, 100, 200, 200};

bool quit = false;

int mouseX, mouseY;

bool initWindow() {
    bool success = true;

    //Try to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL initialization failed" << std::endl;
        success = false;
    } else {
        //Try to create the window
        window = SDL_CreateWindow("SDL Hello World Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if (window == NULL) {
            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;
        } else {
            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (windowRenderer == NULL) {
                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;
            } else {
                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }

        }
    }

    return success;
}

void destroyWindow() {
    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL
    SDL_Quit();
}

int main(int argc, char **argv) {

//    int nrOfErrors = 0;
//
//    nrOfErrors += ecg::testVec2Implementation();
//    nrOfErrors += ecg::testVec3Implementation();
//    nrOfErrors += ecg::testVec4Implementation();
//    nrOfErrors += ecg::testMat3Implementation();
//    nrOfErrors += ecg::testMat4Implementation();
//    nrOfErrors += ecg::test2DTransformImplementation();
//    nrOfErrors += ecg::test3DTransformImplementation();
//
//    std::cout << "Number of errors: " << nrOfErrors << std::endl;
//
//    std::getchar();
    if (!initWindow()) {
        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    vec3 P1(100, 100, 1),
            P2(100, 200, 1),
            P3(200, 100, 1),
            P4(200, 200, 1);


    while (!quit) {
        //Handle events on queue
        if (SDL_WaitEvent(&currentEvent) != 0) {
            //User requests quit
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
            }

            //Mouse event -> pressed button
            if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
                if (currentEvent.button.button == SDL_BUTTON_LEFT) {
                    SDL_GetMouseState(&mouseX, &mouseY);
                    std::cout << "Mouse click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
                }
            }

            //Mouse event -> mouse movement
            if (currentEvent.type == SDL_MOUSEMOTION) {
                if (currentEvent.button.button == SDL_BUTTON_LEFT) {
                    SDL_GetMouseState(&mouseX, &mouseY);
                    std::cout << "Mouse move => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
                }
            }

            //Keyboard event
            if (currentEvent.type == SDL_KEYDOWN) {
                mat3 trans1, rot1, transInv;
                float posXMiddle = (P1.x + P2.x + P3.x + P4.x) / 4;
                float posYMiddle = (P1.y + P2.y + P3.y + P4.y) / 4;
                float posXMin = std::min(std::min(P1.x, P2.x), std::min(P3.x, P4.x));
                float posYMin = std::min(std::min(P1.y, P2.y), std::min(P3.y, P4.y));
                float minSum = std::min(std::min(P1.x + P1.y, P2.x + P2.y), std::min(P3.x + P3.y, P4.x + P4.y));
                vec2 topLeft;

                if (minSum == P1.x + P1.y) {
                    topLeft.x = P1.x;
                    topLeft.y = P1.y;
                } else if (minSum == P2.x + P2.y) {
                    topLeft.x = P2.x;
                    topLeft.y = P2.y;
                } else if (minSum == P3.x + P3.y) {
                    topLeft.x = P3.x;
                    topLeft.y = P3.y;
                } else if (minSum == P4.x + P4.y) {
                    topLeft.x = P4.x;
                    topLeft.y = P4.y;
                }

                switch (currentEvent.key.keysym.sym) {
                    case SDLK_RIGHT:
                        trans1 = translate(posXMiddle, posYMiddle);
                        rot1 = rotate(10);
                        transInv = translate(-posXMiddle, -posYMiddle);
                        trans1 = trans1 * rot1 * transInv;
                        P1 = trans1 * P1;
                        P2 = trans1 * P2;
                        P3 = trans1 * P3;
                        P4 = trans1 * P4;
                        break;
                    case SDLK_LEFT:
                        trans1 = translate(posXMiddle, posYMiddle);
                        rot1 = rotate(-10);
                        transInv = translate(-posXMiddle, -posYMiddle);
                        trans1 = trans1 * rot1 * transInv;
                        P1 = trans1 * P1;
                        P2 = trans1 * P2;
                        P3 = trans1 * P3;
                        P4 = trans1 * P4;
                        break;

                    case SDLK_UP:
                        trans1 = translate(topLeft.x, topLeft.y);
                        rot1 = scale(1.1, 1.1);
                        transInv = translate(-topLeft.x, -topLeft.y);
                        trans1 = trans1 * rot1 * transInv;
                        P1 = trans1 * P1;
                        P2 = trans1 * P2;
                        P3 = trans1 * P3;
                        P4 = trans1 * P4;
                        break;

                    case SDLK_DOWN:
                        trans1 = translate(topLeft.x, topLeft.y);
                        rot1 = scale(0.9, 0.9);
                        transInv = translate(-topLeft.x, -topLeft.y);
                        trans1 = trans1 * rot1 * transInv;
                        P1 = trans1 * P1;
                        P2 = trans1 * P2;
                        P3 = trans1 * P3;
                        P4 = trans1 * P4;
                        break;
//
//                    default:
//                        break;
                }
            }

            SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
            SDL_RenderClear(windowRenderer);


            SDL_SetRenderDrawColor(windowRenderer, 0, 0, 255, 255);
            SDL_RenderDrawLine(windowRenderer, P1.x, P1.y, P2.x, P2.y);
            SDL_RenderDrawLine(windowRenderer, P1.x, P1.y, P3.x, P3.y);
            SDL_RenderDrawLine(windowRenderer, P2.x, P2.y, P4.x, P4.y);
            SDL_RenderDrawLine(windowRenderer, P3.x, P3.y, P4.x, P4.y);

            SDL_RenderPresent(windowRenderer);
        }
    }

    destroyWindow();
    return 0;
}
