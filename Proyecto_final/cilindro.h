
#pragma once
#ifndef CILINDRO_H_
#define CILINDRO_H_
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
class Cilindro
{
public:
	Cilindro(GLfloat radio);
	void init();
	//void load();
	void render();
	virtual ~Cilindro();

private:

	float radio;

	GLuint cilindro_VAO[4], cilindro_VBO[4], cilindro_index[MERIDIANOS*(PARALELOS - 1) * 6];
	GLfloat cilindro_pos[PARALELOS * MERIDIANOS * 3];

};

#endif // !CILINDRO_H_