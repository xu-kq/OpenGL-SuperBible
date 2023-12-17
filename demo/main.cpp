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
    my_application() : tex_index{0} {
    }

    void render(double currentTime) override {
        static const GLfloat gray[] = {0.2f, 0.2f, 0.2f, 1.0f};
        static const GLfloat ones[] = {1.0f};

        glClearBufferfv(GL_COLOR, 0, gray);
        glClearBufferfv(GL_DEPTH, 0, ones);

        glViewport(0, 0, info.windowWidth, info.windowHeight);

        glBindTexture(GL_TEXTURE_2D, tex_object[tex_index]);

        glUseProgram(program);

        vmath::mat4 proj_matrix = vmath::perspective(60.0f, (float) info.windowWidth / (float) info.windowHeight, 0.1f,
                                                     1000.0f);
        vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -3.0f) *
                                vmath::rotate((float) currentTime * 19.3f, 0.0f, 1.0f, 0.0f) *
                                vmath::rotate((float) currentTime * 21.1f, 0.0f, 0.0f, 1.0f);

        glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, mv_matrix);
        glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, proj_matrix);

        object.render();
    }

    void startup() override {
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
        static const GLubyte tex_data[] =
        {
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
            B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
            W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        };
#undef B
#undef W

        glGenTextures(1, &tex_object[0]);
        glBindTexture(GL_TEXTURE_2D, tex_object[0]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        tex_object[1] = sb7::ktx::file::load("media/textures/pattern1.ktx");

        object.load("media/objects/torus_nrms_tc.sbm"); {
            sp_program.reset(new Demo::Shader_Program());
            sp_program->compile(GL_VERTEX_SHADER)
                    .compile(GL_FRAGMENT_SHADER)
                    .link();
            program = sp_program->get();
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);


        uniforms.mv_matrix = glGetUniformLocation(program, "mv_matrix");
        uniforms.proj_matrix = glGetUniformLocation(program, "proj_matrix");
    }

    virtual void onKey(int key, int action) override {
        if (action) {
            switch (key) {
                case 'T':
                    tex_index++;
                    if (tex_index > 1)
                        tex_index = 0;
                    break;
            }
        }
    }

    void shutdown() override {
        glDeleteProgram(program);
        glDeleteTextures(2, tex_object);
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
    GLuint render_prog;

    GLuint tex_object[2];
    GLuint tex_index;

    struct {
        GLint mv_matrix;
        GLint proj_matrix;
    } uniforms;

    sb7::object object;

private:
    GLuint program;
    GLuint vao;
    std::shared_ptr<Demo::Shader_Program> sp_program;
};

DECLARE_MAIN(my_application);
