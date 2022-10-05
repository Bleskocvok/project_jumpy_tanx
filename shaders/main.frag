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

layout(location = 2) uniform mat4 view; // need for fog
layout(location = 3) uniform vec3 eye_pos;

layout(location = 8) uniform vec3 mat_amb_col;
layout(location = 9) uniform vec3 mat_diff_col;
layout(location = 10) uniform vec3 mat_spec_col;
layout(location = 11) uniform float mat_shine;

layout(binding = 0) uniform sampler2D mat_texture;
layout(binding = 1) uniform sampler2D mat_light_map;

layout(location = 12) uniform bool use_light_map;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_tex_coord;

layout(location = 0) out vec4 final_color;


vec3 calc_light(vec3 material_texture, float shine_map, Light light) {
	vec3 light_vector = light.pos.xyz - fs_position * light.pos.w;
	vec3 norm_light = normalize(light_vector);
	vec3 normal = normalize(fs_normal);
	vec3 norm_eye = normalize(eye_pos - fs_position);
	vec3 half_vec = normalize(norm_light + norm_eye);

	float dot_diff = max(dot(normal, norm_light), 0.0);
	float dot_reflect = max(dot(normal, half_vec), 0.00001);

	
	vec3 result_ambient = mat_amb_col * light.amb.rgb;
	vec3 result_diffuse = mat_diff_col * material_texture * light.diff.rgb;
	vec3 result_specular = mat_spec_col * light.spec.rgb;
	
	float diff_coeff = dot_diff;
	float shine_coeff = pow(dot_reflect, mat_shine);
	
	vec3 result_color = result_ambient
		+ diff_coeff * result_diffuse
		+ shine_coeff * result_specular * shine_map;
	
	float light_intensity = 1.0;

	if (light.pos.w == 1.0) {
		float dist2 = pow(length(light_vector), 2);
		light_intensity = clamp(1.0 - dist2 / (light.spec.w * light.spec.w), 0.0, 1.0);
		light_intensity *= light_intensity;
	}
	return result_color * light_intensity;
}

void main() {
    vec4 full_mat_tex_col = texture(mat_texture, fs_tex_coord);
    if (full_mat_tex_col.a < 0.1) {
        discard;
    }
	vec3 mat_tex_col = full_mat_tex_col.rgb;
	float diff_value = 0.0;
	float shine_value = 1.0;
	if (use_light_map) {
		vec3 mat_light_col = texture(mat_light_map, fs_tex_coord).rgb;
		diff_value = mat_light_col.g;
		shine_value = mat_light_col.r;
	}
	vec3 result_color;
	for (int i = 0; i < lights.length(); ++i) {
		result_color += calc_light(mat_tex_col, shine_value, lights[i]);
	}
	vec3 result_mat_color = mix(result_color, mat_tex_col * mat_diff_col, diff_value);
	
	vec4 view_space = view * vec4(fs_position, 1.0);
	// vec3 fog_color = vec3(0.3, 0.3, 0.3);
	vec3 fog_color = vec3(0.0, 0.0, 0.0);
	float fog_start = 70.0;
	float fog_end = 110.0;
	float fog_factor = (fog_end - abs(view_space.z)) / (fog_end - fog_start);
	fog_factor = clamp(fog_factor, 0.0, 1.0);
	result_mat_color = mix(fog_color, result_mat_color, fog_factor);
	
	final_color = vec4(result_mat_color, 1.0);
}


