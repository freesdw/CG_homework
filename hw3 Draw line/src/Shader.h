#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader {
public:
	unsigned int ID;
	//����shader
	Shader(const char* vertexPath, const char* fragmentPath) {
		//���ļ�·���л�ȡ����/Ƭ����ɫ��
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		//��֤ifstream��������׳��쳣
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			//���ļ�
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			//��ȡ�ļ��Ļ������ݵ���������
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//�ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			//ת����������string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCESSFULLY_READ" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		//������ɫ��
		unsigned int vertex, fragment;
		//������ɫ��
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		//Ƭ����ɫ��
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		//��ɫ������
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		//ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	//activate the shader
	void use() {
		glUseProgram(ID);
	}
	//utility uniform functions
	void setBool(const string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
private:
	//�����ɫ������/���Ӵ���
	void checkCompileErrors(unsigned int shader, string type) {
		int success;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "/n" << infoLog << endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << endl;
			}
		}
	}
};