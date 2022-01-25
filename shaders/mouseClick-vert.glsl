#version 410

layout (location = 1) in vec2 position;
layout (location = 2) in float distance;
layout (location = 3) in vec3 position3D;
layout (location = 5) in float absoluteMagnitude;
layout (location = 0) in uint id;

layout (location = 6) in float colorIndex;
layout (location = 4) in vec3 velocity;
layout (location = 8) in float speedModule;

out vec3 color;

uniform mat4 mModelViewProj;

uniform float apMagModifier;
uniform float absMagModifier;

uniform float lowerAbsMagFilter;
uniform float upperAbsMagFilter;

uniform float lowerApMagFilter;
uniform float upperApMagFilter;

uniform float lowerColorIndexFilter;
uniform float upperColorIndexFilter;

uniform float lowerSpeedModuleFilter;
uniform float upperSpeedModuleFilter;

uniform vec3 cameraPos;

uniform float potencia;

uniform float epoch;

uniform float fixedSize;
float distanceToCamera = 1.0f;
float appMag = 1.0f;

uniform float sizeFactor;
uniform float sizeOffset;
uniform float sizeSlope;

float e = 2.71828182845904523536028747;

subroutine float selectSizeFunc();
subroutine uniform selectSizeFunc selectSizeFuncUniform;

subroutine(selectSizeFunc)
float fixedSizeFunc() {
    return fixedSize;
}

subroutine(selectSizeFunc)
float distanceBasedSizeFunc() {

    float pseudoSize;
    if(appMag - sizeOffset < 0.0f) {
        pseudoSize = sizeFactor;
    } else {
        pseudoSize = sizeFactor * pow(e, -pow((appMag - sizeOffset),2.0f)/(2.0f * pow(sizeSlope, 2.0f))) + 2.2;
    }

    float size = pseudoSize;

    if(size < 2.2f) {
        size = 2.2f;
    }

    return size;

}

void discart() {
    gl_Position = vec4(5.0f, 0.0f, 0.0f, 1.0f);
}

void main() {

uint r = (id << 24) >> 24;
uint g = ((id >> 8) << 24) >> 24;
uint b = ((id >> 16) << 24) >> 24;

    color = unpackUnorm4x8(id).xyz;

    vec3 pos = position3D;

    pos = pos + velocity * epoch;

    distanceToCamera = length(cameraPos - pos);
    appMag = 5.0f * (log(distanceToCamera / 10.0f) / log(10.0f)) + absoluteMagnitude;

    float thisRadius = selectSizeFuncUniform();

    if(thisRadius > 0.0f && absoluteMagnitude >= lowerAbsMagFilter && absoluteMagnitude <= upperAbsMagFilter
    && colorIndex >= lowerColorIndexFilter && colorIndex <= upperColorIndexFilter
    && speedModule >= lowerSpeedModuleFilter && speedModule <= upperSpeedModuleFilter
    ) {
        gl_PointSize = thisRadius;
        gl_Position = mModelViewProj * vec4(pos, 1.0);
    } else {
        discart();
    }

}
