#version 410

in vec3 color;

out vec4 colorFragmento;

void main() {

    float dist = length(gl_PointCoord - vec2(0.5f)); // Distance to the center in pixels

    if(dist > 0.5f) { discard; }

    colorFragmento = vec4(color.rgb, 1.0f);

}