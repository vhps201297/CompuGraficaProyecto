#include "Cono.h"


Cono::Cono(GLfloat radio) :

	radio(radio) {
	cono_VAO[4] = (-1, -1, -1, -1);
	cono_VBO[4] = (-1, -1, -1, -1);

}


Cono::~Cono()
{
	glDeleteVertexArrays(4, cono_VAO);
	glDeleteBuffers(4, cono_VBO);
}


void Cono::init()
{
	const int n = PARALELOS * MERIDIANOS * 3;
	GLfloat cono_nor[n]; // normal

	GLfloat x, y, z, a, b, da,db, r = 0.5, h = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS); // párticiones de las circunferencias
	db = (GLfloat)M_PI / GLfloat(PARALELOS - 1);
	// Vértices de la circunferencia
	// puntos de la superficie de un cono unitario
	for (ix = 0, b = -1, ib = 0; ib < PARALELOS; ib++, b += 1)
		for (a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
		{
			// Ecuación paramétrica del cono
			x = b*cos(a);
			y = b;
			z = b*sin(a);
			cono_pos[ix + 0] = x * r;
			cono_pos[ix + 1] = y * r;
			cono_pos[ix + 2] = z * r;
			cono_nor[ix + 0] = x;
			cono_nor[ix + 1] = y;
			cono_nor[ix + 2] = z;
		}
	// Generacion de arreglo de índices para el relleno
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS; ib++)
	{
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// 
			cono_index[ix] = iy;      ix++;
			cono_index[ix] = iy + 1;  ix++;
			cono_index[ix] = iy + MERIDIANOS;   ix++;

			cono_index[ix] = iy + MERIDIANOS;   ix++;
			cono_index[ix] = iy + 1;    ix++;
			cono_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		// índices para cerrar el cono, une los índices finales con los iniciales

		cono_index[ix] = iy;       ix++;
		cono_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		cono_index[ix] = iy + MERIDIANOS;    ix++;

		cono_index[ix] = iy + MERIDIANOS;    ix++;
		cono_index[ix] = iy - MERIDIANOS + 1;  ix++;
		cono_index[ix] = iy + 1;     ix++;
		iy++;
	}

	GLuint i;
	glGenVertexArrays(4, cono_VAO);
	glGenBuffers(4, cono_VBO);
	glBindVertexArray(cono_VAO[0]);
	i = 0; // Arreglo de vértices
	glBindBuffer(GL_ARRAY_BUFFER, cono_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cono_pos), cono_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // Arreglo normal 
	glBindBuffer(GL_ARRAY_BUFFER, cono_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cono_nor), cono_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // Arreglo de índices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cono_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cono_index), cono_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Cono::render()
{
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glEnable(GL_LIGHTING);

	//glEnable(GL_LIGHT0);
	glBindVertexArray(cono_VAO[0]);
	glDrawElements(GL_TRIANGLES, sizeof(cono_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}





