//
// Created by xukq on 23/12/10.
//
#define _USE_MATH_DEFINES
#include "sb7.h"
#include "shader_compiler.h"
#include <sb7color.h>
#include <vmath.h>


class my_application : public sb7::application {
public:
    void render(double currentTime) override {
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float f = (float) currentTime * (float) M_PI * 0.1f;
        vmath::mat4 mv_matrix =
                vmath::translate(0.0f, 0.0f, -4.0f) *
                vmath::translate(sinf(2.1f * f) * 0.5f,
                                 cosf(1.7f * f) * 0.5f,
                                 sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
                vmath::rotate((float) currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
                vmath::rotate((float) currentTime * 81.0f, 1.0f, 0.0f, 0.0f);

        glClear(GL_DEPTH_BUFFER_BIT);

        glClearBufferfv(GL_COLOR, 0, sb7::color::Green);
        glUseProgram(program);

        glUniformMatrix4fv(glGetUniformLocation(program, "mv_matrix"), 1, GL_FALSE, mv_matrix);
        glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, proj_matrix);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void startup() override {
        glEnable(GL_DEPTH_TEST);
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        static const GLfloat vertex_positions[] =
        {
            // down
            -0.25f, 0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            0.25f, -0.25f, -0.25f,

            0.25f, -0.25f, -0.25f,
            0.25f, 0.25f, -0.25f,
            -0.25f, 0.25f, -0.25f,

            // up
            -0.25f, 0.25f, 0.25f,
            -0.25f, -0.25f, 0.25f,
            0.25f, -0.25f, 0.25f,

            0.25f, -0.25f, 0.25f,
            0.25f, 0.25f, 0.25f,
            -0.25f, 0.25f, 0.25f,

            // front
            0.25, -0.25, -0.25,
            0.25, -0.25, 0.25,
            0.25, 0.25, 0.25,

            0.25, 0.25, 0.25,
            0.25, 0.25, -0.25,
            0.25, -0.25, -0.25,

            // back
            -0.25, -0.25, -0.25,
            -0.25, -0.25, 0.25,
            -0.25, 0.25, 0.25,

            -0.25, 0.25, 0.25,
            -0.25, 0.25, -0.25,
            -0.25, -0.25, -0.25,

            //  left
            -0.25f, 0.25f, -0.25f,
            0.25f, 0.25f, -0.25f,
            0.25f, 0.25f, 0.25f,

            0.25f, 0.25f, 0.25f,
            -0.25f, 0.25f, 0.25f,
            -0.25f, 0.25f, -0.25f,

            //  right
            -0.25f, -0.25f, -0.25f,
            0.25f, -0.25f, -0.25f,
            0.25f, -0.25f, 0.25f,

            0.25f, -0.25f, 0.25f,
            -0.25f, -0.25f, 0.25f,
            -0.25f, -0.25f, -0.25f,
        };
        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(vertex_positions),
                     vertex_positions,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);


        sp_program.reset(new Demo::Shader_Program());
        sp_program->compile(GL_VERTEX_SHADER)
                .compile(GL_FRAGMENT_SHADER)
                .link();
        program = sp_program->get();

        aspect = (float) info.windowWidth / (float) info.windowHeight;
        proj_matrix = vmath::perspective(50.f,
                                         aspect,
                                         0.1f,
                                         1000.f);
    }

    void onResize(int w, int h) {
        sb7::application::onResize(w, h);

        aspect = (float) info.windowWidth / (float) info.windowHeight;
        proj_matrix = vmath::perspective(50.f,
                                         aspect,
                                         0.1f,
                                         1000.f);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
    }

private:
    GLuint program;
    GLuint vao;
    std::shared_ptr<Demo::Shader_Program> sp_program;

    float aspect;
    vmath::mat4 proj_matrix;
};

DECLARE_MAIN(my_application);
