#version 120

attribute vec2 position;
attribute vec2 texcoord;
varying vec2 otexcoord;

void main()
{
    otexcoord = texcoord;
    gl_Position = vec4(position, 0, 1.0);
}
