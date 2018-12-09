CXX = g++
CXXFLAGS = -g -std=c++11 -Wall 
LDFLAGS = -lallegro -lallegro_main \
    -lallegro_color -lallegro_font -lallegro_primitives -lallegro_image

all: principal

Tela.o: Tela.cpp Tela.hpp geom.hpp
mapa.o: mapa.cpp mapa.hpp  Tela.hpp geom.hpp
jogo.o: jogo.cpp jogo.hpp  mapa.hpp  grafo.hpp Tela.hpp geom.hpp
grafo.o: grafo.cpp grafo.hpp Tela.hpp geom.hpp
main.o: main.cpp jogo.hpp geom.hpp
principal: main.o Tela.o  grafo.o mapa.o jogo.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(LDFLAGS)

clean:
	rm -f principal *.o
