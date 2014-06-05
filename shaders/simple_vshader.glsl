#version 120

attribute vec2 position;

void main()
{
    /*gl_Position = vec4(vec2(position.x*2-1, -position.y*2+1), 0.0, 1.0);*/
    gl_Position = vec4(position, 0.0, 1.0);
}
