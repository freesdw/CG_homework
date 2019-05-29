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

#define WIN_WIDTH 700         //窗口宽度
#define WIN_HEIGHT 650        //窗口长度
//一些全局变量
bool is_running = false;
vector<glm::vec2> control_points;   //控制点

//窗口大小改变处理
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	//如果正在运行bonus的动画，阻塞鼠标事件
	if (is_running) return;

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		control_points.pop_back();
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//获取光标位置
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		//坐标归一化
		control_points.push_back(glm::vec2(((float)x / (float)WIN_WIDTH * 2.0f) - 1, -(((float)y / (float)WIN_HEIGHT * 2.0f) - 1)));
	}
}

//处理键盘输入
void processInput(GLFWwindow* window) {
	//Esc输入
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//输入回车，设置参数播放逼近过程动画
	if (!is_running && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {	
		is_running = true;
	}
}

//计算二项式系数
int biCoe(int n, int m) {
	if (m << 1 > n) m = n - m;
	if (m == 0) {
		return 1;
	}
	int coe = n;
	for (int i = 1; i < m; i++) {
		coe *= (n - i);
		coe /= (i + 1);
	}
	return coe;
}

//计算Q(t) = P * B(t)
glm::vec2 bezier(double t) {
	int order = control_points.size() - 1;

	double alpha_1 = 1;
	double alpha_2 = glm::pow(1 - t, order);

	double x = 0;
	double y = 0;
	for (int i = 0; i <= order; i++) {
		x += biCoe(order, i) * alpha_1 * alpha_2 * control_points[i].x;
		y += biCoe(order, i) * alpha_1 * alpha_2 * control_points[i].y;
		alpha_1 *= t;
		alpha_2 /= (1 - t);
	}
	return glm::vec2(x, y);
}


int main(void) {
	//glfw初始化配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Bezier Curve", NULL, NULL);

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
	glfwSetMouseButtonCallback(window, mouseCallback);

	// 激活深度测试
	//glEnable(GL_DEPTH_TEST);

	//shader
	Shader bezier_shader("bezier.vs", "bezier.fs");

	//顶点对象缓存处理
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Imgui配置
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	
	float speed = 0.01f;     //逼近动画速度控制
	double run_time = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		//输入处理，缓冲处理
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		bezier_shader.use();

		//设置imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//设置菜单样式
		{
			ImGui::Begin("Menu");
			ImGui::Text("Speed of Animation:");
			ImGui::SliderFloat("speed", &speed, 0.005f, 0.05f, "speed = %.3f");
			ImGui::End();
		}
		// render
		// ------
		glPointSize(8.0f);
		vector<float> renderPoints;
		for (int i = 0; i < control_points.size(); ++i) {
			vector<float> P{ control_points[i].x, control_points[i].y, 0.0f, 0.0f, 0.0f, 1.0f };
			renderPoints.insert(renderPoints.end(), P.begin(), P.end());
			//画出辅助的直线
			if (i > 0) {
				float* ptr = renderPoints.data() + (i - 1) * 6;
				glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), ptr, GL_STATIC_DRAW);
				glDrawArrays(GL_LINES, 0, 2);
			}
		}

		//画出控制点
		glBufferData(GL_ARRAY_BUFFER, renderPoints.size() * sizeof(float), renderPoints.data(), GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, control_points.size());

		//当控制点为两个以上时，绘制bezier曲线
		if (control_points.size() > 2) {
			vector<float> bezierPoints;   //bezier曲线上的点
			//计算bezier曲线上的点
			for (double t = 0.0f; t < 1.0f; t += 0.001) {
				glm::vec2 point(bezier(t));
				vector<float> P{ point.x, point.y, 0.0f, 1.0f, 0.0f, 0.0f };
				bezierPoints.insert(bezierPoints.end(), P.begin(), P.end());
			}
			//绘制曲线
			glPointSize(1.5f);
			glBufferData(GL_ARRAY_BUFFER, bezierPoints.size() * sizeof(float), bezierPoints.data(), GL_STATIC_DRAW);
			glDrawArrays(GL_POINTS, 0, bezierPoints.size());
			//动画过程
			if (is_running) {
				//使用循环实现递归过程
				vector<float> renderTempPoints;
				vector<glm::vec2> tempPoints(control_points);
				while (tempPoints.size() > 1) {
					vector<glm::vec2> next;   //记录下一轮递归用到的点
					for (int i = 1; i < tempPoints.size(); ++i) {
						float tempLine[]{
							tempPoints[i - 1].x, tempPoints[i - 1].y, 0.0f, 1.0f, 0.0f, 1.0f,
							tempPoints[i].x, tempPoints[i].y, 0.0f, 1.0f, 0.0f, 1.0f
						};
						//画出辅助直线
						glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), tempLine, GL_STATIC_DRAW);
						glDrawArrays(GL_LINES, 0, 2);

						glm::vec2 nextPoint;
						nextPoint.x = tempPoints[i].x * run_time + tempPoints[i - 1].x * (1 - run_time);
						nextPoint.y = tempPoints[i].y * run_time + tempPoints[i - 1].y * (1 - run_time);
						next.push_back(nextPoint);
						vector<float> temp{ nextPoint.x, nextPoint.y, 0.0f, 0.0f, 1.0f, 0.0f };
						renderTempPoints.insert(renderTempPoints.end(), temp.begin(), temp.end());
					}
					tempPoints = next;
				}
				glPointSize(4.5f);
				glBufferData(GL_ARRAY_BUFFER, renderTempPoints.size() * sizeof(float), renderTempPoints.data(), GL_STATIC_DRAW);
				glDrawArrays(GL_POINTS, 0, renderTempPoints.size());
				run_time += speed;
				if (run_time >= 1.0f) {
					run_time = 0.0f;
					is_running = false;
				}
			}
			else {
				is_running = false;
				run_time = 0.0f;
			}
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