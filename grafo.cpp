#include"grafo.hpp"

namespace grafo{

    void Grafo::cria_grafo(char** mapa, unsigned int lin , unsigned int col){
        unsigned int i,j, cont = 0;
        Ponto  p = {0, 60};

        for(i=0;i<lin;i++){

            for(j=0;j<col;j++){

                if(mapa[i][j]!='P'){
                    grafo[cont] = Vertice();
                    grafo[cont].dist = 0;
                    grafo[cont].ponto.x = p.x+10;
                    grafo[cont].ponto.y = p.y+10;
                    cont++;
                }

                p.x += 20;
            }

            p.x  = 0;
            p.y += 20;
        }
    }


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
    
    std::vector<Vertice*> Grafo::busca_vizinhos(Ponto p){
        std::vector<Vertice*> vizinhos;
        Vertice* v  = busca_vertice(p);
        for(auto it : v->adj){
            vizinhos.push_back(it);
        }
        return vizinhos;
    }

    void Grafo::busca_largura_lab(Vertice* saida, Vertice* entrada, std::vector<Vertice*> fantasmas){
        std::queue<Vertice*> fila;
        fila.push(saida);
        Vertice* aux;
        it = grafo.begin();
        while(it!=grafo.end()){
            it->second.cor =  BRANCO;
            it++;
        }
        
        for(auto i: fantasmas)
            i->cor = PRETO;
        
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



    std::vector<Vertice*> Grafo::caminho_curto(Ponto fonte,Ponto destino){
        std::vector<Vertice*> caminho;
        caminho_curto_(fonte, destino, &caminho);
        return caminho;
    }

    void Grafo::caminho_curto_(Ponto fonte,Ponto destino, std::vector<Vertice*>*caminho){
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
            caminho->push_back(saida);
            return;
        }
        if(entrada->ant == NULL){
            std::cout<<"Nao existe caminho \n";
        } else {
            caminho_curto_(fonte, entrada->ant->ponto, caminho );
            caminho->push_back(entrada);

        }
    }


}
