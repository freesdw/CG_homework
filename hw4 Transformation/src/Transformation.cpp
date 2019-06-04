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

#define WIN_WIDTH 650         //窗口宽度
#define WIN_HEIGHT 650        //窗口长度

//窗口大小改变处理
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//处理键盘输入
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(void) {
	//glfw初始化配置
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

	//glad处理
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Faild to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 是否激活深度测试
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);

	//shader
	Shader my_shader("t_shader.vs", "t_shader.fs");

	// 正方体六个面顶点处理，每个面由两个三角形合成
	float vertices[] = {
		// 正面
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f, 1.0f,
		2.0f, -2.0f,  2.0f,   0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f,   0.0f, 0.0f, 1.0f,
		2.0f,  2.0f,  2.0f,   0.0f, 0.0f, 1.0f,
		-2.0f, 2.0f, 2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, 2.0f,  0.0f, 0.0f, 1.0f,
		// 上面
		-2.0f,  2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, -2.0f,   1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 2.0f,   1.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 2.0f,   1.0f, 0.0f, 0.0f,
		-2.0f,  2.0f, 2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f, 2.0f, -2.0f,  1.0f, 0.0f, 0.0f,
		// 左面
		-2.0f,  2.0f, 2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, 2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, 2.0f,  0.0f, 1.0f, 0.0f,
		// 右面
		2.0f, 2.0f, 2.0f,   0.0f, 1.0f, 1.0f,
		2.0f,  2.0f, -2.0f,   0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,   0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, -2.0f,   0.0f, 1.0f, 1.0f,
		2.0f, -2.0f, 2.0f,   0.0f, 1.0f, 1.0f,
		2.0f,  2.0f, 2.0f,   0.0f, 1.0f, 1.0f,
		// 后面
		-2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		2.0f, -2.0f, -2.0f,   1.0f, 1.0f, 0.0f,
		2.0f, 2.0f, -2.0f,   1.0f, 1.0f, 0.0f,
		2.0f,  2.0f, -2.0f,   1.0f, 1.0f, 0.0f,
		-2.0f, 2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		// 下面
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

	// world space positions of our cubes
	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f, -5.0f, 0.0f),
		glm::vec3(0.0f, -1.5f, 0.0f),
		glm::vec3(0.0f, 3.0f, 0.0f),
		glm::vec3(0.0f, 8.5f, 0.0f),
		glm::vec3(0.0f, 17.0f, 0.0f),
		glm::vec3(0.0f, 26.0f, 0.0f),
		glm::vec3(0.0f, 39.5f, 0.0f),
		glm::vec3(0.0f, 64.5f, 0.0f),
		glm::vec3(0.0f, 135.0f, 0.0f)
	};

	//正方形的缩放处理
	glm::vec3 cube_scale[] = {
		glm::vec3(0.65f, 0.65f, 0.65f),
		glm::vec3(0.55f, 0.55f, 0.55f),
		glm::vec3(0.45f, 0.45f, 0.45f),
		glm::vec3(0.35f, 0.35f, 0.35f),
		glm::vec3(0.25f, 0.25f, 0.25f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(0.15f, 0.15f, 0.15f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.05f, 0.05f, 0.05f)
	};

	//用于控制正方形旋转的速率
	float rotate_rate[] = {
		-0.5f, 1.5f, -2.5f, 3.5f, -4.5f, 5.5f, -6.5f, 7.5f, -8.5f
	};

	//Imgui配置
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui::StyleColorsDark();

	//正方形模式
	int mode = 0;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//设置imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//设置菜单样式
		{
			ImGui::Begin("Menu");
			ImGui::Text("Choose the mode:");
			if (ImGui::Button("Static"))
				mode = 0;
			ImGui::SameLine();
			if (ImGui::Button("Translation"))
				mode = 1;
			ImGui::SameLine();
			if (ImGui::Button("Rotation"))
				mode = 2;
			ImGui::SameLine();
			if (ImGui::Button("Scaling"))
				mode = 3;
			ImGui::SameLine();
			if (ImGui::Button("Bonus"))
				mode = 4;

			switch (mode)
			{
			case 0:
				ImGui::Text("Cube is static");
				break;
			case 1:
				ImGui::Text("Cube translating");
				break;
			case 2:
				ImGui::Text("Cube rotating");
				break;
			case 3:
				ImGui::Text("Cube scaling");
				break;
			case 4:
				ImGui::Text("Bonus: Rotating tower.");
				break;
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

		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 5.0f, 20.0f), 
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(45.0f, (float)WIN_WIDTH / (float)WIN_HEIGHT, 1.0f, 100.0f);
		my_shader.use();
		my_shader.setMat4("view", glm::value_ptr(view));
		my_shader.setMat4("projection", glm::value_ptr(projection));

		// 处理各个模式
		switch (mode)
		{
		case 0:		
			my_shader.setMat4("model", glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 1:
			model = glm::translate(model, (float)sin(glfwGetTime()) * glm::vec3(2.0f, 0.0f, 0.0f));
			my_shader.setMat4("model", glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 2:
			model = glm::rotate(model, (float)glfwGetTime() * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			my_shader.setMat4("model", glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 3:
			model = glm::scale(model, (float)abs(sin(glfwGetTime()))*glm::vec3(2.0f, 2.0f, 2.0f));
			my_shader.setMat4("model", glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			break;
		case 4:
			glBindVertexArray(VAO);
			
			for (unsigned int i = 0; i < 9; i++)
			{
				// calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::scale(model, cube_scale[i]);
				model = glm::rotate(model, (float)glfwGetTime()*rotate_rate[i], glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::translate(model, cube_positions[i]);

				my_shader.setMat4("model", glm::value_ptr(model));
				
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			break;
		default:
			break;
		}

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