#pragma once 
#include "Tela.hpp"
namespace mapa
{
   	struct Mapa
   	{
   		char** map;
   		unsigned int lin;
		unsigned int col;
		float           xini;
		float	        yini;
		//Função que cria um mapa a partir de um arquivo txt
 		void cria(const char* nome_input);
 		//Função de deleta a memória ocupada pelo mapa
   	        void libera();
   	        //Função que desenha o mapa na tela T
		void desenha(tela::Tela t);

   	};
};
