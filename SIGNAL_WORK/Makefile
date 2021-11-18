NAME	+=		bibArt
SRC		+=		ForkThread.cpp	\
				main.cpp

OBJ		+=		$(SRC:.cpp=.o)

OUTFILE	+=		out.txt
ERRFILE	+=		err.txt

$(NAME):		$(OBJ)

all:			$(NAME)
		g++ $(OBJ) -g -lpthread --std=c++17 -o $(NAME)

 clean:
		rm -rf $(OBJ)
		rm -rf out.txt err.txt

fclean:	clean
		rm -rf $(NAME)

execute: 	all
		./$(NAME) > ./$(OUTFILE) 2> ./$(ERRFILE)