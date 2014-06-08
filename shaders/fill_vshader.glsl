#version 120

attribute vec2 position;
attribute vec2 texcoord;
varying vec2 otexcoord;
varying vec2 pos;

void main()
{
    otexcoord = texcoord;
    pos = position;
    gl_Position = vec4(position, 0, 1.0);
}
