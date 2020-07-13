#pragma once

#include <glm\vec3.hpp>
#include <unordered_map>

struct Config {
	const int WIDTH;
	const int HEIGHT;
	const char* TITLE;
	const char* VERTEX_START_STRING;
	const char* FRAGMENT_START_STRING;
	const glm::vec3 BG_COLOR;
	const float SPEED;
	const double VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP;
	const double SENSITIVITY;

	Config();
};

Config& c();