#version 460 core

layout (location = 0) in vec2 pos;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;
uniform mat4 u_ProjMat;

// Width of x-axis and y-axis
uniform float xAxis, yAxis;

out float vertexX;
out float vertexY;


void main() {
	gl_Position = u_ProjMat * u_ViewMat * vec4(pos, 0.0f, 1.0);
	vertexX = pos.x/xAxis;
	vertexY = pos.y/yAxis;
}