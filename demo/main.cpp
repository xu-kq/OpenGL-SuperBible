//
// Created by xukq on 23/12/10.
//

#include "sb7.h"
#include "shader_compiler.h"

GLuint compile_shaders();

class my_application : public sb7::application {
public:
    void render(double currentTime) override {
        const GLfloat green[] = {0.0f, 0.25f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, green);

        glUseProgram(rendering_program);

        GLfloat attrib[] = {
            float(sin(currentTime)) * 0.5f,
            float(cos(currentTime)) * 0.6f,
            0.f,
            0.f
        };
        glVertexAttrib4fv(0, attrib);
        glDrawArrays(GL_PATCHES, 0, 3);
    }

    void startup() override {
        sp_program.reset(new Demo::Shader_Program);

        sp_program->compile(GL_VERTEX_SHADER)
                .compile(GL_TESS_CONTROL_SHADER)
                .compile(GL_TESS_EVALUATION_SHADER)
                .compile(GL_FRAGMENT_SHADER)
                .link();
        rendering_program = sp_program->get();

        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void shutdown() override {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteProgram(rendering_program);
    }

private:
    GLuint rendering_program;
    GLuint vertex_array_object;
    std::shared_ptr<Demo::Shader_Program> sp_program;

};

DECLARE_MAIN(my_application);