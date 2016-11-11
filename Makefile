CC = g++
FLAGS = -m32 -shared -Wall -fPIC -I./ `mysql_config --libs --include`

OUTFILE = m2online-mysql.so

SRC = $(wildcard ./*.cpp ./SDK/*.cpp)

all: m2online-mysql

m2online-mysql:
	$(CC) $(FLAGS) -o $(OUTFILE) $(SRC)
	strip --strip-unneeded $(OUTFILE)

clean:
	rm -f $(OBJ)