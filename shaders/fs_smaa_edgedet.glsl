#version 130
#include "shaders/fs_smaa_common.h"

in vec2 _texcoord;
in vec2 _pos;
in vec4 _offset[3];
out vec4 outColor;

uniform sampler2D tex;

void main() {
    //outColor = vec4(1, 0, 0, 1);
    outColor.rg = SMAALumaEdgeDetectionPS(_texcoord, _offset, tex).xy;
    outColor.a = 1;
    //outColor = texture2D(tex, _texcoord);
}
