//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include "SkyBox.hpp"

enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT };
enum ROTATE_DIRECTION { ROATE_UP, ROTATE_DOWN, ROTATE_RIGHT, ROTATE_LEFT };
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);

GLuint shadowMapFBO;
GLuint depthMapTexture;

int glWindowWidth = 2000;
int glWindowHeight = 900;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 3048;
const unsigned int SHADOW_HEIGHT = 3048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
glm::mat4 view1;
GLuint viewLoc;
glm::mat4 projection;
glm::mat4 projection1;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::mat3 lightDirMatrix;
GLuint lightDirMatrixLoc;

glm::vec3 lightPos1;
GLuint lightPos1Loc;
glm::vec3 lightPos2;
GLuint lightPos2Loc;
glm::vec3 lightPos3;
GLuint lightPos3Loc;
glm::vec3 lightPos4;
GLuint lightPos4Loc;
glm::vec3 lightPos5;
GLuint lightPos5Loc;


GLuint inHouseLoc;

glm::vec3 lightDir;
GLuint lightDirLoc;

glm::vec3 lightColor;
int enableFog;
GLuint lightColorLoc;
GLuint enableFogLoc;

glm::vec3 far1;
GLuint far1Loc;

glm::vec3 far2;
GLuint far2Loc;

glm::vec3 far3;
GLuint far3Loc;

glm::vec3 far4;
GLuint far4Loc;




GLuint enviroMapLoc;
float camX = sin(glfwGetTime());

gps::Camera myCamera(
				glm::vec3(0.0f, -1.0f, 1.0f), 
				glm::vec3(0.0f, 0.0f, -2.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));

bool pressedKeys[1024];
float angleY;
bool enableNight;
GLfloat lightAngle;

// tour animation variables
bool ok = false;
bool presentation = false;
int i = 0;

float alpha; //transparenta
GLuint alphaLoc;


GLuint glassLoc;//reflexie
bool glass;

//lumina masina
GLuint masinaPornitaLoc;
bool masinaPornita;

gps::Model3D geamuri;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D ocean;
gps::Model3D piscina;
gps::Model3D lights;
gps::Model3D scena;
gps::Model3D obiecte;
gps::Model3D usa;
gps::Model3D colac;
gps::Model3D masina;
gps::Model3D piese;
gps::Model3D pat;
gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader depthShader;
gps::Shader screenQuadShader;

//variablie pentru animatia usii
float doorPositonX;
float doorPositonY;
float doorAngle;
bool openDoor;
bool inHouse;


//variabile pentru palpait
int enableLight;
GLuint lightEnableLoc;

//pentru skybox
std::vector<const GLchar*> faces;
void param() {
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");
}

gps::Shader skyboxShader;
gps::SkyBox mySkyBox;



bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;


//GLuint shadowMapFBO;
//GLuint depthMapTexture;

bool showDepthMap = false;

GLenum glCheckError_(const char *file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	myCustomShader.useShaderProgram();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

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

	myCamera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	myCamera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void processMovement()
{
	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glWindow, true);


	//pentru prezentarea scenei
	if (glfwGetKey(glWindow, GLFW_KEY_T)) {
		myCamera.setCameraPos(glm::vec3(4.45f, 6.6f, -40.69f));
		presentation = true;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_Y)) {
		presentation = false;
	}

	if (pressedKeys[GLFW_KEY_4]) {
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //zi
		enableNight = false;
	}

	if (pressedKeys[GLFW_KEY_5]) {
		lightColor = glm::vec3(0.1f, 0.1f, 0.1f); //noapte
		enableNight = true;
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		angleY -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angleY += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_1) == GLFW_PRESS) { //wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_2) == GLFW_PRESS) {//solid
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(glWindow, GLFW_KEY_3) == GLFW_PRESS) {//polygonal
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (pressedKeys[GLFW_KEY_Z]) { //smooth
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS) {
		myCamera.move(gps::MOVE_FORWARD, deltaTime);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS) {
		myCamera.move(gps::MOVE_BACKWARD, deltaTime);
	}
	   
	if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS) {
		myCamera.move(gps::MOVE_LEFT, deltaTime);	
	}

	if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS) {
		myCamera.move(gps::MOVE_RIGHT, deltaTime);
	}

	if (glfwGetKey(glWindow, GLFW_KEY_O) == GLFW_PRESS) {
		openDoor = 1;
		inHouse = true;
	}
	if (glfwGetKey(glWindow, GLFW_KEY_C) == GLFW_PRESS) {
		openDoor = 0;
		inHouse = false;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_0) == GLFW_PRESS) {
		masinaPornita = true;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_9) == GLFW_PRESS) {
		masinaPornita = false;
	}

}



bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	//glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetScrollCallback(glWindow, scroll_callback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);
	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	scena.LoadModel("objects/s.obj");
	geamuri.LoadModel("objects/geamuri.obj");
	lightCube.LoadModel("objects/lumina.obj");
	usa.LoadModel("objects/usa.obj");
	piscina.LoadModel("objects/piscina.obj");
	colac.LoadModel("objects/colac.obj");
	masina.LoadModel("objects/masina.obj");
	screenQuad.LoadModel("objects/quad.obj");
	piese.LoadModel("objects/piese.obj");
	pat.LoadModel("objects/pat.obj");
	
}

void initShaders() {
	glShadeModel(GL_SMOOTH);
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();

	mySkyBox.Load(faces);

	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram(); 
	
    lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
    lightShader.useShaderProgram();

	depthShader.loadShader("shaders/shader.vert", "shaders/shader.frag");
	depthShader.useShaderProgram();

}

void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//lightDirMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirMatrix");

	enviroMapLoc = glGetUniformLocation(myCustomShader.shaderProgram, "enviroMap");

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(1.0f, 1.0f, 1.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");	
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//primul felinar
	lightPos1 = normalize(glm::vec3(44.9f, -8.1823f, -27.209f));
	lightPos1Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos1");
	glUniform3fv(lightPos1Loc, 1, glm::value_ptr(lightPos1));

	//al doilea felinar
	lightPos2 = glm::vec3(31.951f, 0.29f, -43.76f);
	lightPos2Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos2");
	glUniform3fv(lightPos2Loc, 1, glm::value_ptr(lightPos2));

	//lumina becului din casa
	lightPos3 = glm::vec3(-33.097f, 3.2102f, -11.498f);
	lightPos3Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos3");
	glUniform3fv(lightPos3Loc, 1, glm::value_ptr(lightPos3));

	//lumina care bate pe masa
	lightPos4 = glm::vec3(-34.467f, -0.4402f, -12.7f);
	lightPos4Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos4");
	glUniform3fv(lightPos4Loc, 1, glm::value_ptr(lightPos4));

	//lumina frunzei
	lightPos5 = glm::vec3(-33.467f, 3.3402f, -11.7f);
	lightPos5Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos5");
	glUniform3fv(lightPos5Loc, 1, glm::value_ptr(lightPos5));


	//faruri masina
	far1 = glm::vec3(-6.5464f, -6.3621f, -14.755f);
	far1Loc = glGetUniformLocation(myCustomShader.shaderProgram, "far1");
	glUniform3fv(far1Loc, 1, glm::value_ptr(far1));

	
	far2 = glm::vec3(-5.7959f, -6.2752f, -14.652f);
	far2Loc = glGetUniformLocation(myCustomShader.shaderProgram, "far2");
	glUniform3fv(far2Loc, 1, glm::value_ptr(far2));

	far3 = glm::vec3(-2.2743f, -6.4994f, -15.982f);
	far3Loc = glGetUniformLocation(myCustomShader.shaderProgram, "far3");
	glUniform3fv(far3Loc, 1, glm::value_ptr(far3));


	far4 = glm::vec3(-1.5959f, -6.4752f, -15.952f);
	far4Loc = glGetUniformLocation(myCustomShader.shaderProgram, "far4");
	glUniform3fv(far4Loc, 1, glm::value_ptr(far4));

	

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

}

void initFBO() {
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture,0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	glm::vec3 lightDirLoc = glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir;

	glm::vec3 pos = glm::vec3(1.0f, 1.0f, 1.0f);

	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)); //ca sa misc umbra
	glm::mat4 lightView = glm::lookAt(lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
	const GLfloat near_plane = -50.0f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);

	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView * lightRotation;

	return lightSpaceTrMatrix;
}
void drawObjects(gps::Shader shader, bool depthPass) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader.useShaderProgram();
	//USA
	glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
	glUniform1f(glassLoc, false);
	if (openDoor) {
		if (doorAngle < 70) {
			doorAngle += 1;
		}
	}else {
		if (doorAngle > 0) {
			doorAngle -= 1;
		}
	}
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-26.67f, -1.7871f, -17.241f));
	model = glm::rotate(model , glm::radians(doorAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(26.67f, 1.7871f, 17.241f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	usa.Draw(shader);

	 //SCENA
	glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
	glUniform1f(glassLoc, false);
	///model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	inHouseLoc = glGetUniformLocation(shader.shaderProgram, "inHouse");
	glUniform1f(inHouseLoc, inHouse);

	scena.Draw(shader);
	//PIESE
	glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
	glUniform1f(glassLoc, false);
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	piese.Draw(shader);

	
	//MASINA

	masinaPornitaLoc = glGetUniformLocation(shader.shaderProgram, "masinaPornita");
	glUniform1f(masinaPornitaLoc, masinaPornita);

		glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
		glUniform1f(glassLoc, true);
	
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
    masina.Draw(shader);

	//GEAMURI
	if (openDoor) {
		glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
		glUniform1f(glassLoc, false);
	}
	else{
	glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
	glUniform1f(glassLoc, true);
	}

	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	alpha = 0.7;
	alphaLoc = glGetUniformLocation(shader.shaderProgram, "alpha");
	glUniform1f(alphaLoc, alpha);
	piscina.Draw(shader);
	mySkyBox.Draw(skyboxShader, view, projection);
	geamuri.Draw(shader);
	glDisable(GL_BLEND); //disable blending

	
	//PISCINA
	glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
	glUniform1f(glassLoc, true);
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	alpha = 0.5;
	alphaLoc = glGetUniformLocation(shader.shaderProgram, "alpha");
	glUniform1f(alphaLoc, alpha);
	piscina.Draw(shader);
	glDisable(GL_BLEND); //disable blending

	//COLAC
	glassLoc = glGetUniformLocation(shader.shaderProgram, "glass");
	glUniform1f(glassLoc, true);
	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0, 1, 0));
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	colac.Draw(shader);
}

void renderScene() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	std::cout << glm::to_string(myCamera.getCameraPos()) << std::endl;

	
	// processMovement();

	 if (presentation == 1) {
		 if (i++ > 1400) {
			 i = 0;
			 presentation = false;
		 }
		 else {
			 if (i <= 180) {
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(-28.4f, 1.1f, -18.2f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_FORWARD, 0.035);
			 }
			 else if (i > 180 && i <= 280) {
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(-28.6f, 0.46f, -12.08f) - myCamera.getCameraPos()));
				 openDoor = true;
				 inHouse = true;
				 myCamera.move(gps::MOVE_LEFT, 0.015f);
			 }
			 else if (i > 280 && i <= 380) {
				
				 myCamera.move(gps::MOVE_FORWARD, 0.015f);
			 }
			 else if (i > 380 && i <= 520) {
				 lightColor = glm::vec3(0.1f, 0.1f, 0.1f); //noapte
				 enableNight = true;
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(-30.6f, 1.1f, -11.08f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_LEFT, 0.015f);
		      }
			
			 else if (i > 520 && i <= 800) {
				 openDoor = false;
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(-20.6f, -0.04f, -16.08f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_RIGHT, 0.02f);
				 masinaPornita = true;
			 }

			 else if (i > 800 && i <= 950) {
				 //openDoor = false;
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(16.4f, -5.04f, -16.08f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_FORWARD, 0.03);
				 masinaPornita = true;
			 }
			 else if (i >950  && i < 1100) {
				 //openDoor = false;
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(21.5f, -7.03f, -23.08f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_LEFT, 0.02);
				 masinaPornita = true;
			 }
			 else {
				 masinaPornita = false;
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(34.5f, 0.83f, -61.08f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_BACKWARD, 0.02);
				 if (i > 1200) {
					 masinaPornita = true;
				 }
			 }
			



			 /*
			 else if (i > 400 && i < 500) {
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(40.1f, 8.2f, -29.752f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_FORWARD, 0.02);
			 }

			 else if (i > 500 && i < 800) {
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(-31.0f, -34.2f, -17.752f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_BACKWARD, 0.05);
			 }
			 else if (i > 800 && i < 900) {
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(35.0f, 10.5f, -4.752f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_LEFT, 0.04);
			 }
			 else {
				 myCamera.setCameraTarget(glm::normalize(glm::vec3(-19.1f, -0.102f, 20.752f) - myCamera.getCameraPos()));
				 myCamera.move(gps::MOVE_LEFT, 0.03);
			 }*/
		 }
	 }



	 depthShader.useShaderProgram();
	 glUniformMatrix4fv(glGetUniformLocation(depthShader.shaderProgram, "lightSpaceTrMatrix"),
		 1,
		 GL_FALSE,
		 glm::value_ptr(computeLightSpaceTrMatrix()));
	 glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	 glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	 glClear(GL_DEPTH_BUFFER_BIT);
	 drawObjects(depthShader, false);
	 //RenderTheScene();
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 if (showDepthMap) {
		 glViewport(0, 0, retina_width, retina_height);

		 glClear(GL_COLOR_BUFFER_BIT);

		 screenQuadShader.useShaderProgram();

		 //bind the depth map
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		 glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		 glDisable(GL_DEPTH_TEST);
		 screenQuad.Draw(screenQuadShader);
		 glEnable(GL_DEPTH_TEST);
	 }
	 else {

		 glViewport(0, 0, retina_width, retina_height);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 myCustomShader.useShaderProgram();

		 glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
		
		 view = myCamera.getViewMatrix();
		 glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	    glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		 //bind the shadow map
		 glActiveTexture(GL_TEXTURE3);
		 glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		 glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		 glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			 1,
			 GL_FALSE,
			 glm::value_ptr(computeLightSpaceTrMatrix()));
		 
		 drawObjects(myCustomShader, false);

		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_CUBE_MAP, mySkyBox.GetTextureId());
		 glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "skyBox"), 0);

		 /*if (enableLight == 1) {
			 enableLight = 0;
		 }

		 if (enableLight == 0) {
			 enableLight = 1;
		 }

		// myCustomShader.useShaderProgram();
		// lightEnableLoc = glGetUniformLocation(myCustomShader.shaderProgram, "enableLight");
		// glUniform1i(lightEnableLoc, enableLight);*/

		 //draw a white cube around the light
		 
		// lightShader.useShaderProgram();

		 //glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		 //model = lightRotation;
		// model = glm::translate(model, 1.0f * lightDir);
		// model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	//	 glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//		 lightCube.Draw(lightShader);

		 mySkyBox.Draw(skyboxShader, view, projection);
	}
}

void cleanup() {
	glDeleteTextures(1,& depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char * argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}
	param();

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processMovement();
		renderScene();		

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
