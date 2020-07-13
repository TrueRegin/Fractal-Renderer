$vertex
#version 460 core

location(layout = 0) in vec3 position;
uniform mat4 u_ProjMat;
uniform mat4 u_ModelMat;
uniform mat4 u_ViewMat;

void main() {
	gl_Position = vec4(position, 1.0);
}

$fragment
#version 460 core

location(layout = 0) out vec4 color;

void main() {
	/* Sets the background color to gray with an alpha of 1.0f */
	color = vec4(0.5, 0.5, 0.5, 1.0);
}