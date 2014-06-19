#version 120

attribute vec2 position;
attribute vec2 texcoord;
varying vec2 _texcoord;
varying vec2 _pos;

void main()
{
    _texcoord = texcoord;
    _pos = position;
    gl_Position = vec4(position, 0.0, 1.0);
}
