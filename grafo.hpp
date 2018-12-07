#pragma once

#include<iostream>
#include<list>
#include<vector>
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

        void cria_grafo(char** mapa, unsigned int lin , unsigned int col);

        void cria_lista_adj();
        
        std::vector<Vertice*> busca_vizinhos(Ponto p);

         void busca_largura_lab(Vertice* saida, Vertice* entrada, std::vector<Vertice*> fantasmas);

        Vertice* busca_vertice(Ponto p);

        std::vector<Vertice*> caminho_curto(Ponto fonte,Ponto destino);

        void caminho_curto_(Ponto fonte,Ponto destino, std::vector<Vertice*>* caminho);


    };
}
