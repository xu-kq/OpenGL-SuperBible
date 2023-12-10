//
// Created by xukq on 23/12/10.
//

#include "sb7.h"

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
        rendering_program = compile_shaders();
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
};

DECLARE_MAIN(my_application);

GLuint compile_shaders() {
    GLuint vertex_shader;
    GLuint tcs;
    GLuint tes;
    GLuint fragment_shader;
    GLuint program;


    static const GLchar* vertex_shader_source[] = {
        "#version 450 core                                                                  \n"
        "layout(location = 0) in vec4 offset;                                               \n"
        "void main(void) {                                                                  \n"
        "   const vec4 vertices[] = vec4[](vec4(0.25, -0.25, 0.5, 1.0),                     \n"
        "                                   vec4(-0.25, -0.25, 0.5, 1.0),                   \n"
        "                                   vec4(0.25, 0.25, 0.5, 1.0));                    \n"
        "   gl_Position = vertices[gl_VertexID] + offset;                                   \n"
        "}                                                                                  \n"
    };

    static const GLchar* tessellation_control_shader_source[] = {
        "#version 450 core                                                                  \n"
        "layout(vertices = 3) out;                                                          \n"
        "void main() {                                                                      \n"
        "   if(gl_InvocationID == 0) {                                                      \n"
        "       gl_TessLevelInner[0] = 5.0;                                                 \n"
        "       gl_TessLevelOuter[0] = 5.0;                                                 \n"
        "       gl_TessLevelOuter[1] = 5.0;                                                 \n"
        "       gl_TessLevelOuter[2] = 5.0;                                                 \n"
        "   }                                                                               \n"
        "   gl_out[gl_InvocationID].gl_Position =  gl_in[gl_InvocationID].gl_Position;      \n"
        "}                                                                                  \n"
    };
    static const GLchar* tessellation_evaluation_shader_source[] = {
        "#version 450 core                                                                  \n"
        "layout(triangles, equal_spacing, cw) in;                                           \n"
        "                                                                                   \n"
        "void main() {                                                                      \n"
        "   gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                         \n"
        "                   (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
        "                   (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
        "}                                                                                  \n"
    };

    static const GLchar* fragment_shader_source[] = {
        "#version 450 core                                                                  \n"
        "out vec4 color;                                                                    \n"
        "                                                                                   \n"
        "void main(void)                                                                    \n"
        "{                                                                                  \n"
        "   color = vec4(0.0, 0.8, 1.0, 1.0);                                               \n"
        "}                                                                                  \n"
    };

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tcs, 1, tessellation_control_shader_source, NULL);
    glCompileShader(tcs);
    tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tes, 1, tessellation_evaluation_shader_source, NULL);
    glCompileShader(tes);


    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, tcs);
    glAttachShader(program, tes);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    {
        GLint vertex_compiled;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);

        FILE* fp = fopen("log.txt", "w");
        fprintf(fp, "Vertex Shader:\n");

        if (vertex_compiled != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(vertex_shader, 1024, &log_length, message);

            fprintf(fp, "%s", message);
        }
        fclose(fp);
    }

    {
        {
            GLint tcs_compiled;
            glGetShaderiv(tcs, GL_COMPILE_STATUS, &tcs_compiled);

            FILE* fp = fopen("log.txt", "a");
            fprintf(fp, "Tessellation Control Shader:\n");
            if (tcs_compiled != GL_TRUE)
            {
                GLsizei log_length = 0;
                GLchar message[1024];
                glGetShaderInfoLog(vertex_shader, 1024, &log_length, message);

                fprintf(fp, "%s", message);
            }
            fclose(fp);
        }

        {
            GLint tes_compiled;
            glGetShaderiv(tes, GL_COMPILE_STATUS, &tes_compiled);
            FILE* fp = fopen("log.txt", "a");
            fprintf(fp, "Tessellation Evaluation Shader:\n");

            if (tes_compiled != GL_TRUE)
            {
                GLsizei log_length = 0;
                GLchar message[1024];
                glGetShaderInfoLog(tes, 1024, &log_length, message);

                fprintf(fp, "%s", message);
            }
            fclose(fp);
        }
    }

    {
        GLint frag_compiled;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &frag_compiled);
        FILE* fp = fopen("log.txt", "a");
        fprintf(fp, "Fragment Shader:\n");

        if (frag_compiled != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(fragment_shader, 1024, &log_length, message);

            fprintf(fp, "%s", message);
        }
        fclose(fp);
    }

    {
        GLint program_linked;
        glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
        FILE* fp = fopen("log.txt", "a");

        fprintf(fp, "Link Program:\n");
        if (program_linked != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(program, 1024, &log_length, message);

            fprintf(fp, "%s", message);
        }
        fclose(fp);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(tcs);
    glDeleteShader(tes);
    glDeleteShader(fragment_shader);

    return program;
}
