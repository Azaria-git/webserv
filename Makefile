CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = webserv.cpp\
	Socket.cpp\
	ClientSocket.cpp\
	ServerSocket.cpp\
	Epoll.cpp

OBJ = $(SRC:.cpp=.o)
NAME = webserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

reclean: re
	make clean

.PHONY: all clean fclean re reclean