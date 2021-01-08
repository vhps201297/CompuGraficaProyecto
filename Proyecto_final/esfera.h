#pragma once
#ifndef ESFERA_H_
#define ESFERA_H_

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOS 16
#define PARALELOS 8

class Esfera
{
	public:
		Esfera(GLfloat radio);
		void init();
		//void load();
		void render();
		virtual ~Esfera();
	private:
		float radio;

		GLuint esfera_VAO[4], esfera_VBO[4], esfera_index[MERIDIANOS*(PARALELOS - 1) * 6];
		GLfloat esfera_pos[PARALELOS * MERIDIANOS * 3];
};

#endif // !ESFERA_H_