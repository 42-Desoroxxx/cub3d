##############################################################################################
#                                                                                            #
#   /$$$$$$                                      /$$      /$$           /$$                  #
#  /$$__  $$                                    | $$$    /$$$          | $$                  #
# | $$  \__/  /$$$$$$  /$$$$$$$$ /$$   /$$      | $$$$  /$$$$  /$$$$$$ | $$   /$$  /$$$$$$   #
# | $$       /$$__  $$|____ /$$/| $$  | $$      | $$ $$/$$ $$ |____  $$| $$  /$$/ /$$__  $$  #
# | $$      | $$  \ $$   /$$$$/ | $$  | $$      | $$  $$$| $$  /$$$$$$$| $$$$$$/ | $$$$$$$$  #
# | $$    $$| $$  | $$  /$$__/  | $$  | $$      | $$\  $ | $$ /$$__  $$| $$_  $$ | $$_____/  #
# |  $$$$$$/|  $$$$$$/ /$$$$$$$$|  $$$$$$$      | $$ \/  | $$|  $$$$$$$| $$ \  $$|  $$$$$$$  #
#  \______/  \______/ |________/ \____  $$      |__/     |__/ \_______/|__/  \__/ \_______/  #
#                                /$$  | $$                                                   #
#        )))                    |  $$$$$$/                                    Version 1.7.3  #
#       (((                      \______/                                                    #
#     +-----+                                   __..--''``---....___   _..._    __           #
#     |     |]      /    //    // //  /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /  #
#     `-----'   //  //  ///  ///     ///_.-' _..--.'_    \                    `( ) ) // //   #
#         /   / // / ///  /      /// / (_..-' // (< _     ;_..__               ; `' / ///    #
#     ///  //  /  /       // ///  /   / // // //  `-._,_)' // / ``--...____..-' /// / //     #
#    / // / /// //  /// / / // //   //  /// //  /  ///  //  // /// / /  ///   /   / ///  //  #
##############################################################################################

.PHONY: all bonus debug clean clean_self fclean re re_bonus re_debug sane re_sane help version

NAME = cub3d
MAKE_MODE ?= release

# Colors
GREEN = \033[32m
BLUE = \033[34m
RED = \033[31m
RESET = \033[0m

# Compiler
COMPILER = cc
BASE_FLAGS = \
	-Wall -Wextra \
	-std=c23 \
 	-Werror=vla -pedantic-errors -Werror=int-conversion -Werror=incompatible-pointer-types -Werror=implicit-function-declaration -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-MMD \
	-flto=thin
RELEASE_FLAGS = \
	-Werror \
	-O3 -ffast-math -march=native
DEBUG_FLAGS = \
	-gfull -O0 -fno-builtin -mno-omit-leaf-frame-pointer -fno-omit-frame-pointer -DDEBUG=true
SANE_FLAGS = \
	-fsanitize=address,pointer-compare,pointer-subtract,leak,undefined,shift,shift-exponent,shift-base,integer-divide-by-zero,unreachable,vla-bound,null,signed-integer-overflow,bounds,alignment,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,pointer-overflow,builtin -fsanitize-address-use-after-scope

# Libraries
LIB_DIRS = libft MacroLibX
LIB_FILES = libft/libft.a MacroLibX/libmlx.so
LDFLAGS = -lSDL2

# Directories
SRC = src
OBJ = obj
INCLUDES = -Iincludes $(foreach lib,$(LIB_DIRS),-isystem $(lib)/includes) $(shell sdl2-config --cflags)

# Sources
SRC_FILES := \
	cub3d.c events.c hooks.c \
	parsing/map_checker.c parsing/map_flood.c parsing/map_parser.c \
	utils/error_utils.c utils/free_utils.c utils/map_utils.c utils/parsing_utils.c
SRCS := $(addprefix $(SRC)/,$(SRC_FILES))
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

ifeq ($(MAKE_MODE),release)
	CFLAGS = $(BASE_FLAGS) $(RELEASE_FLAGS)
    LIB_TARGET = all
else
	ifeq ($(MAKE_MODE),debug)
		CFLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS)
		LIB_TARGET = debug
	else
	 ifeq ($(MAKE_MODE),sane)
	 	CFLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS) $(SANE_FLAGS)
	 	LIB_TARGET = sane
	 else
		$(error Unknown mode)
		endif
	endif
endif

COMPILER_VERSION := $(shell $(COMPILER) -v 2>&1 | grep -oP 'version \K\d+')
ifeq ($(shell expr $(COMPILER_VERSION) \>= 15 2>/dev/null), 1)
    DEBUG_FLAGS += -fstrict-flex-arrays=3
    LDFLAGS += -fuse-ld=lld
endif

all:
	@$(MAKE) -j --no-print-directory $(NAME)

bonus: all

debug:
	@echo "$(GREEN)Building in debug mode$(RESET)"
	@$(MAKE) -j --no-print-directory MAKE_MODE=debug

sane:
	@echo "$(GREEN)Building in sane mode$(RESET)"
	@$(MAKE) -j --no-print-directory MAKE_MODE=sane

$(OBJ):
	@mkdir -p $@ $(dir $(OBJS))

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ) libs
	@echo "$(BLUE)Compiling$(RESET) $<..."
	@$(COMPILER) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)

libs:
	@set -e; \
	for lib in $(LIB_DIRS); do \
		echo "$(BLUE)Building $$lib in $(MAKE_MODE) mode...$(RESET)"; \
		if [ "$(LIB_TARGET)" = "sane" ] && [ "$$lib" != "libft" ]; then \
			$(MAKE) -j --no-print-directory -C $$lib debug; \
		else \
			$(MAKE) -j --no-print-directory -C $$lib $(LIB_TARGET); \
		fi; \
	done

$(NAME): $(OBJS) | libs
	@echo "$(GREEN)Linking$(RESET) $@..."
	@$(COMPILER) $(CFLAGS) $(OBJS) $(LIB_FILES) -o $@ $(LDFLAGS)
	@echo "$(GREEN)Successfully compiled $(NAME)!$(RESET)"

clean_self:
	@echo "$(RED)Cleaning$(RESET) $(NAME)'s object files..."
	@rm -rf $(OBJ)

clean: clean_self
	@for lib in $(LIB_DIRS); do \
		$(MAKE) --no-print-directory -s -C $$lib clean; \
	done

fclean: clean_self
	@echo "$(RED)Removing$(RESET) $(NAME)..."
	@rm -f $(NAME)
	@for lib in $(LIB_DIRS); do \
		$(MAKE) --no-print-directory -s -C $$lib fclean; \
	done

re: fclean
	@$(MAKE) -j --no-print-directory all

re_bonus: fclean
	@$(MAKE) -j --no-print-directory bonus

re_debug: fclean
	@$(MAKE) -j --no-print-directory debug

re_sane: fclean
	@$(MAKE) -j --no-print-directory sane

version:
	@echo "$(RED)Cozy Make version:$(RESET) 1.7.3"
	@$(COMPILER) --version

help:
	@echo "$(GREEN)Available targets:$(RESET)"
	@echo "  $(BLUE)all       $(RESET)- Build in release mode (default)"
	@echo "  $(BLUE)bonus     $(RESET)- Build in release mode with bonus"
	@echo "  $(BLUE)debug     $(RESET)- Build with debug symbols"
	@echo "  $(BLUE)sane      $(RESET)- Build with sanitizers"
	@echo "  $(BLUE)clean     $(RESET)- Remove object files"
	@echo "  $(BLUE)fclean    $(RESET)- Remove all build artifacts"
	@echo "  $(BLUE)re        $(RESET)- Rebuild in release mode"
	@echo "  $(BLUE)re_bonus  $(RESET)- Rebuild in release mode with bonus"
	@echo "  $(BLUE)re_debug  $(RESET)- Rebuild in debug mode"
	@echo "  $(BLUE)re_sane   $(RESET)- Rebuild with sanitizers"
