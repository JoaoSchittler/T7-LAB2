#include"grafo.hpp"

namespace grafo{


    void Grafo::cria_lista_adj(){
        for(it = grafo.begin();it!=grafo.end();it++){

            for(it_ = grafo.begin();it_!=grafo.end();it_++){

                float dist_x = abs(it->second.ponto.x-it_->second.ponto.x);
                float dist_y = abs(it->second.ponto.y-it_->second.ponto.y);
                if((dist_x == 20 && dist_y == 0) || (dist_y == 20 && dist_x == 0)) {
                    it->second.adj.push_back(&it_->second);
                }

            }

        }
    }

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
            for(Vertice* i : aux->adj){
                if(i->cor == BRANCO){
                    i->cor = CINZA;
                    i->dist = aux->dist+1;
                    i->ant = aux;
                    fila.push(i);
                }
            }
            aux->cor = PRETO;
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
        if(saida == nullptr){
            std::cout<<"Saida eh invalido";
            return;
        }
        Vertice* entrada = busca_vertice(destino);
        if(entrada == nullptr){
            std::cout<<"Entrada eh invalido";
            return;
        }

        if(entrada == saida){
            Circulo c = {{saida->ponto.x,saida->ponto.y}, 4};
            t.circulo(c, {0.1, 0.1, 0.9});
            return;
        }
        if(entrada->ant == NULL){
            std::cout<<"Nao existe caminho \n";
        } else {
            caminho_curto(fonte, entrada->ant->ponto );
            Circulo c = {{entrada->ponto.x,entrada->ponto.y}, 4};
            t.circulo(c, {0.1, 0.1, 0.9});
        }
    }


}
