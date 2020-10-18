#version 450

layout(binding = 0) uniform sampler2D mat_texture;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_tex_coord;

layout(location = 0) out vec4 final_color;

void main() {
	vec4 tex_color = texture(mat_texture, fs_tex_coord);
	final_color = tex_color;// + vec4(0.8, 0.8, 0.0, 0.3);
}


