//
// Created by xukq on 23/12/10.
//

#include "sb7.h"
GLuint compile_shaders();

class my_application : public sb7::application {
public:
    void render(double currentTime) override {
        const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, color);

        glUseProgram(rendering_program);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void startup() override {
        rendering_program = compile_shaders();
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);

    }
private:
    GLuint rendering_program;
    GLuint vertex_array_object;
};

DECLARE_MAIN(my_application);

GLuint compile_shaders() {
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint program;


    static const GLchar* vertex_shader_source[] = {
        "#version 450 core                          \n"
        "                                           \n"
        "void main(void) {                          \n"
        "   const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),   \n"
        "                                   vec4(-0.25, -0.25, 0.5, 1.0),   \n"
        "                                   vec4(0.25, 0.25, 0.5, 1.0));    \n"
        "   gl_Position = vertices[gl_VertexID];                            \n"
        "}                                          \n"
    };
    static const GLchar* fragment_shader_source[] = {
        "#version 450 core                          \n"
        "                                           \n"
        "out vec4 color;                            \n"
        "                                           \n"
        "void main(void)                            \n"
        "{                                          \n"
        "   color = vec4(0.0, 0.8, 1.0, 1.0);       \n"
        "}"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}
