run: 
	gcc ./src/code/*.c -o main -Wall -Wno-unused-variable -Wno-unused-parameter -lmingw32 -ISDL2/include -LSDL2/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
	./main