# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#              #
#    Updated: 2022/03/27 09:31:00 by scarboni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 				= webserv

OBJ_PATH			= bin/

CXX 				= c++

CPPFLAGS 			=  -Wall -Wextra -Werror -g3  -std=c++98

SRC_PATH			= ./srcs/

SRCS = 	main.cpp Node.cpp ConfigConsumer.cpp ActionForKey.cpp AllConfig.cpp ConfigOneServer.cpp MasterServer.cpp

SRC_FILES	+=	$(SRCS)


SRC			= 	$(addprefix $(SRC_PATH), $(SRC_FILES))
OBJ 		= 	$(addprefix $(OBJ_PATH), $(SRC_FILES:cpp=o))

OBJ_PATHS_INIT			= $(OBJ_PATH)

all: $(NAME)

$(OBJ_PATHS_INIT)	:
	@echo "Generating bin folder and subfolders" $@
	@mkdir -p  $@  

$(NAME): $(OBJ_PATHS_INIT)  $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(HEADERS_FILES)
	${CXX} ${CPPFLAGS}   -c $< -o $@ 

clean:
	@echo "\033[0;31m\nDeleting objects..."
	@rm -rf $(OBJ_PATH)
	@echo "\033[0m"

fclean:
	@echo "\033[0;31m\nDeleting objects..."
	@rm -rf $(OBJ_PATH)
	@echo "\nDeleting executable..."
	@rm -f $(NAME)
	@echo "\033[0m"		

re: fclean all


LEAKS = 
LEAKS = valgrind --leak-check=full
CONFIG_FOLDER = ./config_files/
SEPARATOR = @echo "____.--.--.____.--.--.____.--.--.____.--.--.__Weeeeeeeeee__.--.--.____.--.--.____.--.--.____.--.--.____"

test:	$(NAME)
	$(LEAKS) ./$(NAME)
	$(SEPARATOR)
	$(LEAKS) ./$(NAME) $(CONFIG_FOLDER)IDontExist.conf too much args
	$(SEPARATOR)
	$(LEAKS) ./$(NAME) $(CONFIG_FOLDER)IDontExist.conf
	$(SEPARATOR)
	$(LEAKS) ./$(NAME) $(CONFIG_FOLDER)bad_syntax.conf
	$(SEPARATOR)
	$(LEAKS) ./$(NAME) $(CONFIG_FOLDER)example_nginx_base.conf
	$(SEPARATOR)
	$(LEAKS) ./$(NAME) $(CONFIG_FOLDER)almost_empty.conf


.PHONY: clean fclean re bonus