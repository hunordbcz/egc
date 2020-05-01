//
//  All the auxiliary code (window creation/destruction, validation method and others) has been moved to "auxiliary.h" to keep the main file as simple as possible
//
//  Copyright ? 2016 CGIS. All rights reserved.
//

#include <iostream>
#include "SDL2/SDL.h"

#include "auxiliary.h"


bool quit = false;

int mouseX, mouseY;
float rotationAngle = 0.0f;


ecg::mat4 viewTransformMatrix;
ecg::mat4 cameraMatrix;
ecg::mat4 perspectiveMatrix;
ecg::mat4 modelMatrix;
ecg::Camera myCamera;

ecg::vec2 viewportTopLeftCorner(30, 30);
ecg::vec2 viewportDimensions(400, 400);

bool backFaceCulling = true;
bool displayNormals = false;

void handleMouseEvents() {
    //Mouse event -> pressed button
    if (currentEvent.type == SDL_MOUSEBUTTONDOWN) {
        if (currentEvent.button.button == SDL_BUTTON_LEFT) {
            SDL_GetMouseState(&mouseX, &mouseY);

        }

        if (currentEvent.button.button == SDL_BUTTON_RIGHT) {
            SDL_GetMouseState(&mouseX, &mouseY);

        }
    }
    /*
    //Mouse event -> mouse movement
    if (currentEvent.type == SDL_MOUSEMOTION)
    {
    if (currentEvent.button.button & SDL_BUTTON_LMASK)
    {
    SDL_GetMouseState(&mouseX, &mouseY);
    }

    if (currentEvent.button.button & SDL_BUTTON_RMASK)
    {
    SDL_GetMouseState(&mouseX, &mouseY);

    }
    }
    */
}

void handleKeyboardEvents() {
    //Keyboard event
    if (currentEvent.type == SDL_KEYDOWN) {
        switch (currentEvent.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;

            case SDLK_a:
                rotationAngle += 10.0f;
                break;

            case SDLK_d:
                rotationAngle -= 10.0f;
                break;

            case SDLK_w:
                cameraZ -= 0.5f;
                break;

            case SDLK_s:
                cameraZ += 0.5f;
                break;

            case SDLK_z:
                backFaceCulling = true;
                break;

            case SDLK_c:
                backFaceCulling = false;
                break;

            case SDLK_q:
                displayNormals = true;
                break;

            case SDLK_e:
                displayNormals = false;
                break;

            default:
                break;
        }
    }
}

//draw a triangle line by line
void drawWireframeTriangle(SDL_Renderer *renderer, const std::vector<ecg::vec4> &triangle) {
    for (int i = 0; i < 3; i++)
        SDL_RenderDrawLine(renderer, triangle.at((i + 1) % 3).x, triangle.at((i + 1) % 3).y, triangle.at(i).x,
                           triangle.at(i).y);
}

void renderMesh(SDL_Renderer *renderer, std::vector<tinyobj::shape_t> shapes) {
    ecg::vec3 normalVector;
    ecg::vec4 triangleCenter;

    int vertexId = -1;

    //for each mesh in the 3d model representation
    for (size_t i = 0; i < shapes.size(); i++) {
        //for each triangle
        std::vector<ecg::vec4> triangle;
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            //update the triangle with vertices coordinates
            for (int k = 0; k < 3; k++) {
                vertexId = shapes[i].mesh.indices[3 * f + k];
                triangle.push_back(ecg::vec4(shapes[i].mesh.positions[3 * vertexId + 0],
                                             shapes[i].mesh.positions[3 * vertexId + 1],
                                             shapes[i].mesh.positions[3 * vertexId + 2], 1));
            }

            //compute the coordinates in view (camera) space
            for (int k = 0; k < 3; k++)
                triangle.at(k) = cameraMatrix * modelMatrix * triangle.at(k);

            //compute the normal vector and triangle center
            normalVector = ecg::findNormalVectorToTriangle(triangle);
            triangleCenter = findCenterPointOfTriangle(triangle);

            //apply back-face culling
            if (!isTriangleVisible(triangle, normalVector) && backFaceCulling) {
                //clear the triangle
                triangle.clear();
                continue;
            }

            //apply the perspective matrix
            for (int k = 0; k < 3; k++)
                triangle.at(k) = perspectiveMatrix * triangle.at(k);

            if (clipTriangleInHomegeneousCoordinates(triangle)) {
                //clear the triangle
                triangle.clear();
                continue;
            }

            //apply the perspective divide and the viewport transformation matrix
            for (int k = 0; k < 3; k++) {
                ecg::perspectiveDivide(triangle.at(k));

                triangle.at(k) = viewTransformMatrix * triangle.at(k);
            }

            //draw the triangle
            SDL_SetRenderDrawColor(renderer, 96, 96, 96, 0);
            drawWireframeTriangle(renderer, triangle);

            //display the normal vectors
            if (displayNormals)
                displayNormalVectors(normalVector, triangleCenter, renderer, viewTransformMatrix, perspectiveMatrix);

            //clear the triangle
            triangle.clear();
        }
    }
}


int main(int argc, char *argv[]) {
    if (!initWindow()) {
        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    myCamera = ecg::Camera(ecg::vec3(-0.3f, 1.5f, cameraZ), ecg::vec3(-0.3f, 1.5f, -10.0f),
                           ecg::vec3(0.0f, 1.0f, 0.0f));
    viewTransformMatrix = ecg::defineViewTransformMatrix(viewportTopLeftCorner.x, viewportTopLeftCorner.y,
                                                         viewportDimensions.x, viewportDimensions.y);
    cameraMatrix = ecg::defineCameraMatrix(myCamera);
    perspectiveMatrix = ecg::definePerspectiveProjectionMatrix(45.0f, 1.0, -0.1f, -10.0f);

    validateViewingTransformations();

    std::vector<tinyobj::shape_t> shapes = readOBJ("bunny.obj");

    SDL_Rect viewportRectangle = {static_cast<int>(viewportTopLeftCorner.x), static_cast<int>(viewportTopLeftCorner.y),
                                  static_cast<int>(viewportDimensions.x), static_cast<int>(viewportDimensions.y)};

    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&currentEvent) != 0) {
            //User requests quit
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
            }

            handleMouseEvents();
            handleKeyboardEvents();

            //Clear screen
            SDL_SetRenderDrawColor(renderer, 224, 224, 224, 0);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            SDL_RenderDrawRect(renderer, &viewportRectangle);

            modelMatrix = ecg::rotateY(rotationAngle) * ecg::scale(15.0f, 15.0f, 15.0f);
            myCamera.cameraPosition.z = cameraZ;
            cameraMatrix = ecg::defineCameraMatrix(myCamera);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            renderMesh(renderer, shapes);

            //Update screen
            SDL_RenderPresent(renderer);
        }
    }

    destroyWindow();
    return 0;
}

