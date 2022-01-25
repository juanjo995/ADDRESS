#version 410

layout (points) in;
layout (line_strip, max_vertices = 5) out;

in VS_OUT {
	vec2 position;
	float ar;
} vs_in[];

uniform float radius;

void main() {

	float x = vs_in[0].position.x;
	float y = vs_in[0].position.y;

	float size = radius;
	float a = size;
	float b = size * vs_in[0].ar;

	gl_Position = vec4(x - a, y + b, 0.0f, 1.0f); // top left
	EmitVertex();

	gl_Position = vec4(x + a, y + b, 0.0f, 1.0f); // top right
	EmitVertex();

	gl_Position = vec4(x + a, y - b, 0.0f, 1.0f); // bottom right
	EmitVertex();

	gl_Position = vec4(x - a, y - b, 0.0f, 1.0f); // bottom left
	EmitVertex();

	gl_Position = vec4(x - a, y + b, 0.0f, 1.0f); // top left
	EmitVertex();

	EndPrimitive();

}