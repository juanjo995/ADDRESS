#version 410

layout (location = 3) in vec3 position3D;
layout (location = 4) in vec3 velocity;

layout (location = 5) in float absoluteMagnitude;
layout (location = 6) in float colorIndex;
layout (location = 8) in float speedModule;

uniform float distanceLimit;
uniform vec3 cameraPos;
uniform float epoch;
uniform mat4 mModelViewProj;

uniform float lowerAbsMagFilter;
uniform float upperAbsMagFilter;

uniform float lowerColorIndexFilter;
uniform float upperColorIndexFilter;

uniform float lowerSpeedModuleFilter;
uniform float upperSpeedModuleFilter;

float PI     = 3.1415926535897932384626433832795;
float halfPI = 1.5707963267948966192313216916398;

out VS_OUT {
	vec4 tail;
	vec4 head;
	vec4 left;
	vec4 right;
	float z;
    float test;
	float alfa;
} vs_out;

void discart() {
	gl_Position = vec4(5.0f, 0.0f, 0.0f, 1.0f);
}

void main() {

	vec3 pos = position3D;
	pos = pos + velocity * epoch;

	vec4 projV = mModelViewProj * vec4(pos + normalize(velocity), 1.0);
	vec4 projP = mModelViewProj * vec4(pos, 1.0);

	vs_out.z = projP.z / projP.w;

	vs_out.tail = vec4(projP.x / projP.w, projP.y / projP.w, 0.0f, 1.0f);
	vs_out.head = vec4(projV.x / projV.w, projV.y / projV.w, 0.0f, 1.0f);

	vec2 rt = 0.05f * normalize(vec2(vs_out.head.x, vs_out.head.y) - vec2(vs_out.tail.x, vs_out.tail.y));

	// create left right point
	vec2 lp = vec2(-rt.y, rt.x);
	vec2 rp = vec2(rt.y, -rt.x);

	vs_out.left = vec4(0.1f * lp  + vec2(vs_out.tail.x, vs_out.tail.y) + rt * 0.666f, 0.0f, 1.0f);
	vs_out.right = vec4(0.1f * rp  + vec2(vs_out.tail.x, vs_out.tail.y) + rt * 0.666f, 0.0f, 1.0f);

	vs_out.head.x = rt.x + vs_out.tail.x;
	vs_out.head.y = rt.y + vs_out.tail.y;

	float dist = length(cameraPos - pos);

	if(absoluteMagnitude >= lowerAbsMagFilter && absoluteMagnitude <= upperAbsMagFilter
	&& colorIndex >= lowerColorIndexFilter && colorIndex <= upperColorIndexFilter
	&& speedModule >= lowerSpeedModuleFilter && speedModule <= upperSpeedModuleFilter
	&& dist < distanceLimit
	&& vs_out.z <= 1.0f && projP.z > 0.0f){ // projP.z must be positive, otherwise we'll be rendering speed vectors of stars behind the camera
		vs_out.test = 1.0f;
		vs_out.alfa = 1.0f - (dist / distanceLimit);
	} else {
		vs_out.test = 0.0f;
		vs_out.alfa = 1.0f;
	}

	gl_Position = projP;

}
