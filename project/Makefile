# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#              #
#    Updated: 2022/05/23 11:02:32 by scarboni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 				= webserv

OBJ_PATH			= bin/

CXX 				= c++

CPPFLAGS 			=  -Wall -Wextra -Werror -g3  -std=c++98 -DGRAMMAR_FILE='"./grammar/grammar_request_onliners_light.gram"'

SRC_PATH			= ./srcs/
CONFIG_PATH			= config/
SERVER_PATH			= server/
REQUEST_PATH		= requests/
UTIL_PATH			= util/

TESTCONFIGRULE		= testConfigRule
TESTCONFIG			= testConfig
TESTPARSERRULE		= testParserRule
TESTPARSER			= testParser

SRCS = 	main.cpp

ifeq ($(TESTS), $(TESTPARSERRULE))
	SRCS = mainParserTest.cpp
	NAME = $(TESTPARSER)
	CPPFLAGS += -DDEBUG=true
else
	CPPFLAGS += -DDEBUG=false
endif

ifndef TESTS
	TESTS=""
endif

ifndef LEAKS
	LEAKS = 
	LEAKS = valgrind --leak-check=full
endif

OBJ_PATHS			+= $(OBJ_PATH) $(addprefix $(OBJ_PATH), $(CONFIG_PATH) $(SERVER_PATH) $(REQUEST_PATH) $(UTIL_PATH) )

REQUESTS_FILES = 	GrammarParser.cpp \
					GrammarVariables.cpp \
					GrammarParserBuilderMarker.cpp \
					Statements.cpp \
					ResponseBuilder.cpp
					
SRCS += $(addprefix $(REQUEST_PATH), $(REQUESTS_FILES))
UTIL_FILES = parse.cpp 
SRCS += $(addprefix $(UTIL_PATH), $(UTIL_FILES))

SERVER_FILES =  MasterServer.cpp \
				OneServer.cpp \
				AServerItem.cpp
SRCS += $(addprefix $(SERVER_PATH), $(SERVER_FILES))

CONFIG_FILES =  Node.cpp \
				ConfigConsumer.cpp \
				ActionForKey.cpp
SRCS += $(addprefix $(CONFIG_PATH), $(CONFIG_FILES))

SRC_FILES	+=	$(SRCS)

SRC			= 	$(addprefix $(SRC_PATH), $(SRC_FILES))
OBJ 		= 	$(addprefix $(OBJ_PATH), $(SRC_FILES:cpp=o))

OBJ_PATHS_INIT			= $(OBJ_PATHS)

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



define tester_sep
	printf "\n\n\n\n____.--.--.____.--.--.____.--.--.____.--.--.__** $(1) **__.--.--.____.--.--.____.--.--.____.--.--.____\n" ;\
	$(MAKE) $(1) TESTS=$(1) LEAKS="$(LEAKS)"
endef

define launch_one_test_without_sep
	printf "Command : $(LEAKS) ./$(NAME) $(1) \n"  ;\
	$(LEAKS) ./$(NAME) $(1)
endef

define launch_one_test_with_sep
	$(call launch_one_test_without_sep,$(1)) ;\
	printf "____.--.--.____.--.--.____.--.--.____.--.--.__Weeeeeeeeee__.--.--.____.--.--.____.--.--.____.--.--.____\n"
endef

define launch_only_legal_tests
	@ if [ $(TESTS) = $(1) ]; then \
		$(2)
	else \
		$(call tester_sep,$(1)) ;\
	fi ;
endef

PARSER_FILE_EXT = .yml
REQUESTS_FOLDER=requests_samples/
# EXAMPLE_REQUESTS_RAW=get_likeAPony post_almost_empty_file post_pdf post_xpm_file 
EXAMPLE_REQUESTS_RAW=get_likeAPony
EXAMPLE_REQUESTS_PREF=$(addprefix $(REQUESTS_FOLDER), $(EXAMPLE_REQUESTS_RAW))
EXAMPLE_REQUESTS=$(addsuffix $(PARSER_FILE_EXT), $(EXAMPLE_REQUESTS_PREF))

define launch_test_from_array_args
	COUNT=0;\
	for ARG in $(1) ; do \
		COUNT=$$(( 1 + $$COUNT ));\
		LAST=$$ARG;\
		[ "$(words $(1))" -eq $$COUNT ] && break ;\
		$(call launch_one_test_with_sep,$$ARG) ;\
	done ;\
	$(call launch_one_test_without_sep,$$LAST)
endef

$(TESTPARSERRULE):	$(NAME)
	$(call launch_only_legal_tests,$(TESTPARSERRULE),\
		$(call launch_test_from_array_args,$(EXAMPLE_REQUESTS)) ;\)


CONFIG_FOLDER = ./config_files/
CONFIG_FILE_EXT = .conf
ARGS_CONFIGS_TESTS_RAW = IDontExist bad_syntax example_nginx_base simple
ARGS_CONFIGS_TESTS_RAW_PREF = $(addprefix $(CONFIG_FOLDER), $(ARGS_CONFIGS_TESTS_RAW))
ARGS_CONFIGS_TESTS = $(addsuffix $(CONFIG_FILE_EXT), $(ARGS_CONFIGS_TESTS_RAW_PREF))


$(TESTCONFIG): $(NAME)
	$(call launch_only_legal_tests,$(TESTCONFIG),\
		$(call launch_one_test_with_sep) ;\
		$(call launch_one_test_with_sep, Too much args) ;\
		$(call launch_test_from_array_args,$(ARGS_CONFIGS_TESTS)) ;\)

allTests: $(TESTCONFIG) $(TESTPARSERRULE) 


.PHONY: clean fclean re bonus