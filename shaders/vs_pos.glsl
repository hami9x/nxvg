#version 120

attribute vec2 position;
varying vec2 _pos;

void main()
{
    _pos = position;
    gl_Position = vec4(position, 0.0, 1.0);
}
