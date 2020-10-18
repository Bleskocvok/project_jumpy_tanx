#version 450

layout(location = 1) uniform mat4 model;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec3 fs_position;
layout(location = 2) out vec2 fs_tex_coord;


void main() {
	fs_position = vec3(model * vec4(position, 1.0));
	fs_tex_coord = tex_coord;
	
    gl_Position = model * vec4(position, 1.0);
}

