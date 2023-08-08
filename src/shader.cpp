#include "shader.h"



Shader::Shader()
{
}



Shader::~Shader()
{
    glDeleteProgram(id);
}



void Shader::create(const char* vertex_path, const char* fragment_path)
{
    // 1. Retrieve the vertex/fragment source code from file_path
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // Ensure ifstream objects can throw exceptions:
    v_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream, f_shader_stream;

        // Read file's buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        // Close file handlers
        v_shader_file.close();
        f_shader_file.close();

        // Convert stream into string
        vertex_code   = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR:SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();


    // 2. Compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &v_shader_code, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &f_shader_code, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}



void Shader::use()
{
    glUseProgram(id);
}



void Shader::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

// ------------------------------------------------------------------------

void Shader::set_int(const std::string &name, int value) const
{
    //std::cout << name << std::endl;
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    //glUniform1i(glGetUniformLocation(id, "re1"), value);
}

// ------------------------------------------------------------------------

void Shader::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

// ------------------------------------------------------------------------

void Shader::set_vec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec2(const std::string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

// ------------------------------------------------------------------------

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

// ------------------------------------------------------------------------

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::set_vec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

// ------------------------------------------------------------------------

void Shader::set_mat2(const std::string &name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------

void Shader::set_mat3(const std::string &name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// ------------------------------------------------------------------------

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



void Shader::check_compile_errors(unsigned int shader, std::string type)
{
    int success;
    char info_log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
