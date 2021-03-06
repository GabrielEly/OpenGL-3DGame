// C?digo feito por Gabriel Ely Konrath e Guilherme Closs Fraga
// Adaptado de Sabertooth dispon?vel no github
// Projeto de um jogo de FPS b?sico

#include "Mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh() {
	groups.clear();
	vertices.clear();
	normais.clear();
	textures.clear();
}

glm::vec3* Mesh::vertice(int id) {
	return vertices[id];
}

glm::vec3* Mesh::normal(int id) {
	return normais[id];
}

glm::vec2* Mesh::texture(int id) {
	return textures[id];
}

int Mesh::addVertice(vec3* vec)
{
	vertices.push_back(vec);
	return vertices.size() - 1;
}

int Mesh::addNormal(vec3* vec)
{
	normais.push_back(vec);
	return normais.size() - 1;
}

int Mesh::addTexture(vec2* vec)
{
	textures.push_back(vec);
	return textures.size() - 1;
}

int Mesh::addGroup(Group* group) {
	groups.push_back(group);
	return groups.size() - 1;
}