// C�digo feito por Gabriel Ely Konrath e Guilherme Closs Fraga
// Adaptado de Sabertooth dispon�vel no github
// Projeto de um jogo de FPS b�sico

#include "Group.h"

Group::Group() {}

Group::~Group() {
	faces.clear();
}

int Group::addFace(Face* face) {
	faces.push_back(face);
	return faces.size() - 1;
}