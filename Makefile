SRC = $(wildcard Src/*.cpp)
OBJ = $(patsubst Src/%.cpp,Obj/%.o,$(SRC))
LDFLAGS = -L/opt/homebrew/Cellar/allegro/5.2.7.0/lib
LIBS = -lallegro_main -lallegro_acodec -lallegro_primitives -lallegro_video -lallegro_audio -lallegro_dialog -lallegro_color -lallegro_ttf -lallegro_font -lallegro_image -lallegro_memfile -lallegro_physfs -lallegro -lallegro_ttf
CPPFLAGS = -I/opt/homebrew/Cellar/allegro/5.2.7.0/include --std=c++11 -g

all: final

final: $(OBJ)
	clang++ $(LDFLAGS) $(LIBS) -o $@ $^

Obj/%.o: Src/%.cpp
	clang++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm $(OBJ) final
