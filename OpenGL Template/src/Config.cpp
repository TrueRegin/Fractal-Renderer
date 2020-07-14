#include "Config.h"

Config::Config()
	:
	WIDTH(640),
	HEIGHT(640),
	TITLE("Fractal Renderer"),
	VERTEX_START_STRING("$vertex"),
	FRAGMENT_START_STRING("$fragment"),
	BG_COLOR({ 0, 0, 0 }),
	SPEED(5),
	VIEW_LEFT(-320.f),
	VIEW_RIGHT(320.f),
	VIEW_BOTTOM(-320.f),
	VIEW_TOP(320.f),
	CELL_WIDTH(160),
	AXIS_SIZE({5.f, 5.f}),
	SENSITIVITY(0.5)
{}

Config& c() {
	static Config configData;
	return configData;
}