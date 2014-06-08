//uniform float weight[5] = float[](0.16, 0.15, 0.12, 0.09, 0.05);
#version 120
uniform sampler2D tex; // the texture with the scene you want to blur
varying vec2 otexcoord;
uniform vec2 resolution;
uniform float weight[5] = float[](1./9, 1./9, 1./9, 1./9, 1./9);
//uniform float weight[5] = float[](0.16, 0.15, 0.13, 0.10, 0.07);

float blurSize = 1/resolution.y;

vec4 blurstep(float offx, float offy, float b) {
    int offs = int(max(abs(offx), abs(offy)));
    float multfactor = 1 - b*0*offs;
    return texture2D(tex, vec2(otexcoord.x+blurSize*offx, otexcoord.y+blurSize*offy))*weight[offs];
}

void main(void)
{
    vec4 sum = vec4(0.0);

    // blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    for (int i=-10; i<=10; i++) {
        sum += blurstep(i, 0, texture2D(tex, vec2(otexcoord.x, otexcoord.y)).b);
    }
//    sum += texture2D(RTScene, vec2(otexcoord.x - 4.0*blurSize, otexcoord.y));
//    sum += texture2D(RTScene, vec2(otexcoord.x - 3.0*blurSize, otexcoord.y));
//    sum += texture2D(RTScene, vec2(otexcoord.x - 2.0*blurSize, otexcoord.y));
//    sum += texture2D(RTScene, vec2(otexcoord.x - blurSize, otexcoord.y));
//
//    sum += texture2D(RTScene, vec2(otexcoord.x + blurSize, otexcoord.y));
//    sum += texture2D(RTScene, vec2(otexcoord.x + 2.0*blurSize, otexcoord.y));
//    sum += texture2D(RTScene, vec2(otexcoord.x + 3.0*blurSize, otexcoord.y));
//    sum += texture2D(RTScene, vec2(otexcoord.x + 4.0*blurSize, otexcoord.y));

//    vec4 ccol = texture2D(RTScene, vec2(otexcoord.x, otexcoord.y));
//    float factor = 7*ccol.b + 1;
//    if (factor > 1) {
//        discard;
//    }
//    gl_FragColor = (ccol*factor + sum)/(factor+8);
//    gl_FragColor.b = ccol.b;

    gl_FragColor = sum;
}
