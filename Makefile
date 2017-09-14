FLAGS= -Wall -O2 $(shell pkg-config --libs allegro-5.0 allegro_image-5.0)
VERSION=0.1

all:
	g++ main.cpp -o Bomberman-$(VERSION) $(FLAGS)

clean:
	rm Bomberman-*
