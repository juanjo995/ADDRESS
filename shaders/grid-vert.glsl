#version 410

layout (location = 0) in vec3 position;

uniform mat4 mModelViewProj;
uniform vec4 color;

out vec4 outColor;

void main() {
    outColor = color;
    gl_Position = mModelViewProj * vec4(position, 1.0);

}
