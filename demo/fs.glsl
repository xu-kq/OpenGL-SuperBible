#version 450 core

out vec4 color;

in VS_OUT
{
   vec4 color;
} fs_in;

void main(void)
{
   color = vec4(fs_in.color.xyz, 1.0);
}