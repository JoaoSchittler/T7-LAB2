#pragma once

#include<iostream>
#include<list>
#include<queue>
#include<map>

#include"Tela.hpp"

using namespace tela;

namespace grafo{

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

        void cria_lista_adj();

        void busca_largura_lab(Vertice* saida, Vertice* entrada);

        Vertice* busca_vertice(Ponto p);

        void caminho_curto(Ponto fonte,Ponto destino);


    };
}
