NAME		=		bibArt
SRC			+=		ForkThread.cpp	\
					main.cpp

CC			=		g++

OBJ			+=		$(SRC:.cpp=.o)

CXXFLAGS	+=		--std=c++20 -fgnu-tm

OUTFILE		+=		./out.txt
ERRFILE		+=		./err.txt
COMPILOUT	+=		./compilout.txt

$(NAME):		$(OBJ)
		$(CC) $(OBJ) $(CXXFLAGS) -o $(NAME)

all:			$(NAME)

clean:
		rm -rf $(OBJ) $(OUTFILE) $(ERRFILE)

fclean:			clean
		rm -rf $(NAME) $(COMPILOUT)

re:				clean fclean all

execute:
		echo "You can execute 'make continuousForkCheck' or 'make progressCheck' to follow the test from other terminals"
		echo "You can test end outstreams consistance with: make testFinalOut"
		./$(NAME) > $(OUTFILE) 2> $(ERRFILE)
		ps -aux | grep $(NAME)

progressCheck:
		./pidLoopChecker.sh

testFinalOut:
		./testControlChecker.sh $(OUTFILE)

continuousForkCheck:
		./checkContinuom.sh

reset:
		pkill -9 $(NAME)
		pkill -9 $(NAME)

.PHONY:			clean fclean re all execute progressCheck testFinalOut continuousForkCheck reset 
