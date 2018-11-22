#include"grafo.hpp"


///Busca largura com o algoritimo do jv
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


///Essa parte eu ainda nao sei o que fazer diretio para achar o
/// vertice desejado. Ainda nao mudei a chave do std::map para um Ponto
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


///Faz o caminho mais curto entre dois pontos
///e desenha as bolinhas azuis nos vertices que é para passar
///os if para ver se é nullptr da para retirar(eu acho)
/// é só passar um ponto que seja um vertice sempre
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
        t.cor({0.1, 0.1, 0.9});
        t.circulo(c);
        return;
    }
    ///No jogo do pacman sempre ha um caminho
    if(entrada->ant == NULL){
        std::cout<<"Nao existe caminho \n";
    } else {
        caminho_curto(fonte, entrada->ant->ponto );
        Circulo c = {{entrada->ponto.x,entrada->ponto.y}, 4};
        t.cor({0.1, 0.1, 0.9});
        t.circulo(c);
    }
}
