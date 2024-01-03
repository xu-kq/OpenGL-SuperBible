//
// Created by xukq on 23/12/10.
//
#include <memory>
#include <sb7.h>
#include <vmath.h>

#include <object.h>
#include <sb7ktx.h>
#include "shader_compiler.h"


class my_application : public sb7::application {
public:
    void init() override {
        static const char title[] = "OpenGL - Instance";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void render(double currentTime) override {
        static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 0.0f};
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(program);
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, 4);
    }

    void startup() override {
        static const GLfloat square_vertices[] = {
            -1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f,
        };

        static const GLfloat instance_colors[] = {
            1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
        };

        static const GLfloat instance_positions[] = {
            -2.0f, -2.0f, 0.0f, 0.0f,
             2.0f, -2.0f, 0.0f, 0.0f,
             2.0f,  2.0f, 0.0f, 0.0f,
            -2.0f,  2.0f, 0.0f, 0.0f
        };

        GLuint offset = 0;

        glCreateBuffers(1, &square_vbo);
        glNamedBufferData(square_vbo,
                          sizeof(square_vertices) + sizeof(instance_colors) + sizeof(instance_positions),
                          nullptr, GL_STATIC_DRAW
        );
        glNamedBufferSubData(square_vbo, offset, sizeof(square_vertices), square_vertices);
        offset += sizeof(square_vertices);
        glNamedBufferSubData(square_vbo, offset, sizeof(instance_colors), instance_colors);
        offset += sizeof(instance_colors);
        glNamedBufferSubData(square_vbo, offset, sizeof(instance_positions), instance_positions);
        offset += sizeof(instance_positions);

        glCreateVertexArrays(1, &square_vao);
        glEnableVertexArrayAttrib(square_vao, 0);
        glEnableVertexArrayAttrib(square_vao, 1);
        glEnableVertexArrayAttrib(square_vao, 2);
        glVertexArrayAttribFormat(square_vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribFormat(square_vao, 1, 4, GL_FLOAT, GL_FALSE, sizeof(square_vertices));
        glVertexArrayAttribFormat(square_vao, 2, 4, GL_FLOAT, GL_FALSE, sizeof(square_vertices) + sizeof(instance_colors));
        glVertexArrayAttribBinding(square_vao, 0, 0);
        glVertexArrayAttribBinding(square_vao, 1, 1);
        glVertexArrayAttribBinding(square_vao, 2, 1);

        glVertexArrayVertexBuffer(square_vao, 0, square_vbo, 0, 16);
        glVertexArrayVertexBuffer(square_vao, 1, square_vbo, 0, 16);
        glVertexArrayVertexBuffer(square_vao, 1, square_vbo, 0, 16);

        glVertexArrayBindingDivisor(square_vao, 1, 1);

        glBindVertexArray(square_vao);

        Demo::Shader_Program myProgram;
        program = myProgram
                .compile(GL_VERTEX_SHADER)
                .compile(GL_FRAGMENT_SHADER)
                .link()
                .get();
    }

    void shutdown() override {
        glDeleteProgram(program);
    }

private:
    GLuint square_vao;
    GLuint square_vbo;

    GLuint program;

    struct {
        GLuint mvpMatrix;
    } uniforms;
};

DECLARE_MAIN(my_application);
