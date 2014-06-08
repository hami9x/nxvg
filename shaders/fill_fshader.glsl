#version 120
varying vec2 otexcoord;
uniform vec4 color;

uniform sampler2D tex;

void main()
{
	//float texA = texture(tex, Texcoord).a;
	vec4 texc = texture2D(tex, otexcoord);
	//float sa = smoothstep(0, 1, texc.a);
	gl_FragColor = vec4(color.rgb, smoothstep(0.9, 0.98, texc.a));
}
