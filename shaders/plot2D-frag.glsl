#version 410

in vec3 color;
in float alfa;

out vec4 colorFragmento;

uniform float haloFactor;
uniform float haloOffset;
uniform float haloSlope;

float e = 2.71828182845904523536028747;

float dist = 0.0f;

subroutine float selectHaloFunc();
subroutine uniform selectHaloFunc selectHaloFuncUniform;

subroutine(selectHaloFunc)
float noHalo() {
    return 1.0f;
}

subroutine(selectHaloFunc)
float getHalo() {
    return haloFactor * pow(e, -(pow(dist - haloOffset , 2.0f) / (2.0f * pow(haloSlope, 2.0f))));
}

void main() {

    dist = length(gl_PointCoord - vec2(0.5f)); // Distance to the center in pixels

    if(dist > 0.5f) { discard; }

    float transparency = alfa * selectHaloFuncUniform();

    colorFragmento = vec4(color.rgb, transparency);

}