//
// Created by xukq on 23/12/11.
//
#pragma once
#include <memory>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "sb7.h"

namespace Demo {
    class Shader_Program {
    public:
        Shader_Program();

        Shader_Program& compile(GLenum ShaderType);

        Shader_Program& link();

        ~Shader_Program();

        GLuint get() {
            return program;
        }

    private:
        GLuint program;
        std::unordered_map<GLenum, std::string> shader_file_map;
        FILE* fp;
        std::vector<GLuint> shaderID_queue;
    };
}
