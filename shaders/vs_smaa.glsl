#version 130
#define SMAA_INCLUDE_PS 0
#include "shaders/fs_smaa_common.h"
in vec2 position;
in vec2 texcoord;
out vec2 _texcoord;
out vec2 _pos;
out vec4 _offset[3];
out vec2 _pixcoord;
uniform int mode;

void main()
{
    _texcoord = texcoord;
    _pos = position;
    gl_Position = vec4(position, 0.0, 1.0);
    if (mode == 0) {
        SMAAEdgeDetectionVS(texcoord, _offset);
    } else if (mode == 1) {
        SMAABlendingWeightCalculationVS(texcoord, _pixcoord, _offset);
    } else if (mode == 2) {
        SMAANeighborhoodBlendingVS(texcoord, _offset[0]);
    }
}

