#version 410

in vec4 color_to_fragment;

out vec4 color;

void main() {
    color = color_to_fragment;
}