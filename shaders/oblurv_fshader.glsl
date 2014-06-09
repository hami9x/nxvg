#version 120
uniform sampler2D tex; // the texture with the scene you want to blur
varying vec2 otexcoord;
uniform vec2 resolution;

float blurSize = 1/resolution.y;

float blurstep(float offx, float offy) {
    int offs = int(max(abs(offx), abs(offy)));
    vec4 texc = texture2D(tex, vec2(otexcoord.x+blurSize*offx, otexcoord.y+blurSize*offy));
//    if (texc.b >= 0.96 && offs > 2) {
//        return 0.;
//    }
    return texc.a != 0 ? 0.1 : 0.;
}

void main(void)
{
    float sum = 0;

    for (int i=-3; i<=3; i++) {
        sum += blurstep(0, i);
    }

    //gl_FragColor.b = 0.5;
    gl_FragColor = vec4(0, 0, 0, sum);
}
