# Nome do executável
TARGET = jogo

# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -I./src

# Bibliotecas do OpenGL
LIBS = -lGL -lGLU -lglut

# Encontra todos os arquivos .cpp nas subpastas de src/
SRCS = $(wildcard src/*.cpp) \
       $(wildcard src/core/*.cpp) \
       $(wildcard src/entities/*.cpp) \
       $(wildcard src/rendering/*.cpp)

# Transforma a lista de .cpp em lista de .o (arquivos objeto)
OBJS = $(SRCS:.cpp=.o)

# Regra padrão
all: $(TARGET)

# Como linkar o executável
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

# Como compilar os arquivos .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os compilados
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
