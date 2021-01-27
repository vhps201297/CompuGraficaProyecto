/*------------------------------------------------------------------------*/
/* ---------------------         Proyecto final      ---------------------*/
/* ---------------------             CGeIHC          ---------------------*/
/* ---------------------             2021-1          ---------------------*/
/* ---------------------            Alumnos          ---------------------*/
/* --------------------- Granados Gómez Nanci Noelia ---------------------*/
/* ---------------------   Pólito Seba Víctor Hugo   ---------------------*/
/*------------------------------------------------------------------------*/
//#include <glew.h>
#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>
#include<MMSystem.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

#include "cilindro.h"
#include "Cono.h"

//#pragma comment(lib, "winmm.lib")

Cilindro cilindro(1.0);
Cono cono(1.0);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void myData(void);
void display(Shader);
void getResolution(void);
void animate(void);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint skyboxVBO, skyboxVAO;

// camera
Camera camera(glm::vec3(0.0f, 100.0f, 350.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f, lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

float	mov_pelota_x = 0.0f,
		mov_pelota_y = 0.0f,
		mov_pelota_z = 0.0f,
		mov_globo_x = 0.0f,
		mov_globo_z = 0.0f,
		mov_globo_y = 0.0f,
		mov_auto_x = 0.0f,
		mov_auto_y = 0.0f,
		mov_auto_z = 0.0f,
		mov_fenix = 0.0f,
		orienta = 0.0f;

bool	animacion_globo = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		recorrido6 = false,
		recorrido7 = false,
		recorrido8 = false,
		recorrido9 = false,
		recorrido10 = false;

bool	animacion_pelota = false,
		recorrido1_pelota = true,
		recorrido2_pelota = false,
		recorrido3_pelota = false,
		recorrido4_pelota = false,
		recorrido5_pelota = false,
		recorrido6_pelota = false,
		recorrido7_pelota = false;

bool anim_auto = false;
bool rec_auto1 = true;
bool rec_auto2 = false;
bool rec_auto3 = false;
boolean auto_in = false;
bool esta_auto0 = true;
bool esta_auto1 = false;
bool esta_auto2 = false;
bool esta_auto3 = false;
bool esta_auto4 = false;
bool esta_auto5 = false;
bool esta_auto6 = false;
bool esta_auto7 = false;
bool esta_auto8 = false;
bool esta_estacionado = true;

//Keyframes (Manipulación y dibujo)
float	mov_brazo_der = 0.0f,
		mov_brazo_izq = 0.0f,
		mov_cuerpo_Y = 0.0f,
		mov_cuerpo_X = 0.0f,
		mov_cuerpo_Z = 0.0f,
		gira_cuerpo = 0.0f;

float mov_techo;

float	inc_mov_brazo_der = 0.0f,
		inc_mov_brazo_izq = 0.0f,
		inc_mov_cuerpo_Y = 0.0f,
		inc_mov_cuerpo_X = 0.0f,
		inc_mov_cuerpo_Z = 0.0f,
		inc_gira_cuerpo = 0.0f;

//Molino
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f,
		scale = 8.0f;

float rot_helice;

#define MAX_FRAMES 10
int i_max_steps = 60;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float mov_brazo_der = 0.0f;
	float mov_brazo_izq = 0.0f;
	float mov_cuerpo_Y = 0.0f;
	float mov_cuerpo_X = 0.0f;
	float mov_cuerpo_Z = 0.0f;
	float gira_cuerpo = 0.0f;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 27;			//introducir datos
bool play = false;
int playIndex = 0;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].mov_brazo_der = mov_brazo_der;
	KeyFrame[FrameIndex].mov_brazo_izq = mov_brazo_izq;
	KeyFrame[FrameIndex].mov_cuerpo_Y = mov_cuerpo_Y;
	KeyFrame[FrameIndex].gira_cuerpo = gira_cuerpo;
	KeyFrame[FrameIndex].mov_cuerpo_X = mov_cuerpo_X;
	KeyFrame[FrameIndex].mov_cuerpo_Z = mov_cuerpo_Z;

	FrameIndex++;
}

void resetElements(void)
{
	mov_brazo_der = KeyFrame[0].mov_brazo_der;
	mov_brazo_izq = KeyFrame[0].mov_brazo_izq;
	mov_cuerpo_Y = KeyFrame[0].mov_cuerpo_Y;
	gira_cuerpo = KeyFrame[0].gira_cuerpo;
	mov_cuerpo_X = KeyFrame[0].mov_cuerpo_X;
	mov_cuerpo_Z = KeyFrame[0].mov_cuerpo_Z;
}

void interpolation(void)
{
	inc_mov_brazo_der = (KeyFrame[playIndex + 1].mov_brazo_der - KeyFrame[playIndex].mov_brazo_der) / i_max_steps;
	inc_mov_brazo_izq = (KeyFrame[playIndex + 1].mov_brazo_izq - KeyFrame[playIndex].mov_brazo_izq) / i_max_steps;
	inc_mov_cuerpo_Y = (KeyFrame[playIndex + 1].mov_cuerpo_Y - KeyFrame[playIndex].mov_cuerpo_Y) / i_max_steps;
	inc_gira_cuerpo = (KeyFrame[playIndex + 1].gira_cuerpo - KeyFrame[playIndex].gira_cuerpo) / i_max_steps;
	inc_mov_cuerpo_X = (KeyFrame[playIndex + 1].mov_cuerpo_X - KeyFrame[playIndex].mov_cuerpo_X) / i_max_steps;
	inc_mov_cuerpo_Z = (KeyFrame[playIndex + 1].mov_cuerpo_Z - KeyFrame[playIndex].mov_cuerpo_Z) / i_max_steps;
}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}


void animate(void)
{
	mov_fenix = (mov_fenix < 360) ? mov_fenix + 1.0f : 0.0f;
	rot_helice = (rot_helice < 360) ? rot_helice + 1.0f : 0.0f;

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			mov_brazo_der += inc_mov_brazo_der;
			mov_brazo_izq += inc_mov_brazo_izq;
			mov_cuerpo_Y += inc_mov_cuerpo_Y;
			mov_cuerpo_X += inc_mov_cuerpo_X;
			mov_cuerpo_Z += inc_mov_cuerpo_Z;
			gira_cuerpo += inc_gira_cuerpo;

			i_curr_steps++;
		}
	}

	if (animacion_pelota)
	{
		if (recorrido1_pelota)
		{
			mov_pelota_z += 0.05f;
			mov_pelota_x -= 0.45f;
			if (mov_pelota_x < -60.0f)
			{
				recorrido1_pelota = false;
				recorrido2_pelota = true;
			}
		}
		if (recorrido2_pelota)
		{
			mov_pelota_x += 0.2f;
			mov_pelota_z += 0.4f;
			if (mov_pelota_x > -46.0f)
			{
				recorrido2_pelota = false;
				recorrido3_pelota = true;
			}
		}
		if (recorrido3_pelota)
		{
			mov_pelota_x += 0.2f;
			mov_pelota_z -= 0.4f;
			if (mov_pelota_z < -5.0f)
			{
				recorrido3_pelota = false;
				recorrido4_pelota = true;
			}
		}
		if (recorrido4_pelota)
		{
			mov_pelota_x += 0.3f;
			mov_pelota_z += 0.3f;
			if (mov_pelota_x > 13.0f)
			{
				recorrido4_pelota = false;
				recorrido5_pelota = true;
			}
		}
		if (recorrido5_pelota)
		{
			mov_pelota_x -= 0.1f;
			mov_pelota_z += 0.4f;
			if (mov_pelota_z > 88.0f)
			{
				recorrido5_pelota = false;
				recorrido6_pelota = true;
			}
		}
		if (recorrido6_pelota)
		{
			mov_pelota_x -= 0.3f;
			mov_pelota_z -= 0.3f;
			if (mov_pelota_x < -20.0f)
			{
				recorrido6_pelota = false;
				recorrido7_pelota = true;
			}
		}
		if (recorrido7_pelota)
		{
			mov_pelota_x += 0.1f;
			mov_pelota_z -= 0.31f;
			if (mov_pelota_z <= 0.0f)
			{
				recorrido7_pelota = false;
				recorrido1_pelota = true;
			}
		}

	}

	if (animacion_globo)
	{
		if (recorrido1)
		{
			mov_globo_z -= 0.6f;
			mov_globo_y += 0.2;
			if (mov_globo_z <= -75.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			std::cout << "recorrido 2\n";
			mov_globo_z -= 0.4f;
			mov_globo_x -= 0.2f;
			if (mov_globo_z < -125.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3)
		{
			std::cout << "recorrido 3\n";
			mov_globo_x -= 0.2f;
			mov_globo_z -= 0.2f;
			if (mov_globo_z < -150.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			std::cout << "recorrido 4\n";
			mov_globo_x -= 0.2f;
			mov_globo_z -= 0.2f;
			if (mov_globo_z < -175.0f)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5)
		{
			std::cout << "recorrido 5\n";

			std::cout << "altura: " << mov_globo_y;
			mov_globo_z += 0.2f;
			mov_globo_x -= 0.6f;
			mov_globo_y += 0.2;
			if (mov_globo_z > -150.0f)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		if (recorrido6)
		{
			mov_globo_x += 0.1f;
			mov_globo_z += 0.5f;
			mov_globo_y -= 0.1f;
			if (mov_globo_z > -25.0f)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		if (recorrido7)
		{
			mov_globo_x += 0.2f;
			mov_globo_z += 0.2f;
			if (mov_globo_z > 0.0f)
			{
				recorrido7 = false;
				recorrido8 = true;
			}
		}
		if (recorrido8)
		{
			mov_globo_x += 0.4f;
			mov_globo_z += 0.2f;
			mov_globo_y -= 0.2f;
			if (mov_globo_z > 25.0f)
			{
				recorrido8 = false;
				recorrido9 = true;
			}
		}
		if (recorrido9)
		{
			mov_globo_x += 0.4f;
			if (mov_globo_x > -25.0f)
			{
				recorrido9 = false;
				recorrido10 = true;
			}
		}

		if (recorrido10)
		{
			mov_globo_x += 0.2f;
			mov_globo_z -= 0.2f;
			if (mov_globo_x > 0.0f)
			{
				recorrido10 = false;
				recorrido1 = true;
			}
		}

	}

	
	if (anim_auto && esta_estacionado)
	{
		if (rec_auto1) {

			mov_auto_z += 0.4f;
			orienta = 0.0f;
			if (mov_auto_z > 50.0f) {
				rec_auto1 = false;
				rec_auto2 = true;
			}
		}

		if (rec_auto2) {
			mov_auto_z += 0.4f;
			mov_auto_x -= 0.4f;
			orienta = -45.0f;
			if (mov_auto_z > 90.0f) {
				rec_auto2 = false;
				rec_auto3 = true;
			}
		}

		if (rec_auto3) {
			mov_auto_x -= 0.8f;
			orienta = -90.0f;
			if (mov_auto_x < -150.0f) {
				rec_auto3 = false;
				rec_auto1 = true;
				anim_auto = false;
				esta_estacionado = false;
			}
		}
	}

	if (auto_in && !esta_estacionado) {

		if (esta_auto0) {
			mov_auto_x -= 0.8f;
			orienta = -90.0f;
			if (mov_auto_x < -175.0f) {
				esta_auto0 = false;
				esta_auto1 = true;
			}
		}

		if (esta_auto1) {
			mov_auto_x -= 0.6f;
			mov_auto_z -= 0.6f;
			orienta = -135.0f;
			if (mov_auto_x < -200.0f) {
				esta_auto1 = false;
				esta_auto2 = true;
			}
		}

		if (esta_auto2) {
			mov_auto_z -= 0.4;
			orienta = 180.0f;
			if (mov_auto_z < 90.0f) {
				esta_auto2 = false;
				esta_auto3 = true;
			}
		}

		if (esta_auto3) {
			mov_auto_z += 0.4;
			if (mov_auto_z > 75.0f) {
				esta_auto3 = false;
				esta_auto4 = true;
			}
		}

		if (esta_auto4) {
			mov_auto_z += 0.4;
			mov_auto_x -= 0.4;
			orienta = 135.0f;
			if (mov_auto_z > 90.0f) {
				esta_auto4 = false;
				esta_auto5 = true;
			}
		}

		if (esta_auto5) {
			mov_auto_x -= 0.4;
			orienta = 90.0f;
			if (mov_auto_x < -200.0f) {
				esta_auto5 = false;
				esta_auto6 = true;
			}
		}

		if (esta_auto6) {
			mov_auto_x += 1.0;
			if (mov_auto_x > 40.0f) {
				esta_auto6 = false;
				esta_auto7 = true;
			}
		}

		if (esta_auto7) {
			mov_auto_x -= 1.0f;
			mov_auto_z -= 1.0f;
			orienta = 45.0f;
			if (mov_auto_z < 50.0) {
				esta_auto7 = false;
				esta_auto8 = true;
			}
		}

		if (esta_auto8) {
			mov_auto_z -= 2.0f;
			orienta = 0.0f;
			if (mov_auto_z < 0) {
				esta_auto8 = false;
				esta_auto0 = true;
				auto_in = false;
				esta_estacionado = true;
			}
		}
		
	}
}

void display(Shader shader)
{

	shader.use();

	// create transformations and Projection
	glm::mat4 temp1 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);

	model = temp1 = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.75f, 5.5f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.835f, 0.729f, 0.545f));
	cilindro.render();	//cilindro

	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, 1.25f, 0.0f));
	model = glm::scale(model, glm::vec3(4.75f, 2.5f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	cono.render();	//cono

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, -0.75f, 1.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.45f, 0.435f, 0.431f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, 0.0f, 0.5f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	cilindro.render();	//cilindro 2

	glm::mat4 temp2 = glm::mat4(1.0f);

	model = glm::translate(temp1, glm::vec3(0.0f, 0.0f, 1.0f));
	model = temp2 = glm::rotate(model, glm::radians(rot_helice), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	cono.render();	//cono giro


	// Primer élice
	model = temp1 = glm::translate(temp2, glm::vec3(0.6f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
	cilindro.render();	//cilindro 2

	glBindVertexArray(VAO);

	model = temp1 = glm::translate(temp1, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 0.15f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = temp1 = glm::translate(temp1, glm::vec3(0.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.5f, 1.5f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	glDrawArrays(GL_QUADS, 0, 24);

	// Segunda élice
	model = temp1 = glm::translate(temp2, glm::vec3(0.0f, 0.6f, -0.3f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
	cilindro.render();	//cilindro 2

	glBindVertexArray(VAO);

	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f, 4.0f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, 0.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.5f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	glDrawArrays(GL_QUADS, 0, 24);

	// Tercera élice
	model = temp1 = glm::translate(temp2, glm::vec3(-0.6f, 0.0f, -0.3f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
	cilindro.render();	//cilindro 2

	glBindVertexArray(VAO);

	model = temp1 = glm::translate(temp1, glm::vec3(-2.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 0.15f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = temp1 = glm::translate(temp1, glm::vec3(-0.25f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.5f, 1.5f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	glDrawArrays(GL_QUADS, 0, 24);

	// Cuarta élice
	model = temp1 = glm::translate(temp2, glm::vec3(0.0f, -0.6f, -0.3f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.25f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
	cilindro.render();	//cilindro 2

	glBindVertexArray(VAO);

	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.15f, 4.0f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
	glDrawArrays(GL_QUADS, 0, 24);

	model = temp1 = glm::translate(temp1, glm::vec3(0.0f, -0.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 3.5f, 0.15f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
	glDrawArrays(GL_QUADS, 0, 24);


	//glBindVertexArray(0);

}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	//monitors = glfwGetPrimaryMonitor();
	//getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	


	//glewInit();
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	//Datos a utilizar
	myData();
	cilindro.init();
	cono.init();
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	// build and compile shaders
	// -------------------------
	//Shader staticShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	//Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	
	// load models
	// -----------
	Model cama("resources/objects/Cama/cama.obj");
	Model cama_prin("resources/objects/Cama/cama_prin.obj");
	Model ropero("resources/objects/Ropero/ropero.obj");
	Model buro("resources/objects/Ropero/buro.obj");
	Model sofa_prin("resources/objects/Sillones/sofa_prin.obj");
	Model sala("resources/objects/Sillones/sala.obj");
	Model mesa_centro("resources/objects/Sillones/mesa_centro.obj");
	Model silla_rec("resources/objects/Sillones/silla_rec.obj");
	Model silla_albe("resources/objects/Sillones/silla_alb.obj");
	Model esc_estudio("resources/objects/Escritorio/escritorio_estudio.obj");
	Model arbol("resources/objects/Arbol/arbol_1.obj");
	Model casa_mod("resources/objects/Casa_mod/casa_modelo.obj");
	Model techo("resources/objects/Casa_mod/techo.obj");
	Model barra("resources/objects/Cocina/barra.obj");
	Model silla_barra("resources/objects/Cocina/silla_barra.obj");
	Model alacena("resources/objects/Cocina/alacena.obj");
	Model cocina("resources/objects/Cocina/cocina.obj");
	Model fregadero("resources/objects/Cocina/fregadero.obj");
	Model refri("resources/objects/Cocina/refri.obj");
	Model libros("resources/objects/Escritorio/libros.obj");
	Model librero("resources/objects/Escritorio/librero.obj");
	Model silla_esc("resources/objects/Escritorio/silla_esc.obj");
	Model comedor("resources/objects/Comedor/comedor.obj");
	Model wc("resources/objects/banio/wc/toilet.obj");
	Model lavamanos("resources/objects/banio/lavamanos/lavamanos.obj");
	Model coche("resources/objects/Coche/car.obj");
	Model lavadora("resources/objects/cuarto_lavado/lavadora.obj ");
	Model estante("resources/objects/cuarto_lavado/estante.obj ");
	Model servicio("resources/objects/cuarto_lavado/servicio.obj ");
	Model ducha("resources/objects/banio/ducha/ducha.obj");
	Model mueble_tv("resources/objects/Mueble_tv/mueble_tv.obj ");
	Model tv("resources/objects/TV/tv.obj");
	Model globo("resources/objects/Globo/globo.obj");
	Model nino_cuerpo("resources/objects/Nino/nino_cuerpo.obj");
	Model nino_brazo_der("resources/objects/Nino/brazo_derecho.obj");
	Model nino_brazo_izq("resources/objects/Nino/brazo_izquierdo.obj");
	Model perro_cabeza("resources/objects/Perrito/perrito_cabeza.obj ");
	Model perro_cuerpo("resources/objects/Perrito/perrito_cuerpo.obj ");
	Model perrito("resources/objects/Perro/perro.obj ");
	Model fenix("resources/objects/Fenix/fenix.obj ");
	Model pelota("resources/objects/Pelota/pelota.obj ");
	Model stitch("resources/objects/Stitch/stitch_f.obj");
	Model bugs("resources/objects/Bugs/bugs_bunny.obj");

	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	KeyFrame[0].mov_brazo_der = 0;
	KeyFrame[0].mov_brazo_izq = 0;
	KeyFrame[0].gira_cuerpo = 0;
	KeyFrame[0].mov_cuerpo_Y = 0;
	KeyFrame[0].mov_cuerpo_X = 0;
	KeyFrame[0].mov_cuerpo_Z = 0;

	KeyFrame[1].mov_brazo_der = 60;
	KeyFrame[1].mov_brazo_izq = 60;
	KeyFrame[1].gira_cuerpo = 180;
	KeyFrame[1].mov_cuerpo_Y = 0;
	KeyFrame[1].mov_cuerpo_X = 20;
	KeyFrame[1].mov_cuerpo_Z = 0;
	
	KeyFrame[2].mov_brazo_der = -60;
	KeyFrame[2].mov_brazo_izq = -60;
	KeyFrame[2].gira_cuerpo = 180;
	KeyFrame[2].mov_cuerpo_Y = 0;
	KeyFrame[2].mov_cuerpo_X = 20;
	KeyFrame[2].mov_cuerpo_Z = -46;

	KeyFrame[3].mov_brazo_der = 60;
	KeyFrame[3].mov_brazo_izq = 60;
	KeyFrame[3].gira_cuerpo = 180;
	KeyFrame[3].mov_cuerpo_Y = 0;
	KeyFrame[3].mov_cuerpo_X = 20;
	KeyFrame[3].mov_cuerpo_Z = -92;

	KeyFrame[4].mov_brazo_der = -60;
	KeyFrame[4].mov_brazo_izq = -60;
	KeyFrame[4].gira_cuerpo = 180;
	KeyFrame[4].mov_cuerpo_Y = 0;
	KeyFrame[4].mov_cuerpo_X = 20;
	KeyFrame[4].mov_cuerpo_Z = -138;

	KeyFrame[5].mov_brazo_der = 60;
	KeyFrame[5].mov_brazo_izq = 60;
	KeyFrame[5].gira_cuerpo = 180;
	KeyFrame[5].mov_cuerpo_Y = 0;
	KeyFrame[5].mov_cuerpo_X = 20;
	KeyFrame[5].mov_cuerpo_Z = -184;

	KeyFrame[6].mov_brazo_der = -60;
	KeyFrame[6].mov_brazo_izq = -60;
	KeyFrame[6].gira_cuerpo = 270;
	KeyFrame[6].mov_cuerpo_Y = 0;
	KeyFrame[6].mov_cuerpo_X = 20;
	KeyFrame[6].mov_cuerpo_Z = -230;

	KeyFrame[7].mov_brazo_der = 60;
	KeyFrame[7].mov_brazo_izq = 60;
	KeyFrame[7].gira_cuerpo = 270;
	KeyFrame[7].mov_cuerpo_Y = 0;
	KeyFrame[7].mov_cuerpo_X = -24;
	KeyFrame[7].mov_cuerpo_Z = -230;

	KeyFrame[8].mov_brazo_der = -60;
	KeyFrame[8].mov_brazo_izq = -60;
	KeyFrame[8].gira_cuerpo = 270;
	KeyFrame[8].mov_cuerpo_Y = 0;
	KeyFrame[8].mov_cuerpo_X = -68;
	KeyFrame[8].mov_cuerpo_Z = -230;

	KeyFrame[9].mov_brazo_der = 60;
	KeyFrame[9].mov_brazo_izq = 60;
	KeyFrame[9].gira_cuerpo = 270;
	KeyFrame[9].mov_cuerpo_Y = 0;
	KeyFrame[9].mov_cuerpo_X = -112;
	KeyFrame[9].mov_cuerpo_Z = -230;

	KeyFrame[10].mov_brazo_der = -60;
	KeyFrame[10].mov_brazo_izq = -60;
	KeyFrame[10].gira_cuerpo = 270;
	KeyFrame[10].mov_cuerpo_Y = 0;
	KeyFrame[10].mov_cuerpo_X = -156;
	KeyFrame[10].mov_cuerpo_Z = -230;

	KeyFrame[11].mov_brazo_der = 60;
	KeyFrame[11].mov_brazo_izq = 60;
	KeyFrame[11].gira_cuerpo = 270;
	KeyFrame[11].mov_cuerpo_Y = 0;
	KeyFrame[11].mov_cuerpo_X = -200;
	KeyFrame[11].mov_cuerpo_Z = -230;

	KeyFrame[12].mov_brazo_der = -60;
	KeyFrame[12].mov_brazo_izq = -60;
	KeyFrame[12].gira_cuerpo = 0;
	KeyFrame[12].mov_cuerpo_Y = 0;
	KeyFrame[12].mov_cuerpo_X = -244;
	KeyFrame[12].mov_cuerpo_Z = -230;

	KeyFrame[13].mov_brazo_der = 60;
	KeyFrame[13].mov_brazo_izq = 60;
	KeyFrame[13].gira_cuerpo = 0;
	KeyFrame[13].mov_cuerpo_Y = 0;
	KeyFrame[13].mov_cuerpo_X = -244;
	KeyFrame[13].mov_cuerpo_Z = -184;

	KeyFrame[14].mov_brazo_der = -60;
	KeyFrame[14].mov_brazo_izq = -60;
	KeyFrame[14].gira_cuerpo = 0;
	KeyFrame[14].mov_cuerpo_Y = 0;
	KeyFrame[14].mov_cuerpo_X = -244;
	KeyFrame[14].mov_cuerpo_Z = -138;

	KeyFrame[15].mov_brazo_der = 60;
	KeyFrame[15].mov_brazo_izq = 60;
	KeyFrame[15].gira_cuerpo = 0;
	KeyFrame[15].mov_cuerpo_Y = 0;
	KeyFrame[15].mov_cuerpo_X = -244;
	KeyFrame[15].mov_cuerpo_Z = -92;

	KeyFrame[16].mov_brazo_der = -60;
	KeyFrame[16].mov_brazo_izq = -60;
	KeyFrame[16].gira_cuerpo = 0;
	KeyFrame[16].mov_cuerpo_Y = 0;
	KeyFrame[16].mov_cuerpo_X = -244;
	KeyFrame[16].mov_cuerpo_Z = -46;

	KeyFrame[17].mov_brazo_der = 60;
	KeyFrame[17].mov_brazo_izq = 60;
	KeyFrame[17].gira_cuerpo = 90;
	KeyFrame[17].mov_cuerpo_Y = 0;
	KeyFrame[17].mov_cuerpo_X = -244;
	KeyFrame[17].mov_cuerpo_Z = 0;

	KeyFrame[18].mov_brazo_der = -60;
	KeyFrame[18].mov_brazo_izq = -60;
	KeyFrame[18].gira_cuerpo = 90;
	KeyFrame[18].mov_cuerpo_Y = 0;
	KeyFrame[18].mov_cuerpo_X = -202.5;
	KeyFrame[18].mov_cuerpo_Z = 0;

	KeyFrame[19].mov_brazo_der = 60;
	KeyFrame[19].mov_brazo_izq = 60;
	KeyFrame[19].gira_cuerpo = 90;
	KeyFrame[19].mov_cuerpo_Y = 0;
	KeyFrame[19].mov_cuerpo_X = -161;
	KeyFrame[19].mov_cuerpo_Z = 0;

	KeyFrame[20].mov_brazo_der = -60;
	KeyFrame[20].mov_brazo_izq = -60;
	KeyFrame[20].gira_cuerpo = 90;
	KeyFrame[20].mov_cuerpo_Y = 0;
	KeyFrame[20].mov_cuerpo_X = -119.5;
	KeyFrame[20].mov_cuerpo_Z = 0;

	KeyFrame[21].mov_brazo_der = 60;
	KeyFrame[21].mov_brazo_izq = 60;
	KeyFrame[21].gira_cuerpo = 180;
	KeyFrame[21].mov_cuerpo_Y = 0;
	KeyFrame[21].mov_cuerpo_X = -78;
	KeyFrame[21].mov_cuerpo_Z = 0;

	KeyFrame[22].mov_brazo_der = 80;
	KeyFrame[22].mov_brazo_izq = -80;
	KeyFrame[22].gira_cuerpo = 270;
	KeyFrame[22].mov_cuerpo_Y = 0;
	KeyFrame[22].mov_cuerpo_X = -78;
	KeyFrame[22].mov_cuerpo_Z = -83;

	KeyFrame[23].mov_brazo_der = -80;
	KeyFrame[23].mov_brazo_izq = 80;
	KeyFrame[23].gira_cuerpo = 180;
	KeyFrame[23].mov_cuerpo_Y = 0;
	KeyFrame[23].mov_cuerpo_X = -118;
	KeyFrame[23].mov_cuerpo_Z = -83;

	KeyFrame[24].mov_brazo_der = 80;
	KeyFrame[24].mov_brazo_izq = -80;
	KeyFrame[24].gira_cuerpo = 90;
	KeyFrame[24].mov_cuerpo_Y = 0;
	KeyFrame[24].mov_cuerpo_X = -118;
	KeyFrame[24].mov_cuerpo_Z = -171;

	KeyFrame[25].mov_brazo_der = 80;
	KeyFrame[25].mov_brazo_izq = -80;
	KeyFrame[25].gira_cuerpo = 90;
	KeyFrame[25].mov_cuerpo_Y = 3;
	KeyFrame[25].mov_cuerpo_X = -104;
	KeyFrame[25].mov_cuerpo_Z = -171;

	KeyFrame[26].mov_brazo_der = 80;
	KeyFrame[26].mov_brazo_izq = -80;
	KeyFrame[26].gira_cuerpo = 90;
	KeyFrame[26].mov_cuerpo_Y = 0;
	KeyFrame[26].mov_cuerpo_X = -90;
	KeyFrame[26].mov_cuerpo_Z = -171;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//PlaySound(TEXT("sound/pajaros.wav"), NULL, SND_LOOP|SND_ASYNC);
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.8f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 temp1 = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 4000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		//staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		// Molino
		//display(staticShader);

		model = temp1 = glm::translate(glm::mat4(1.0f), glm::vec3(140.0f, 30+5.0f, -130.0f));
		model = glm::scale(model, glm::vec3(scale*4.75f, scale*5.5f, scale*4.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.835f, 0.729f, 0.545f));
		cilindro.render();	//cilindro

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f, 1.25f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*4.75f, scale*2.5f, scale*4.75f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		cono.render();	//cono

		glBindVertexArray(VAO);
		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f,-0.75f, 1.5f));
		model = glm::scale(model, glm::vec3(scale*1.0f, scale*1.0f, scale*1.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.45f, 0.435f, 0.431f));
		glDrawArrays(GL_QUADS, 0, 24);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*0.5f, scale*1.0f, scale*0.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		cilindro.render();	//cilindro 2


		glm::mat4 temp2 = glm::mat4(1.0f);

		model = glm::translate(temp1, scale*glm::vec3(0.0f, 0.0f, 1.0f));
		model = temp2 = glm::rotate(model, glm::radians(rot_helice), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*0.5f, scale*0.5f, scale*0.5f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		cono.render();	//cono giro


		// Primer élice
		model = temp1 = glm::translate(temp2, scale* glm::vec3(0.6f, 0.0f, -0.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale* glm::vec3(0.25f, 0.5f, 0.25f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
		cilindro.render();	//cilindro 2

		glBindVertexArray(VAO);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*4.0f, scale*0.15f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
		glDrawArrays(GL_QUADS, 0, 24);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*3.5f, scale*1.5f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		glDrawArrays(GL_QUADS, 0, 24);

		// Segunda élice
		model = temp1 = glm::translate(temp2, scale* glm::vec3(0.0f, 0.6f, -0.3f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale*0.25f, scale*0.5f, scale*0.25f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
		cilindro.render();	//cilindro 2

		glBindVertexArray(VAO);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f, 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*0.15f, scale*4.0f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
		glDrawArrays(GL_QUADS, 0, 24);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f, 0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*1.5f, scale*3.5f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		glDrawArrays(GL_QUADS, 0, 24);

		// Tercera élice
		model = temp1 = glm::translate(temp2, scale* glm::vec3(-0.6f, 0.0f, -0.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale*0.25f, scale*0.5f, scale*0.25f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
		cilindro.render();	//cilindro 2

		glBindVertexArray(VAO);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(-2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*4.0f, scale*0.15f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
		glDrawArrays(GL_QUADS, 0, 24);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(-0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*3.5f, scale*1.5f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		glDrawArrays(GL_QUADS, 0, 24);

		// Cuarta élice
		model = temp1 = glm::translate(temp2, scale* glm::vec3(0.0f, -0.6f, -0.3f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale*0.25f, scale*0.5f, scale*0.25f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.47f, 0.329f, 0.227f));
		cilindro.render();	//cilindro 2

		glBindVertexArray(VAO);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale*0.15f, scale*4.0f, scale*0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.329f, 0.231f, 0.16f));
		glDrawArrays(GL_QUADS, 0, 24);

		model = temp1 = glm::translate(temp1, scale* glm::vec3(0.0f, -0.25f, 0.0f));
		model = glm::scale(model, scale*glm::vec3(1.5f, 3.5f, 0.15f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("aColor", glm::vec3(0.172f, 0.133f, 0.094f));
		glDrawArrays(GL_QUADS, 0, 24);

		//glBindVertexArray(0);

		// -----------------------Termina molino---------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 53.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		casa_mod.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-30.0f, mov_techo, -8.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		techo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Arboles
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 40.0f, -70.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 40.0f, 90.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Recamara principal
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-113.0f, 0.0f, 50.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		cama_prin.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-118.0f, 0.0f, 38.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-118.0f, 0.0f, 61.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-112.0f, 0.0f, 24.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		ropero.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 24.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		ropero.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Recamara individual 1
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-112.0f, 0.0f, -9.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-88.0f, 0.0f, -8.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		ropero.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-102.0f, 0.0f, -14.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-111.0f, 0.0f, 8.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		staticShader.setMat4("model", model);
		silla_rec.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Recamara individual 2
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-93.5f, 0.0f, -48.5f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, -50.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		ropero.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-103.5f, 0.0f, -53.5f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cocina
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(19.0f, 0.0f, -18.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		cocina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(19.0f, 5.0f, -18.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		alacena.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(18.0f, 0.0f, -3.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		fregadero.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 8.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		refri.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, -16.0f));
		model = glm::scale(model, glm::vec3(0.09f));
		staticShader.setMat4("model", model);
		barra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-18.0f, 0.0f, -22.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.12f));
		staticShader.setMat4("model", model);
		silla_barra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-18.0f, 0.0f, -12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.12f));
		staticShader.setMat4("model", model);
		silla_barra.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-18.0f, 0.0f, -2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.12f));
		staticShader.setMat4("model", model);
		silla_barra.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Comedor
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-47.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f));
		staticShader.setMat4("model", model);
		comedor.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cuarto de lavado
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-103.0f, 0.0f, -101.0f));
		model = glm::scale(model, glm::vec3(0.045f));
		staticShader.setMat4("model", model);
		lavadora.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-80.0f, 0.0f, -103.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Cuarto de servicio
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-118.0f, 6.0f, -95.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		servicio.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Baños
		// -------------------------------------------------------------------------------------------------------------------------
		// baño principal
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-102.0f, 0.0f, -62.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		wc.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-95.0f, 0.0f, -61.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		lavamanos.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-106.0f, 0.0f, -71.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		staticShader.setMat4("model", model);
		ducha.Draw(staticShader);

		//Baño de recamara principal
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-68.0f, 0.0f, 54.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		wc.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-83.0f, 0.0f, 54.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		lavamanos.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-79.0f, 0.0f, 54.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		staticShader.setMat4("model", model);
		ducha.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(50.0f + mov_auto_x, 0.0f + mov_auto_y, 40.0f + mov_auto_z));
		model = glm::rotate(model,glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		coche.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Stitch
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-16.0f, 5.0f, 37.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		stitch.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Bugs
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -30.0f));
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 80.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		staticShader.setMat4("model", model);
		bugs.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Estudio
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 65.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.03f));
		staticShader.setMat4("model", model);
		esc_estudio.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, 60.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f));
		staticShader.setMat4("model", model);
		silla_esc.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-60.0f, 0.0f, 43.0f));
		model = glm::scale(model, glm::vec3(0.007f));
		staticShader.setMat4("model", model);
		libros.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-52.0f, 7.0f, 59.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.17f));
		staticShader.setMat4("model", model);
		librero.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Sala
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 55.0f));
		model = glm::rotate(model, glm::radians(-87.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5f));
		staticShader.setMat4("model", model);
		sala.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-16.0f, 0.0f, 48.0f));
		model = glm::scale(model, glm::vec3(0.08f));
		staticShader.setMat4("model", model);
		mesa_centro.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-16.0f, 5.5f, 64.0f));
		model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		tv.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-16.0f, 0.0f, 64.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		mueble_tv.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Perrito
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 2.0f, 83.0f));
		model = glm::translate(model, glm::vec3(-110.0f, 2.0f, 90.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		perrito.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Perro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 5.0f, 85.6f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		perro_cabeza.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 84.6f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		perro_cuerpo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Fenix
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(mov_fenix), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(120.0f, 70.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		fenix.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Jardín
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -70.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		silla_albe.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -90.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		silla_albe.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -50.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		staticShader.setMat4("model", model);
		silla_albe.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Globo
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(90.0f + mov_globo_x, 15.0f + mov_globo_y, 110.0f + mov_globo_z));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		globo.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Pelota
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f + mov_pelota_x, -0.5f, -90.0f + mov_pelota_z));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		pelota.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Niño
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(93.4f + mov_cuerpo_X, 0.0f + mov_cuerpo_Y, 108.2f + mov_cuerpo_Z));
		tmp = model = glm::rotate(model, glm::radians(gira_cuerpo), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		nino_cuerpo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 11.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.2f, 0.0f, -0.59f));
		model = glm::rotate(model, glm::radians(mov_brazo_der), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		nino_brazo_der.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 11.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.3f, 0.0f, -0.59f));
		model = glm::rotate(model, glm::radians(mov_brazo_izq), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		nino_brazo_izq.Draw(staticShader);
	
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//Subir y bajar techo
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		if (mov_techo <= -1.0f)
			mov_techo++;
	}
		
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (mov_techo >= -60.0f)
			mov_techo--;
	}
	// Coche animation
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		anim_auto ^= true;

	// Coche animation
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
		auto_in ^= true;

	// Pelota animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion_pelota ^= true;

	// Animación del globo
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		animacion_globo ^= true;
	
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}