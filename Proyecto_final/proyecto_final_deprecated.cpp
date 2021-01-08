/*------------------------------------------------------------------------*/
/* ---------------------         Proyecto final      ---------------------*/
/* ---------------------             CGeIHC          ---------------------*/
/* ---------------------             2021-1          ---------------------*/
/* ---------------------            Alumnos          ---------------------*/
/* --------------------- Granados Gómez Nanci Noelia ---------------------*/
/* ---------------------   Pólito Seba Víctor Hugo   ---------------------*/
/*------------------------------------------------------------------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


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

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

void myData(void);
void display(Shader);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 180.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false,
		recorrido5 = false,
		recorrido6 = false,
		recorrido7 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
GLuint VBO, VAO, EBO;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

//Texturas
unsigned int	t_cochera,
t_contorno;

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{

	t_cochera = generateTextures("Texturas/cochera.png", 1);
	t_contorno = generateTextures("Texturas/contorno_house.png", 1);

}

void myData()
{
	float vertices[] = {

		//Cochera				//
		-0.7f, 0.0f, 0.0f,		0.0f, 0.75f, 	// bottom left - Cochera
		-0.2f, 0.0f, 0.0f,		0.5f, 0.75f, 	// bottom right
		-0.2f, 0.25f, 0.0f,		0.5f, 1.0f, 	// top right
		-0.7f, 0.25f, 0.0f,		0.0f, 1.0f, 	// top left

		-0.7f, 0.0f, -0.3f,		0.0f, 0.5f, 	//bottom left - Lateral-izq-cochera
		-0.7f, 0.0f, 0.0f,		0.298f, 0.5f, 	//bottom right
		-0.7f, 0.25f, -0.3f,	0.0f, 0.75f, 	//top left
		-0.7f, 0.25f, 0.0f,		0.298f, 0.75f, 	//top right

		-0.2f, 0.25f, -0.3f,	0.298f, 0.75f, 	// top right- Lateral-der-cochera
		-0.2f, 0.25f, 0.0f,		0.0f, 0.75f, 	// top left
		-0.2f, 0.0f, 0.0f,		0.0f, 0.5f, 	// bottom left
		-0.2f, 0.0f, -0.3f,		0.298f, 0.5f, 	//bottom right

		-0.7f, 0.25f, 0.0f,		0.5f, 0.8f, 	// bottom left - Cochera-triangulo frontal - 12
		-0.2f, 0.25f, 0.0f,		1.0f, 0.8f, 	// bottom right
		-0.45f, 0.45f, 0.0f,	0.75f, 1.0f, 	// top

		-0.7f, 0.25f, -0.3f,	0.5f, 0.8f, 	// bottom left - Cochera-triangulo trasero 
		-0.2f, 0.25f, -0.3f,	1.0f, 0.8f, 	// bottom right
		-0.45f, 0.45f, -0.3f,	0.75f, 1.0f, 	// top

		-0.75f, 0.2f, 0.03f,	0.51f, 0.781f, 	// bottom left - Grosor techo - izq 18
		-0.45f, 0.45f, 0.03f,	0.609f, 0.781f, // bottom right
		-0.75f, 0.225f, 0.03f,	0.51f, 0.8f, 	// top left
		-0.45f, 0.475f, 0.03f,	0.609f, 0.8f, 	// top right

		-0.45f, 0.45f, 0.03f,	0.51f, 0.781f, 	// bottom left - Grosor techo - der 22
		-0.15f, 0.2f, 0.03f,	0.609f, 0.781f, // bottom right
		-0.45f, 0.475f, 0.03f,	0.51f, 0.8f, 	// top left
		-0.15f, 0.225f, 0.03f,	0.609f, 0.8f, 	// top right

		-0.45f, 0.475f, -0.3f,	0.0f, 0.5f,		// top left - Techo izq -26
		-0.45f, 0.475f, 0.03f,	0.33f, 0.5f,	// top right
		-0.75f, 0.225f, -0.3f,	0.0f, 0.109f,	// bottom left 
		-0.75f, 0.225f, 0.03f,	0.33f, 0.109f,	// bottom right 

		-0.45f, 0.475f, 0.03f,	0.0f, 0.5f,		// top left - Techo der 30
		-0.15f, 0.225f, 0.03f,	0.0f, 0.109f,	// bottom left
		-0.45f, 0.475f, -0.3f,	0.33f, 0.5f,	// top right 
		-0.15f, 0.225f, -0.3f,	0.33f, 0.109f,	// bottom right 

		-0.75f, 0.225f, -0.3f,	0.51f, 0.8f,		// top left - grosor Techo izq 
		-0.75f, 0.225f, 0.03f,	0.609f, 0.8f,	// top right
		-0.75f, 0.2f, -0.3f,	0.51f, 0.781f,	// bottom left 
		-0.75f, 0.2f, 0.03f,	0.609f, 0.781f,	// bottom right 

		-0.15f, 0.225f, 0.03f,	0.51f, 0.8f,		// top left - Grosor Techo der 38
		-0.15f, 0.2f, 0.03f,	0.51f, 0.781f,	// bottom left
		-0.15f, 0.225f, -0.3f,	0.609f, 0.8f,	// top right 
		-0.15f, 0.2f, -0.3f,	0.609f, 0.781f,	// bottom right 

		-0.75f, 0.2f, -0.3f,	0.51f, 0.781f, 	// bottom left - Grosor trasero - techo - izq 42
		-0.45f, 0.45f, -0.3f,	0.609f, 0.781f, // bottom right
		-0.75f, 0.225f, -0.3f,	0.51f, 0.8f, 	// top left
		-0.45f, 0.475f, -0.3f,	0.609f, 0.8f, 	// top right

		-0.45f, 0.45f, -0.3f,	0.51f, 0.781f, 	// bottom left - Grosor trasero - techo - der 46
		-0.15f, 0.2f, -0.3f,	0.609f, 0.781f, // bottom right
		-0.45f, 0.475f, -0.3f,	0.51f, 0.8f, 	// top left
		-0.15f, 0.225f, -0.3f,	0.609f, 0.8f, 	// top right

		// Paredes y techos
		-0.7f, 0.0f, -0.9f,		0.6f, 0.85f, 	//bottom left - Lateral-izq 50
		-0.7f, 0.0f, -0.3f,		0.89f, 0.85f, 	//bottom right
		-0.7f, 0.3f, -0.9f,		0.6f, 1.0f, 	//top left
		-0.7f, 0.3f, -0.3f,		0.89f, 1.0f, 	//top right

		-0.7f, 0.0f, -0.3f,		0.0f, 0.85f, 	// bottom left - frontal 
		0.5f, 0.0f, -0.3f,		0.6f, 0.85f, 	// bottom right
		0.5f, 0.3f, -0.3f,		0.6f, 1.0f, 	// top right
		-0.7f, 0.3f, -0.3f,		0.0f, 1.0f, 	// top left

		-0.7f, 0.0f, -0.9f,		0.0f, 0.7f, 	// bottom left - trasera  58
		0.6f, 0.0f, -0.9f,		0.64f, 0.7f, 	// bottom right
		0.6f, 0.3f, -0.9f,		0.64f, 0.85f, 	// top right
		-0.7f, 0.3f, -0.9f,		0.0f, 0.85f, 	// top left

		0.6f, 0.0f, -0.45f,		0.64f, 0.7f, 	//bottom left - Lateral-der-1 
		0.6f, 0.0f, -0.9f,		0.84f, 0.7f, 	//bottom right
		0.6f, 0.3f, -0.45f,		0.64f, 0.85f, 	//top left
		0.6f, 0.3f, -0.9f,		0.84f, 0.85f, 	//top right

		0.5f, 0.0f, -0.45f,		0.84f, 0.701f, 	//bottom left - Lateral-der-2 66
		0.6f, 0.0f, -0.45f,		0.915f, 0.701f, //bottom right
		0.5f, 0.3f, -0.45f,		0.84f, 0.85f, 	//top left
		0.6f, 0.3f, -0.45f,		0.915f, 0.85f, 	//top right

		0.5f, 0.0f, -0.3f,		0.84f, 0.701f, 	//bottom left - Lateral-der-3
		0.5f, 0.0f, -0.45f,		0.915f, 0.701f, //bottom right
		0.5f, 0.3f, -0.3f,		0.84f, 0.85f, 	//top left
		0.5f, 0.3f, -0.45f,		0.915f, 0.85f, 	//top right

		0.625f, 0.275f, -0.925f,	0.226f, 0.49f, 	// bottom left - Techo trasero 74
		-0.725f, 0.275f, -0.925f,	0.87f, 0.49f, 	// bottom right
		-0.725f, 0.6f, -0.6f,		0.87f, 0.7f, 	// top right
		0.625f, 0.6f, -0.6f,		0.226f, 0.7f, 	// top left

		-0.725f, 0.425f, -0.425f,		0.226f, 0.59f, 	// bottom left - Techo frontal-sup 
		0.625f, 0.425f, -0.425f,		0.87f, 0.59f, 	// bottom right
		0.625f, 0.6f, -0.6f,		0.87f, 0.7f, 	// top right
		-0.725f, 0.6f, -0.6f,		0.226f, 0.7f, 	// top left

		-0.725f, 0.275f, -0.275f,	0.226f, 0.49f, 	// bottom left - Techo frontal-inf 82
		0.525f, 0.275f, -0.275f,	0.87f, 0.49f, 	// bottom right
		0.525f, 0.425f, -0.425f,		0.87f, 0.59f, 	// top right
		-0.725f, 0.425f, -0.425f,		0.226f, 0.59f, 	// top left

		//Grosores casa

		-0.725f, 0.25f, -0.275f,	0.226f, 0.48f, 	// bottom left - Grosor-frontal
		0.525f, 0.25f, -0.275f,		0.42f, 0.48f, 	// bottom right
		0.525f, 0.275f, -0.275f,	0.42f, 0.489f, 	// top right
		-0.725f, 0.275f, -0.275f,	0.226f, 0.489f, 	// top left

		0.625f, 0.25f, -0.925f,		0.226f, 0.48f, 	// bottom left - Grosor-trasero 90
		-0.725f, 0.25f, -0.925f,	0.42f, 0.48f, 	// bottom right
		-0.725f, 0.275f, -0.925f,	0.42f, 0.489f, 	// top right
		0.625f, 0.275f, -0.925f,	0.226f, 0.489f, 	// top left

		0.625f, 0.4f, -0.425f,		0.226f, 0.48f, 	// bottom left - Grosor-techo frontal sup der
		0.625f, 0.575f, -0.6f,		0.42f, 0.48f, 	// bottom right
		0.625f, 0.6f, -0.6f,		0.42f, 0.489f, 	// top right
		0.625f, 0.425f, -0.425f,	0.226f, 0.489f, 	// top left

		0.525f, 0.4f, -0.425f,		0.226f, 0.48f, 	// bottom left - Grosor-techo frontal sup der-bajo 98
		0.625f, 0.4f, -0.425f,		0.42f, 0.48f, 	// bottom right
		0.625f, 0.425f, -0.425f,	0.42f, 0.489f, 	// top right
		0.525f, 0.425f, -0.425f,	0.226f, 0.489f, 	// top left

		0.525f, 0.25f, -0.275f,		0.226f, 0.48f, 	// bottom left - Grosor-techo frontal inf der
		0.525f, 0.4f, -0.425f,		0.42f, 0.48f, 	// bottom right
		0.525f, 0.425f, -0.425f,	0.42f, 0.489f, 	// top right
		0.525f, 0.275f, -0.275f,	0.226f, 0.489f, // top left

		0.625f, 0.575f, -0.6f,		0.226f, 0.48f, 	// bottom left - Grosor-techo trasero der 106
		0.625f, 0.25f, -0.925f,		0.42f, 0.48f, 	// bottom right
		0.625f, 0.275f, -0.925f,	0.42f, 0.489f, 	// top right
		0.625f, 0.6f, -0.6f,		0.226f, 0.489f, 	// top left

		-0.725f, 0.25f, -0.925f,	0.226f, 0.48f, 	// bottom left - Grosor-techo trasero izq
		-0.725f, 0.575f, -0.6f,		0.42f, 0.48f, 	// bottom right
		-0.725f, 0.6f, -0.6f,		0.42f, 0.489f, 	// top right
		-0.725f, 0.275f, -0.925f,	0.226f, 0.489f, 	// top left

		-0.725f, 0.575f, -0.6f,		0.226f, 0.48f, 	// bottom left - Grosor-techo frontal izq 114
		-0.725f, 0.25f, -0.275f,	0.42f, 0.48f, 	// bottom right
		-0.725f, 0.275f, -0.275f,	0.42f, 0.489f, 	// top right
		-0.725f, 0.6f, -0.6f,		0.226f, 0.489f, 	// top left

		// Entrada 

		-0.1f, 0.0f, -0.25f,		0.0f, 0.452f, 	// bottom left - frontal entrada
		0.15f, 0.0f, -0.25f,		0.125f, 0.452f, 	// bottom right
		0.15f, 0.5f, -0.25f,		0.125f, 0.7f, 	// top right
		-0.1f, 0.5f, -0.25f,		0.0f, 0.7f, 	// top left

		-0.1f, 0.0f, -0.45f,		0.125f, 0.452f, 	// bottom left - entrada -izq 122
		-0.1f, 0.0f, -0.25f,		0.225f, 0.452f, 	// bottom right
		-0.1f, 0.5f, -0.25f,		0.225f, 0.7f, 	// top right
		-0.1f, 0.5f, -0.45f,		0.125f, 0.7f, 	// top left

		0.15f, 0.0f, -0.25f,		0.125f, 0.452f, 	// bottom left - entrada -der 
		0.15f, 0.0f, -0.45f,		0.225f, 0.452f, 	// bottom right
		0.15f, 0.5f, -0.45f,		0.225f, 0.7f, 	// top right
		0.15f, 0.5f, -0.25f,		0.125f, 0.7f, 	// top left

		0.15f, 0.0f, -0.45f,		0.125f, 0.452f, 	// bottom left - entrada atrás 130
		-0.1f, 0.0f, -0.45f,		0.225f, 0.452f, 	// bottom right
		-0.1f, 0.5f, -0.45f,		0.225f, 0.7f, 	// top right
		0.15f, 0.5f, -0.45f,		0.125f, 0.7f, 	// top left

		-0.125f, 0.475f, -0.475f,	0.0f, 0.354f, 	// bottom left - entrada -techo- izq 
		-0.125f, 0.475f, -0.225f,	0.1f, 0.354f, 	// bottom right
		0.025f, 0.65f, -0.225f,		0.1f, 0.451f, 	// top right
		0.025f, 0.65f, -0.475f,		0.0f, 0.451f, 	// top left

		0.175f, 0.475f, -0.225f,	0.0f, 0.354f, // bottom left - entrada -techo- der 138
		0.175f, 0.475f, -0.475f,	0.1f, 0.354f, // bottom right
		0.025f, 0.65f, -0.475f,		0.1f, 0.451f, 	// top right
		0.025f, 0.65f, -0.225f,		0.0f, 0.451f, 	// top left

		-0.125f, 0.45f, -0.225f,	0.226f, 0.48f, 	// bottom left - Grosor-frontal izq
		0.025f, 0.625f, -0.225f,	0.42f, 0.48f, 	// bottom right
		0.025f, 0.65f, -0.225f,		0.42f, 0.489f, 	// top right
		-0.125f, 0.475f, -0.225f,	0.226f, 0.489f, // top left

		0.025f, 0.625f, -0.225f,	0.226f, 0.48f, 	// bottom left - Grosor-frontal der 146
		0.175f, 0.45f, -0.225f,		0.42f, 0.48f, 	// bottom right
		0.175f, 0.475f, -0.225f,	0.42f, 0.489f, 	// top right
		0.025f, 0.65f, -0.225f,		0.226f, 0.489f, // top left

		-0.125f, 0.45f, -0.475f,	0.226f, 0.48f, 	// bottom left - Grosor-trasero der
		0.025f, 0.625f, -0.475f,	0.42f, 0.48f, 	// bottom right
		0.025f, 0.65f, -0.475f,		0.42f, 0.489f, 	// top right
		-0.125f, 0.475f, -0.475f,	0.226f, 0.489f, // top left

		0.025f, 0.625f, -0.475f,	0.226f, 0.48f, 	// bottom left - Grosor-trasero izq 154
		0.175f, 0.45f, -0.475f,		0.42f, 0.48f, 	// bottom right
		0.175f, 0.475f, -0.475f,	0.42f, 0.489f, 	// top right
		0.025f, 0.65f, -0.475f,		0.226f, 0.489f, // top left

		-0.125f, 0.45f, -0.475f,	0.226f, 0.48f, 	// bottom left - Grosor- izq 
		-0.125f, 0.45f, -0.225f,	0.42f, 0.48f, 	// bottom right
		-0.125f, 0.475f, -0.225f,	0.42f, 0.489f, 	// top right
		-0.125f, 0.475f, -0.475f,	0.226f, 0.489f, // top left

		0.175f, 0.45f, -0.475f,		0.226f, 0.48f, 	// bottom left - Grosor- der 162
		0.175f, 0.45f, -0.225f,		0.42f, 0.48f, 	// bottom right
		0.175f, 0.475f, -0.225f,	0.42f, 0.489f, 	// top right
		0.175f, 0.475f, -0.475f,	0.226f, 0.489f, // top left


		0.6f, 0.3f, -0.45f,		0.0f, 0.88f, 	//bottom left - Lateral-der-1 arriba
		0.6f, 0.3f, -0.9f,		0.15f, 0.88f, 	//bottom right
		0.6f, 0.45f, -0.45f,	0.0f, 1.0f, 	//top left
		0.6f, 0.6f, -0.6f,		0.15f, 1.0f, 	//top right

		0.5f, 0.3f, -0.45f,		0.0f, 0.88f, 	//bottom left - Lateral-der-2 arriba 170
		0.6f, 0.3f, -0.45f,		0.15f, 0.88f, //bottom right
		0.5f, 0.45f, -0.45f,	0.0f, 1.0f, 	//top left
		0.6f, 0.45f, -0.45f,	0.15f, 1.0f, 	//top right

		// Triangulos restantes
		0.5f, 0.3f, -0.3f,		0.0f, 0.88f, 	//bottom left - Lateral-der-3 triang 174
		0.5f, 0.3f, -0.45f,		0.15f, 0.88f, //bottom right
		0.5f, 0.45f, -0.45f,	0.07f, 1.0f, 	//top 

		-0.7f, 0.3f, -0.9f,		0.0f, 0.88f, 	//bottom left - Lateral-izq triang 177
		-0.7f, 0.3f, -0.3f,		0.15f, 0.88f, //bottom right
		-0.7f, 0.6f, -0.6f,		0.07f, 1.0f, 	//top 

		-0.1f, 0.5f, -0.25f,	0.125f, 0.452f, //bottom left - entrada triang frontal 180
		0.15f, 0.5f, -0.25f,	0.225f, 0.452f, //bottom right
		0.025f, 0.65f, -0.25f,	0.175f, 0.526f, //top 

		-0.1f, 0.5f, -0.45f,	0.125f, 0.452f, 	//bottom left - entrada triang trasera 183
		0.15f, 0.5f, -0.45f,	0.225f, 0.452f, //bottom right
		0.025f, 0.65f, -0.45f,	0.175f, 0.526f, 	//top 


	};

	/**
		Esta variable nos permitirá identificar la configuración del vertex shader, para que así se conformen las figurras,
		para este caso formamos dos triangulos por cada cara rectangular.
	**/
	unsigned int indices[] = {
		0, 1, 3, // cochera- frontal
		1, 2, 3,  // 

		4, 5, 7, // cochera-lat-izq 6
		4, 6, 7,

		8, 9, 11, // cochera lat-der
		9, 10, 11,

		12, 13, 14, // triang 18
		15, 16, 17,

		18, 19, 21, // Grosor-izq
		18, 20, 21,

		22, 23, 25, // Grosor-der 30
		22, 24, 25,

		26, 27, 29, // techo izq
		26, 28, 29,

		30, 31, 33,	// techo der 42
		30, 32, 33,

		34, 35, 37, // Grosor techo izq
		34, 36, 37,

		38, 39, 41, // Grosoe techo der 54
		38, 40, 41,

		42, 43, 45, //grosor trasero izq
		42, 44, 45,

		46, 47, 49, // Grosor trasero deer 66
		46, 48, 49,


		50, 51, 53, // Lateral izq
		50, 52, 53,

		54, 55, 57, // Frontal 78
		55, 56, 57,

		58, 59, 61, // Trasera
		59, 60, 61,

		62, 63, 65, // Lateral der 1 90
		62, 64, 65,

		66, 67, 69, // Lateral der 2 
		66, 68, 69,

		70, 71, 73, // Lateral der 3 102
		70, 72, 73,

		74, 75, 77, // Techo trasero 
		75, 76, 77,

		78, 79, 81, // Techo frontal sup 114
		79, 80, 81,

		82, 83, 85, // Techo frontal inf
		83, 84, 85,

		86, 87, 89, //grosor frontal 126
		87, 88, 89,

		90, 91, 93, //grosor trasero
		91, 92, 93,

		94, 95, 97, //grosor frontal sup der 138
		95, 96, 97,

		98, 99, 101, //grosor frontal sup bajo der 
		99, 100, 101,

		102, 103, 105, //grosor frontal inf der 150
		103, 104, 105,

		106, 107, 109, //grosor trasera der
		107, 108, 109,

		110, 111, 113, //grosor trasera izq 162
		111, 112, 113,

		114, 115, 117, //grosor frontal izq
		115, 116, 117,

		118, 119, 121, //entrada frontal 174
		119, 120, 121,

		122, 123, 125, //entrada izq
		123, 124, 125,

		126, 127, 129, //entrada der 186
		127, 128, 129,

		130, 131, 133, //entrada atrás
		131, 132, 133,

		134, 135, 137, //entrada - techo izq 198
		135, 136, 137,

		138, 139, 141, //entrada - techo der
		139, 140, 141,

		142, 143, 145, //entrada - grosor front izq 210
		143, 144, 145,

		146, 147, 149, //entrada - grosor front der
		147, 148, 149,

		150, 151, 153, //entrada - grosor tras der 222
		151, 152, 153,

		154, 155, 157, //entrada - grosor tras izq
		155, 156, 157,

		158, 159, 161, //entrada - grosor izq 234
		159, 160, 161,

		162, 163, 165, //entrada - grosor der
		163, 164, 165,

		166, 167, 169, //Lateral-der-1 arriba 246
		166, 168, 169,

		170, 171, 173, //Lateral-der-2 arriba
		170, 172, 173,

		174, 175, 176, //Lateral - der - 3 triang 258

		177, 178, 179, //Lateral-izq triang 261

		180, 181, 182, //entrada triang frontal 264

		183, 184, 185, //entrada triang trasera 267

	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void display(Shader shader)
{
	//Enable Shader
	shader.use();

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);

	for (int i = 0; i < 3; i++) { //Muros de la cochera
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_cochera);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}

	shader.setMat4("model", model); // Triángulo frontal de la cochera
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_cochera);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(18 * sizeof(float)));

	shader.setMat4("model", model); // Triángulo trasero de cochera
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_cochera);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(21 * sizeof(float)));

	for (int i = 0; i < 8; i++) { // Techo de la cochera
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_cochera);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((24 + (6 * i)) * sizeof(float)));
	}

	for (int i = 0; i < 31; i++) { // Muros, entradas, techo y grosor del techo
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_contorno);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((72 + (6 * i)) * sizeof(float)));
	}

	for (int i = 0; i < 4; i++) // Triángulos para rellenar la casa
	{
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_contorno);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)((258 + (3 * i)) * sizeof(float)));
	}


	glBindVertexArray(0);

} 

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}


void animate(void)
{
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
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	if (animacion)
	{
		if (recorrido1)
		{
			movAuto_z -= 0.9f;
			orienta = -180.0f;
			if (movAuto_z < -150.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x -= 1.0f;
			orienta = -90.0f;
			if (movAuto_x < -200.0f)
			{
				recorrido2 = false;
				recorrido5 = true;
			}
		}
		if (recorrido3)
		{
			movAuto_z += 1.5f;
			orienta = 0.0f;
			if (movAuto_z < -150.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			movAuto_x += 0.7f;
			orienta = 90.0f;
			if (movAuto_x > 0.0f)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}
		if (recorrido5)
		{
			movAuto_x += 2.0f;
			movAuto_z += 3.0f;
			orienta = 33.69f; //angtan de la pendiente
			if (movAuto_z >= 150.0f)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		if (recorrido6)
		{
			movAuto_x -= 1.0f;
			orienta = -90.0f;
			if (movAuto_x < -200.0f)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		if (recorrido7)
		{
			movAuto_x += 4.0f;
			movAuto_z -= 3.0f;
			orienta = 126.13f; //angtan de la pendiente
			if (movAuto_z <= 0.0f)
			{
				recorrido7 = false;
				recorrido1 = true;
			}
		}

	}
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glewInit();
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	
	
	
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");


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
	Model piso("resources/objects/piso/piso.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model casaVieja("resources/objects/casa/OldHouse.obj");
	Model casaDoll("resources/objects/casa/DollHouse.obj");
	Model cama("resources/objects/Cama/cama.obj");
	Model cama_prin("resources/objects/Cama/cama_prin.obj");
	Model ropero("resources/objects/Ropero/ropero.obj");
	Model buro("resources/objects/Ropero/buro.obj");
	Model sofa_prin("resources/objects/Sillones/sofa_prin.obj");
	Model esc_estudio("resources/objects/Escritorio/escritorio_estudio.obj");
	
	Model wc("resources/objects/banios/toilet/toilet.obj");
	Model lavamanos("resources/objects/banios/sink/lavamanos.obj");

	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	LoadTextures();
	myData();
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
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
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

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		//casaVieja.Draw(staticShader);
		lavamanos.Draw(staticShader);
		//wc.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casaDoll.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //rotacion sobre el eje y para que coincida con la dirección
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));  ///15, -1, 0 de manera local para la animacion es 0, 0, 0
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		


		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		//display(projectionShader);
		
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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

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