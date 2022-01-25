#version 410

layout (location = 0) in vec3 position;

uniform mat4 mModelViewProj;

out vec3 pos;

void main() {

    pos = position;

    gl_Position = mModelViewProj * vec4(position, 1.0);

}
