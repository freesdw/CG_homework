#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
using namespace std;

#include "Shader.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"

#define WIN_WIDTH 650         //���ڿ��
#define WIN_HEIGHT 650        //���ڳ���

Camera camera(glm::vec3(0.0f, 3.0f, 12.0f)); //���������

bool isFirstFPS = false;    //�����жϸ�֡�Ƿ����Bonus���������ı���̵��������
int mode = 0;             //�����л�
float lastX = WIN_WIDTH / 2.0f;
float lastY = WIN_HEIGHT / 2.0f;
bool firstMouse = false;
float deltaTime = 0.0f;   //������¼��֮֡���ʱ����
float lastFrame = 0.0f;   //��¼��һ֡��ʱ��

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
	//����qʱ���˳�Bonusģʽ
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		isFirstFPS = false;
		mode = 0;
	}

	//WASD�������봦��
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.moveForward(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.moveLeft(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.moveBack(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.moveRight(deltaTime);
	}

}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

int main(void) {
	//glfw��ʼ������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Transformation", NULL, NULL);

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//shader
	Shader my_shader("t_shader.vs", "t_shader.fs");

	// �����������涥�㴦��ÿ���������������κϳ�
	float vertices[] = {
		// ����
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, 1.0f,
		2.0f, -2.0f,  2.0f,   0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f,   0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f,   0.0f, 0.0f, 1.0f,
		-2.0f, 2.0f, 2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f,  0.0f, 0.0f, 1.0f,
		// ����
		-2.0f,  2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, -2.0f,   1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 2.0f,   1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 2.0f,   1.0f, 0.0f, 0.0f,
		-2.0f,  2.0f, 2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f, 2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		// ����
		-2.0f,  2.0f, 2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, 2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, 2.0f,  0.0f, 1.0f, 0.0f,
		// ����
		2.0f, 2.0f, 2.0f,   0.0f, 1.0f, 1.0f,
		2.0f,  2.0f, -2.0f,   0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,   0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,   0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, 2.0f,   0.0f, 1.0f, 1.0f,
		2.0f,  2.0f, 2.0f,   0.0f, 1.0f, 1.0f,
		// ����
		-2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		2.0f, -2.0f, -2.0f,   1.0f, 1.0f, 0.0f,
		2.0f, 2.0f, -2.0f,   1.0f, 1.0f, 0.0f,
		2.0f,  2.0f, -2.0f,   1.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		// ����
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,
		2.0f, -2.0f, -2.0f,   1.0f, 0.0f, 1.0f,
		2.0f, -2.0f,  2.0f,   1.0f, 0.0f, 1.0f,
		2.0f, -2.0f,  2.0f,   1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int VBO;
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//Imgui����
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui::StyleColorsDark();

	//������ʼ��
	float cubePosition[3] = { -1.5f, 0.5f, -1.5f };
	float cameraPos[3] = { 0.0f, 10.0f, 30.0f };
	float lookAtCenter[3] = { 0.0f, 0.0f, 0.0f };
	float ortho[4] = { -5.0f, 5.0f, -5.0f, 5.0f };
	float perspective[3] = { 1.0f, 1.0f, 1.0f };
	float eye_space[2] = { 0.1f, 150.0f };

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		//����imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//���ò˵���ʽ
		{
			ImGui::Begin("Menu");
			ImGui::Text("Choose the mode:");
			ImGui::RadioButton("Ortho Projection", &mode, 0);
			ImGui::SameLine();
			ImGui::RadioButton("Perspective Projection", &mode, 1);
			ImGui::RadioButton("View changing", &mode, 2);
			ImGui::SameLine();
			ImGui::RadioButton("Bonus: Camera", &mode, 3);
			//���ò�ͬģʽ�µĲ�������
			switch (mode)
			{
			case 0:
				ImGui::Text("Ortho Projection");
				ImGui::InputFloat3("Cube Position", cubePosition, 2);
				ImGui::InputFloat3("Camera Position", cameraPos, 2);
				ImGui::InputFloat4("Othro paras", ortho, 2);
				ImGui::InputFloat2("Near and Far", eye_space, 2);
				break;
			case 1:
				ImGui::Text("Perspective Projection");
				ImGui::InputFloat3("Cube Position", cubePosition, 2);
				ImGui::InputFloat3("Camera Position", cameraPos, 2);
				ImGui::InputFloat3("Perspective paras", perspective, 2);
				ImGui::InputFloat2("Near and Far", eye_space, 2);
				break;
			case 2:
				ImGui::Text("View changing");
				break;
			case 3:
				ImGui::Text("Using WASD to change view");
				isFirstFPS = true;
				firstMouse = true;
				break;;
			default:
				break;
			}
			ImGui::End();
		}
		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		// �������ģʽ
		float Radius = 20.0f;
		float camPosX = (float)sin(glfwGetTime()) * Radius;
		float camPosZ = (float)cos(glfwGetTime()) * Radius;
		switch (mode)
		{
		case 0:		
			model = glm::translate(model, glm::vec3(cubePosition[0], cubePosition[1], cubePosition[2]));
			view = glm::lookAt(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]),
				glm::vec3(lookAtCenter[0], lookAtCenter[1], lookAtCenter[2]),
				glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::ortho(ortho[0], ortho[1], ortho[2], ortho[3], eye_space[0], eye_space[1]);
			break;
		case 1:
			model = glm::translate(model, glm::vec3(cubePosition[0], cubePosition[1], cubePosition[2]));
			view = glm::lookAt(glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]),
				glm::vec3(lookAtCenter[0], lookAtCenter[1], lookAtCenter[2]),
				glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(perspective[0], perspective[1] / perspective[2], eye_space[0], eye_space[1]);
			break;
		case 2:			
			view = glm::lookAt(glm::vec3(camPosX, 5.0f, camPosZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f));
			projection = glm::perspective(45.0f, (float)WIN_WIDTH / (float)WIN_WIDTH, 0.1f, 100.0f);
			break;
		case 3:
			//����ս����ģʽ����������ģʽ
			if (isFirstFPS) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			view = camera.GetViewMatrix();
			projection = glm::perspective(45.0f, (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 150.0f);
			break;
		default:
			break;
		}

		my_shader.use();
		my_shader.setMat4("model", glm::value_ptr(model));
		my_shader.setMat4("view", glm::value_ptr(view));
		my_shader.setMat4("projection", glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}