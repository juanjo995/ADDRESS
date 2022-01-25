#version 410

layout (location = 1) in vec2 position;
layout (location = 2) in float distance;
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

uniform float plot2DSizeFactor;
uniform vec2 offset;
uniform float aspectRatio;

uniform float sizeFactor;
uniform float sizeOffset;
uniform float sizeSlope;

float PI    = 3.1415926535897932384626433832795;   /* reasonable value for pi */
float TwoPI = 6.283185307179586476925286766559;	/* float pi! */
float e = 2.7182818284590452353602874713526;

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

    color = unpackUnorm4x8(id).xyz;

    float x;

    if(position.x > PI) {
        x = ((-position.x + PI) * 0.15915494309189533 * 2.0f) + 1.0f;
    } else {
        x = ((-position.x - PI) * 0.15915494309189533 * 2.0f) + 1.0f;
    }

    float y = position.y * 0.63661977236758134 * 0.5f * aspectRatio;

    x += offset.x;
    y += offset.y;

    x *= plot2DSizeFactor;
    y *= plot2DSizeFactor;

    appMag = 5.0f * (log(distance / 10.0f) / log(10.0f)) + absoluteMagnitude;
    float thisRadius = selectSizeFuncUniform();

    gl_PointSize = thisRadius;

    if(thisRadius > 0.0f && absoluteMagnitude >= lowerAbsMagFilter && absoluteMagnitude <= upperAbsMagFilter
    && colorIndex >= lowerColorIndexFilter && colorIndex <= upperColorIndexFilter
    && speedModule >= lowerSpeedModuleFilter && speedModule <= upperSpeedModuleFilter
    ) {
        gl_Position = vec4(x, y, 0.0f, 1.0f);
    } else {
        discart();
    }


}
