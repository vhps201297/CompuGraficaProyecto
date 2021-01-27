#pragma once
#ifndef CONO_H_
#define CONO_H_
//#include <glew.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>
#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOS 30 // 
#define PARALELOS 2   //numero de cortes del cilindro, 2 por circunferencias bases
class Cono
{
public:
	Cono(GLfloat radio); //Constructor
	void init();
	void render();
	virtual ~Cono(); //Destructor

private:

	float radio;

	GLuint cono_VAO[4], cono_VBO[4], cono_index[MERIDIANOS *( PARALELOS - 1)* 6];
	GLfloat cono_pos[PARALELOS * MERIDIANOS * 3];

};

#endif // !CONO_H_
