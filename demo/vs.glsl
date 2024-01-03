#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 offset;

out Fragment{
    vec4 color;
} fragment;


void main(void) {
    gl_Position =(position + offset)  * vec4(0.25, 0.25, 1.0, 1.0);
    fragment.color = color;
}