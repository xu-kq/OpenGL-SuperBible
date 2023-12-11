//
// Created by xukq on 23/12/11.
//

#include "shader_compiler.h"


namespace Demo {
    Shader_Program::Shader_Program() : shader_file_map {
                           {GL_VERTEX_SHADER,           "../demo/vs.glsl"},
                           {GL_TESS_CONTROL_SHADER,     "../demo/tcs.glsl"},
                           {GL_TESS_EVALUATION_SHADER,  "../demo/tes.glsl"},
                           {GL_GEOMETRY_SHADER,         "../demo/gs.glsl"},
                           {GL_FRAGMENT_SHADER,         "../demo/fs.glsl"},
                       } {
        fp = fopen("Log_ShaderProgram.txt", "w");
        program = glCreateProgram();
        if (!program) {
            fprintf(fp, "[Create Program]:\n Error.\n");
        } else {
            fprintf(fp, "[Create Program]:\n Program created successful...\n");
        }
    }

    Shader_Program& Shader_Program::compile(GLenum ShaderType) {

        std::ifstream shader_file;
        shader_file.open(shader_file_map[ShaderType]);
        std::stringstream buffer;
        buffer << shader_file.rdbuf();
        std::string shader_code = buffer.str();
        const char* p_shader_code = shader_code.c_str();

        GLuint shaderID = glCreateShader(ShaderType);
        glShaderSource(shaderID, 1, &p_shader_code, NULL);
        glCompileShader(shaderID);

        GLint compiled_success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled_success);

        // Compiling Check
        fprintf(fp, "[Shader %s]:\n", shader_file_map[ShaderType].c_str());
        if (compiled_success != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(shaderID, 1024, &log_length, message);

            fprintf(fp, "%s", message);
        }
        else {
            fprintf(fp, "Shader compiled successful...\n");
        }

        glAttachShader(program, shaderID);
        return *this;
    }

    Shader_Program& Shader_Program::link() {
        glLinkProgram(program);

        GLint program_linked;
        glGetProgramiv(program, GL_LINK_STATUS, &program_linked);

        fprintf(fp, "[Link Program]:\n");
        if (program_linked != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(program, 1024, &log_length, message);

            fprintf(fp, "%s", message);
        }
        else {
            fprintf(fp, "Program linked successful!\n");
        }

        for(const auto&  shaderID: shaderID_queue) {
            glDeleteShader(shaderID);
        }
        return *this;
    }
    Shader_Program::~Shader_Program() {
        glDeleteProgram(program);
        fclose(fp);
    }
}
