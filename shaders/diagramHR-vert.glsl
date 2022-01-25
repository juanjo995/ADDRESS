#version 410

layout (location = 1) in vec2 position;
layout (location = 2) in float distance;
layout (location = 7) in vec3 starColor;
layout (location = 5) in float absoluteMagnitude;
layout (location = 6) in float colorIndex;
layout (location = 8) in float speedModule;

out vec3 color;
out float alfa;

uniform float diagramHRSizeFactor;
uniform vec2 offset;
uniform float aspectRatio;

uniform float lowerAbsMagFilter;
uniform float upperAbsMagFilter;

uniform float lowerColorIndexFilter;
uniform float upperColorIndexFilter;

uniform float lowerSpeedModuleFilter;
uniform float upperSpeedModuleFilter;

uniform float sizeAtt;
uniform float alfaAtt;
uniform float fixedSize;

uniform float sizeFactor;
uniform float sizeOffset;
uniform float sizeSlope;

uniform float alfaFactor;
uniform float alfaOffset;
uniform float alfaSlope;

float thisRadius = 0.0f;
float starDistance = 1.0f;
float appMag = 1.0f;

float e = 2.7182818284590452353602874713526;

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

    float x = colorIndex * 0.1;
    float y = -absoluteMagnitude * aspectRatio * 0.3f * 0.1f;

    x += offset.x;
    y += offset.y;

    x *= diagramHRSizeFactor;
    y *= diagramHRSizeFactor;

    starDistance = distance;
    appMag = 5.0f * (log(starDistance / 10.0f) / log(10.0f)) + absoluteMagnitude;
    thisRadius = selectSizeFuncUniform();
    alfa = selectAlfaFuncUniform();
    color = starColor;

    gl_PointSize = thisRadius;

    if(absoluteMagnitude >= lowerAbsMagFilter && absoluteMagnitude <= upperAbsMagFilter
        && colorIndex >= lowerColorIndexFilter && colorIndex <= upperColorIndexFilter
    && speedModule >= lowerSpeedModuleFilter && speedModule <= upperSpeedModuleFilter)
    {
        gl_Position = vec4(x, y, 0.0f, 1.0f);
    } else {
        discart();
    }

}
