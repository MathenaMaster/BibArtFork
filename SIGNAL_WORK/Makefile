NAME		=		bibArt
SRC			+=		osthings.cpp	\
					bibArtOut.cpp 	\
					ForkThread.cpp	\
					main.cpp

CC			=		g++

OBJ			+=		$(SRC:.cpp=.o)

CXXFLAGS	+=		--std=c++20 -fgnu-tm -g

LDLIBS		+=		-lstdc++

OUTFILE		+=		./out.txt
ERRFILE		+=		./err.txt
COMPILOUT	+=		./compilout.txt

TESTSOUTDIR	+=		all_tests_out

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

ps_more:
		ps -aux | more

ps_wc:
		ps | wc -l

execute:
		echo "You can execute 'make rollCheck' or 'make progressCheck' to follow the test from other terminals"
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

MegaMasterTestScript:
		./MegaMasterTestScript.sh $(TESTSOUTDIR) $(NAME) 100 1000

MegaMasterMixTestScript:
		./MegaMasterTestScript_Mixcopy.sh $(TESTSOUTDIR) $(NAME) 10 1000

.PHONY:			all clean fclean re ps ps_more ps_wc execute ForkTestLoop rollCheck progressCheck testFinalOut continuousForkCheck gdb reset MegaMasterTestScript MegaMasterMixTestScript
