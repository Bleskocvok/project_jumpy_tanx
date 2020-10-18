#version 450


struct Light {
	vec4 pos;
	vec4 amb;
	vec4 diff;
	vec4 spec;
};

layout(binding = 0, std430) buffer Lights {
	Light lights[];
};



layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 model;
layout(location = 2) uniform mat4 view;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_tex_coord;


void main() {
	fs_position = vec3(model * vec4(position, 1.0));
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	fs_normal = normal_matrix * normal;
	fs_tex_coord = tex_coord;
	
    gl_Position = proj * view * model * vec4(position, 1.0);
}

