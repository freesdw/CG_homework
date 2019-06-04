#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

#include "Shader.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"

#define WIN_WIDTH 650         //���ڿ��
#define WIN_HEIGHT 650        //���ڳ���

glm::vec3 lightPos(-1.2f, 0.0f, 5.0f);   //��Դλ��

//���ڴ�С�ı䴦��
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//�����������
void processInput(GLFWwindow* window) {
	//Esc����
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(void) {
	//glfw��ʼ������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Lights and Shading", NULL, NULL);

	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//glad����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Faild to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//shader
	Shader phong_shader("Phong.vs", "Phong.fs");
	Shader gouraud_shader("Gouraud.vs", "Gouraud.fs");
	Shader lamp_shader("Lamp.vs", "Lamp.fs");

	// �����������涥�㴦��ÿ���������������κϳ�
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int VBO;
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//��Դ����
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Imgui����
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui::StyleColorsDark();

	//������ʼ��
	float cubePosition[3] = { 0.0f, 0.0f, 0.0f };
	float cameraPos[3] = { 1.0f, 0.0f, 4.0f };
	float lookAtCenter[3] = { 0.0f, 0.0f, 0.0f };
	float perspective[4] = { 5.0f, 5.0f, 0.1f, 100.0f };
	float angle = 45.0f;

	//light paras
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	int shininess = 32;
	float diffuseCoe = 1.0f;

	int mode = 0;
	bool is_moving_lamp = false;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//����imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//���ò˵���ʽ
		{
			ImGui::Begin("Menu");
			ImGui::Text("Choose the mode:");
			ImGui::RadioButton("Phong Shading", &mode, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Gouraud Shading", &mode, 1);
			ImGui::SameLine();
			ImGui::Checkbox("Lamp moving", &is_moving_lamp);

			if (ImGui::CollapsingHeader("Camera params")) {
				ImGui::InputFloat3("Cube center", cubePosition, 1);
				ImGui::InputFloat3("Camera center", cameraPos, 1);
				ImGui::InputFloat3("Lookat center", lookAtCenter, 1);
				ImGui::SliderFloat("Perspective angle", &angle, 0.0f, 360.0f, "angle = %.1f");
			}

			if (ImGui::CollapsingHeader("light params")) {
				ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f, "ambientStrength = %.1f"); //ambient����
				ImGui::SliderFloat("diffuseCoe", &diffuseCoe, 0.0f, 1.0f, "diffuseCoe = %.1f");  //diffuse����
				ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 1.0f, "specularStrength = %.1f"); //specular����
				ImGui::SliderInt("shininess", &shininess, 0, 255, "shininess = %d");             //�����				
				ImGui::SliderFloat("lightPos.x", &lightPos.x, -10.0f, 10.0f, "X = %.1f");
				ImGui::SliderFloat("lightPos.y", &lightPos.y, -10.0f, 10.0f, "Y = %.1f");
				ImGui::SliderFloat("lightPos.z", &lightPos.z, -10.0f, 10.0f, "Z = %.1f");
			}

			ImGui::End();
		}
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader lightingShader;
		if (mode == 0) {
			lightingShader = phong_shader;
		}
		else {
			lightingShader = gouraud_shader;
		}

		//����shader
		lightingShader.use();
		lightingShader.setFloat3("objectColor", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
		lightingShader.setFloat3("lightColor", glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		lightingShader.setFloat("ambientStrength", ambientStrength);
		lightingShader.setFloat("specularStrength", specularStrength);
		lightingShader.setInt("shininess", shininess);
		lightingShader.setFloat("diffuseCoe", diffuseCoe);

		//transformations
		glm::mat4 view = glm::lookAt(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]),
			glm::vec3(lookAtCenter[0], lookAtCenter[1], lookAtCenter[2]),
			glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(angle, (float)perspective[0] / (float)perspective[1], perspective[2], perspective[3]);
		
		if (is_moving_lamp) {
			lightPos.x = -1.0f + sin(glfwGetTime()) * 1.0f;
			lightPos.y = sin(glfwGetTime()) * 1.0f;
		}
		glm::mat4 model;

		lightingShader.setMat4("view", glm::value_ptr(view));
		lightingShader.setMat4("projection", glm::value_ptr(projection));
		lightingShader.setFloat3("lightPos", glm::value_ptr(lightPos));
		lightingShader.setFloat3("viewPos", glm::value_ptr(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2])));
		lightingShader.setMat4("model", glm::value_ptr(model));

		//render cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//draw the lamp object
		lamp_shader.use();
		lamp_shader.setMat4("projection", glm::value_ptr(projection));
		lamp_shader.setMat4("view", glm::value_ptr(view));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.15f));
		lamp_shader.setMat4("model", glm::value_ptr(model));
		
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}