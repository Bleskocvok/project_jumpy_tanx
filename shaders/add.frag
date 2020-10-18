#version 450

layout(binding = 0) uniform sampler2D one;
layout(binding = 1) uniform sampler2D two;

layout(location = 0) out vec4 final_color;



void main() {
	vec3 color1 = texelFetch(one, ivec2(gl_FragCoord.xy), 0).rgb;
	vec3 color2 = texelFetch(two, ivec2(gl_FragCoord.xy), 0).rgb;
	final_color = vec4(color1 + color2, 1.0);
}
