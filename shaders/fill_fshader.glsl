#version 120
varying vec2 otexcoord;
uniform vec4 color;

uniform sampler2D tex;

void main()
{
	//float texA = texture(tex, Texcoord).a;
	vec4 texc = texture2D(tex, otexcoord);
	//float sa = smoothstep(0, 1, texc.a);
//	if (texc.a < 0 || texc.a > 0.75) {
//        discard;
//        return;
//	}
	//gl_FragColor = vec4(color.rgb, smoothstep(0.1, 0.5, texc.a));
//	if (texc.b > 0) {
//        gl_FragColor = vec4(0, 0, 1, 1);
//        return;
//	}

    if (texc.a > 0) {
        //smoothstep(0.6, 0.8, texc.a)
        gl_FragColor = vec4(color.rgb, smoothstep(0.75, 0.84, texc.a));
        //gl_FragColor = vec4(0, 0, 1, 1);
        return;
	} else {
        gl_FragColor = vec4(color.rgb, texc.a);
    }
}
