
#include <fstream>
#include <iostream>
#include "mapa.hpp"

using namespace  tela;

namespace mapa
{ 
 //Retorna uma representação matricial do mapa
 void Mapa::cria(const char* nome_input)
 {
       std::ifstream input_map;
       std::string linha;
       input_map.open(nome_input);
       lin = 0;
       col = 0;
       xini = 0;
       yini = 60;
       unsigned int max = 0;
       //Calcula o numero de linhas e colunas do mapa
       while(std::getline(input_map,linha))
       {
          if(linha.length() > max ) max = linha.length();
          lin++;
       }
       col = max; 

       //Aloca a matriz map
       map = new char* [lin];
       for (unsigned int i = 0; i < lin;i++)
       {
          map[i] =  new char [col];
       }
       //Preenche a matriz map
       input_map.clear();
       input_map.seekg(0, std::ios::beg);
       unsigned int i,j;
       for(i=0;i<lin;i++)
       {
          std::getline(input_map,linha);
          for(j=0;j<col;j++)
          {
              map[i][j] = linha[j];
          }    
       }
       input_map.close();
       std::cout << "Mapa criado, " << lin << " linhas e " << col << " colunas\n";
 } 
 void Mapa::desenha(Tela t)
 {

       Retangulo ret;
       ret.tam.alt  = 20;
       ret.tam.larg = 20;
       ret.pos = {xini,yini};
       Cor c = {0,255,255};
       Cor c2 = {255,255,102};
       //Lê o arquivo texto, desenha o mapa na tela e salva o conteudo desse arquivo na matriz mapa
       unsigned int j,i;
       for(j=0;j<lin;j++)
       {
          for(i=0;i<col;i++)
          {
              t.retangulo(ret,{0,0,0});            
              if(map[j][i]=='P')
                   t.retangulo(ret,c);
              else if(map[j][i]=='C')
              {
                   Circulo circ;
                   circ.raio = 2;
                   circ.centro.x = ret.pos.x+10;
                   circ.centro.y = ret.pos.y+10; 
                   t.circulo(circ,c2); 
              } 
              ret.pos.x+=20;  
          }
          ret.pos.x = 0;
          ret.pos.y+=20;
          c.b-=5; 
          c.r+=15;
       }
       ret.pos.y = 0;
 }
 void Mapa::libera()
 {
    unsigned int i;
    for(i=0;i<lin;i++) delete [] map[i];
    delete [] map;  
 }
}
