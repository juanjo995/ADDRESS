#version 410

layout (location = 1) in vec2 position;
layout (location = 2) in float distance;
layout (location = 3) in vec3 position3D;
layout (location = 7) in vec3 starColor;
layout (location = 5) in float absoluteMagnitude;
layout (location = 6) in float colorIndex;
layout (location = 4) in vec3 velocity;
layout (location = 8) in float speedModule;

out vec3 color;
out float nMag;
out float alfa;

uniform vec3 cameraPos;

uniform mat4 mModelViewProj;

uniform float epoch;

uniform float time;
uniform float magFilter;

uniform float apMagModifier;
uniform float absMagModifier;

uniform float lowerAbsMagFilter;
uniform float upperAbsMagFilter;

uniform float lowerColorIndexFilter;
uniform float upperColorIndexFilter;

uniform float lowerSpeedModuleFilter;
uniform float upperSpeedModuleFilter;

uniform float AValue;
uniform float BValue;

uniform float sizeAtt;
uniform float alfaAtt;
uniform float fixedSize;

uniform float sizeFactor;
uniform float sizeOffset;
uniform float sizeSlope;

uniform float alfaFactor;
uniform float alfaOffset;
uniform float alfaSlope;

float PI    = 3.1415926535897932384626433832795;   /* reasonable value for pi */
float TwoPI = 6.283185307179586476925286766559;	/* float pi! */
float PI2 = 1.5707963267948966192313216916398;   /* half pi */
float PI4 = 0.7853981633974483096156608458198; /* pi over 4 */
float Rads  = 0.01745329251994329576923690768489;  /* radians to degrees and back */
float e = 2.7182818284590452353602874713526;

float thisRadius = 0.0f;
float distanceToCamera = 1.0f;
float appMag = 1.0f;

subroutine float selectAlfaFunc();
subroutine uniform selectAlfaFunc selectAlfaFuncUniform;

subroutine(selectAlfaFunc)
float noAlfa() {
    return 1.0f;
}

subroutine(selectAlfaFunc)
float distanceBasedAlfa() {

    if(appMag - alfaOffset < 0.0f) {
        return alfaFactor;
    } else {
        return alfaFactor * pow(e, -pow((appMag - alfaOffset),2.0f)/(2.0f * pow(alfaSlope, 2.0f)));
    }

}

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

    vec3 pos = position3D;

    pos = pos + velocity * epoch;

    distanceToCamera = length(cameraPos - pos);
    appMag = 5.0f * (log(distanceToCamera / 10.0f) / log(10.0f)) + absoluteMagnitude;

    thisRadius = selectSizeFuncUniform();

    alfa = selectAlfaFuncUniform();

    color = starColor;

    if(thisRadius > 0.0f && absoluteMagnitude >= lowerAbsMagFilter && absoluteMagnitude <= upperAbsMagFilter
    && colorIndex >= lowerColorIndexFilter && colorIndex <= upperColorIndexFilter
    && speedModule >= lowerSpeedModuleFilter && speedModule <= upperSpeedModuleFilter
    ) {

    gl_PointSize = thisRadius;

    gl_Position = mModelViewProj * vec4(pos, 1.0);

    }
    else {
        discart();
    }

}
