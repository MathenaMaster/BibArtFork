NAME		=		bibArt
SRC			+=		ForkThread.cpp	\
					main.cpp

CC			=		g++

OBJ			+=		$(SRC:.cpp=.o)

CXXFLAGS	+=		--std=c++20 -fgnu-tm -g

LDLIBS		+=		-lstdc++

OUTFILE		+=		./out.txt
ERRFILE		+=		./err.txt
COMPILOUT	+=		./compilout.txt

$(NAME):		$(OBJ)
		$(CC) $(OBJ) $(CXXFLAGS) $(LDLIBS) -o $(NAME)

all:			$(NAME)

clean:
		rm -rf $(OBJ) $(OUTFILE) $(ERRFILE)

fclean:			clean
		rm -rf $(NAME) $(COMPILOUT)

re:				clean fclean all

ps:
		ps -aux | grep $(NAME)

execute:
		echo "You can execute 'make continuousForkCheck' or 'make progressCheck' to follow the test from other terminals"
		echo "You can test end outstreams consistance with: make testFinalOut"
		./$(NAME) > $(OUTFILE) 2> $(ERRFILE)
		make ps

ForkTestLoop:
		./continuousForkTest.sh

rollCheck:
		./rollCheck.sh

progressCheck:
		./pidLoopChecker.sh

testFinalOut:
		./testControlChecker.sh $(OUTFILE)

continuousForkCheck:
		./checkContinuom.sh

gdb:
		gdb ./$(NAME) --command=gdbthings.gdb
reset:
		pkill -9 $(NAME)
		pkill -9 $(NAME)

.PHONY:			all clean fclean re ps execute ForkTestLoop rollCheck progressCheck testFinalOut continuousForkCheck gdb reset
