#ifndef PROGRAM_H
#define PROGRAM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"
#include "Mesh.h"
#include "ObjReader.h"

class Program
{
private:
	// Screen
	const GLint WIDTH = 800, HEIGHT = 600;
	int screenWidth, screenHeight;

public:
	GLFWwindow* window;
	Shader coreShader;

public:
	Program();
	~Program();

	GLFWwindow* GLFWInit();
	void ObjectsInit(map<string, Mesh*> meshs, map<string, char*> textures, string initial);
	void Run(Shader shader, GLFWwindow* window, GLuint VAO, glm::vec3 objectsPositions[], glm::vec3 cubesPositions[], map<string, Mesh*> meshs);
	void Finish(GLuint VAO);
};

#endif