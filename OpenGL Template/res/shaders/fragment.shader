#version 460 core

layout (location = 0) out vec4 color;
in vec2 vertex;

uniform float r_Comp;
uniform float z_Comp;
uniform float rFac, gFac, bFac;
uniform bool mandelbrot;

// Visible Dimensions on screen
uniform vec2 visible;
uniform dvec2 corner;
uniform float cellSize; // 160
uniform float scale;
uniform float rotationAngle;

void main() {

	color = vec4(0, 0, 0, 1);
	//const int max_itrs = 600;
	float max_itrs = 20 + sqrt(scale);
	if (max_itrs > 700) max_itrs = 700;

	dvec2 pos = dvec2(corner.x + vertex.x * visible.x, corner.y + vertex.y * visible.y);
	float c = cos(rotationAngle);
	float s = sin(rotationAngle);
	dvec2 newPos = dvec2(pos.x * c - pos.y * s, pos.x * s + pos.y * c);
	pos = newPos;

	if (!mandelbrot) {
		double r = pos.x;
		double z = pos.y;
		for (int i = 0; i < max_itrs; i++) {
			double rr = r * r - z * z + r_Comp;
			double zz = 2 * r * z + z_Comp;
			r = rr;
			z = zz;

			if (r * r + z * z > 16) {
				color = vec4(rFac * i / 10, gFac * i / 10, bFac * i / 10, 1);
				return;
			}
		}
	}
	else {
		double r = 0;
		double z = 0;
		
		for (int i = 0; i < max_itrs; i++) {
			double rr = r * r - z * z + pos.x;
			double zz = 2 * r * z + pos.y;
			r = rr;
			z = zz;

			if (r * r + z * z > 16) {
				color = vec4(rFac * i / 10, gFac * i / 10, bFac * i / 10, 1);
				return;
			}
		}
	}
}