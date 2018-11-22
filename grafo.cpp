#include"grafo.hpp"

void Grafo::busca_largura_lab(Vertice* saida, Vertice* entrada){
    std::queue<Vertice*> fila;
    fila.push(saida);
    Vertice* aux;
    it = grafo.begin();
    while(it!=grafo.end()){
        it->second.cor =  BRANCO;
        it++;
    }
    while(fila.empty()==false){
        aux = fila.front();
        fila.pop();
        for(auto i : aux->adj){
            if(i.cor == BRANCO){
                i.cor = CINZA;
                i.dist = aux->dist+1;
                i.ant = aux;
                fila.push(&i);
            }
        }
    }
}

Vertice* Grafo::busca_vertice(Ponto p){
    it = grafo.begin();
    while(it!=grafo.end()){
        if(it->second.ponto.x == p.x
           && it->second.ponto.y == p.y){
            return &it->second;
        }
        it++;
    }
    return nullptr;
}

void Grafo::caminho_curto(Ponto fonte,Ponto destino){
    Vertice* saida = busca_vertice(fonte);
    Vertice* entrada = busca_vertice(destino);
    if(entrada == saida){
        std::cout<<saida->ponto.x<<","<<saida->ponto.y<<" ";
        return;
    }
    if(entrada->ant == NULL){
        std::cout<<"Nao existe caminho \n";
    } else {
        caminho_curto(fonte, entrada->ant->ponto );
        std::cout<<entrada->ponto.x<<","<<entrada->ponto.y<<" ";
    }
}

Vertice* Grafo::busca_vertice_lista(std::list<Vertice*> lista, Vertice* desejado){
    for(auto i: lista){
        if(i == desejado){
            return desejado;
        }
    }
    return nullptr;
}

void Grafo::busca_largura_ed(Vertice* entrada, Vertice* saida){
    std::list<Vertice*> lista;
    lista.push_back(entrada);
    for(auto i: lista){
        for (auto j: i->adj){
               Vertice* aux = busca_vertice_lista(lista,&j);
               if(aux == nullptr){
                    lista.push_back(&j);
                    j.ant = i;
                    if(&j == saida){
                        caminho_curto(j.ponto, saida->ponto);
                    }
               }

        }
    }

}
