#
## Makefile compilation for M2Online MySQL Module
## Author : Tyldar
#

CC	= g++

RM	= rm -rf

LDFLAGS	+= 

CFLAGS	+= -I./ `mysql_config --cflags` `mysql_config --libs`

CUSTOMF	+= -m32 -shared -Wall -fPIC -fpermissive
CXXFLAGS += $(CUSTOMF)
FLAGS	+= $(CUSTOMF)

SRCS	:= $(wildcard ./*.cpp ./SDK/*.cpp ./MySQL/*.cpp)

OBJS	:= $(SRCS:.cpp=.o)

NAME	:= m2online-mysql.so

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
	strip --strip-unneeded $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all re clean fclean