SRC = $(wildcard Src/*.cpp)
OBJ = $(patsubst Src/%.cpp,Obj/%.o,$(SRC))
LDFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_acodec-5 allegro_primitives-5 allegro_video-5 allegro_audio-5 allegro_dialog-5 allegro_memfile-5 allegro_color-5 allegro_ttf-5 allegro_image-5 allegro_physfs-5 allegro_memfile-5 allegro_physfs-5 --libs)
CPPFLAGS = $(shell pkg-config allegro-5 --cflags) --std=c++11 -g

all: final

final: $(OBJ)
	clang++ $(LDFLAGS) -o $@ $^

Obj/%.o: Src/%.cpp
	clang++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm $(OBJ) final
