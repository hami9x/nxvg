#version 120
varying vec2 _texcoord;

void main()
{
    float u = _texcoord.x;
    float v = _texcoord.y;
    if (u*u - v < 0) {
        gl_FragColor = vec4(0, 0, 0, 1);
    } else {
        discard;
    }
}
