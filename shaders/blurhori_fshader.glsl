//uniform float weight[5] = float[](0.16, 0.15, 0.12, 0.09, 0.05);
#version 120
uniform sampler2D tex; // the texture with the scene you want to blur
varying vec2 otexcoord;
uniform vec2 resolution;

float blurSize = 1/resolution.y;

float blurstep(float offx, float offy) {
    int offs = int(max(abs(offx), abs(offy)));
    vec4 texc = texture2D(tex, vec2(otexcoord.x+blurSize*offx, otexcoord.y+blurSize*offy));
    return texc.a * 1/9;
}

void main(void)
{
    float sum = 0;

    // blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    for (int i=-4; i<=4; i++) {
        sum += blurstep(i, 0);
    }
    //}
    gl_FragColor = vec4(0, 0, 0, sum);
}
