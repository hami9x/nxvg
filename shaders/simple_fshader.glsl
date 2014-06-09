#version 120
uniform vec4 color;
varying vec2 pos;
uniform float cornersx[3];
uniform float cornersy[3];

void main()
{
	gl_FragColor = color;
	if (color.rgb == vec3(1, 0, 0)) {
        return;
	}
    float d = 0, mind=1;
    for (int i=0; i<3; i++) {
        vec2 corner = vec2(cornersx[i], cornersy[i]);
        d = distance(pos, corner);
        if (d < mind) {
           gl_FragColor.b = 1-d;
           mind=d;
        }
    }
}
