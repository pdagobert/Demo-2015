OBJ_PATH = Obj/
COMMON_PATH = ../Common

LIBS_WINDOWS = -lSOIL -lglfw3dll -lopengl32 -lglew32 -lassimp -lSDL2_mixer
LIBS_UBUNTU = -lGL -lSDL2 -lGLEW -lassimp -lSDL2_image -lSDL2_mixer

LIBS = $(LIBS_WINDOWS)
FLAGS = -W -Wall -std=c++11 -I $(COMMON_PATH)

SOURCES = Main.cpp Application.cpp Universe.cpp Skybox.cpp Window.cpp Shader.cpp Mesh.cpp Geometry.cpp Utility.cpp Camera.cpp
HEADERS = Application.hpp Elements.hpp Universe.hpp Skybox.hpp Window.hpp Shader.hpp Mesh.hpp Geometry.hpp Utility.hpp Camera.hpp

OBJS = $(SOURCES:%.cpp=$(OBJ_PATH)%.o)
VPATH = src:$(COMMON_PATH)

all: $(OBJS)
	g++ $(OBJS) $(LIBS)

$(OBJ_PATH)%.o: %.cpp $(HEADERS)
	g++ -c $< -o $@ $(FLAGS)

clean:
	rm -rf $(OBJ_PATH)*.o
