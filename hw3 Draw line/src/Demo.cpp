#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace std;

#define WIN_WIDTH 600            //窗口宽度
#define WIN_HEIGHT 600           //窗口高度

//按键输入处理
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//窗口大小应答
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*
* 用于记录一组需要渲染的线段的坐标
*/
typedef struct {
	vector<int> gl_x;
	vector<int> gl_y;
} Line;

/*
* 返回a，b中的最大值
*/
int max(int a, int b) {
	return a > b ? a : b;
}

/*
* 返回a，b中的最小值
*/
int min(int a, int b) {
	return a > b ? b : a;
}

/*
* bresenham算法画线段。适合0 <= m <= 1的线段
* value：用于存储计算后y的序列
* p：上一个p值
* depth： 当前递归深度
* num：需要计算出的点的数目
* dx，dy：两个方向上的，始点和终点的差分
*/
void bresenham(int value[], int p, int depth, int num, int dx, int dy) {
	if (depth == num) return; //计算出了所需要的所有点，结束递归
	int next_p;
	if (p <= 0) {
		value[depth] = value[depth - 1];
		next_p = p + 2 * dy;
	}
	else {
		value[depth] = value[depth - 1] + 1;
		next_p = p + 2 * dy - 2 * dx;
	}
	bresenham(value, next_p, depth + 1, num, dx, dy);
}

/*
* 画任意线段的处理。主要考虑线段与x轴垂直，线段与y轴垂直（为加快计算，单独考虑），|m| <= 1，|m| > 1四种情况
* x1,y1,x2,y2: 决定线段的两个端点
* line：记录线段上所有像素点的坐标
*/
void drawLine(int x1, int y1, int x2, int y2, Line& line) {
	int x[WIN_WIDTH*2], y[WIN_HEIGHT*2]; //辅助存储
	int start_x = x1, start_y = y1, end_x = x2, end_y = y2;
	int length;   //线段上像素的数目
	//垂直x轴的直线
	if (start_x == end_x) {
		if (start_y > end_y) { //规定从下往上处理
			start_y = y2;
			end_y = y1;
		}
		length = end_y - start_y + 1;
		for (int i = 0; i < length; ++i) {
			x[i] = start_x;
			y[i] = start_y + i;
		}
	} 
	else if(start_y == end_y) {  //垂直y轴的直线
		if (start_x > end_x) {
			start_x = x2;
			end_x = x1;
		}
		length = end_x - start_x + 1;
		for (int i = 0; i < length; ++i) {
			x[i] = start_x + i;
			y[i] = start_y;
		}
	}
	else {
		float m = (float)(end_y - start_y) / (end_x - start_x); //斜率
		int dx, dy;
		//当|m|<=1时，x轴递增，计算y的值
		if (fabs(m) <= 1) {
			if (end_x < start_x) {  //从左往右计算
				start_x = x2;
				start_y = y2;
				end_x = x1;
				end_y = y1;
			}
			dx = end_x - start_x;
			dy = end_y - start_y;
			m = (float)dy / dx;  //重新计算斜率
			length = end_x - start_x + 1;

			for (int i = 0; i < length; ++i) {
				x[i] = start_x + i;
			}
			//0 <= m <= 1，此时可直接使用bresenham函数处理，线段方向为左下至右上
			if (dy >= 0) {
				y[0] = start_y;
				y[length - 1] = end_y;
				int p0 = 2 * dy - dx;
				bresenham(y, p0, 1, length, dx, dy);
			}
			/*
			*  -1 <= m < 0，此时线段方向为从左上至右下，为了能使用bresenham函数，不妨先计算线段关于x轴的对称线段
			* 即将y方向上所有的处理取反。得到对称线段的像素点坐标后，将对称线段的所有像素点在y轴上取反，则得到原线段的
			* 像素点坐标
			*/
			else 
			{
				y[0] = -start_y;
				y[length - 1] = -end_y;
				int p0 = 2 * (-dy) - dx;
				bresenham(y, p0, 1, length, dx, -dy);
				for (int i = 0; i < length; ++i) {
					y[i] = -y[i];
				}
			}
		}
		//当|m|>1时，y轴递增，计算x的值。实际为将上述中x与y的身份调换即可
		else {
			if (end_y < start_y) {
				start_x = x2;
				start_y = y2;
				end_x = x1;
				end_y = y1;
			}
			dx = end_x - start_x;
			dy = end_y - start_y;
			m = (float)dy / dx;
			length = end_y - start_y + 1;
			for (int i = 0; i < length; ++i) {
				y[i] = start_y + i;
			}
			if (dx >= 0) { //m > 1
				x[0] = start_x;
				x[length - 1] = end_x;
				int p0 = 2 * dx - dy;
				bresenham(x, p0, 1, length, dy, dx);
			}
			else {  //m < -1
				x[0] = -start_x;
				x[length - 1] = -end_x;
				int p0 = 2 * (-dx) - dy;
				bresenham(x, p0, 1, length, dy, -dx);
				for (int i = 0; i < length; ++i) {
					x[i] = -x[i];
				}
			}
		}
	}
	//将计算结果保存打line中
	for (int i = 0; i < length; ++i) {
		line.gl_x.push_back(x[i]);
		line.gl_y.push_back(y[i]);
	}
}

/*
* 将保存在Line结构中的坐标值转换为OpenGL能渲染的点。包括属性扩展以及坐标值归为（-1,1）
* lines: 需要转换的坐标集合
* point_num:坐标集合中像素点的数目
*/
float* createVertices(vector<Line>& lines, int& point_num) {
	point_num = 0;
	//计算有多少个像素点
	for (int i = 0; i < lines.size(); i++) {
		point_num += lines[i].gl_x.size();
	}
	//用于存储转换后结果，每个像素点有6个值，包括位置属性和颜色属性
	float* vertices = new float[point_num * 6];
	int index = 0;
	//进行转换
	for (int i = 0; i < lines.size(); i++) {
		for (int j = 0; j < lines[i].gl_x.size(); ++j) {
			//position
			vertices[index++] = (float)lines[i].gl_x[j]/WIN_WIDTH;
			vertices[index++] = (float)lines[i].gl_y[j]/WIN_HEIGHT;
			vertices[index++] = 0.0f;
			//colors
			vertices[index++] = 0.0f;
			vertices[index++] = 0.0f;
			vertices[index++] = 1.0f;
		}
	}
	return vertices;
}

/*
* 画圆时，根据八分对称原则，根据一个点得出八个点的坐标
* x0, y0: 圆心坐标
* x, y: 当前处理的点
* cc: 用于存储圆的坐标
*/
void getRejectionPoints(int x0, int y0, int x, int y, Line& cc) {
	cc.gl_x.push_back(x0 + x);
	cc.gl_y.push_back(y0 + y);

	cc.gl_x.push_back(x0 + x);
	cc.gl_y.push_back(y0 - y);

	cc.gl_x.push_back(x0 - x);
	cc.gl_y.push_back(y0 + y);

	cc.gl_x.push_back(x0 - x);
	cc.gl_y.push_back(y0 - y);

	cc.gl_x.push_back(x0 + y);
	cc.gl_y.push_back(y0 + x);

	cc.gl_x.push_back(x0 + y);
	cc.gl_y.push_back(y0 - x);

	cc.gl_x.push_back(x0 - y);
	cc.gl_y.push_back(y0 + x);

	cc.gl_x.push_back(x0 - y);
	cc.gl_y.push_back(y0 - x);
}

/*
* 使用bresenham算法画圆
* x0, y0:圆心坐标
* r: 圆的半径
* circle：用于存储构成圆的像素点坐标
*/
void drawCircle(int x0, int y0, int r, vector<Line>& circle) {
	int x = 0, y = r, d = 3 - 2 * r;
	Line cc;
	getRejectionPoints(x0, y0, x, y, cc);
	while (x < y) { //从（0，r)开始，顺时针画1/8圆，并通过八分对称原则，得出其他部分
		if (d < 0) {
			d = d + 4 * x + 6;
		}
		else {
			d = d + 4 * (x - y) + 10;
			--y;
		}
		++x;
		getRejectionPoints(x0, y0, x, y, cc);
	}
	circle.push_back(cc);
}

/*
* 根据两个端点，求出线段的一般表达式（Ax + By + C = 0)
* A = y2 - y1； B = x1 - x2； C = x2 * y1 - x1 * y2
* x1,y1,x2,y2:两个端点的坐标
*/
vector<int> lineExpression(int x1, int y1, int x2, int y2) {
	//line expression:Ax + By + C = 0
	vector<int> res;
	res.push_back(y2 - y1);
	res.push_back(x1 - x2);
	res.push_back(x2 * y1 - x1 * y2);
	
	return res;
}

/*
* 使用Edge Equations对三角形进行光栅化
* x1,y1,x2,y2,x3,y3:三角形三个顶点的坐标
* rasterized: 光栅化中新增的需要渲染的像素点的坐标
*/
void rasterizeTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Line& rasterized) {
	//找出x，y的最大值和最小值。即找出三角形的外接矩阵
	int max_x = max(x1, max(x3, x2));
	int min_x = min(x1, min(x2, x3));
	int max_y = max(y1, max(y2, y3));
	int min_y = min(y1, min(y2, y3));

	//求出三条边的一般表达式
	vector<vector<int>> lines;
	lines.push_back(lineExpression(x1, y1, x2, y2));
	lines.push_back(lineExpression(x1, y1, x3, y3));
	lines.push_back(lineExpression(x2, y2, x3, y3));

	//中心化
	for (int i = 0; i < 3; i++) {
		int temp_x, temp_y;
		if (i == 0) {
			temp_x = x3;
			temp_y = y3;
		}
		else if (i == 1) {
			temp_x = x2;
			temp_y = y2;
		}
		else {
			temp_x = x1;
			temp_y = y1;
		}
		if (lines[i][0] * temp_x + lines[i][1] * temp_y + lines[i][2] < 0) {
			for (int j = 0; j < 3; ++j) {
				lines[i][j] *= -1;
			}
		}
	}
	//获得像素点
	for (int x = min_x; x <= max_x; ++x) {
		for (int y = min_y; y <= max_y; ++y) {
			bool isInside = true;
			for (int i = 0; i < 3; i++) {
				if (lines[i][0] * x + lines[i][1] * y + lines[i][2] < 0) {
					isInside = false;
					break;
				}
			}
			if (isInside) {
				rasterized.gl_x.push_back(x);
				rasterized.gl_y.push_back(y);
			}
		}
	}
}

/*
* 画三角形
* vertexs: 三角形的三个顶点坐标
* triangle：用于存储画出的三角形的像素坐标
* isRas: 是否进行光栅化
*/
void drawTriangle(int vertexs[], vector<Line>& triangle, bool isRas) {
	Line line1, line2, line3;
	//画出三角形的三条边
	drawLine(vertexs[0], vertexs[1], vertexs[2], vertexs[3], line1);
	drawLine(vertexs[0], vertexs[1], vertexs[4], vertexs[5], line2);
	drawLine(vertexs[2], vertexs[3], vertexs[4], vertexs[5], line3);
	//存储三条边
	triangle.push_back(line1);
	triangle.push_back(line2);
	triangle.push_back(line3);
	//如果需要，进行光栅化
	if (isRas) {
		Line ras;
		rasterizeTriangle(vertexs[0], vertexs[1], vertexs[2], vertexs[3], vertexs[4], vertexs[5], ras);
		triangle.push_back(ras);
	}
}

int main(void) {
	//glfw基本设置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "GL_HW3", NULL, NULL);

	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//Viewport
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//着色器
	Shader myShader("shader.vs", "shader.fs");

	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//搭建ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();

	bool drawing_triangle = true; //是否在画三角形，若否，则是在画圆
	bool if_ras = false;   //三角形是否需要光栅化

	//三角形初始三个顶点坐标
	int triVertexs[] = {
		0, 200, -100, -200, 200, -200
	};
	//圆心及半径
	int x0 = 0, y0 = 0, R = 50;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glfwPollEvents();
		//建立ImGui窗口，并设置窗口样式
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//存储需要画的图形的像素点
		vector<Line> graphic;

		//画圆
		if (drawing_triangle == false) {
			ImGui::Begin("Circle Window", &drawing_triangle);
			ImGui::Text("Change the shape of the circle");
			ImGui::SliderInt("R", &R, 5, 600);
			ImGui::SliderInt("X_0", &x0, -550, 550);
			ImGui::SliderInt("Y_0", &y0, -550, 550);
			if (ImGui::Button("Triangle window")) {
				drawing_triangle = true;
			}
			ImGui::End();
			drawCircle(x0, y0, R, graphic);
		}
		//画三角形
		else {
			ImGui::Begin("Triangle Window", &drawing_triangle);
			ImGui::SliderInt("X_1", &triVertexs[0], -500, 500);
			ImGui::SliderInt("Y_1", &triVertexs[1], -500, 500);
			ImGui::SliderInt("X_2", &triVertexs[2], -500, 500);
			ImGui::SliderInt("Y_2", &triVertexs[3], -500, 500);
			ImGui::SliderInt("X_3", &triVertexs[4], -500, 500);
			ImGui::SliderInt("Y_3", &triVertexs[5], -500, 500);
			if (ImGui::Button("Cricle Window")) {
				drawing_triangle = false;
			}
			ImGui::Checkbox("rasterization?", &if_ras);
			ImGui::End();
			drawTriangle(triVertexs, graphic, if_ras);
		}
		int point_num;
		//得出可渲染的像素点坐标
		float* vertices = createVertices(graphic, point_num);	

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();

		//VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, point_num * 6 * sizeof(float), vertices, GL_STREAM_DRAW);
		//Link vertex contributes, tell opengl how to explain the vertex datas
		//Location
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);
		delete vertices;

		glDrawArrays(GL_POINTS, 0, point_num);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}