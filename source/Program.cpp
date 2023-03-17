#include "Program.hpp"

Program::Program()
{
    WindowInit();
    Shader ftshader("./shader/shader.vs", "./shader/shader.fs");
    Model fthumangl("./model/model.dae");
    Model _ak47("./model/model_ak47.dae");
    ak47 = &_ak47;
    shader = &ftshader;
    humangl = &fthumangl;
    srand((unsigned int)time(NULL));


    planes.push_back(plane({
    { 0.0f, -10.0f, 0.0f }, // position
    { 60.0f, 1.0f, 40.0f }, // dimensions
    { 0.46f, 0.21f, 0.0f }, // color
    { 0.0f, 0.0f, 1.0f }, RADIAN(180.0f) })); // rotationAxis

    T = 0.0f;
    objects.push_back(object({
        { -17.6f, -5.25f, 0.0f }, // position
        2.0f, // dimensions
        { 0.5f, 0.5f, 0.5f }, // color
        { 10.5f, 13.5f, 0.0f },// direction
        1.0f }));

    int isOn = 1;
    float space = 0.002f;
    for (int i = 0; i < 50 * isOn; i++) {
        objects.push_back(object({
        { (float)(i % 5) * (1.0f + space) - 2.5f + 15.0f,
        (int)(i * 0.2f) * (1.0f + space) - 9.5f + space,
        0.0f - 2.5f},
        1.0f,
        { 0.5f, 0.5f, 0.5f },
        { 0.0f, 0.0f, 0.0f }, 0.0f,
        {0,0,0}, }));

        objects.push_back(object({
        { (float)(i % 5) * (1.0f + space) - 2.5f + 15.0f,
        (int)(i * 0.2f) * (1.0f + space) - 9.5f + space,
        1.5f + space * 6 },
        1.0f,
        { 0.5f, 0.5f, 0.5f },
        { 0.0f, 0.0f, 0.0f }, 0.0f,
        {0,0,0}, }));
    }
    for (int i = 0; i < 30 * isOn; i++) {
        objects.push_back(object({
        { -2.5f + 15.0f,
        (int)(i * 0.33334f) * (1.0f + space) - 9.5f + space,
        (float)(i % 3) * (1.0f + space) + (1.0f + space) - 2.5f},
        1.0f,
        { 0.5f, 0.5f, 0.5f },
        { 0.0f, 0.0f, 0.0f }, 0.0f,
        {0,0,0}, }));

        objects.push_back(object({
        { 1.5f + space * 5 + 15.0f,
        (int)(i * 0.33334f) * (1.0f + space) - 9.5f + space,
        (float)(i % 3) * (1.0f + space) + (1.0f + space) - 2.5f},
        1.0f,
        { 0.5f, 0.5f, 0.5f },
        { 0.0f, 0.0f, 0.0f }, 0.0f,
        {0,0,0}, }));
    }

    shader->setMat4("projection", Perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 1000.0f));
    shader->setVec3("lightColor", { 1.0f, 1.0f, 1.0f });
    shader->setVec3("lightPos", { 0.0f, 25.0f, -25.0f });
    FrameRate();
    while (isRunning) {
        FrameRate();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
        shader->setMat4("view", LookAt(cameraPos, (cameraPos + cameraFront), cameraUp));
        shader->setVec3("viewPos", cameraPos);

        if (rot > 0.0f) {
            if (timeSpeed < 1.0f) timeDiff *= timeSpeed;
            for (float i = 0; i < timeSpeed; i++) {
                UpdatePositions();
                CheckCollisions();
            }
            if (timeSpeed < 1.0f) timeDiff /= timeSpeed;
        }
        DrawMeshs();
        Event();
        GuiSetup();
        SDL_GL_SwapWindow(window);
    }
        
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
        
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Program::GuiSetup()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("GUI");

        ImGui::Text("Setup Plane");
        ImGui::InputFloat3("P-position", Pposition);
        ImGui::InputFloat3("P-dimension", Pdimension);
        ImGui::SliderFloat3("P-color", Pcolor, 0, 1);
        ImGui::InputFloat3("P-rotation", Protation);
        ImGui::SliderFloat("P-degree", &Pdegree, -180.0f, 180.0f);
        ImGui::SliderFloat("P-masse", &Pmasse, 0.0f, 5.0f);

        if (ImGui::Button("Create Plane")) {
            planes.push_back(plane({
                { Pposition[0], Pposition[1], Pposition[2] }, // position
                { Pdimension[0], Pdimension[1], Pdimension[2] }, // dimensions
                { Pcolor[0], Pcolor[1], Pcolor[2] }, // color
                { Protation[0], Protation[1], Protation[2] }, // rotation
                RADIAN(Pdegree) })); // degree
        }
        if (ImGui::Button("Delete last Plane")) {
            if (planes.size() > 0) 
                planes.pop_back();
        }

        ImGui::Text(" ");
        ImGui::Text("Setup Sphere");
        ImGui::InputFloat3("S-position", Sposition);
        ImGui::SliderFloat("S-dimension", &Sdimension, 0.5f, 3.0f);
        ImGui::SliderFloat3("S-color", Scolor, 0, 1);
        ImGui::SliderFloat3("S-direction", Sdirection, -100.0f, 100.0f);
        ImGui::SliderFloat("S-masse", &Smasse, 0.1f, 10.0f);

        if (ImGui::Button("Create Sphere")) {
            objects.push_back(object({
                { Sposition[0], Sposition[1], Sposition[2] }, // position
                Sdimension, // dimensions
                { Scolor[0], Scolor[1], Scolor[2] }, // color
                { Sdirection[0], Sdirection[1], Sdirection[2]},
                Smasse })); // direction
        }
        if (ImGui::Button("Delete last Sphere")) {
            for (auto it = objects.rbegin(); it != objects.rend(); it++) {
                if (it->type == SPHERE)
                    objects.erase(--(it.base()));
            }
        }

        ImGui::Text(" ");
        ImGui::Text("Setup Cube");
        ImGui::InputFloat3("C-position", Cposition);
        ImGui::SliderFloat("C-dimension", &Cdimension, 0.5f, 3.0f);
        ImGui::SliderFloat3("C-color", Ccolor, 0, 1);
        ImGui::SliderFloat3("C-rotation", Crotation, 0, 1);
        ImGui::SliderFloat("C-degree", &Cdegree, 0.0f, 1.0f);
        ImGui::SliderFloat3("C-direction", Cdirection, -100.0f, 100.0f);
        ImGui::SliderFloat("C-masse", &Cmasse, 0.1f, 10.0f);

        if (ImGui::Button("Create Cube")) {
            objects.push_back(object({
                { Cposition[0], Cposition[1], Cposition[2] }, // position
                { Cdimension }, // dimensions
                { Ccolor[0], Ccolor[1], Ccolor[2] }, // color
                { Crotation[0], Crotation[1], Crotation[2] }, // rotation
                Cdegree,
                { Cdirection[0], Cdirection[1], Cdirection[2] },
                Cmasse })); // degree
        }
        if (ImGui::Button("Delete last Cube")) {
            for (auto it = objects.rbegin(); it != objects.rend(); it++) {
                if (it->type == CUBE)
                    objects.erase(--(it.base()));
            }
        }

        ImGui::Text(" ");
        ImGui::SliderFloat("acceleration", &rot, 0.0f, 5.0f);
        ImGui::InputFloat3("gravite", G);
        GRAVITY_FORCE.x = G[0], GRAVITY_FORCE.y = G[1], GRAVITY_FORCE.z = G[2];
        if (showGun) {
            if (ImGui::Button("Disable Gun")) {
                showGun = false;
            }
            ImGui::InputFloat3("GunPos", gP);
            gunPos.x = gP[0], gunPos.y = gP[1], gunPos.z = gP[2];
        }
        else {
            if (ImGui::Button("Enable Gun")) {
                showGun = true;
            }
        }

        ImGui::Text("(%.1f FPS)", 1.0f / timeDiff);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Program::UpdatePositions()
{
    for (object& mesh : objects) {
        if (mesh.positionForce * mesh.positionForce == 0.0f) {
            mesh.timerPrev = 0.0f;
            mesh.timer = timeDiff;
        }
        else {
            mesh.timerPrev = mesh.timer;
            mesh.timer += timeDiff;
        }
        mesh.positionPrev = mesh.position;
        mesh.UpdatePositionByTimer();
    }
}

void Program::CheckCollisions()
{
    /* Récupère les tailles des vectors */
    auto planesSize = planes.size();
    auto objectsSize = objects.size();

    std::vector<collision> collisions;
    auto c = 0;
    /* Créé les groupes de collisions */
    for (auto i = 0; i < objectsSize; i++) {
        if (objects[i].position.y < -100.0f) continue;
        objects[i].isCollided = false;
        collisions.push_back(::collision());
        collisions[c].objectsCollided.push_back(&objects[i]);
        /* Plans */
        for (auto j = 0; j < planesSize; j++) {
            float distance = fabs((objects[i].position - planes[j].position) * planes[j].normal) / sqrt(planes[j].normal * planes[j].normal);
            float distanceMinimum = objects[i].radius;
            if (distance <= distanceMinimum) {
                if ((objects[i].type == CUBE &&
                    SeparatingCubePlane(objects[i], planes[j]) == true) ||
                    (objects[i].type == SPHERE &&
                        SeparatingSpherePlane(objects[i], planes[j]) == true))
                {
                    collisions[c].planesCollided.push_back(&planes[j]);
                    objects[i].isCollided = true;
                }
            }
        }
        /* Objets */
        for (auto j = i + 1; j < objectsSize; j++) {
            vec3 vector = (objects[i].position - objects[j].position);
            float distance = sqrt(vector * vector);
            float distanceMinimum = objects[i].radius + objects[j].radius;
            if (distance <= distanceMinimum) {
                if ((objects[i].type + objects[j].type == CUBE + CUBE &&
                    SeparatingCubeCube(objects[i], objects[j]) == true) ||
                    (objects[i].type == CUBE && objects[j].type == SPHERE &&
                        SeparatingCubeSphere(objects[i], objects[j]) == true) ||
                    (objects[i].type == SPHERE && objects[j].type == CUBE &&
                        SeparatingCubeSphere(objects[j], objects[i]) == true) ||
                    (objects[i].type + objects[j].type == SPHERE + SPHERE &&
                        SeparatingSphereSphere(objects[i], objects[j]) == true))
                {
                    collisions[c].objectsCollided.push_back(&objects[j]);
                    objects[i].isCollided = true;
                    objects[j].isCollided = true;
                }
            }
        }
        collisions[c].GetVectorSizes();
        if (collisions[c].objectsCollidedSize + collisions[c].planesCollidedSize == 1) {
            collisions.pop_back();
        }
        else c++;
    }

    /* Calcules des groupes de collisions */
    for (auto collision : collisions) {
        collision.Caculate1();
    }
    for (auto collision : collisions) {
        collision.Caculate2();
    }
}


void Program::DrawMeshs() {


    mat4 model;
    model.Translate({ -15.05f, -5.95f, 0.0f });
    if (rot && T < 180.0f) T += 15 * timeSpeed;
    model.RotateArbitraryAxis({ 0, 0, 1 }, RADIAN(T + 45.0f));
    model.RotateArbitraryAxis({ 0, 1, 0 }, RADIAN(90.0f));
    model.Scaling({ 1.0f, 1.0f, 1.0f });
    shader->setMat4("model", model);

    shader->setVec3("objectColor", { 0.5f, 0.5f, 0.5f });
    humangl->meshes[3].Draw(*shader);

    model = mat4{};
    model.Translate({ -15.0f, -10.0f, 0.0f });
    model.RotateArbitraryAxis({ 0, 1, 0 }, RADIAN(90.0f));
    model.Scaling({ 1.0f, 1.0f, 1.0f });
    shader->setMat4("model", model);

    shader->setVec3("objectColor", { 0.5f, 0.5f, 0.5f });
    humangl->meshes[4].Draw(*shader);

    glDisable(GL_CULL_FACE);
    for (plane& mesh : planes) {
        mat4 model;
        model.Translate(mesh.position);
        if (mesh.degree != 0.0f)
            model.RotateArbitraryAxis(mesh.rotationAxis, -mesh.degree);
        model.Scaling(mesh.dimensions);
        shader->setMat4("model", model);

        //if (mesh.isCollided == true)
        //    shader->setVec3("objectColor", { 0.0f, 1.0f, 0.0f });
        //else
        shader->setVec3("objectColor", mesh.color);

        humangl->meshes[0].Draw(*shader);
    }
    glEnable(GL_CULL_FACE);

    for (object& mesh : objects) {
        mat4 model;
        model.Translate(mesh.position);
        if (mesh.type == CUBE) {
            if (mesh.degree != 0.0f)
                model.RotateArbitraryAxis(mesh.rotationAxis, -mesh.degree);
            if (mesh.degreeOrigin != 0.0f)
                model.RotateArbitraryAxis(mesh.rotationAxisOrigin, -mesh.degreeOrigin);
        }
        model.Scaling(vec3{ mesh.size, mesh.size, mesh.size });
        shader->setMat4("model", model);

        //if (mesh.isCollided == true)
        //    shader->setVec3("objectColor", { 0.0f, 1.0f, 0.0f });
        //else
        shader->setVec3("objectColor", mesh.color);

        humangl->meshes[mesh.type].Draw(*shader);
    }

    if (showGun)
    {
        mat4 model;

        model.Translate(cameraPos + gunPos);
        model.RotateArbitraryAxis({ 1.0f,0.0f,0.0f }, RADIAN(90.0f));
        model.RotateArbitraryAxis({ 0.0f,0.0f,1.0f }, RADIAN(180.0f));
        model.RotateArbitraryAxis({ 0.0f, 0.0f, 0.0f }, 0);
        float dimension = 0.75f;
        model.Scaling({ dimension, dimension, dimension });

        shader->setMat4("model", model);
        shader->setVec3("objectColor", { 0.0f, 0.2f, 1.0f });

        shader->setMat4("view", LookAt(cameraPos, (cameraPos + vec3{0.0f, 0.0f, 1.0f}), cameraUp));

        for (int i = 0; i < 5; i++)
            ak47->meshes[i].Draw(*shader);
    }
}

void Program::WindowInit()
{
    // init
    if (SDL_Init(SDL_INIT_EVERYTHING) != NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    // set attribs
    if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) < NULL) // 32 not working with glew
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    // create window
    if ((window = SDL_CreateWindow("An SDL2 window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight,
        SDL_WINDOW_OPENGL)) == NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    // create context
    if ((context = SDL_GL_CreateContext(window)) == NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());

    glewExperimental = GL_TRUE;
    if (GLenum err = glewInit() != GLEW_OK || !GLEW_VERSION_2_1)
        throw std::runtime_error("[ERROR] " + (std::string)((char*)glewGetErrorString(err)));
    // others
    glEnable(GL_DEPTH_TEST); if (glGetError() != GL_NO_ERROR)
        throw std::runtime_error("[ERROR] " + glGetError());
    glEnable(GL_CULL_FACE); if (glGetError() != GL_NO_ERROR)
        throw std::runtime_error("[ERROR] " + glGetError());
    glCullFace(GL_FRONT); if (glGetError() != GL_NO_ERROR)
        throw std::runtime_error("[ERROR] " + glGetError());

    if (SDL_SetRelativeMouseMode(SDL_TRUE) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
    if (SDL_GL_SetSwapInterval(SDL_TRUE) < NULL)
        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());

    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_SetCursor(cursor);
    SDL_ShowCursor(true);

    std::cout << std::endl;
    std::cout << "GL_RENDERER " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GL_VERSION " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW_VERSION " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << std::endl;

    SDL_WarpMouseInWindow(window, (int)(0.5f * windowWidth), (int)(0.5f * windowHeight));

    // imGUI //

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void Program::Event()
{
    static SDL_Event event;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            isRunning = false;
            break;
        }
        else if (event.type == SDL_MOUSEMOTION && !io.WantCaptureMouse) {
            Mouse();
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                if (rot == 0.0f)
                    rot = 1.0f;
                else rot = 0.0f;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_1) {
                objects.push_back(object({
                    cameraPos + cameraFront, // position
                    (float)(rand() % 5 + 1), // dimensions
                    { 1.0f, 0.5f, 0.5f }, // color
                    cameraFront * 50.0f,
                    1.0f })),// direction

                    cout(objects.size(), "num spheres");
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_2) {
                objects.push_back(object({
                    cameraPos + cameraFront, // position
                    (float)(rand() % 3 + 1), // dimensions
                    { 0.5f, 0.5f, 1.0f }, // color
                    {(float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f }, (float)(rand() % 360), // rotationAxis & degree
                    cameraFront * 50.0f,
                    1.0f })),// direction

                    cout(objects.size(), "num objects");
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_I) {
                if (io.WantCaptureMouse == true) {
                    io.WantCaptureMouse = false;
                    if (SDL_SetRelativeMouseMode(SDL_TRUE) < NULL)
                        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
                }
                else {
                    io.WantCaptureMouse = true;
                    if (SDL_SetRelativeMouseMode(SDL_FALSE) < NULL)
                        throw std::runtime_error("[ERROR] " + (std::string)SDL_GetError());
                }
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_O) {
                if (polyMod) {
                    polyMod = false;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                else {
                    polyMod = true;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_R) {
                objects.clear();
                std::vector<object>().swap(objects);
                objects.clear();
                T = 0.0f;
                rot = 0;
                objects.push_back(object({
                    { -17.6f, -5.25f, 0.0f }, // position
                    2.0f, // dimensions
                    { 0.5f, 0.5f, 0.5f }, // color
                    { 10.5f, 13.5f, 0.0f },// direction
                    1.0f }));

                int isOn = 1;
                float space = 0.002f;
                for (int i = 0; i < 50 * isOn; i++) {
                    objects.push_back(object({
                    { (float)(i % 5) * (1.0f + space) - 2.5f + 15.0f,
                    (int)(i * 0.2f) * (1.0f + space) - 9.5f + space,
                    0.0f - 2.5f},
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));

                    objects.push_back(object({
                    { (float)(i % 5) * (1.0f + space) - 2.5f + 15.0f,
                    (int)(i * 0.2f) * (1.0f + space) - 9.5f + space,
                    1.5f + space * 6 },
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));
                }
                for (int i = 0; i < 30 * isOn; i++) {
                    objects.push_back(object({
                    { -2.5f + 15.0f,
                    (int)(i * 0.33334f) * (1.0f + space) - 9.5f + space,
                    (float)(i % 3) * (1.0f + space) + (1.0f + space) - 2.5f},
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));

                    objects.push_back(object({
                    { 1.5f + space * 5 + 15.0f,
                    (int)(i * 0.33334f) * (1.0f + space) - 9.5f + space,
                    (float)(i % 3) * (1.0f + space) + (1.0f + space) - 2.5f},
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));
                }
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_T) {
                objects.clear();
                std::vector<object>().swap(objects);
                objects.clear();

                int isOn = 1;
                float space = 0.002f;
                for (int i = 0; i < 50 * isOn; i++) {
                    objects.push_back(object({
                    { (float)(i % 5) * (1.0f + space) - 2.5f + 15.0f,
                    (int)(i * 0.2f) * (1.0f + space) - 9.5f + space,
                    0.0f - 2.5f},
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));

                    objects.push_back(object({
                    { (float)(i % 5) * (1.0f + space) - 2.5f + 15.0f,
                    (int)(i * 0.2f) * (1.0f + space) - 9.5f + space,
                    1.5f + space * 6 },
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));
                }
                for (int i = 0; i < 30 * isOn; i++) {
                    objects.push_back(object({
                    { -2.5f + 15.0f,
                    (int)(i * 0.33334f) * (1.0f + space) - 9.5f + space,
                    (float)(i % 3) * (1.0f + space) + (1.0f + space) - 2.5f},
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));

                    objects.push_back(object({
                    { 1.5f + space * 5 + 15.0f,
                    (int)(i * 0.33334f) * (1.0f + space) - 9.5f + space,
                    (float)(i % 3) * (1.0f + space) + (1.0f + space) - 2.5f},
                    1.0f,
                    { 0.5f, 0.5f, 0.5f },
                    { 0.0f, 0.0f, 0.0f }, 0.0f,
                    {0,0,0}, }));
                }
            }
        }
    }
    if (keystates[SDL_SCANCODE_ESCAPE]) isRunning = false;
    float cameraSpeed = timeDiff * 10.0f;
    if (keystates[static_cast<unsigned int>(SDL_GetScancodeFromKey('z'))])
        cameraPos = (cameraPos + (cameraFront * cameraSpeed));
    if (keystates[SDL_SCANCODE_S])
        cameraPos = (cameraPos - (cameraFront * cameraSpeed));

    if (keystates[static_cast<unsigned int>(SDL_GetScancodeFromKey('q'))])
        cameraPos = (cameraPos + (((cameraFront ^ cameraUp)).Normalize()) * cameraSpeed);
    if (keystates[SDL_SCANCODE_D])
        cameraPos = (cameraPos - (((cameraFront ^ cameraUp)).Normalize()) * cameraSpeed);

    if (keystates[static_cast<unsigned int>(SDL_GetScancodeFromKey('a'))])
        cameraPos = (cameraPos + (cameraUp * cameraSpeed));
    if (keystates[SDL_SCANCODE_E])
        cameraPos = (cameraPos - (cameraUp * cameraSpeed));

}

void Program::Mouse()
{
    static int prevx = 0, prevy = 0;
    int x, y; SDL_GetRelativeMouseState(&x, &y);
    if (x != 0 || y != 0)
    {
        static float yaw = 90.0f, pitch = 0.0f, sensitivity = -0.1f;
        static float xoffset = -(x + prevx), yoffset = -(y + prevy);

        yaw += (xoffset = (xoffset + x + prevx) * sensitivity);
        pitch += (yoffset = (yoffset + y + prevy) * sensitivity);
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        vec3 direction;
        direction.x = cos(RADIAN(yaw)) * cos(RADIAN(pitch));
        direction.y = sin(RADIAN(pitch));
        direction.z = sin(RADIAN(yaw)) * cos(RADIAN(pitch));
        cameraFront = direction.Normalize();

        SDL_WarpMouseInWindow(window, (int)(0.5f * windowWidth), (int)(0.5f * windowHeight));
    }
    prevx = x, prevy = y;
}

void Program::FrameRate()
{
    static float prevTime = 0.0f;
    static float crntTime = 0.0f;
    static float timePrev = 0.0f;
    timeDiff = 0.0f;
    while (timeDiff < 0.016f) {
        crntTime = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        timeDiff = (crntTime - prevTime) * 1.0f + timePrev;
    }
    prevTime = crntTime;
    SDL_SetWindowTitle(window, ("[" + std::to_string(1.0f / timeDiff) + "]").c_str());

    float round = (int)(timeDiff * 1000.0f) * 0.001f;
    timePrev = timeDiff - round;
    timeDiff = round;
    if (timeDiff > 0.016f / timeSpeed) {
        round += timeDiff - 0.016f;
        timeDiff = 0.016f;
    }

}
