#version 330 core

uniform vec4 shape_color;
out vec4 fragment_color;

void main()
{
    fragment_color = shape_color;
}
