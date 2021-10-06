// Código feito por Gabriel Ely Konrath e Guilherme Closs Fraga
// Adaptado de Sabertooth disponível no github
// Projeto de um jogo de FPS básico

#include "Program.h"
#include "Mesh.h"

// functions
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// mouse
bool firstMouse = true;
float yawa = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitcha = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0 / 2.0f;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// gun
bool shotPressed = false;
int iterations = 0;
glm::vec3 shotMove;
glm::vec3 lastCameraFront;

// aux
int cont = 0;

Program::Program() {}

Program::~Program() {}

GLFWwindow* Program::GLFWInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "FPS_Game", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return 0;
	}
	glfwMakeContextCurrent(window);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return 0;
	}
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	return window;
}

void Program::ObjectsInit(map<string, Mesh*> meshs, map<string, char*> textures, string initial)
{
	coreShader = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader.Use();

	for (std::pair<const std::string, Mesh*> keyValuePair : meshs) {

		Mesh* mesh = keyValuePair.second;
		char* texturePath = textures.find(keyValuePair.first)->second;
		coreShader.Use();
		coreShader.LoadTexture(texturePath, "texture1", "stoneTexture");

		for (Group* group : mesh->getGroups()) {
			vector<float> vertices;
			vector<float> normais;
			vector<float> textures;

			for (Face* face : group->getFaces()) {
				for (int verticeID : face->getVertices()) {
					glm::vec3* vertice = mesh->vertice(verticeID - 1);
					vertices.push_back(vertice->x);
					vertices.push_back(vertice->y);
					vertices.push_back(vertice->z);

					group->increaseNumVertices();
				}

				for (int normalID : face->getNormais()) {
					glm::vec3* normal = mesh->normal(normalID - 1);
					normais.push_back(normal->x);
					normais.push_back(normal->y);
					normais.push_back(normal->z);
				}

				for (int textureID : face->getTextures()) {
					glm::vec2* texture = mesh->texture(textureID - 1);
					textures.push_back(texture->x);
					textures.push_back(texture->y);
				}
			}

			GLuint VBOvertices, VBOnormais, VBOtextures, VAO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBOvertices);
			glGenBuffers(1, &VBOnormais);
			glGenBuffers(1, &VBOtextures);

			// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
			glBindVertexArray(VAO);

			// Vertices
			glBindBuffer(GL_ARRAY_BUFFER, VBOvertices);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			// Normais
			glBindBuffer(GL_ARRAY_BUFFER, VBOnormais);
			glBufferData(GL_ARRAY_BUFFER, normais.size() * sizeof(float), normais.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);

			// Textures
			glBindBuffer(GL_ARRAY_BUFFER, VBOtextures);
			glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), textures.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(2);

			group->setVAO(&VAO);
			glBindVertexArray(0); // Unbind VAO
		}
	}
}

void Program::Run(Shader shader, GLFWwindow* window, GLuint VAO, glm::vec3 objectsPositions[], glm::vec3 cubesPositions[], map<string, Mesh*> meshs)
{
	// game loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		coreShader.UseTexture("stoneTexture");
		coreShader.Use();

		glm::mat4 view(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		coreShader.setMatrix4fv("projection", proj);
		coreShader.setMatrix4fv("view", view);

		// objects
		glUniform1i(glGetUniformLocation(coreShader.program, "tex"), 0);
		for (std::pair<const std::string, Mesh*> keyValuePair : meshs) {
			for (Group* group : keyValuePair.second->getGroups()) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				model = glm::translate(model, objectsPositions[cont]);
				glBindVertexArray(group->getVAO());

				coreShader.setMatrix4fv("model", model);
				glDrawArrays(GL_TRIANGLES, 0, group->getNumVertices());
				//glBindVertexArray(0);
			}
		}
		cont = 0;

		// render boxes
		glUniform1i(glGetUniformLocation(coreShader.program, "tex"), 1);
		coreShader.Use();
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			//if (!checkColision(cubePositions[i])) {
				// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubesPositions[i]);
			coreShader.setMatrix4fv("model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//}
		}

		// shots
		glUniform1i(glGetUniformLocation(coreShader.program, "tex"), 2);
		if (shotPressed) {
			if (iterations == 0) {
				glm::vec3 temp = cameraPos;
				temp[1] = 1.3f;
				shotMove = temp;
				lastCameraFront = cameraFront;
			}
			iterations += 1;
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, shotMove);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			float shotSpeed = 70.0 * deltaTime;
			shotMove += shotSpeed * lastCameraFront;
			model = glm::translate(model, shotMove);
			coreShader.setMatrix4fv("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			if (iterations >= 150) {
				shotPressed = false;
				iterations = 0;
			}
		}

		glfwSwapBuffers(window);
	}
}

void Program::Finish(GLuint VAO)
{
	glDeleteVertexArrays(1, &VAO);
	coreShader.Delete();
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 8.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		shotPressed = true;
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

	float sensitivity = 0.4f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yawa += xoffset;
	pitcha += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitcha > 89.0f)
		pitcha = 89.0f;
	if (pitcha < -89.0f)
		pitcha = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yawa)) * cos(glm::radians(pitcha));
	front.y = 0;//sin(glm::radians(pitcha));
	front.z = sin(glm::radians(yawa)) * cos(glm::radians(pitcha));
	cameraFront = glm::normalize(front);
}