#version 450

layout(binding = 0) uniform sampler2D screen;

layout(location = 0) out vec4 final_color;

float intensity(vec3 color) {
	return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main() {
	vec3 color = texelFetch(screen, ivec2(gl_FragCoord.xy), 0).rgb;
	if (intensity(color) < 0.6) {
	    color = vec3(0.0);
	}
	final_color = vec4(color, 1.0);
}
