# Makefile для сборки проекта Lab9

CXX = g++
CXXFLAGS = -Wall -g
TARGET = my_program

# Исходные файлы
SOURCES = main.cpp functions.cpp file_io.cpp graph.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Правила сборки
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Правило для компиляции исходных файлов в объектные
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для препроцессинга
preprocess:
	$(CXX) $(CXXFLAGS) -E main.cpp > main.i

# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET) main.i
