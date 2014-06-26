#version 130
#include "shaders/fs_smaa_common.h"

in vec2 _texcoord;
in vec2 _pos;
in vec4 _offset[3];
in vec2 _pixcoord;
out vec4 outColor;

uniform sampler2D edgesTex;
uniform sampler2D areaTex;
uniform sampler2D searchTex;

void main() {
    outColor = SMAABlendingWeightCalculationPS(_texcoord, _pixcoord, _offset, edgesTex, areaTex, searchTex, ivec4(0));
    //outColor = texture2D(edgesTex, _texcoord);
}
