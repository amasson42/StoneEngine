#include "config.h"
#include "Scene/Shader/ShaderGenerator.hpp"
#include "Utils/FileSystem.hpp"
#include "Utils/Json.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>

static time_t getLastmodifiedTimeOfFile(const char *filename) {
	struct stat result;
	if (stat(filename, &result) == 0) {
		auto mod_time = result.st_mtime;
		return mod_time;
	} else
		throw std::runtime_error("File does not exist");
}

Stone::Scene::ShaderParameters parseShaderParameters(const Json::Value &json) {
	Stone::Scene::ShaderParameters params;

	auto &params_obj = json.get<Json::Object>();

	for (auto [key, value] : params_obj) {
		Stone::Scene::ShaderParameters::Type type;
		if (value.is<std::string>()) {
			const std::string &type_str = value.get<std::string>();
			if (type_str == "scalar")
				type = Stone::Scene::ShaderParameters::Type::Scalar;
			else if (type_str == "vector")
				type = Stone::Scene::ShaderParameters::Type::Vector;
			else if (type_str == "texture")
				type = Stone::Scene::ShaderParameters::Type::Texture;
			else
				throw std::runtime_error("Invalid type " + type_str);
		} else
			throw std::runtime_error("Invalid type for key " + key);
		params.setParamWithName(key, type);
	}

	return params;
}

std::string to_string(Stone::Scene::ShaderParameters::Type type) {
	switch (type) {
	case Stone::Scene::ShaderParameters::Type::None: return "none";
	case Stone::Scene::ShaderParameters::Type::Scalar: return "scalar";
	case Stone::Scene::ShaderParameters::Type::Vector: return "vector";
	case Stone::Scene::ShaderParameters::Type::Texture: return "texture";
	}
	return "";
}

void generateShaderOutput(const char *input_file, const char *output_file) {

	Json::Value input_json;
	Json::parseFile(input_file, input_json);

	std::ofstream output_stream(output_file, std::ios::out | std::ios::trunc);

	Stone::Scene::ShaderParameters params = parseShaderParameters(input_json);

	Stone::Scene::ShaderGenerator generator;
	std::cout << "Generating shader: {" << std::endl;
#define __PRINT_SHADER_PARAM(param) std::cout << " -" << #param << " " << to_string(params.param) << std::endl;
	FOR_EACH_SHADER_PARAMETERS(__PRINT_SHADER_PARAM)
	std::cout << "}" << std::endl;

	generator.generateFragmentShader(params, output_stream);
}

std::string input;
std::string output;

void generateShader() {
	try {
		generateShaderOutput(input.c_str(), output.c_str());
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void printUsage() {
	std::cout << "Usage: glsl_generator <input> <output> [-f]" << std::endl;
}

int main(int argc, const char *argv[]) {
	if (argc < 3) {
		printUsage();
		return 1;
	}

	input = argv[1];
	output = argv[2];

	generateShader();

	if (argc >= 4 && std::string(argv[3]) == "-f") {
		while (true) {
			time_t last_modified = getLastmodifiedTimeOfFile(input.c_str());
			while (last_modified == getLastmodifiedTimeOfFile(input.c_str())) {
				// sleep for a short period
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			}

			generateShader();
		}
		return 0;
	}
}
