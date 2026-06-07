TARGET = M024.exe
COMMAND = gcc -g -Wall -Iinclude
OBJ = build/interpret.o \
	build/memoryHandler.o \
	build/error.o \
 	build/lexer.o \
	build/indentation.o \
	build/parser.o \
	build/variableRegistry.o \
	build/functionRegistry.o \
	build/evaluate.o \
	build/callStack.o \
	build/globals.o \
	build/imports.o

ifeq ($(OS),Windows_NT)
	TARGET = M024.exe
	CLEAN = del /F build\*.o M024.exe
else
	TARGET = M024
	CLEAN = rm -f build/*.o M024
endif
$(TARGET) : $(OBJ)
	$(COMMAND) $(OBJ) -o $(TARGET)
build/interpret.o : interpret.c
	$(COMMAND) -c interpret.c -o build/interpret.o
build/memoryHandler.o : scripts/memoryHandler.c 
	$(COMMAND) -c scripts/memoryHandler.c -o build/memoryHandler.o
build/error.o : scripts/error.c 
	$(COMMAND) -c scripts/error.c -o build/error.o
build/lexer.o : scripts/lexer.c
	$(COMMAND) -c scripts/lexer.c -o build/lexer.o
build/indentation.o : scripts/indentation.c
	$(COMMAND) -c scripts/indentation.c -o build/indentation.o
build/parser.o : scripts/parser.c
	$(COMMAND) -c scripts/parser.c -o build/parser.o
build/variableRegistry.o : scripts/variableRegistry.c
	$(COMMAND) -c scripts/variableRegistry.c -o build/variableRegistry.o
build/functionRegistry.o : scripts/functionRegistry.c
	$(COMMAND) -c scripts/functionRegistry.c -o build/functionRegistry.o
build/evaluate.o : scripts/evaluate.c
	$(COMMAND) -c scripts/evaluate.c -o build/evaluate.o
build/callStack.o : scripts/callStack.c
	$(COMMAND) -c scripts/callStack.c -o build/callStack.o
build/globals.o : scripts/globals.c
	$(COMMAND) -c scripts/globals.c -o build/globals.o
build/imports.o : scripts/imports.c
	$(COMMAND) -c scripts/imports.c -o build/imports.o
clean:
	$(CLEAN)