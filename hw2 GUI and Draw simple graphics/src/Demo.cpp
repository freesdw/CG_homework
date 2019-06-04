
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "shader.h"
#include <iostream>

using namespace std;

#define STRIDE 6 //�������Ը���


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int WIN_WIDTH = 800;     //���ڿ��
const unsigned int WIN_HEIGHT = 600;    //���ڸ߶�

int main()
{
	// glfw: ��ʼ��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ����glfw����
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "GUI and Draw simple graphics", NULL, NULL);
	if (window == NULL) //������ڴ���ʧ��
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ����glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Use the shader.h
	Shader myShader;

	//Viewport
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	//���嶥������
	float vertices[] = {
		//triangle
		//position          //color
		 0.0f, 0.4f, 0.0f, 1.0f, 0.0f, 0.0f, //top
		 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //right
		-0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //left

		 0.4f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f, //right
		-0.4f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f, //left

		-0.6f, 0.4f, 0.0f, 1.0f, 1.0f, 0.0f, //top
		-0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //right
		-0.8f, 0.8f, 0.0f, 1.0f, 1.0f, 0.0f, //left

		0.6f, 0.4f, 0.0f, 1.0f, 1.0f, 0.0f, //top
		0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //right
		0.8f, 0.8f, 0.0f, 1.0f, 1.0f, 0.0f, //left

		//lines
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -0.2f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -0.2f, 0.0f, 1.0f, 0.0f, 1.0f,

		//points
		0.5f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.7f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.65f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.65f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.6f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.6f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.55f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.55f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.45f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.4f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.4f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.35f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.35f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f
		
	};

	//���������������
	unsigned int indices[] = { 
		0, 1, 2,   //��1��������
		0, 4, 3,   //��2��������
		5, 6, 7,   //��3��������
		8, 9, 10   //��4��������
	};

	//����VAO��VBO��EBO
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//�ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();

	//�ı���������ɫʱ�����ڴ洢�����������������ɫ
	vector<ImVec4> colors;
	colors.push_back(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	colors.push_back(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	//�ı���������ɫʱ���ṩ��ɫ�����εĸı�
	ImVec4 whole_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
	//ÿ��1ms�����»���
	glfwSwapInterval(1);

	/* δʹ��GUIʱ�ģ���Basics�е�1��2�� */
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	////position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	////color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//myShader.use();

	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/

	//������ʶ�Ǹı����������ε���ɫ�����Ǹı������������ɫ
	bool change_each_color = false;

	//ѭ��
	while (!glfwWindowShouldClose(window)) {
		processInput(window); //��׽����
		glfwPollEvents();	  //��������е��¼�
		
		//ImGui::SetWindowSize("wee",ImVec2(510, 170));
		//ImGui::SetWindowPos("w",ImVec2(0, 0));

		//����ImGui���ڣ������ô�����ʽ
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("change color");
		ImGui::SetWindowFontScale(1.2);

		ImGui::Text("change the whole color");
		ImGui::ColorEdit3("whole color", (float*)&whole_color, 1);

		ImGui::Checkbox("change color of each vertex", &change_each_color);
		ImGui::ColorEdit3("top", (float*)&colors[0]);
		ImGui::ColorEdit3("right", (float*)&colors[1]);
		ImGui::ColorEdit3("left", (float*)&colors[2]);

		//�ı�ÿ���������ɫ
		if (change_each_color) {
			for (int i = 0; i < 3; i++) {
				vertices[i * 6 + 3] = colors[i].x;
				vertices[i * 6 + 4] = colors[i].y;
				vertices[i * 6 + 5] = colors[i].z;
			}			
		}
		else { //�ı��������ɫ
			for (int i = 0; i < 3; i++) {
				vertices[i * 6 + 3] = whole_color.x;
				vertices[i * 6 + 4] = whole_color.y;
				vertices[i * 6 + 5] = whole_color.z;
			}
		}

		ImGui::End();
		
		//�����ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//����VAO��VBO��EBO������
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//��ʼ��Ⱦ
		myShader.use();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//����ͼ��
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	//	glDrawArrays(GL_TRIANGLES, 0, 11);
		glDrawArrays(GL_LINES, 11, 4);
		glDrawArrays(GL_POINTS, 15, 26);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}