CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

# config

CONFIG_DIR = Server-config
CONFIG_SRC = \
			$(CONFIG_DIR)/Config.cpp\
			$(CONFIG_DIR)/number_utils.cpp\
			$(CONFIG_DIR)/path_utils.cpp

# -------

SRC = \
	main.cpp\
	Signal.cpp\
	Socket.cpp\
	ClientSocket.cpp\
	ServerSocket.cpp\
	Epoll.cpp\
	Webserv.cpp\
	$(CONFIG_SRC)

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