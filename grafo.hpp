#ifndef GRAFO_HPP_INCLUDED
#define GRAFO_HPP_INCLUDED

#include<iostream>
#include<list>
#include<queue>
#include<map>

#include"geom.hpp"
#include"Tela.hpp"

using namespace geom;
using namespace tela;

enum COLOR{BRANCO, CINZA, PRETO};

struct Vertice{
    Ponto ponto;
    Vertice* ant;
    int dist;
    COLOR cor;
    std::list<Vertice*> adj;
};

struct Grafo{
    Tela t;
    std::map<int, Vertice> grafo;
    std::map<int, Vertice>::iterator it;
    std::map<int, Vertice>::iterator it_;

    void busca_largura_lab(Vertice* saida, Vertice* entrada);

    Vertice* busca_vertice(Ponto p);

    void caminho_curto(Ponto fonte,Ponto destino);

};




#endif // GRAFO_HPP_INCLUDED

