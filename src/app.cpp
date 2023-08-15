#define MINIAUDIO_IMPLEMENTATION
#define GLT_IMPLEMENTATION

#include "app.h"


const char* music  = "res/audio/music_spooky_ambience.wav";
const char* charge = "res/audio/FreeSFX/Voices/Nightmare Mode.wav";

/***************************/
/*        MiniAudio        */
/***************************/
ma_result ma_init_result;
ma_engine audio_engine;
ma_sound bg_music;


Camera camera(glm::vec3(0.0f, 0.0f, 60.0f));
std::shared_ptr<Object> debug_cube;
float delta_time = 0.0f;
float last_frame = 0.0f;
bool first_mouse = true;
float last_x = 0.0f;
float last_y = 0.0f;

Game_State state = Game_State::MENU;

Timer torpedo_cooldown_timer(100);
bool torpedo_charged = true;

bool DEBUG = false;
float SPEED_SCALAR = 1.0f;


App::App(int _screen_width, int _screen_height)
    : screen_width(_screen_width), screen_height(_screen_height)
{
}



bool App::create()
{
    gl_config();
    load_text();
    load_shaders();
    load_models();

    return true;
}



void App::run()
{
    /***********************************/
    /*        Init Audio Engine        */
    /***********************************/
    ma_init_result = ma_engine_init(NULL, &audio_engine);
    if (ma_init_result != MA_SUCCESS)
        std::cout << "ERROR::MINIAUDIO\n";

    ma_init_result = ma_sound_init_from_file(&audio_engine, music, 0, NULL, NULL, &bg_music);
    if (ma_init_result != MA_SUCCESS)
        std::cout << "ERROR::MINIAUDIO\n";


    /***************************************/
    /*        Init Particle Systems        */
    /***************************************/
    ship_bubbles_ps = std::make_shared<Particle_System>(1000, 0.2f);


    ma_sound_set_looping(&bg_music, true);
    ma_sound_start(&bg_music);
    
    state = Game_State::MENU;

    while (!glfwWindowShouldClose(window))
    {
        process_input();
        update();
        render();
    }

    ma_engine_uninit(&audio_engine);

    glfwTerminate();
}



void App::update()
{
    float current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    glm::vec3 iceberg_vel;
    glm::vec3 ocean_floor_vel;


    /****************************/
    /*                          */
    /*        Case: MENU        */
    /*                          */
    /****************************/
    switch(state)
    {
        case Game_State::MENU:
            {
                iceberg_vel     = glm::vec3(0.0f);
                ocean_floor_vel = glm::vec3(0.0f);
                break;
            }


        case Game_State::RUNNING:
            {
                iceberg_vel     = glm::vec3(-2.0f, 0.0f, 0.0f) * delta_time;
                ocean_floor_vel = glm::vec3(-0.3f, 0.0f, 0.0f) * delta_time;

                torpedo_cooldown_timer.update();
                if (torpedo_cooldown_timer.check())
                {
                    torpedo_charged = true;
                    torpedo_cooldown_timer.reset();
                }


                /**********************************/
                /*                                */
                /*        Check Collisions        */
                /*                                */
                /**********************************/
                /*********************************************/
                /*        Ship and Iceberg Collisions        */
                /*********************************************/
                if (DEBUG)
                    break;

                for (auto &iceberg : icebergs)
                {
                    if (Box_Collider::aabb_collide(iceberg->collider, ship->collider))
                    {
                        collision = true;
                        state = Game_State::LOSE;
                        lose_timer.start();
                        break;
                    }
                    else
                    {
                        collision = false;
                    }
                }

                for (auto &m : mines)
                {
                    mine->collider->update_pos(m.pos);

                    if (Box_Collider::aabb_collide(mine->collider, ship->collider))
                    {
                        collision = true;
                        state = Game_State::LOSE;
                        lose_timer.start();
                        break;
                    }
                    else
                    {
                        collision = false;
                    }
                }


                /*********************************************/
                /*        Torpedo and Mine Collisions        */
                /*********************************************/
                test_for_torpedo_hit();

                for (auto &t : torpedos)
                {
                    torpedo->collider->update_pos(t.pos);
                }


                if (base->pos.x <= 0.0f)
                    state = Game_State::WIN;
                break;
            }


        case Game_State::LOSE:
            iceberg_vel     = glm::vec3(0.0f);
            ocean_floor_vel = glm::vec3(0.0f);
            if (lose_timer.check())
            {
                lose_timer.reset();
                restart_game();
            }
            else
            {
                lose_timer.update();
            }
            break;


        case Game_State::WIN:
            {
                iceberg_vel     = glm::vec3(0.0f);
                ocean_floor_vel = glm::vec3(0.0f);
                break;
            }


        case Game_State::RESTART:
            {
                iceberg_vel     = glm::vec3(0.0f);
                ocean_floor_vel = glm::vec3(0.0f);
                restart_game();
                break;
            }
    }


    /********************************/
    /*                              */
    /*        Update Objects        */
    /*                              */
    /********************************/
    /*****************************************/
    /*        Update Particle Systems        */
    /*****************************************/
    ship_bubbles_ps->update(delta_time, ship, 1, glm::vec2(-10.0f, 0.0f));


    /*****************************/
    /*        Update Ship        */
    /*****************************/
    ship->update(delta_time);


    /*********************************/
    /*        Update Torpedos        */
    /*********************************/
    for (auto &t : torpedos)
        t.update(delta_time);


    /************************************/
    /*        Update Ocean Floor        */
    /************************************/
    ocean_floor->update(ocean_floor_vel * SPEED_SCALAR);


    /*********************************/
    /*        Update Icebergs        */
    /*********************************/
    for (auto &iceberg : icebergs)
        iceberg->update(iceberg_vel * SPEED_SCALAR);


    /******************************/
    /*        Update Mines        */
    /******************************/
    for (auto &m : mines)
    {
        m.pos.x += iceberg_vel.x * SPEED_SCALAR;
        m.update(delta_time);
    }


    /*****************************/
    /*        Update Base        */
    /*****************************/
    base->update(iceberg_vel * SPEED_SCALAR);
}



void App::render()
{
    if (DEBUG)
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    else
        glClearColor(0.0f, 0.01f, 0.18f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /**********************************************/
    /*                                            */
    /*        Set View/Projection Matrices        */
    /*             And Other Uniforms             */
    /*                                            */
    /**********************************************/
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / (float)screen_height, 0.1f, 10000.0f);

    shader.use();
    shader.set_mat4("projection", projection);
    shader.set_mat4("view", view);
    shader.set_vec3("view_pos", camera.pos);

    bg_shader.use();
    bg_shader.set_mat4("projection", projection);
    bg_shader.set_mat4("view", view);
    bg_shader.set_float("u_time", glfwGetTime()*0.5);
    bg_shader.set_vec3("view_pos", camera.pos);

    ice_shader.use();
    ice_shader.set_bool("debug", DEBUG);
    ice_shader.set_bool("is_bg", true);
    ice_shader.set_mat4("projection", projection);
    ice_shader.set_mat4("view", view);
    ice_shader.set_float("u_time", glfwGetTime()*0.5);
    ice_shader.set_vec3("view_pos", camera.pos);

    debug_shader.use();
    debug_shader.set_mat4("projection", projection);
    debug_shader.set_mat4("view", view);

    particle_shader.use();
    particle_shader.set_mat4("projection", projection);
    particle_shader.set_mat4("view", view);


    /**********************/
    /*                    */
    /*        Draw        */
    /*                    */
    /**********************/
    switch(state)
    {
        case Game_State::MENU:
        {
            glm::vec2 t_pos(screen_width * 0.5f, screen_height * 0.3f);
            render_text("Subarctic", t_pos - glm::vec2(0.0f, 100.0f), text_color, 4.0f, true);
            render_text("Objective: Eliminate as many mines as possible", t_pos, text_color, 1.8f, true);
            render_text("Press Space To Begin", t_pos + glm::vec2(0.0f, 80.0f), text_color, 2.4f, true);
            break;
        }


        case Game_State::RUNNING:
            break;


        case Game_State::LOSE:
        {
            glm::vec2 t_pos(screen_width * 0.5f, screen_height * 0.3f);
            render_text("CRITICAL HULL BREACH", t_pos, text_color, 2.0f, true);
            render_text("Press Space to restart", t_pos + glm::vec2(0.0f, 100.0f), text_color, 1.0f, true);
            break;
        }


        case Game_State::WIN:
        {
            glm::vec2 t_pos(screen_width * 0.5f, screen_height * 0.3f);
            render_text("Mission Complete", t_pos, text_color, 2.0f, true);
            render_text("Press Space to restart", t_pos + glm::vec2(0.0f, 100.0f), text_color, 1.0f, true);
            break;
        }
    }


    glm::vec2 t_pos(50.0f, 50.0f);
    std::string score_str = "Score: " + std::to_string(player_score);
    render_text(score_str.c_str(), t_pos, text_color, 2.4f, false);
    t_pos.y += 50.0f;
    render_text("Controls:", t_pos + glm::vec2(0.0f, 40.0f), text_color, 1.8f, false);
    render_text("W - Pitch Down", t_pos + glm::vec2(0.0f, 80.0f), text_color, 1.5f, false);
    render_text("S - Pitch Down", t_pos + glm::vec2(0.0f, 120.0f), text_color, 1.5f, false);
    render_text("Enter - Fire Torpedo", t_pos + glm::vec2(0.0f, 160.0f), text_color, 1.5f, false);

    /*********************************/
    /*        Draw Background        */
    /*********************************/
    bg_shader.use();
    background->draw(bg_shader);


    /***************************/
    /*        Draw Ship        */
    /***************************/
    shader.use();
    ship->draw(shader);
//    if (DEBUG)
//    {
//        debug_shader.use();
//        ship->collider->draw(debug_shader);
//    }


    /********************************/
    /*        Draw Particles        */
    /********************************/
    particle_shader.use();

    ship_bubbles_ps->draw(particle_shader);


    /*********************************/
    /*        Draw Torpedo(s)        */
    /*********************************/
    for (auto& t : torpedos)
    {
        shader.use();
        torpedo->pos = t.pos;
        torpedo->rot_angle = t.rot_angle;
        torpedo->draw(shader);

        if (DEBUG)
        {
            glm::vec3 collider_pos = t.pos + glm::vec3(0.0f, 0.4f, 0.0f);
            torpedo->collider->update_pos(collider_pos);
            debug_shader.use();
            torpedo->collider->draw(debug_shader);
        }
    }


    /***********************************/
    /*        Draw  Ocean Floor        */
    /***********************************/
    ice_shader.use();
    ocean_floor->draw(ice_shader);


    /*******************************/
    /*        Draw Icebergs        */
    /*******************************/
    ice_shader.set_bool("is_bg", false);
    for (auto &iceberg : icebergs)
    {
        ice_shader.use();
        iceberg->draw(ice_shader);

        if (DEBUG)
        {
            debug_shader.use();
            iceberg->collider->draw(debug_shader);
        }
    }


    /****************************/
    /*        Draw Mines        */
    /****************************/
    for (auto &m : mines)
    {
        shader.use();
        mine->pos = m.pos;
        mine->draw(shader);

        if (DEBUG)
        {
            mine->collider->update_pos(m.pos);
            debug_shader.use();
            mine->collider->draw(debug_shader);
        }
    }


//    ice_shader.use();
//    base->draw(ice_shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
}



void App::render_text(const char* str, glm::vec2 pos, glm::vec3 color, float scale, bool aligned)
{
    gltBeginDraw();
    gltSetText(screen_text, str);
    gltColor(color.x, color.y, color.z, 1.0f);

    if (aligned)
    {
        gltDrawText2DAligned(screen_text,
                (GLfloat)pos.x,
                (GLfloat)pos.y,
                scale,
                GLT_CENTER, GLT_CENTER);
    }
    else
    {
        gltDrawText2D(screen_text, (GLfloat)pos.x, (GLfloat)pos.y, scale); // text, x, y, scale
    }

    gltEndDraw();
}



bool App::gl_config()
{
    glfwInit();

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    window = glfwCreateWindow(screen_width, screen_height, "Subarctic", NULL, NULL);

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
    glfwSetWindowAspectRatio(window, 4, 3);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

//    if (glewInit() != GLEW_OK)
//    {
//        std::cout << "\nERROR: Failed to initialize GLEW\n";
//        return false;
//    }

    if (!gltInit())
    {
        std::cout << "\nERROR: Failed to initialize glText\n";
        glfwTerminate();
        return false;
    }

    stbi_set_flip_vertically_on_load(false);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



/******************************/
/*                            */
/*        Load Shaders        */
/*                            */
/******************************/
void App::load_shaders()
{
    shader.create("shaders/multiple_lights_vs.shader", "shaders/multiple_lights_fs.shader");
    shader.use();
    shader.set_vec3("dir_light.direction", glm::vec3(0.5f, -0.5f, 0.7f));
    shader.set_vec3("dir_light.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.set_vec3("dir_light.diffuse", glm::vec3(0.4f, 0.4f, 0.5f));
    shader.set_vec3("dir_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    shader.set_vec3("spot_light.position", 0.0f, 10.0f, 0.0f);
    shader.set_vec3("spot_light.direction", 0.0f, -1.0f, 0.0f);
    shader.set_vec3("spot_light.diffuse", 8.0f, 8.0f, 8.0f);
    shader.set_vec3("spot_light.ambience", 0.0f, 0.0f, 0.0f);
    shader.set_vec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
    shader.set_float("spot_light.constant", 1.0f);
    shader.set_float("spot_light.linear", 0.09f);
    shader.set_float("spot_light.quadratic", 0.032f);
    shader.set_float("spot_light.cut_off", glm::cos(glm::radians(5.0)));
    shader.set_float("spot_light.outer_cut_off", glm::cos(glm::radians(5.0)));
    shader.set_vec3("fog_color", fog_color);
    shader.set_float("brightness", brightness);


    bg_shader.create("shaders/noise_vs.shader", "shaders/noise_fs.shader");
    bg_shader.use();
    bg_shader.set_vec2("u_resolution", glm::vec2(screen_width * .4, screen_height * .4));
    bg_shader.set_vec3("fog_color", fog_color);
    bg_shader.set_float("brightness", brightness);


    debug_shader.create("shaders/color_vs.shader", "shaders/color_fs.shader");


    ice_shader.create("shaders/ice_vs.shader", "shaders/ice_fs.shader");
    ice_shader.use();
    ice_shader.set_vec3("dir_light.direction", glm::vec3(0.5f, -0.5f, 0.7f));
    ice_shader.set_vec3("dir_light.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    ice_shader.set_vec3("dir_light.diffuse", glm::vec3(0.4f, 0.4f, 0.5f));
    ice_shader.set_vec3("dir_light.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    ice_shader.set_vec2("u_resolution", glm::vec2(screen_width * .5, screen_height * .5));

    ice_shader.set_vec3("spot_light.position", 0.0f, 10.0f, 0.0f);
    ice_shader.set_vec3("spot_light.direction", 0.0f, -1.0f, 0.0f);
    ice_shader.set_vec3("spot_light.diffuse", 15.0f, 15.0f, 20.0f);
    ice_shader.set_vec3("spot_light.ambience", 0.0f, 0.0f, 0.0f);
    ice_shader.set_vec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
    ice_shader.set_float("spot_light.constant", 1.0f);
    ice_shader.set_float("spot_light.linear", 0.09f);
    ice_shader.set_float("spot_light.quadratic", 0.032f);
    ice_shader.set_float("spot_light.cut_off", glm::cos(glm::radians(25.0)));
    ice_shader.set_float("spot_light.outer_cut_off", glm::cos(glm::radians(30.0)));
    ice_shader.set_vec3("fog_color", fog_color);
    ice_shader.set_float("fog_scalar_min", fog_scalar_min);
    ice_shader.set_float("fog_scalar_max", fog_scalar_max);
    ice_shader.set_float("brightness", brightness);


    particle_shader.create("shaders/particle_vs.shader", "shaders/particle_fs.shader");
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



void App::load_text()
{
    screen_text = gltCreateText();
}



/*****************************/
/*                           */
/*        Load Models        */
/*                           */
/*****************************/
void App::load_models()
{
    glm::vec3 pos(0.0f);
    float rot = 0.0f;

    /********************************/
    /*        Load Submarine        */
    /********************************/
    ship = std::make_shared<Player>("res/vehicles/submarine/sub3/sub_v03.obj", pos, rot, 0.2f, glm::vec3(18.0f, 2.0f, 2.0f));

    /******************************/
    /*        Load Torpedo        */
    /******************************/
    torpedo = std::make_shared<Object>("res/vehicles/torpedo/torpedo.obj", pos, rot, 0.2f, glm::vec3(2.0f, 0.8f, 0.8f));

    /*********************************/
    /*        Load Debug Cube        */
    /*********************************/
    glm::vec3 tmp = glm::vec3(0.0f, 0.0f, 0.0f);
    debug_cube = std::make_shared<Object>("res/environments/backgrounds/ocean/cube.obj", tmp, rot, 1.0f, glm::vec3(20.0f));

    /*******************************/
    /*        Load Backdrop        */
    /*******************************/
    pos.z = -100.0f;
    background = std::make_shared<Object>("res/environments/backgrounds/ocean/long_quad_25d.obj", pos, rot, 1.0f);

    /************************************/
    /*        Load Ocean Surface        */
    /************************************/
    pos.x = 0.0f;
    pos.y = 25.0f;
    pos.z = 0.0f;
    ocean_surface = std::make_shared<Object>("res/environments/backgrounds/ocean/surface.obj", pos, rot, 1.0);

    /**********************************/
    /*        Load Ocean Floor        */
    /**********************************/
    ocean_floor_start_position = glm::vec3(180.0f, -45.0f, -60.0f);
    pos.x = 180.0f;
    pos.y = -45.0f;
    pos.z = -60.0f;
    ocean_floor = std::make_shared<Object>("res/environments/objects/ice/undersea_mountains_01.obj", pos, rot, 30.0);

    /*******************************/
    /*        Load Icebergs        */
    /*******************************/
    std::vector<glm::vec3> collider_scales;
    collider_scales.push_back(glm::vec3(23.0f));
    collider_scales.push_back(glm::vec3(12.0f,  56.0f, 2.0f));
    collider_scales.push_back(glm::vec3(24.0f,  58.0f, 2.0f));
    collider_scales.push_back(glm::vec3(20.0f,  38.0f, 2.0f));
    collider_scales.push_back(glm::vec3( 9.0f, 158.0f, 2.0f));
    collider_scales.push_back(glm::vec3(25.0f,  55.0f, 2.0f));
    collider_scales.push_back(glm::vec3( 8.0f, 110.0f, 2.0f));
    collider_scales.push_back(glm::vec3(24.0f,  50.0f, 2.0f));
    iceberg_start_positions = load_position_data("res/environments/objects/iceberg_positions_v08.txt");
    for (int i = 0; i < iceberg_start_positions.size() - 1; i++)
    {
        std::string path = "res/environments/objects/ice/iceberg_0" + std::to_string(i + 1) + ".obj";
        icebergs.push_back(std::make_shared<Object>(path, iceberg_start_positions[i], rot, 10.0, collider_scales[i]));
    }


    /****************************/
    /*        Load Mines        */
    /****************************/
    mine_start_positions = load_position_data("res/environments/objects/mine_positions_v03.txt");
    mine = std::make_shared<Object>("res/environments/objects/mines/naval_mine_v02.obj", glm::vec3(0.0f), rot, 0.7);
    for (int i = 0; i < mine_start_positions.size(); i++)
    {
        mines.push_back(Pos_Vel_Rot(mine_start_positions[i]));
    }


    /***************************/
    /*        Load Base        */
    /***************************/
    std::string path = "res/environments/objects/ice/base.obj";
    int index = iceberg_start_positions.size() - 1;
    base_start_position = iceberg_start_positions[index];
    base = std::make_shared<Object>(path, base_start_position, rot, 10.0);
}



void App::test_for_torpedo_hit()
{
    /*********************************************/
    /*        Torpedo and Mine Collisions        */
    /*********************************************/
    bool hit = false;

    std::vector<Pos_Vel_Rot>::iterator t_it;
    for (auto t_it = torpedos.begin(); t_it != torpedos.end();)
    {
        glm::vec3 collider_pos = t_it->pos + glm::vec3(0.0f, 1.0f, 0.0f);
        torpedo->collider->update_pos(collider_pos);
        std::vector<Pos_Vel_Rot>::iterator m_it;

        /**********************************/
        /*        Torpedo and Mine        */
        /**********************************/
        for (auto m_it = mines.begin(); m_it != mines.end();)
        {
            mine->collider->update_pos(m_it->pos);
            if (!Box_Collider::aabb_collide(torpedo->collider, mine->collider))
            {
                m_it++;
                continue;
            }
            player_score += 8;
            hit = true;
            mines.erase(m_it);
            break;
        }

        /*************************************/
        /*        Torpedo and Iceberg        */
        /*************************************/
        for (auto &iceberg : icebergs)
        {
            if (!Box_Collider::aabb_collide(torpedo->collider, iceberg->collider))
                continue;

            hit = true;
            break;
        }

        if (hit)
        {
            torpedos.erase(t_it);
        }
        else
        {
            t_it++;
        }
    }
}



void App::restart_game()
{
    ocean_floor->pos = ocean_floor_start_position;
    ship->pos = glm::vec3(0.0f);
    player_score = 0;

    for (int i = 0; i < iceberg_start_positions.size() - 1; i++)
    {
        icebergs[i]->pos = iceberg_start_positions[i];
    }

    base->pos = base_start_position;
    
    state = Game_State::MENU;
}



void App::process_input()
{

    /********************************/
    /*                              */
    /*        Debug Controls        */
    /*                              */
    /********************************/

    /*********************************/
    /*        Move Debug Cube        */
    /*********************************/
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        debug_cube->pos.y += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        debug_cube->pos.y -= 0.1f;

    /*****************************/
    /*        Move camera        */
    /*****************************/
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.pos.y -= 0.5;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera.pos.y += 0.5;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        camera.pos.z -= 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        camera.pos.z += 1.0;

    /************************************************/
    /*        Fast-Forward/Rewind Map Scroll        */
    /************************************************/
    if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        SPEED_SCALAR = 40.0f;
    else if(glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        SPEED_SCALAR = -40.0f;
    else
        SPEED_SCALAR = 1.0f;

    if (DEBUG)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.process_keyboard(Camera_Movement::UP, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.process_keyboard(Camera_Movement::DOWN, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.process_keyboard(Camera_Movement::LEFT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.process_keyboard(Camera_Movement::RIGHT, delta_time);
    }
    else
    {
        /******************************/
        /*                            */
        /*        Player Input        */
        /*                            */
        /******************************/
        /***************************/
        /*        Move Ship        */
        /***************************/
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            ship->input_dir = Movement::DOWN;
            ship->vel.y -= move_speed * delta_time;
        }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            ship->input_dir = Movement::UP;
            ship->vel.y += move_speed * delta_time;
        }
        else
        {
            ship->input_dir = Movement::NONE;
        }


        /******************************/
        /*        Fire Torpedo        */
        /******************************/
        if (glfwGetKey(window, GLFW_KEY_ENTER) && state == Game_State::RUNNING && torpedo_charged)
        {
            glm::vec3 start_pos = ship->pos;
            start_pos.y -= 1.0f;
            glm::vec3 start_vel = glm::vec3(0.05f, -0.08f, 0.0f);
            float start_rot = ship->rot_angle;
            start_vel.y += ship->vel.y;

            torpedos.push_back(Pos_Vel_Rot(start_pos, start_vel, start_rot, false));
            torpedo_cooldown_timer.start();
            torpedo_charged = false;
        }
    }


    /***************************/
    /*        Exit Game        */
    /***************************/
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



std::vector<glm::vec3> load_position_data(const char* filepath)
{
    std::ifstream fin;
    fin.open(filepath);
    std::vector<std::string> lines;
    std::string next_line;
    glm::vec3 vec;
    std::vector<glm::vec3> out;

    while(std::getline(fin, next_line, '\n'))
    {
        lines.push_back(next_line);
    }

    for (auto &line : lines)
    {
        std::stringstream ss(line);
        std::string num_str;
        ss >> num_str;
        vec.x = std::stof(num_str);
        ss >> num_str;
        vec.y = std::stof(num_str);
        ss >> num_str;
        vec.z = std::stof(num_str);

        out.push_back(vec);
    }

    return out;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        DEBUG = !DEBUG;
        camera.debug = !camera.debug;
    }

    if (state == Game_State::MENU && key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        state = Game_State::RUNNING;
    }

    if (state == Game_State::LOSE && key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        state = Game_State::RESTART;
    }

    if (state == Game_State::WIN && key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        state = Game_State::RESTART;
    }
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

    camera.process_mouse_movement(xoffset, yoffset);
}




void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.process_mouse_scroll(static_cast<float>(yoffset));
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
