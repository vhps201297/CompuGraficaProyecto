#include "esfera.h"

Esfera::Esfera(GLfloat radio) :
	radio(radio){
	esfera_VAO[4] = ( -1,-1,-1,-1 );
	esfera_VBO[4] = (-1, -1, -1, -1);

}

Esfera::~Esfera()
{
	glDeleteVertexArrays(4, esfera_VAO);
	glDeleteBuffers(4, esfera_VBO);
}

void Esfera::init()
{
	const int nn = PARALELOS * MERIDIANOS * 3;
	GLfloat sphere_nor[nn]; // normal
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS);
	db = (GLfloat)M_PI / GLfloat(PARALELOS - 1);
	// [Generate sphere point data]
	// spherical angles a,b covering whole sphere surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < PARALELOS; ib++, b += db)
		for (a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
		{
			// unit sphere
			x = cos(b)*cos(a);
			y = sin(b); 
			z = cos(b)*sin(a);
			esfera_pos[ix + 0] = x * r;
			esfera_pos[ix + 1] = y * r;
			esfera_pos[ix + 2] = z * r;
			sphere_nor[ix + 0] = x;
			sphere_nor[ix + 1] = y;
			sphere_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS; ib++)
	{
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// first half of QUAD
			esfera_index[ix] = iy;      ix++;
			esfera_index[ix] = iy + 1;    ix++;
			esfera_index[ix] = iy + MERIDIANOS;   ix++;
			// second half of QUAD
			esfera_index[ix] = iy + MERIDIANOS;   ix++;
			esfera_index[ix] = iy + 1;    ix++;
			esfera_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		// first half of QUAD
		esfera_index[ix] = iy;       ix++;
		esfera_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		esfera_index[ix] = iy + MERIDIANOS;    ix++;
		// second half of QUAD
		esfera_index[ix] = iy + MERIDIANOS;    ix++;
		esfera_index[ix] = iy - MERIDIANOS + 1;  ix++;
		esfera_index[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, esfera_VAO);
	glGenBuffers(4, esfera_VBO);
	glBindVertexArray(esfera_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, esfera_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(esfera_pos), esfera_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, esfera_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_nor), sphere_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, esfera_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(esfera_index), esfera_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Esfera::render()
{
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glBindVertexArray(esfera_VAO[0]);
	//glDrawArrays(GL_LINES,0,sizeof(esfera_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_TRIANGLES, sizeof(esfera_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}
