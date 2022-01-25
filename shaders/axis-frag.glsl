#version 410

out vec4 colorFragmento;

in vec3 pos;

void main() {

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;

    if(pos.x > 0.0f) r = 1.0f;
    else if(pos.y > 0.0f) g = 1.0f;
    else if(pos.z > 0.0f) b = 1.0f;

    colorFragmento = vec4(r, g, b, 1.0f);
}