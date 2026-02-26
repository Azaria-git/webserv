# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: toloandr <toloandr@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/17 07:49:00 by toloandr          #+#    #+#              #
#    Updated: 2026/02/17 07:49:01 by toloandr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = main.cpp  Config.cpp number_utils.cpp path_utils.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all


