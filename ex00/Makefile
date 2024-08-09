################################################################################
##                                   COLORS                                   ##
################################################################################

DEFAULT     := \033[0;39m
GRAY        := \033[0;90m
RED         := \033[0;91m
GREEN       := \033[0;92m
YELLOW      := \033[0;93m
BLUE        := \033[0;94m
MAGENTA     := \033[0;95m
CYAN        := \033[0;96m
WHITE       := \033[0;97m
	
################################################################################
##                                    BTC                                     ##
################################################################################

NAME							:= btc
CXX								:=
RM								:=
	
################################################################################
##                                DIRECTORIES                                 ##
################################################################################

OBJ_DIR						:=
SRC_DIR						:=
INC_DIR						:=

vpath
vpath
vpath

HEADERS						:=
SOURCE						:=
OBJECTS						:=
	
################################################################################
##                                   FLAGS                                    ##
################################################################################

CXXFLAGS					:=
INCLUDE						:=
	
################################################################################
##                                PROGRESS_BAR                                ##
################################################################################

NUM_SRC_FILES			:=
NUM_OBJ_FILES			:=
NUM_TO_COMPILE		=

ifeq ()
	NUM_TO_COMPILE	=
endif

COMPILED_FILES		=
COMPILATION_PCT		=
	
################################################################################
##                                COMPILATION                                 ##
################################################################################

all:

$(NAME):
	@printf "\n$(MAGENTA)[$(NAME)] $(DEFAULT)Linking "
	@printf "($(BLUE)$(NAME)$(DEFAULT))..."
	@
	@printf "\r%100s\r$(MAGENTA)[$(NAME)] $(GREEN)Compilation success "
	@printf "🎉!$(DEFAULT)\n"

$(OBJ_DIR)/%.o:
	@$(eval COMPILED_FILES = $(shell expr $(COMPILED_FILES) + 1))
	@printf "$(MAGENTA)\r%100s\r[$(NAME)] $(GREEN)[ %d/%d (%d%%) ]" \
			"" $(COMPILED_FILES) $(NUM_TO_COMPILE) $(COMPILATION_PCT) 
	@printf " $(DEFAULT)Compiling ($(BLUE)$<$(DEFAULT))..."
	@

$(OBJ_DIR):
	@printf "$(MAGENTA)[$(NAME)] $(DEFAULT)Creating objects directory "
	@printf "($(BLUE)$(OBJ_DIR)$(DEFAULT))..."
	@	
	@printf "\r%100s\r$(MAGENTA)[$(NAME)] $(DEFAULT)($(BLUE)$(OBJ_DIR)/$(DEFAULT)) "
	@printf "Created successfully!\n"

clean:
	@printf "$(MAGENTA)[$(NAME)] $(DEFAULT)Cleaning up objects files in "
	@printf "($(RED)$(OBJ_DIR)$(DEFAULT))..."
	@$(RM) $(OBJ_DIR)
	@printf "\r%100s\r$(MAGENTA)[$(NAME)] $(YELLOW)Objects clean success "
	@printf "🧹🧹$(DEFAULT)\n"

fclean: clean
	@printf "$(MAGENTA)[$(NAME)] $(DEFAULT)Cleaning up "
	@printf "($(RED)$(NAME)$(DEFAULT))..."
	@$(RM) $(NAME)
	@printf "\r%100s\r$(MAGENTA)[$(NAME)] $(YELLOW)Full clean success "
	@printf "🧹🧹$(DEFAULT)\n"

re: fclean all

.PHONY: all clean fclean re
