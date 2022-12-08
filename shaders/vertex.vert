#version 410

layout(location=0)in vec3 position;
layout(location=1)in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color_to_fragment;

void main(){
    gl_Position=projection*view*model*vec4(position,1.);

    color_to_fragment=vec4(color,1.);
}