#version 450

const vec2 screen_positions[6] = {
    vec2(-1, -1),
    vec2(1, -1),
    vec2(1, 1),
    
    vec2(-1, -1),
    vec2(1, 1),
    vec2(-1, 1)
};

void main() {
    gl_Position = vec4(screen_positions[gl_VertexID] * 2.0, 0, 1);
}
