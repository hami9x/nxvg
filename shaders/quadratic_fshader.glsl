#version 120
varying vec2 otexcoord;

void main()
{
    float u = otexcoord.x;
    float v = otexcoord.y;
    if (u*u - v < 0) {
        gl_FragColor = vec4(1, 1, 1, 1);
    } else {
        discard;
    }
}
