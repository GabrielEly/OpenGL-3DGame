// C�digo feito por Gabriel Ely Konrath e Guilherme Closs Fraga
// Adaptado de Sabertooth dispon�vel no github
// Projeto de um jogo de FPS b�sico

#include "Face.h"

Face::Face() {}

Face::~Face() {
	vertices.clear();
	normais.clear();
	textures.clear();
}

int Face::addVertice(int v) {
	vertices.push_back(v);
	return vertices.size() - 1;
}

void Face::push(int v, int n, int t) {
	vertices.push_back(v);
	normais.push_back(n);
	textures.push_back(t);
}