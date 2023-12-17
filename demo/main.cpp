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
    void render(double currentTime) override {
        static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 0.0f};
        float t = (float) currentTime;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(program);

        vmath::mat4 proj_matrix = vmath::perspective(60.0f,
                                                     (float) info.windowWidth / (float) info.windowHeight,
                                                     0.1f, 1000.0f);

        glUniform1f(uniforms.offset, t * 0.003f);


        GLuint textures[] = {tex_wall, tex_floor, tex_wall, tex_ceiling};
        for (int i = 0; i < 4; ++i) {
            vmath::mat4 mv_matrix =
                    vmath::rotate(90.0f * (float) i, vmath::vec3(0.0f, 0.0f, 1.0f)) *
                    vmath::translate(-0.5f, 0.0f, -10.0f) *
                    vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f) *
                    vmath::scale(30.0f, 1.0f, 1.0f);
            vmath::mat4 mvp = proj_matrix * mv_matrix;

            glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, mvp);

            glBindTextureUnit(0, textures[i]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    void startup() override {
        sp_program.reset(new Demo::Shader_Program());
        sp_program->compile(GL_VERTEX_SHADER)
                .compile(GL_FRAGMENT_SHADER)
                .link();
        program = sp_program->get();


        uniforms.mvp = glGetUniformLocation(program, "mvp");
        uniforms.offset = glGetUniformLocation(program, "offset");

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        tex_wall = sb7::ktx::file::load("media/textures/brick.ktx");
        tex_ceiling = sb7::ktx::file::load("media/textures/ceiling.ktx");
        tex_floor = sb7::ktx::file::load("media/textures/floor.ktx");

        GLuint textures[] = {tex_floor, tex_wall, tex_ceiling};

        for (int i = 0; i < 3; ++i) {
            glBindTextureUnit(0, textures[i]);
            glTextureParameteri(textures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(textures[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }


    void shutdown() override {
        glDeleteProgram(program);
    }


private:
    struct {
        GLint mvp;
        GLint offset;
    } uniforms;

private:
    GLuint vao;
    GLuint program;
    std::shared_ptr<Demo::Shader_Program> sp_program;

    GLuint tex_wall;
    GLuint tex_ceiling;
    GLuint tex_floor;
};

DECLARE_MAIN(my_application);
