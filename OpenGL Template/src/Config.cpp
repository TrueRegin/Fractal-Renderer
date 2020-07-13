#include "Config.h"

Config::Config()
	:
	WIDTH(640),
	HEIGHT(640),
	TITLE("Fractal Renderer"),
	VERTEX_START_STRING("$vertex"),
	FRAGMENT_START_STRING("$fragment"),
	BG_COLOR({ 0, 0, 1 }),
	SPEED(5),
	VIEW_LEFT(-3.f),
	VIEW_RIGHT(3.f),
	VIEW_BOTTOM(-3.f),
	VIEW_TOP(3.f),
	SENSITIVITY(0.009)
{}

Config& c() {
	static Config configData;
	return configData;
}