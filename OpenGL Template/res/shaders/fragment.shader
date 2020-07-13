#version 460 core

layout (location = 0) out vec4 color;
in float vertexX;
in float vertexY;

uniform float r_Comp;
uniform float z_Comp;
uniform float rFac, gFac, bFac;
uniform bool mandelbrot;

void main() {

	color = vec4(0, 0, 0, 1);
	const int max_itrs = 200;

	if (!mandelbrot) {
		float r = vertexX;
		float z = vertexY;
		for (int i = 0; i < max_itrs; i++) {
			float rr = r * r - z * z + r_Comp;
			float zz = 2 * r * z + z_Comp;
			r = rr;
			z = zz;

			if (r * r + z * z > 16) {
				color = vec4(rFac * i / 10, gFac * i / 10, bFac * i / 10, 1);
				return;
			}
		}
	}
	else {
		float r = 0;
		float z = 0;
		
		for (int i = 0; i < max_itrs; i++) {
			float rr = r * r - z * z + vertexX;
			float zz = 2 * r * z + vertexY;
			r = rr;
			z = zz;

			if (r * r + z * z > 16) {
				color = vec4(rFac * i / 10, gFac * i / 10, bFac * i / 10, 1);
				return;
			}
		}
	}
}