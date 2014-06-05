#version 120
uniform sampler2D RTScene; // the texture with the scene you want to blur
varying vec2 otexcoord;
uniform vec2 resolution;

float blurSize = 1/resolution.y;

void main(void)
{
   vec4 sum = vec4(0.0);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(RTScene, vec2(otexcoord.x - 4.0*blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x - 3.0*blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x - 2.0*blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x - blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x + blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x + 2.0*blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x + 3.0*blurSize, otexcoord.y));
   sum += texture2D(RTScene, vec2(otexcoord.x + 4.0*blurSize, otexcoord.y));

   gl_FragColor = sum/9;
}
