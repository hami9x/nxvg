#version 120
varying vec2 _texcoord;
uniform vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texc = texture2D(tex, _texcoord);

    if (texc != vec4(0) && color.r != 0) {
        gl_FragColor = texc;
        //gl_FragColor = vec4(color.rgb, smoothstep(0, 1, texc.a));
        //gl_FragColor = color;
	} else {
        discard;
    }
    //gl_FragColor = color.r/color.r*texc;
}
