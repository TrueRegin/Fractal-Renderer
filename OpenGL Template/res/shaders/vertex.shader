#version 460 core

layout (location = 0) in vec2 pos;
out vec2 vertex;

void main() {
	gl_Position = vec4(pos, 0.0f, 1.0);
	vertex = vec2((1 + pos.x)/2, (1 + pos.y)/2);
}