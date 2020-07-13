#include "FileLoader.h"

std::string readFile(const char* path) {
	std::fstream file(path);
	std::string line;
	std::stringstream fileContents;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			fileContents << line << "\n";
		}
	}
	else {
		printf("FILELOADER ERROR: No file exists at path: %s\n", path);
	}

	file.close();
	return fileContents.str();
}