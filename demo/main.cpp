//
// Created by xukq on 23/12/10.
//
#include <memory>
#include <sb7.h>
#include <vmath.h>

#include <object.h>
#include <sb7ktx.h>
#include "shader_compiler.h"

static unsigned int seed = 0x13371337;

static inline float random_float() {
    float res;
    unsigned int tmp;

    seed *= 16807;

    tmp = seed ^ (seed >> 4) ^ (seed << 15);

    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

class my_application : public sb7::application {
public:
    void render(double currentTime) override {
        static const GLfloat black[] = {0.0f, 0.0f, 0.0f, 0.0f};
        float t = (float) currentTime;

        glViewport(0, 0, info.windowWidth, info.windowHeight);
        glClearBufferfv(GL_COLOR, 0, black);

        glUseProgram(program);

        // ctor
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, rain_buffer, 0, 256 * sizeof(vmath::vec4));
        // assign value
        vmath::vec4* droplet = (vmath::vec4 *) glMapNamedBufferRange(rain_buffer, 0, 256 * sizeof(vmath::vec4),
                                                                     GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);


        for (int i = 0; i < 256; ++i) {
            droplet[i][0] = droplet_x_offset[i];
            droplet[i][1] = 2.0f - fmodf((t + float(i)) * droplet_fall_speed[i], 4.31f);
            droplet[i][2] = t * droplet_rot_speed[i];
            droplet[i][3] = 0.0f;
        }
        glUnmapNamedBuffer(rain_buffer);

        for (int alien_index = 0; alien_index < 256; ++alien_index) {
            glVertexAttribI1i(0, alien_index);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    void startup() override {
        sp_program.reset(new Demo::Shader_Program());
        sp_program->compile(GL_VERTEX_SHADER)
                .compile(GL_FRAGMENT_SHADER)
                .link();
        program = sp_program->get();


        GLuint tex_alien_array = sb7::ktx::file::load("media/textures/aliens.ktx");
        glBindTextureUnit(0, tex_alien_array);
        glTextureParameteri(tex_alien_array, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glCreateBuffers(1, &rain_buffer);
        // alloc
        glNamedBufferStorage(rain_buffer, 256 * sizeof(vmath::vec4), NULL, GL_MAP_WRITE_BIT);


        for (int i = 0; i < 256; ++i) {
            droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
            droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
            droplet_fall_speed[i] = random_float() + 0.2f;
        }

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }


    void shutdown() override {
        glDeleteProgram(program);
    }

private:
    GLuint vao;
    GLuint program;
    std::shared_ptr<Demo::Shader_Program> sp_program;

    GLuint rain_buffer;

    float droplet_x_offset[256];
    float droplet_rot_speed[256];
    float droplet_fall_speed[256];
};

DECLARE_MAIN(my_application);
