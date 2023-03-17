#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#include <GL/glew.h> 
#include <SDL_opengl.h>
#include <SDL.h>
#include <SDL_image.h>

#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_sdl2.h"
#include "imGUI/imgui_impl_opengl3.h"

#include "Math.hpp"
#include "plane.hpp"
#include "Object.hpp"
#include "Collision.hpp"

#include "Shader.hpp"
#include "Model.hpp"

class Program
{
public:
    Program();
    ~Program() {};
private:
    SDL_Window* window = nullptr;
    const int windowWidth = 1280;
    const int windowHeight = 720;
    Shader* shader;
    Model* humangl;
    Model* ak47;
    ImGuiIO io;
    SDL_GLContext context;
    vec3 gunPos = { 2.0f, -1.3f, 4.0f };
    SDL_Cursor* cursor;
    unsigned int program = 0;
    bool isRunning = true;
    bool polyMod = false;
    bool showGun = true;

    float timeDiff = 0.0f;
    float timeSpeed = 2.0f;
    vec3 cameraPos = { 0.0f, 0.0f, -50.0f };
    vec3 cameraFront = { 0.0f, 0.0f, 1.0f };
    vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

    float gP[3] = { gunPos.x, gunPos.y, gunPos.z };
    float G[3] = { GRAVITY_FORCE.x, GRAVITY_FORCE.y, GRAVITY_FORCE.z };

    float Sposition[3] = { 0 };
    float Sdimension = 1.0f;
    float Scolor[3] = { 0 };
    float Sdirection[3] = { 0 };
    float Smasse = 1.0f;

    float Pposition[3] = { 0 };
    float Pdimension[3] = { 0 };
    float Pcolor[3] = { 0 };
    float Protation[3] = { 0 };
    float Pdegree = 0.0f;
    float Pmasse = 1.0f;

    float Cposition[3] = { 0 };
    float Cdimension = 1.0f;
    float Ccolor[3] = { 0 };
    float Crotation[3] = { 0 };
    float Cdegree = 0.0f;
    float Cdirection[3] = { 0 };
    float Cmasse = 1.0f;

    std::vector<plane> planes;
    std::vector<object> objects;

    float T = 0.0f;
    float rot = 1.0f;

    void WindowInit();
    void Event();
    void Mouse();
    void FrameRate();
    void UpdatePositions();
    void CheckCollisions();
    void DrawMeshs();
    void GuiSetup();

};

#endif