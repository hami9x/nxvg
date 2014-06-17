#version 120
varying vec2 _texcoord;
uniform vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texc = texture2D(tex, _texcoord);

    if (texc.a > 0) {
        gl_FragColor = vec4(color.rgb, smoothstep(0.75, 0.84, texc.a));
	} else {
        discard;
    }
}
