NAME= m2online-mysql
OUTF= m2online-mysql.so
CC = g++
FLAGS = -m32 -shared -Wall -fPIC -I./ `mysql_config --libs --include`

SRC = $(wildcard ./*.cpp ./SDK/*.cpp)

all: $(NAME)

$(NAME):
	$(CC) $(FLAGS) -o $(OUTF) $(SRC)
	strip --strip-unneeded $(OUTFILE)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(OUTF)

re: clean fclean $(NAME)
