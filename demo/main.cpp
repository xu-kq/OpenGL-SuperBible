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
        static const char title[] = "OpenGL - Fragment List";

        sb7::application::init();

        memcpy(info.title, title, sizeof(title));
    }

    void render(double currentTime) override {
        static const GLfloat zeros[] = {0.0f, 0.0f, 0.0f, 0.0f};
        static const GLfloat gray[] = {0.1f, 0.1f, 0.1f, 0.0f};
        static const GLfloat ones[] = {1.0f};
        const float f = (float) currentTime;

        glViewport(0, 0, info.windowWidth, info.windowHeight);

        glMemoryBarrier(
            GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        glUseProgram(clear_program);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glUseProgram(append_program);

        vmath::mat4 model_matrix = vmath::scale(7.0f);
        vmath::vec3 view_position = vmath::vec3(cosf(f * 0.35f) * 120.0f, cosf(f * 0.4f) * 30.0f,
                                                sinf(f * 0.35f) * 120.0f);
        vmath::mat4 view_matrix = vmath::lookat(view_position,
                                                vmath::vec3(0.0f, 30.0f, 0.0f),
                                                vmath::vec3(0.0f, 1.0f, 0.0f));

        vmath::mat4 mv_matrix = view_matrix * model_matrix;
        vmath::mat4 proj_matrix = vmath::perspective(50.0f,
                                                     (float) info.windowWidth / (float) info.windowHeight,
                                                     0.1f,
                                                     1000.0f);

        glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

        static const unsigned int zero = 0;
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
        glNamedBufferSubData(atomic_counter_buffer, 0, sizeof(zero), &zero);


        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fragment_buffer);

        glBindImageTexture(0, head_pointer_image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

        glMemoryBarrier(
            GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        object.render();

        glMemoryBarrier(
            GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        glUseProgram(resolve_program);

        glMemoryBarrier(
            GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void startup() override {
        load_render();

        glCreateBuffers(1, &uniforms_buffer);
        glNamedBufferStorage(uniforms_buffer, sizeof(uniforms_block), nullptr, GL_DYNAMIC_DRAW);

        object.load("media/objects/dragon.sbm");

        glCreateBuffers(1, &fragment_buffer);
        glNamedBufferData(fragment_buffer, 1024 * 1024 * 16, nullptr, GL_DYNAMIC_COPY);

        glCreateBuffers(1, &atomic_counter_buffer);
        glNamedBufferData(atomic_counter_buffer, 4, nullptr, GL_DYNAMIC_COPY);

        glCreateTextures(GL_TEXTURE_2D, 1, &head_pointer_image);
        glTextureStorage2D(head_pointer_image, 1, GL_R32UI, 1024, 1024);

        glCreateVertexArrays(1, &dummy_vao);
        glBindVertexArray(dummy_vao);
    }

    void load_render() {
        Demo::Shader_Program clearProg("w");
        clear_program = clearProg
                .compile(GL_VERTEX_SHADER, "../demo/clear.vs.glsl")
                .compile(GL_FRAGMENT_SHADER, "../demo/clear.fs.glsl")
                .link().get();

        Demo::Shader_Program appendProg("a");
        append_program = appendProg
                .compile(GL_VERTEX_SHADER, "../demo/append.vs.glsl")
                .compile(GL_FRAGMENT_SHADER, "../demo/append.fs.glsl")
                .link().get();

        uniforms.mvp = glGetUniformLocation(append_program, "mvp");

        Demo::Shader_Program resolveProg("a");
        resolve_program = resolveProg
                .compile(GL_VERTEX_SHADER, "../demo/resolve.vs.glsl")
                .compile(GL_FRAGMENT_SHADER, "../demo/resolve.fs.glsl")
                .link().get();
    }

    void shutdown() override {
        glDeleteProgram(clear_program);
        glDeleteProgram(append_program);
        glDeleteProgram(resolve_program);
    }

private:
    GLuint clear_program;
    GLuint append_program;
    GLuint resolve_program;

    struct {
        GLuint color;
        GLuint normal;
    } textures;

    struct uniforms_block {
        vmath::mat4 mv_matrix;
        vmath::mat4 view_matrix;
        vmath::mat4 resolve_matrix;
    };

    GLuint uniforms_buffer;

    struct {
        GLint mvp;
    } uniforms;

    sb7::object object;

    GLuint fragment_buffer;
    GLuint head_pointer_image;
    GLuint atomic_counter_buffer;
    GLuint dummy_vao;
};

DECLARE_MAIN(my_application);
