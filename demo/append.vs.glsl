#version 450 core

layout (location = 0) in vec4 position;

layout (location = 0) uniform mat4 mvp;

out VS_OUT
{
    vec4 pos;
    vec4 color;
} vs_out;


void main()
{
    vec4 p = mvp * position;

    gl_Position = p;
    vs_out.color = vec4(1.0);
    vs_out.pos = p / p.w;
}