CC = gcc
FLAGS = -Wall -std=gnu99
OPT_FLAGS = -lm
EXEC = mishell
LIB_RULES = mycat mychgrp mychmod mychown mycp mydu myecho myls mymkdir mymore mymv mypwd myrm createlib mishell2

all: $(EXEC) clean

mishell: deleteall $(LIB_RULES) 
	@$(CC) $(FLAGS) src/mishell.c src/pile.c -o bin/mishell $(OPT_FLAGS)

deleteall:
	@echo "Compilation..."
	@rm -rf bin
	@mkdir bin
	@mkdir bin/exe

mycat :
	@$(CC) $(FLAGS) src/exe/mycat.c -o bin/exe/mycat $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mycat.c -o bin/mycat.o $(OPT_FLAGS)

mychgrp :
	@$(CC) $(FLAGS) src/exe/mychgrp.c -o bin/exe/mychgrp $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mychgrp.c -o bin/mychgrp.o $(OPT_FLAGS)

mychmod :
	@$(CC) $(FLAGS) src/exe/mychmod.c -o bin/exe/mychmod $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mychmod.c -o bin/mychmod.o $(OPT_FLAGS)

mychown :
	@$(CC) $(FLAGS) src/exe/mychown.c -o bin/exe/mychown $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mychown.c -o bin/mychown.o $(OPT_FLAGS)

mycp :
	@$(CC) $(FLAGS) src/exe/mycp.c -o bin/exe/mycp $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mycp.c -o bin/mycp.o $(OPT_FLAGS)

mydu :
	@$(CC) $(FLAGS) src/exe/mydu.c -o bin/exe/mydu $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mydu.c -o bin/mydu.o $(OPT_FLAGS)

myecho :
	@$(CC) $(FLAGS) src/exe/myecho.c -o bin/exe/myecho $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/myecho.c -o bin/myecho.o $(OPT_FLAGS)

myls :
	@$(CC) $(FLAGS) src/exe/myls.c -o bin/exe/myls $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/myls.c -o bin/myls.o $(OPT_FLAGS)

mymkdir :
	@$(CC) $(FLAGS) src/exe/mymkdir.c -o bin/exe/mymkdir $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mymkdir.c -o bin/mymkdir.o $(OPT_FLAGS)

mymore :
	@$(CC) $(FLAGS) src/exe/mymore.c -o bin/exe/mymore $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mymore.c -o bin/mymore.o $(OPT_FLAGS)

mymv :
	@$(CC) $(FLAGS) src/exe/mymv.c -o bin/exe/mymv $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mymv.c -o bin/mymv.o $(OPT_FLAGS)

mypwd :
	@$(CC) $(FLAGS) src/exe/mypwd.c -o bin/exe/mypwd $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/mypwd.c -o bin/mypwd.o $(OPT_FLAGS)

myrm :
	@$(CC) $(FLAGS) src/exe/myrm.c -o bin/exe/myrm $(OPT_FLAGS)
	@$(CC) $(FLAGS) -c src/libs/myrm.c -o bin/myrm.o $(OPT_FLAGS)

createlib:
	@echo "Compilation librairie..."
	@ar crv bin/libcmd.a bin/*.o
	@ranlib bin/libcmd.a

mishell2:
	@$(CC) $(FLAGS) ./src/mishell2.c ./src/pile.c -o bin/mishell_statique -L./bin/ -lcmd $(OPT_FLAGS)

clean:
	@rm -rf bin/*.o
	@echo "Terminé."