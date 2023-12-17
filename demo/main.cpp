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
        glClearBufferfv(GL_COLOR, 0, sb7::color::Green);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void startup() override {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        static const GLfloat vertex_positions[] =
        {
            -0.75f, -0.75f, 0.5,
            0.75f, -0.75f, 0.5,
            0.75f, 0.75f, 0.5,
        };
        GLuint buffer;
        glGenBuffers(1, &buffer);

        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_DYNAMIC_STORAGE_BIT);

        glVertexArrayVertexBuffer(vao, 0, buffer, 0, 3 * sizeof(float));
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
        glEnableVertexArrayAttrib(vao, 0);


        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        glTextureStorage2D(texture, 1, GL_RGBA32F, 256, 256);

        glBindTexture(GL_TEXTURE_2D, texture); {
            float* data = new float[256 * 256 * 4];
            generate_texture(data, 256, 256);

            glTextureSubImage2D(texture,
                                0,
                                0, 0,
                                256, 256,
                                GL_RGBA,
                                GL_FLOAT,
                                data);
            delete[] data;
        } {
            sp_program.reset(new Demo::Shader_Program());
            sp_program->compile(GL_VERTEX_SHADER)
                    .compile(GL_FRAGMENT_SHADER)
                    .link();
            program = sp_program->get();
        }
    }

    void onResize(int w, int h) {
        // sb7::application::onResize(w, h);
        //
        // aspect = (float) info.windowWidth / (float) info.windowHeight;
        // proj_matrix = vmath::perspective(50.f,
        //                                  aspect,
        //                                  0.1f,
        //                                  1000.f);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
    }

private:
    void generate_texture(float* data, int width, int height) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                data[(y * width + x) * 4 + 0] = (float) (x & y & 0XFF) / 255.0f;
                data[(y * width + x) * 4 + 1] = (float) (x | y & 0XFF) / 255.0f;
                data[(y * width + x) * 4 + 2] = (float) (x ^ y & 0XFF) / 255.0f;
                data[(y * width + x) * 4 + 3] = 1.0f;
            }
        }
    }

private:
    GLuint program;
    GLuint vao;
    std::shared_ptr<Demo::Shader_Program> sp_program;

    // float aspect;
    // vmath::mat4 proj_matrix;
};

DECLARE_MAIN(my_application);
