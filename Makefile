# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

# Archivos
TARGET = sistema_ventas
OBJS = main.o usuarios.o productos.o
HEADERS = usuarios.h productos.h

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "Compilación exitosa: $(TARGET)"

# Compilar main.o
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c

# Compilar usuarios.o
usuarios.o: usuarios.c usuarios.h
	$(CC) $(CFLAGS) -c usuarios.c

# Compilar productos.o
productos.o: productos.c productos.h
	$(CC) $(CFLAGS) -c productos.c

# Generar archivos de ensamblador (.s)
asm: main.s usuarios.s productos.s
	@echo "Archivos de ensamblador generados"

main.s: main.c $(HEADERS)
	$(CC) $(CFLAGS) -S main.c -o main.s

usuarios.s: usuarios.c usuarios.h
	$(CC) $(CFLAGS) -S usuarios.c -o usuarios.s

productos.s: productos.c productos.h
	$(CC) $(CFLAGS) -S productos.c -o productos.s

# Usar objdump para desensamblar archivos objeto
disasm: $(OBJS)
	@echo "Desensamblando archivos objeto..."
	objdump -d -S main.o > main_disasm.s
	objdump -d -S usuarios.o > usuarios_disasm.s
	objdump -d -S productos.o > productos_disasm.s
	@echo "Archivos desensamblados creados: *_disasm.s"

# Limpiar archivos generados
clean:
	rm -f $(TARGET) $(OBJS) *.s *_disasm.s
	@echo "Archivos limpiados"

# Limpiar y recompilar
rebuild: clean all

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run asm disasm setup help