# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scarboni <scarboni@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/19 22:03:00 by scarboni          #+#    #+#              #
#    Updated: 2022/06/05 16:09:58 by scarboni         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME 				= webserv
COMPILE 			= compile

OBJ_PATH			= bin/

CXX 				= c++

CPPFLAGS 			=  -Wall -Wextra -Werror -g3  -std=c++98 -DGRAMMAR_FILE='"./grammar/grammar_request_onliners_light.gram"'

SRC_PATH			= ./srcs/
CONFIG_PATH			= config/
SERVER_PATH			= server/
REQUEST_PATH		= requests/
RESPONSE_PATH		= response/
UTIL_PATH			= util/

TESTSERVERRULE		= testServerRule
TESTSERVER			= testServer
TESTCONFIGRULE		= testConfigRule
TESTCONFIG			= testConfig
TESTPARSERRULE		= testParserRule
TESTPARSER			= testParser
TESTNETWORKRULE		= testNetworkRule
TESTNETWORK			= testNetwork

LAST_RUN_LOGS_FOLDER	= logs
SAVED_LOGS_FOLDER		= pastLogs


CPPFLAGS += -DLOGS_FOLDER='"$(LAST_RUN_LOGS_FOLDER)"'

ifndef TESTS #!Focus on this
	TESTS=""
	SRCS = 	main.cpp
	CPPFLAGS += -DDEBUG=false
else
		CPPFLAGS += -DDEBUG=true # ! to set variable DEBUG = true here, ok I get it
	ifeq ($(TESTS), $(TESTPARSERRULE))
		SRCS = mainParserTest.cpp
		NAME = $(TESTPARSER)
	else
		ifeq ($(TESTS), $(TESTCONFIGRULE)) # ! testConfigRule, give program name: testConfig
			SRCS = mainTestConfig.cpp
			NAME = $(TESTCONFIG)
		else
			ifeq ($(TESTS), $(TESTSERVERRULE))
				SRCS = main.cpp
				NAME = $(TESTSERVER)
			else
				ifeq ($(TESTS), $(TESTNETWORKRULE))
					SRCS = mainNetworkTest.cpp
					NAME = $(TESTNETWORK)
				endif
			endif
		endif
	endif
endif



ifndef LEAKS
	LEAKS = 
	LEAKS = valgrind --leak-check=full
endif

OBJ_PATHS			+= $(OBJ_PATH) $(addprefix $(OBJ_PATH), $(CONFIG_PATH) $(SERVER_PATH) $(RESPONSE_PATH) $(REQUEST_PATH) $(UTIL_PATH) )

RESPONSE_FILES = 	ResponseBuilder.cpp
					
SRCS += $(addprefix $(RESPONSE_PATH), $(RESPONSE_FILES))

REQUESTS_FILES = 	GrammarParser.cpp \
					GrammarVariables.cpp \
					GrammarParserBuilderMarker.cpp \
					Statements.cpp
					
SRCS += $(addprefix $(REQUEST_PATH), $(REQUESTS_FILES))
UTIL_FILES =	parse.cpp \
				numbers.cpp \
				logger.cpp		
SRCS += $(addprefix $(UTIL_PATH), $(UTIL_FILES))

SERVER_FILES =  MasterServer.cpp \
				OneServer.cpp \
				AServerItem.cpp \
				OneLocation.cpp \
				ALocation.cpp
SRCS += $(addprefix $(SERVER_PATH), $(SERVER_FILES))

CONFIG_FILES =  Node.cpp \
				ConfigConsumer.cpp \
				ActionForKey.cpp
SRCS += $(addprefix $(CONFIG_PATH), $(CONFIG_FILES))

SRC_FILES	+=	$(SRCS)

SRC			= 	$(addprefix $(SRC_PATH), $(SRC_FILES))
OBJ 		= 	$(addprefix $(OBJ_PATH), $(SRC_FILES:cpp=o))

PATHS_INIT			= $(OBJ_PATHS) $(SAVED_LOGS_FOLDER) $(LAST_RUN_LOGS_FOLDER)

CLEAN_UNWANTED_PATHS = CleanPreviousPaths
SAVE_LAST_LOGS = SaveLastLogs

all: $(NAME)

$(SAVE_LAST_LOGS)	:
	@echo "Saving previous logs"
	@mv  $(LAST_RUN_LOGS_FOLDER)  $(SAVED_LOGS_FOLDER)/$(shell date "+%y_%m_%d_%H_%M_%S")

$(CLEAN_UNWANTED_PATHS)	: 
	@echo "deleting previous run special paths..." $(LAST_RUN_LOGS_FOLDER)
	@rm -rf  $(LAST_RUN_LOGS_FOLDER) 

$(PATHS_INIT)	: $(CLEAN_UNWANTED_PATHS)
	@echo "Generating bin folder and subfolders" $@
	@mkdir -p  $@  

$(COMPILE): $(PATHS_INIT)  $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)
	
$(NAME): $(COMPILE)

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
	@rm -f $(TESTSERVER)
	@rm -f $(TESTCONFIG)
	@rm -f $(TESTNETWORK)
	@echo "\033[0m"	

cleanLogs:
	@echo "\033[0;31m\nDeleting last logs..."
	@rm -rf $(LAST_RUN_LOGS_FOLDER)
	@echo "\033[0m"		

fcleanLogs:
	@echo "\033[0;31m\nDeleting previous logs..."
	@rm -rf $(SAVED_LOGS_FOLDER)
	@echo "\033[0;31m\nDeleting last logs..."
	@rm -rf $(LAST_RUN_LOGS_FOLDER)
	@echo "\033[0m"		

re: fclean all



define tester_sep
	printf "\n\n\n\n____.--.--.____.--.--.____.--.--.____.--.--.__** $(1) **__.--.--.____.--.--.____.--.--.____.--.--.____\n" ;\
	$(MAKE) $(COMPILE) TESTS=$(1) LEAKS="$(LEAKS)" ;\
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

define launch_only_legal_tests # ! Focus on this
	@ if [ $(TESTS) = $(1) ]; then \
		$(2)
	else \
		$(call tester_sep,$(1)) ;\
	fi ;
endef

# PARSER_FILE_EXT = .yml
REQUESTS_FOLDER=requests_samples/
GET_REQUESTS_FOLDER=$(addprefix $(REQUESTS_FOLDER), "GET/")
DELETE_REQUESTS_FOLDER=$(addprefix $(REQUESTS_FOLDER), "DELETE/")
POST_REQUESTS_FOLDER=$(addprefix $(REQUESTS_FOLDER), "POST/")
ERROR_REQUESTS_FOLDER=$(addprefix $(REQUESTS_FOLDER), "ERROR/")

GET_REQUESTS:= $(addprefix $(GET_REQUESTS_FOLDER), $(shell ls $(GET_REQUESTS_FOLDER)))
DELETE_REQUESTS:= $(addprefix $(DELETE_REQUESTS_FOLDER), $(shell ls $(DELETE_REQUESTS_FOLDER)))
POST_REQUESTS:= $(addprefix $(POST_REQUESTS_FOLDER), $(shell ls $(POST_REQUESTS_FOLDER)))
ERROR_REQUESTS:= $(addprefix $(ERROR_REQUESTS_FOLDER), $(shell ls $(ERROR_REQUESTS_FOLDER)))

EXAMPLE_REQUESTS:= $(GET_REQUESTS) $(DELETE_REQUESTS) $(POST_REQUESTS) $(ERROR_REQUESTS)
# EXAMPLE_REQUESTS:= $(addprefix $(GET_REQUESTS_FOLDER), http_request.yml)

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


$(TESTPARSERRULE):	
	$(call launch_only_legal_tests,$(TESTPARSERRULE),\
		$(call launch_test_from_array_args,$(EXAMPLE_REQUESTS)) ;\)


CONFIG_FOLDER = ./config_files/
CONFIG_FILE_EXT = .conf
ARGS_CONFIGS_TESTS_RAW = IDontExist bad_syntax example_nginx_base simple
ARGS_CONFIGS_TESTS_RAW_PREF = $(addprefix $(CONFIG_FOLDER), $(ARGS_CONFIGS_TESTS_RAW))
ARGS_CONFIGS_TESTS = $(addsuffix $(CONFIG_FILE_EXT), $(ARGS_CONFIGS_TESTS_RAW_PREF))


$(TESTCONFIGRULE): # ! Focus on this
	$(call launch_only_legal_tests,$(TESTCONFIGRULE),\
		$(call launch_one_test_with_sep) ;\
		$(call launch_one_test_with_sep, Too much args) ;\
		$(call launch_test_from_array_args,$(ARGS_CONFIGS_TESTS)) ;\)

$(TESTSERVERRULE): 
	$(call launch_only_legal_tests,$(TESTSERVERRULE),\
		$(call launch_one_test_with_sep, ./config_files/simple.conf) ;\)

$(TESTNETWORKRULE):	
	$(call launch_only_legal_tests,$(TESTNETWORKRULE),\
		$(call launch_test_from_array_args,$(ARGS_CONFIGS_TESTS)) ;\)

allTests: $(TESTCONFIGRULE) $(TESTPARSERRULE) 


.PHONY: clean fclean re bonus