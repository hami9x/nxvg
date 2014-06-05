#version 120
uniform sampler2D RTBlurH; // the texture with the scene you want to blur
varying vec2 otexcoord;
uniform vec2 resolution;

float blurSize = 1/resolution.x;

void main(void)
{
   vec4 sum = vec4(0.0);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y - 4.0*blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y - 3.0*blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y - 2.0*blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y - blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y + blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y + 2.0*blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y + 3.0*blurSize));
   sum += texture2D(RTBlurH, vec2(otexcoord.x, otexcoord.y + 4.0*blurSize));

   gl_FragColor = sum/9;
}
