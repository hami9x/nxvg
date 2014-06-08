#version 120
varying vec2 otexcoord;
varying vec2 pos;
//uniform float cornersx[2];
//uniform float cornersy[2];

void main()
{
    float u = otexcoord.x;
    float v = otexcoord.y;
    float d = 0, mind=1;
    if (u*u - v < 0) {
        gl_FragColor = vec4(0, 0, 0, 1);
//        for (int i=0; i<2; i++) {
//            vec2 corner = vec2(cornersx[i], cornersy[i]);
//            d = distance(pos, corner);
//            if (d < mind) {
//               gl_FragColor.b = 1-d;
//               mind=d;
//            }
//        }
//        gl_FragColor.b = 0;
    } else {
        discard;
    }
}
