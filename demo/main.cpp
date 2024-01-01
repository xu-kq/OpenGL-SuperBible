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
        static const char title[] = "OpenGL - Grass";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void render(double currentTime) override {
        float t = (float) currentTime * 0.02f;
        float r = 550.0f;

        static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};
        static const GLfloat one = 1.0f;
        glClearBufferfv(GL_COLOR, 0, black);
        glClearBufferfv(GL_DEPTH, 0, &one);

        vmath::mat4 mv_matrix = vmath::lookat(vmath::vec3(sinf(t) * r, 25.0f, cosf(t) * r),
                                              vmath::vec3(0.0f, -50.0f, 0.0f),
                                              vmath::vec3(0.0, 1.0, 0.0));

        vmath::mat4 prj_matrix = vmath::perspective(45.0f, (float) info.windowWidth / (float) info.windowHeight, 0.1f,
                                                    1000.0f);

        glUseProgram(grass_program);
        glUniformMatrix4fv(uniforms.mvpMatrix, 1, GL_FALSE, (prj_matrix * mv_matrix));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glViewport(0, 0, info.windowWidth, info.windowHeight);

        glBindVertexArray(grass_vao);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
    }

    void startup() override {
        static const GLfloat grass_blade[] =
        {
            -0.3f, 0.0f,
            0.3f, 0.0f,
            -0.20f, 1.0f,
            0.1f, 1.3f,
            -0.05f, 2.3f,
            0.0f, 3.3f,
        };

        glCreateBuffers(1, &grass_buffer);
        glNamedBufferData(grass_buffer, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

        glCreateVertexArrays(1, &grass_vao);
        glEnableVertexArrayAttrib(grass_vao, 0);
        glVertexArrayAttribFormat(grass_vao, 0, 2, GL_FLOAT, GL_FALSE, 0);

        glVertexArrayAttribBinding(grass_vao, 0, 0);
        glVertexArrayVertexBuffer(grass_vao, 0, grass_buffer, 0, 2 * sizeof(float));

        Demo::Shader_Program myProgram;
        grass_program = myProgram
                .compile(GL_VERTEX_SHADER)
                .compile(GL_FRAGMENT_SHADER)
                .link()
                .get();

        uniforms.mvpMatrix = glGetUniformLocation(grass_program, "mvpMatrix");

        tex_grass_length = sb7::ktx::file::load("media/textures/grass_length.ktx");
        glBindTextureUnit(1, tex_grass_length);
        tex_grass_orientation = sb7::ktx::file::load("media/textures/grass_orientation.ktx");
        glBindTextureUnit(2, tex_grass_orientation);
        tex_grass_color = sb7::ktx::file::load("media/textures/grass_color.ktx");
        glBindTextureUnit(3, tex_grass_color);
        tex_grass_bend = sb7::ktx::file::load("media/textures/grass_bend.ktx");
        glBindTextureUnit(4, tex_grass_bend);
    }

    void shutdown() override {
        glDeleteProgram(grass_program);
    }

private:
    GLuint grass_buffer;
    GLuint grass_vao;

    GLuint grass_program;

    GLuint grasspalette_texture;
    GLuint tex_grass_color;
    GLuint tex_grass_length;
    GLuint tex_grass_orientation;
    GLuint tex_grass_bend;

    struct {
        GLuint mvpMatrix;
    } uniforms;


};

DECLARE_MAIN(my_application);
