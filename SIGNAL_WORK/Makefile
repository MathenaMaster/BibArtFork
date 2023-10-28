NAME		=		bibArt
SRC			+=		osthings.cpp	\
					bibArtOut.cpp 	\
					ForkThread.cpp	\
					main.cpp

CC			=		g++

CXXFLAGS	+=		--std=c++20 #-fgnu-tm # -freport-bug

#CFLAGS		+=		--std=c++20 -fgnu-tm

OBJ			+=		$(SRC:.cpp=.o)

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
		rm -rf $(TESTSOUTDIR)/*

fclean:			clean
		rm -rf $(NAME) $(COMPILOUT)

re:				clean fclean all

ps:
		ps -aux | grep $(NAME)

ps_more:
		ps -aux | more

ps_wc:
		ps -aux | grep $(NAME) | wc -l

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

throw_test_session:
		ps | grep bibArt | awk '{print $0}' | xargs kill -9

fake_throw_test_session:
		ps | grep bash | grep -e "^[0-9]+"


MegaMasterTestScript:
		./MegaMasterTestScript.sh $(TESTSOUTDIR) $(NAME) 100 1000 $(OUTFILE) $(ERRFILE)

MegaMasterMixTestScript:
		./MegaMasterTestScript_Mixcopy.sh $(TESTSOUTDIR) $(NAME) 10 1000

.PHONY:			all clean fclean re ps ps_more ps_wc execute ForkTestLoop rollCheck progressCheck testFinalOut continuousForkCheck gdb reset fake_throw_test_session throw_test_session MegaMasterTestScript MegaMasterMixTestScript
