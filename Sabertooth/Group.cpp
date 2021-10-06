// Código feito por Gabriel Ely Konrath e Guilherme Closs Fraga
// Adaptado de Sabertooth disponível no github
// Projeto de um jogo de FPS básico

#include "Group.h"

Group::Group() {}

Group::~Group() {
	faces.clear();
}

int Group::addFace(Face* face) {
	faces.push_back(face);
	return faces.size() - 1;
}