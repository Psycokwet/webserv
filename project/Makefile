# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#              #
#    Updated: 2022/04/13 07:42:26 by scarboni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 				= webserv

OBJ_PATH			= bin/

CXX 				= c++

CPPFLAGS 			=  -Wall -Wextra -Werror -g3  -std=c++98

SRC_PATH			= ./srcs/

TESTCONFIGRULE		= testConfigRule
TESTCONFIG			= testConfig
TESTPARSERRULE		= testParserRule
TESTPARSER			= testParser

SRCS = 	main.cpp

ifeq ($(TESTS), $(TESTPARSERRULE))
	SRCS = mainParserTest.cpp
	NAME = $(TESTPARSER)
endif

ifndef TESTS
	TESTS=""
endif

ifndef LEAKS
	LEAKS = 
	LEAKS = valgrind --leak-check=full
endif


SRCS += Node.cpp ConfigConsumer.cpp ActionForKey.cpp

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
	@rm -f $(TESTPARSER)
	@echo "\033[0m"		

re: fclean all


CONFIG_FOLDER = ./config_files/

define tester_sep
	printf "\n\n\n\n____.--.--.____.--.--.____.--.--.____.--.--.__** $(1) **__.--.--.____.--.--.____.--.--.____.--.--.____\n" ;\
	$(MAKE) $(1) TESTS=$(1) LEAKS="$(LEAKS)"
endef

define launch_one_test_without_sep
	$(LEAKS) ./$(NAME) $(1)
endef

define launch_one_test_with_sep
	$(call launch_one_test_without_sep,$(1)) ;\
	printf "____.--.--.____.--.--.____.--.--.____.--.--.__Weeeeeeeeee__.--.--.____.--.--.____.--.--.____.--.--.____\n" ;\
	printf "Command : $(LEAKS) ./$(NAME) $(1) \n" 
endef

define launch_only_legal_tests
	@ if [ $(TESTS) = $(1) ]; then \
		$(2)
	else \
		$(call tester_sep,$(1)) ;\
	fi ;
endef

$(TESTPARSERRULE):	$(NAME)
	$(call launch_only_legal_tests,$(TESTPARSERRULE),\
		$(call launch_one_test_with_sep,"example1") ;\
		$(call launch_one_test_without_sep,"example2") ;\)

$(TESTCONFIG): $(NAME)
	$(call launch_only_legal_tests,$(TESTCONFIG),\
		$(call launch_one_test_with_sep) ;\
		$(call launch_one_test_with_sep,$(CONFIG_FOLDER)IDontExist.conf) ;\
		$(call launch_one_test_with_sep,$(CONFIG_FOLDER)bad_syntax.conf) ;\
		$(call launch_one_test_without_sep,$(CONFIG_FOLDER)example_nginx_base.conf) ;\)

test: $(TESTPARSERRULE) $(TESTCONFIG)


.PHONY: clean fclean re bonus