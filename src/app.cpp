#include "app.h"



Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
float delta_time = 0.0f;
float last_frame = 0.0f;
bool first_mouse = true;
float last_x = 0.0f;
float last_y = 0.0f;



App::App(int _screen_width, int _screen_height)
    : screen_width(_screen_width), screen_height(_screen_height)
{
}



bool App::create()
{
    gl_config();
    load_shaders();
    load_models();

    return true;
}



void App::run()
{
    while (!glfwWindowShouldClose(window))
    {
        update();
        render();
    }

    glfwTerminate();
}



void App::update()
{
    float current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    //ship->rot_angles.z = 100 * glfwGetTime();
    ship->update();

    process_input(window, ship, move_speed, background);
}



void App::render()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /*******************************************/
    /*                                         */
    /*        Model / View / Projection        */
    /*                                         */
    /*******************************************/
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 1000.0f);
    shader.set_mat4("projection", projection);
    shader.set_mat4("view", view);

    shader.set_vec3("view_pos", camera.position);


    /****************************/
    /*        Draw Calls        */
    /****************************/
    background->draw(shader);
    ship->draw(shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
}



bool App::gl_config()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screen_width, screen_height, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "\nERROR: Failed to initialize GLEW\n";
        return false;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
}


void App::load_shaders()
{
    // Build and compile shaders
//    shader.create("../../shaders/tex_vs.shader", "../../shaders/tex_fs.shader");
    shader.create("../../shaders/multiple_lights_vs.shader", "../../shaders/multiple_lights_fs.shader");
    shader.use();
    shader.set_vec3("dir_light.direction", glm::vec3(0.5f, -0.5f, 0.0f));
    shader.set_vec3("dir_light.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    shader.set_vec3("dir_light.diffuse", glm::vec3(1.4f, 1.4f, 1.4f));
    shader.set_vec3("dir_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
}



void App::init_framebuffer()
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &texture_colorbuffer);
    glBindTexture(GL_TEXTURE_2D, texture_colorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_colorbuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void App::load_models()
{
    // Load models
    //backpack = std::make_shared<Object>("../../res/backpack/backpack.obj");

    glm::vec3 pos(0.0f);
    glm::vec3 rot(0.0f, -90.0f, 0.0f);

    stbi_set_flip_vertically_on_load(false);
    //ship = std::make_shared<Player>("../../res/vehicles/fighter/fighter.obj", pos, rot, 1.0, true);
    ship = std::make_shared<Player>("../../res/vehicles/submarine/sub3/sub_v03.obj", pos, rot, 0.2, true);
    stbi_set_flip_vertically_on_load(true);

    rot = glm::vec3(0.0f);
    pos.z = -5.0f;
    background = std::make_shared<Object>("../../res/environments/backgrounds/space/Blue Nebula/blue_nebula.obj",
            pos, rot, 10.0f);
}



void process_input(GLFWwindow* window, std::shared_ptr<Player> ship, float move_speed, std::shared_ptr<Object> background)
{
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        background->pos.z -= 0.01;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        background->pos.z += 0.01;

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        camera.position.z -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        camera.position.z += 0.1;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        ship->input_dir = Movement::UP;
        ship->vel.y += move_speed * delta_time;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        ship->input_dir = Movement::DOWN;
        ship->vel.y -= move_speed * delta_time;
    }
    else
    {
        ship->input_dir = Movement::NONE;
    }

//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        ship->vel.x -= move_speed * delta_time;
//
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        ship->vel.x += move_speed * delta_time;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
//    if (key == GLFW_KEY_K && action == GLFW_PRESS)
//    {
//        ship->rot_angle.x = 0.0f;
//        ship->rot_angle.z = 0.0f;
//    }
}



void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in)
{
    float xpos = static_cast<float>(xpos_in);
    float ypos = static_cast<float>(ypos_in);

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;

    last_x = xpos;
    last_y = ypos;

    //camera.process_mouse_movement(xoffset, yoffset);
}




void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.process_mouse_scroll(static_cast<float>(yoffset));
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
