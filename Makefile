all:
	g++ -o compiler main.cpp sources/Lexico.cpp sources/Token.cpp sources/SymbolTable.cpp -I headers/
