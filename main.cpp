//
//  All the auxiliary code (window creation/destruction, validation method and others) has been moved to "auxiliary.h" to keep the main file as simple as possible
//
//  Copyright @ 2020 CGIS. All rights reserved.


// TO DO ===> STEP 0 ==== REPLACE ALL OF THE IMPLEMENTED SOURCES WITH YOUR SOURCES: 
// vec2, vec3, vec4, mat3, mat4, transform, projection
// TO DO - GO TO zbuffer.cpp - implement the "drawTriangleInZBuffer()" function
// Look at the renderMesh function, to understand how the drawing is done
//

#include <iostream>
#include "SDL2/SDL.h"

#include "auxiliary.h"
#include "zbuffer.h"


ecg::mat4 viewTransformMatrix;
ecg::mat4 cameraMatrix;
ecg::mat4 perspectiveMatrix;
ecg::mat4 modelMatrix;
ecg::Camera myCamera;

ecg::vec2 viewportTopLeftCorner(30, 30);
ecg::vec2 viewportDimensions(400, 400);

//THE DEPTH BUFFER ===> EQUAL TO THE SIZE OF THE WINDOW
float depthBuffer[WINDOW_HEIGHT][WINDOW_WIDTH];

float zmin = -2;
float zmax = 2;

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
            //Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
            if (renderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    return success;
}

void renderMesh(SDL_Renderer *renderer, std::vector<tinyobj::shape_t> shapes) {
    ecg::vec3 normalVector;
    ecg::vec4 triangleCenter;

    int vertexId = -1;

    //initialize depth buffer ===> eqiuvalent to filling it with minus infinity
    for (int i = 0; i < WINDOW_HEIGHT; i++)
        for (int j = 0; j < WINDOW_WIDTH; j++)
            depthBuffer[i][j] = -1000;

    zmin = -1000;
    zmax = 1000;

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
            normalVector = findNormalVectorToTriangle(triangle);
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

            //Draw into the Z-buffer
            //These for loops iterate all the vertices of the rabbit, and "draw" the triangles in terms of Z values
            //Instead of drawing to the screen, the function below fills the Zbuffer with the Z value of each "pixel" (internal point of the triangle)
            drawTriangleInZBuffer(triangle, depthBuffer, zmin, zmax);

            //display the normal vectors
            if (displayNormals)
                displayNormalVectors(normalVector, triangleCenter, renderer, viewTransformMatrix, perspectiveMatrix);

            //clear the triangle
            triangle.clear();
        }
    }

    //================================ DRAWING CODE ================================================//
    float depthCoef;
    //Draw the contents of the Zbuffer (these were computed above)
    //Here, we draw to the screen. FIRST, LOOK IN THE DEPTH BUFFER.
    //If the value is not too small (background), paint the pixel. The color is a grey, proportional 
    //to the current Z value over the difference (zmax-zmin) 
    for (int i = 0; i < WINDOW_HEIGHT; i++)
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            if (depthBuffer[i][j] == -1000)
                continue;

            depthCoef = 255 * (1 - (depthBuffer[i][j] - zmin) / (zmax - zmin));
            SDL_SetRenderDrawColor(renderer, depthCoef, depthCoef, depthCoef, 0);

            SDL_RenderDrawPoint(renderer, i, j);
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

