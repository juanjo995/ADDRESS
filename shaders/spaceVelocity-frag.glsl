#version 410

in float alfaValue;
out vec4 colorFragmento;

void main() {
	colorFragmento = vec4(1.0f, 1.0f, 0.0f, alfaValue);
}
