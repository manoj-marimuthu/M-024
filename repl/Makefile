TARGET = repl.exe
COMMAND = gcc -g -Wall -I../include
OBJ = build/repl.o \
	build/interpret_api.o \
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
	build/imports.o \
	build/listobj.o \
	build/value.o \
	build/index.o

ifeq ($(OS),Windows_NT)
	TARGET = repl.exe
	CLEAN = del /F build\*.o repl.exe
else
	TARGET = repl
	CLEAN = rm -f build/*.o repl.exe
endif
$(TARGET) : $(OBJ)
	$(COMMAND) $(OBJ) -o $(TARGET)
build/repl.o : repl.c
	$(COMMAND) -c repl.c -o build/repl.o
build/interpret_api.o : ../scripts/interpret_api.c
	$(COMMAND) -c ../scripts/interpret_api.c -o build/interpret_api.o
build/memoryHandler.o : ../scripts/memoryHandler.c 
	$(COMMAND) -c ../scripts/memoryHandler.c -o build/memoryHandler.o
build/error.o : ../scripts/error.c 
	$(COMMAND) -c ../scripts/error.c -o build/error.o
build/lexer.o : ../scripts/lexer.c
	$(COMMAND) -c ../scripts/lexer.c -o build/lexer.o
build/indentation.o : ../scripts/indentation.c
	$(COMMAND) -c ../scripts/indentation.c -o build/indentation.o
build/parser.o : ../scripts/parser.c
	$(COMMAND) -c ../scripts/parser.c -o build/parser.o
build/variableRegistry.o : ../scripts/variableRegistry.c
	$(COMMAND) -c ../scripts/variableRegistry.c -o build/variableRegistry.o
build/functionRegistry.o : ../scripts/functionRegistry.c
	$(COMMAND) -c ../scripts/functionRegistry.c -o build/functionRegistry.o
build/evaluate.o : ../scripts/evaluate.c
	$(COMMAND) -c ../scripts/evaluate.c -o build/evaluate.o
build/callStack.o : ../scripts/callStack.c
	$(COMMAND) -c ../scripts/callStack.c -o build/callStack.o
build/globals.o : ../scripts/globals.c
	$(COMMAND) -c ../scripts/globals.c -o build/globals.o
build/imports.o : ../scripts/imports.c
	$(COMMAND) -c ../scripts/imports.c -o build/imports.o
build/value.o : ../scripts/value.c
	$(COMMAND) -c ../scripts/value.c -o build/value.o
build/listobj.o : ../scripts/listobj.c
	$(COMMAND) -c ../scripts/listobj.c -o build/listobj.o
build/index.o : ../scripts/index.c
	$(COMMAND) -c ../scripts/index.c -o build/index.o
clean:
	$(CLEAN)