#include "cilindro.h"


Cilindro::Cilindro(GLfloat radio) :

	radio(radio) {
	cilindro_VAO[4] = (-1, -1, -1, -1);
	cilindro_VBO[4] = (-1, -1, -1, -1);

}


Cilindro::~Cilindro()
{
	glDeleteVertexArrays(4, cilindro_VAO);
	glDeleteBuffers(4, cilindro_VBO);
}


void Cilindro::init()
{
	const int n = PARALELOS * MERIDIANOS * 3;
	GLfloat cylinder_nor[n]; // normal
	
	GLfloat x, y, z, a, b, da, r = 0.5, h = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS); // párticiones de las circunferencias

	// Vértices de la circunferencia
	// puntos de la superficie de un cilindro unitario
	for (ix = 0, b = -M_PI/2, ib = 0; ib < PARALELOS; ib++, b += M_PI/2)
		for (a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
		{
			// Ecuación paramétrica del cilindro
			x = sin(a);
			y = b;
			z = cos(a);
			cilindro_pos[ix + 0] = x * r;
			cilindro_pos[ix + 1] = y * r;
			cilindro_pos[ix + 2] = z * r;
			cylinder_nor[ix + 0] = x;
			cylinder_nor[ix + 1] = y;
			cylinder_nor[ix + 2] = z;
		}
	// Generacion de arreglo de índices para el relleno
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS ; ib++)
	{
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// 
			cilindro_index[ix] = iy;      ix++;
			cilindro_index[ix] = iy + 1;  ix++;
			cilindro_index[ix] = iy + MERIDIANOS;   ix++;
			
			cilindro_index[ix] = iy + MERIDIANOS;   ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		// índices para cerrar la circunferencia, une los índices finales con los iniciales

		cilindro_index[ix] = iy;       ix++;
		cilindro_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		cilindro_index[ix] = iy + MERIDIANOS;    ix++;

		cilindro_index[ix] = iy + MERIDIANOS;    ix++;
		cilindro_index[ix] = iy - MERIDIANOS + 1;  ix++;
		cilindro_index[ix] = iy + 1;     ix++;
		iy++;
	}

	GLuint i;
	glGenVertexArrays(4, cilindro_VAO);
	glGenBuffers(4, cilindro_VBO);
	glBindVertexArray(cilindro_VAO[0]);
	i = 0; // Arreglo de vértices
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cilindro_pos), cilindro_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // Arreglo normal 
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_nor), cylinder_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // Arreglo de índices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cilindro_index), cilindro_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Cilindro::render()
{
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glBindVertexArray(cilindro_VAO[0]);
	glDrawElements(GL_TRIANGLES, sizeof(cilindro_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)

	glBindVertexArray(0);
}




