#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0f);\n"
"   vertexColor = aColor;"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 vertexColor;"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor, 1.0f);\n"
"}\n\0";

class Shader
{
public:
	unsigned int ID;
	Shader();
	void use() {
		glUseProgram(ID);
	}
	//void setBool(const string name, bool value) {
	//	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	//}
	//void setInt(const string name, int value) {
	//	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	//}
	//void setFloat(const string name, float value) {
	//	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	//}
private:
	void checkCompileErrors(unsigned int shader, string type);
};

Shader::Shader()
{
	unsigned int vertex, fragment;
	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	//fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	//shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, string type)
{
	int success;
	char infoLog[512];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_LINK_ERROR of type: " << type << "\n" << infoLog << "\n";
		}
	}
}
