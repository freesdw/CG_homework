#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
using namespace std;

//Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

/*
* An abstract camera class that processes input and calculates the corresponding Euler Angles,
* Vectors and Matricees for use in OpenGL
*/
class Camera
{
public:
	//Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
		float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSentivity(SENSITIVITY),MovementSpeed(SPEED), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	
	//Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	/*
	* Processes input received from any keyboard-like input system.
	*/
	void moveForward(GLfloat const deltaTime) {
		Position += Front * MovementSpeed * deltaTime;
	}

	void moveBack(GLfloat const deltaTime) {
		Position -= Front * MovementSpeed * deltaTime;
	}

	void moveRight(GLfloat const deltaTime) {
		Position += Right * MovementSpeed * deltaTime;
	}

	void moveLeft(GLfloat const deltaTime) {
		Position -= Right * MovementSpeed * deltaTime;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSentivity;
		yoffset *= MouseSentivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVectors();
	}

private:
	//Calculate the front vector from thee Camera's (updated) Euler Angles
	void updateCameraVectors() {
		//Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		//Also re-calculatee the Right and Up vetor
		Right = glm::normalize(glm::cross(Front, WorldUp)); //Normalize the vectors
		Up = glm::normalize(glm::cross(Right, Front));
	}

private:
	//Cameera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	//Camera options
	float MovementSpeed;
	float MouseSentivity;
	float Zoom;
};