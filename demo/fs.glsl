#version 450 core


layout (binding =  0, rgba32ui) readonly uniform uimage2D image_in;

layout (binding = 1) uniform writeonly uimage2D image_out;

void main(void)
{
    ivec2 P = ivec2(gl_FragCoord.xy);

    uvec4 data = imageLoad(image_in, P);

    imageStore(image_out, P, data);
}