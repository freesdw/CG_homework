
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "shader.h"
#include <iostream>

using namespace std;

#define STRIDE 6 //顶点属性个数


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int WIN_WIDTH = 800;     //窗口宽度
const unsigned int WIN_HEIGHT = 600;    //窗口高度

int main()
{
	// glfw: 初始化和配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建glfw窗口
	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "GUI and Draw simple graphics", NULL, NULL);
	if (window == NULL) //如果窗口创建失败
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 加载glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Use the shader.h
	Shader myShader;

	//Viewport
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

	//定义顶点属性
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

	//索引缓冲对象内容
	unsigned int indices[] = { 
		0, 1, 2,   //第1个三角形
		0, 4, 3,   //第2个三角形
		5, 6, 7,   //第3个三角形
		8, 9, 10   //第4个三角形
	};

	//生成VAO、VBO和EBO
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//搭建ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();

	//改变三角形颜色时，用于存储三角形三个顶点的颜色
	vector<ImVec4> colors;
	colors.push_back(ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	colors.push_back(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors.push_back(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	//改变三角形颜色时，提供单色三角形的改变
	ImVec4 whole_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
	//每隔1ms，更新缓存
	glfwSwapInterval(1);

	/* 未使用GUI时的，即Basics中的1、2问 */
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

	//用来标识是改变整个三角形的颜色，或是改变三个顶点的颜色
	bool change_each_color = false;

	//循环
	while (!glfwWindowShouldClose(window)) {
		processInput(window); //捕捉输入
		glfwPollEvents();	  //处理队列中的事件
		
		//ImGui::SetWindowSize("wee",ImVec2(510, 170));
		//ImGui::SetWindowPos("w",ImVec2(0, 0));

		//建立ImGui窗口，并设置窗口样式
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

		//改变每个顶点的颜色
		if (change_each_color) {
			for (int i = 0; i < 3; i++) {
				vertices[i * 6 + 3] = colors[i].x;
				vertices[i * 6 + 4] = colors[i].y;
				vertices[i * 6 + 5] = colors[i].z;
			}			
		}
		else { //改变整体的颜色
			for (int i = 0; i < 3; i++) {
				vertices[i * 6 + 3] = whole_color.x;
				vertices[i * 6 + 4] = whole_color.y;
				vertices[i * 6 + 5] = whole_color.z;
			}
		}

		ImGui::End();
		
		//清空颜色缓存
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//处理VAO、VBO和EBO的数据
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

		//开始渲染
		myShader.use();
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//绘制图形
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