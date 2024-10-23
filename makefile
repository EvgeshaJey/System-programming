CC=gcc
CFLAGS=-Iinclude
OBJ=src/main.o src/arithmetic.o
LIB=libarithmetic.a

# Основная цель
all: $(LIB) my_program

# Правило для создания статической библиотеки
$(LIB): src/arithmetic.o
	ar rcs $@ $^

# Правило для сборки исполняемого файла
my_program: $(OBJ) $(LIB)
	$(CC) -o $@ $(OBJ) -L. -larithmetic

# Правила для сборки объектных файлов
src/main.o: src/main.c include/arithmetic.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/arithmetic.o: src/arithmetic.c include/arithmetic.h
	$(CC) $(CFLAGS) -c src/arithmetic.c -o src/arithmetic.o

# Очистка
clean:
	rm -f src/*.o *.a my_program
