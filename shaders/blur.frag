#version 450

layout(binding = 0) uniform sampler2D screen;

layout(location = 0) out vec4 final_color;


const float gauss_blur[7][7] = {
	{0.011362, 0.014962, 0.017649, 0.018648, 0.017649, 0.014962, 0.011362},
	{0.014962, 0.019703, 0.023240, 0.024556, 0.023240, 0.019703, 0.014962},
	{0.017649, 0.023240, 0.027413, 0.028964, 0.027413, 0.023240, 0.017649},
	{0.018648, 0.024556, 0.028964, 0.030603, 0.028964, 0.024556, 0.018648},
	{0.017649, 0.023240, 0.027413, 0.028964, 0.027413, 0.023240, 0.017649},
	{0.014962, 0.019703, 0.023240, 0.024556, 0.023240, 0.019703, 0.014962},
	{0.011362, 0.014962, 0.017649, 0.018648, 0.017649, 0.014962, 0.011362}
};
const int gauss_size = 3;

vec3 blur(ivec2 coord) {
	vec3 result_blur = vec3(0.0);
	for (int x = -gauss_size; x <= gauss_size; ++x) {
		for (int y = -gauss_size; y <= gauss_size; ++y) {
			vec3 color = texelFetch(screen, ivec2(gl_FragCoord.x + x, gl_FragCoord.y + y), 0).rgb;
			result_blur += gauss_blur[x + gauss_size][y + gauss_size] * color;
		}
	}
	return result_blur;// / ((gauss_size * gauss_size) * 4 + 4 * gauss_size + 1);
}

void main() {
	final_color = vec4(blur(ivec2(gl_FragCoord.xy)), 1.0);
}
