#version 460 core

layout (location = 0) in vec2 pos;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

// Width of x-axis and y-axis
uniform vec2 axis;

out float vertexX;
out float vertexY;
out vec2 vec;


void main() {
	gl_Position = u_ProjMat * u_ViewMat * u_ModelMat * vec4(pos, 0.0f, 1.0);
	vec = vec2(pos.x / (640.0 / axis.x), pos.y / (640.0 / axis.y));
	vertexX = pos.x / (640.0 / axis.x);
	vertexY = pos.y / (640.0 / axis.y);
	
}