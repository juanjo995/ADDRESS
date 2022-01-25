#version 410

layout (points) in;
layout (line_strip, max_vertices = 5) out;

out float alfaValue;

in VS_OUT {
	vec4 tail;
	vec4 head;
	vec4 left;
	vec4 right;
	float z;
	float test;
	float alfa;
} vs_in[];

void main() {
	if(vs_in[0].test == 1.0f) {

		alfaValue = vs_in[0].alfa;

		gl_Position = vs_in[0].tail;
		EmitVertex();

		gl_Position = vs_in[0].head;
		EmitVertex();

		gl_Position = vs_in[0].left;
		EmitVertex();

		EndPrimitive();

		gl_Position = vs_in[0].head;
		EmitVertex();

		gl_Position = vs_in[0].right;
		EmitVertex();

		EndPrimitive();
	}

}