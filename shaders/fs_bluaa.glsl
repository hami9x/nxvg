#version 120
uniform sampler2D tex; // the texture with the scene you want to blur
varying vec2 _texcoord;
uniform vec2 resolution;
uniform int mode; //mode 1: vertical outline, 2: horizontal outline, 3: vertical blur, 4: horizonal blur

float blurSize = 1/resolution.y;
int radius = (mode < 3) ? 3 : 4;
bool vert = (mode == 1 || mode == 3) ? true : false;
bool solid = (mode < 3) ? true : false;

float blurstep(float offx, float offy) {
    int offs = int(max(abs(offx), abs(offy)));
    vec4 texc = texture2D(tex, vec2(_texcoord.x+blurSize*offx, _texcoord.y+blurSize*offy));
    if (solid) {
        return texc.a != 0 ? 1.0 : 0;
    }
    return texc.a * 1/9;
}

void main(void)
{
    float sum = 0;

    for (int i=-radius; i<=radius; i++) {
        if (vert) {
            sum += blurstep(0, i);
        } else {
            sum += blurstep(i, 0);
        }
    }

    gl_FragColor = vec4(0, 0, 0, sum);
}
