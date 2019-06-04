#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace std;

#define WIN_WIDTH 600            //���ڿ��
#define WIN_HEIGHT 600           //���ڸ߶�

//�������봦��
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//���ڴ�СӦ��
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*
* ���ڼ�¼һ����Ҫ��Ⱦ���߶ε�����
*/
typedef struct {
	vector<int> gl_x;
	vector<int> gl_y;
} Line;

/*
* ����a��b�е����ֵ
*/
int max(int a, int b) {
	return a > b ? a : b;
}

/*
* ����a��b�е���Сֵ
*/
int min(int a, int b) {
	return a > b ? b : a;
}

/*
* bresenham�㷨���߶Ρ��ʺ�0 <= m <= 1���߶�
* value�����ڴ洢�����y������
* p����һ��pֵ
* depth�� ��ǰ�ݹ����
* num����Ҫ������ĵ����Ŀ
* dx��dy�����������ϵģ�ʼ����յ�Ĳ��
*/
void bresenham(int value[], int p, int depth, int num, int dx, int dy) {
	if (depth == num) return; //�����������Ҫ�����е㣬�����ݹ�
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
* �������߶εĴ�����Ҫ�����߶���x�ᴹֱ���߶���y�ᴹֱ��Ϊ�ӿ���㣬�������ǣ���|m| <= 1��|m| > 1�������
* x1,y1,x2,y2: �����߶ε������˵�
* line����¼�߶����������ص������
*/
void drawLine(int x1, int y1, int x2, int y2, Line& line) {
	int x[WIN_WIDTH*2], y[WIN_HEIGHT*2]; //�����洢
	int start_x = x1, start_y = y1, end_x = x2, end_y = y2;
	int length;   //�߶������ص���Ŀ
	//��ֱx���ֱ��
	if (start_x == end_x) {
		if (start_y > end_y) { //�涨�������ϴ���
			start_y = y2;
			end_y = y1;
		}
		length = end_y - start_y + 1;
		for (int i = 0; i < length; ++i) {
			x[i] = start_x;
			y[i] = start_y + i;
		}
	} 
	else if(start_y == end_y) {  //��ֱy���ֱ��
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
		float m = (float)(end_y - start_y) / (end_x - start_x); //б��
		int dx, dy;
		//��|m|<=1ʱ��x�����������y��ֵ
		if (fabs(m) <= 1) {
			if (end_x < start_x) {  //�������Ҽ���
				start_x = x2;
				start_y = y2;
				end_x = x1;
				end_y = y1;
			}
			dx = end_x - start_x;
			dy = end_y - start_y;
			m = (float)dy / dx;  //���¼���б��
			length = end_x - start_x + 1;

			for (int i = 0; i < length; ++i) {
				x[i] = start_x + i;
			}
			//0 <= m <= 1����ʱ��ֱ��ʹ��bresenham���������߶η���Ϊ����������
			if (dy >= 0) {
				y[0] = start_y;
				y[length - 1] = end_y;
				int p0 = 2 * dy - dx;
				bresenham(y, p0, 1, length, dx, dy);
			}
			/*
			*  -1 <= m < 0����ʱ�߶η���Ϊ�����������£�Ϊ����ʹ��bresenham�����������ȼ����߶ι���x��ĶԳ��߶�
			* ����y���������еĴ���ȡ�����õ��Գ��߶ε����ص�����󣬽��Գ��߶ε��������ص���y����ȡ������õ�ԭ�߶ε�
			* ���ص�����
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
		//��|m|>1ʱ��y�����������x��ֵ��ʵ��Ϊ��������x��y����ݵ�������
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
	//�������������line��
	for (int i = 0; i < length; ++i) {
		line.gl_x.push_back(x[i]);
		line.gl_y.push_back(y[i]);
	}
}

/*
* ��������Line�ṹ�е�����ֵת��ΪOpenGL����Ⱦ�ĵ㡣����������չ�Լ�����ֵ��Ϊ��-1,1��
* lines: ��Ҫת�������꼯��
* point_num:���꼯�������ص����Ŀ
*/
float* createVertices(vector<Line>& lines, int& point_num) {
	point_num = 0;
	//�����ж��ٸ����ص�
	for (int i = 0; i < lines.size(); i++) {
		point_num += lines[i].gl_x.size();
	}
	//���ڴ洢ת��������ÿ�����ص���6��ֵ������λ�����Ժ���ɫ����
	float* vertices = new float[point_num * 6];
	int index = 0;
	//����ת��
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
* ��Բʱ�����ݰ˷ֶԳ�ԭ�򣬸���һ����ó��˸��������
* x0, y0: Բ������
* x, y: ��ǰ����ĵ�
* cc: ���ڴ洢Բ������
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
* ʹ��bresenham�㷨��Բ
* x0, y0:Բ������
* r: Բ�İ뾶
* circle�����ڴ洢����Բ�����ص�����
*/
void drawCircle(int x0, int y0, int r, vector<Line>& circle) {
	int x = 0, y = r, d = 3 - 2 * r;
	Line cc;
	getRejectionPoints(x0, y0, x, y, cc);
	while (x < y) { //�ӣ�0��r)��ʼ��˳ʱ�뻭1/8Բ����ͨ���˷ֶԳ�ԭ�򣬵ó���������
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
* ���������˵㣬����߶ε�һ����ʽ��Ax + By + C = 0)
* A = y2 - y1�� B = x1 - x2�� C = x2 * y1 - x1 * y2
* x1,y1,x2,y2:�����˵������
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
* ʹ��Edge Equations�������ν��й�դ��
* x1,y1,x2,y2,x3,y3:�������������������
* rasterized: ��դ������������Ҫ��Ⱦ�����ص������
*/
void rasterizeTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Line& rasterized) {
	//�ҳ�x��y�����ֵ����Сֵ�����ҳ������ε���Ӿ���
	int max_x = max(x1, max(x3, x2));
	int min_x = min(x1, min(x2, x3));
	int max_y = max(y1, max(y2, y3));
	int min_y = min(y1, min(y2, y3));

	//��������ߵ�һ����ʽ
	vector<vector<int>> lines;
	lines.push_back(lineExpression(x1, y1, x2, y2));
	lines.push_back(lineExpression(x1, y1, x3, y3));
	lines.push_back(lineExpression(x2, y2, x3, y3));

	//���Ļ�
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
	//������ص�
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
* ��������
* vertexs: �����ε�������������
* triangle�����ڴ洢�����������ε���������
* isRas: �Ƿ���й�դ��
*/
void drawTriangle(int vertexs[], vector<Line>& triangle, bool isRas) {
	Line line1, line2, line3;
	//���������ε�������
	drawLine(vertexs[0], vertexs[1], vertexs[2], vertexs[3], line1);
	drawLine(vertexs[0], vertexs[1], vertexs[4], vertexs[5], line2);
	drawLine(vertexs[2], vertexs[3], vertexs[4], vertexs[5], line3);
	//�洢������
	triangle.push_back(line1);
	triangle.push_back(line2);
	triangle.push_back(line3);
	//�����Ҫ�����й�դ��
	if (isRas) {
		Line ras;
		rasterizeTriangle(vertexs[0], vertexs[1], vertexs[2], vertexs[3], vertexs[4], vertexs[5], ras);
		triangle.push_back(ras);
	}
}

int main(void) {
	//glfw��������
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

	//��ɫ��
	Shader myShader("shader.vs", "shader.fs");

	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//�ImGui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();

	bool drawing_triangle = true; //�Ƿ��ڻ������Σ����������ڻ�Բ
	bool if_ras = false;   //�������Ƿ���Ҫ��դ��

	//�����γ�ʼ������������
	int triVertexs[] = {
		0, 200, -100, -200, 200, -200
	};
	//Բ�ļ��뾶
	int x0 = 0, y0 = 0, R = 50;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glfwPollEvents();
		//����ImGui���ڣ������ô�����ʽ
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//�洢��Ҫ����ͼ�ε����ص�
		vector<Line> graphic;

		//��Բ
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
		//��������
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
		//�ó�����Ⱦ�����ص�����
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