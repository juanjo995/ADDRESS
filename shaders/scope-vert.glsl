#version 410

uniform vec2 pos;
uniform float aspectRatio;

out VS_OUT {
	vec2 position;
	float ar;
} vs_out;

void main() {
	vs_out.position.x = pos.x;
	vs_out.position.y = pos.y;
	vs_out.ar = aspectRatio;
}
