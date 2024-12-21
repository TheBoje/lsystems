# Directories
OUT_DIR=build
SRC_DIR=sources
INC_DIR=includes

# Compilation setup
C=clang
CXX=clang++
CXXFLAGS=-g -O0 -std=c++20 -Wpedantic -Wall -Wextra -I/usr/include/GLFW -I${INC_DIR} -I${OUT_DIR} -I${SRC_DIR} -I${INC_DIR}/ast -I${INC_DIR}/semantic_analysis -I${INC_DIR}/imgui -I${INC_DIR}/imgui/backends -DGLFW_USE_WAYLAND=0 -DVK_PROTOTYPES
CXXLINKING=-lfl -lm -lglfw -lvulkan -lGLEW -ldl -lpthread
BISONFLAGS=-t -d --defines=${OUT_DIR}/parser.tab.h -Wconflicts-rr -Wcounterexamples

# Files
SRC=$(wildcard ${SRC_DIR}/*.cpp)
SRC += $(wildcard ${SRC_DIR}/**/*.cpp)
SRC += $(wildcard ${SRC_DIR}/**/**/*.cpp)

# Object files will retain the directory structure under the OUT_DIR
OBJ=$(patsubst ${SRC_DIR}/%.cpp,${OUT_DIR}/%.o,${SRC})

# ANSI escape codes for pretty output
COLOR_RESET=\033[0m
COLOR_BOLD=\033[1m
COLOR_CLEAN=\033[1;31m
COLOR_LINK =\033[1;32m
COLOR_MKDIR=\033[1;33m
COLOR_CXX  =\033[1;34m
COLOR_FLEX =\033[1;35m
COLOR_BISON=\033[1;36m

default: lsystems

lsystems: ${OUT_DIR}/lsystems

run: lsystems examples/default.lsy
	${OUT_DIR}/lsystems examples/default.lsy

${OUT_DIR}/lsystems: ${OBJ} ${OUT_DIR}/lex.yy.cpp ${OUT_DIR}/parser.tab.o
	@echo -e "${COLOR_LINK}[CLANG]${COLOR_RESET} Linking executable ${COLOR_LINK}${COLOR_BOLD}$@${COLOR_RESET}"
	@${CXX} ${CXXFLAGS} ${CXXLINKING} -o $@ ${OUT_DIR}/lex.yy.cpp ${OUT_DIR}/parser.tab.o ${OBJ}
	@echo -e "${COLOR_LINK}[OTHER]${COLOR_RESET} Building ${COLOR_LINK}${COLOR_BOLD}$@${COLOR_RESET} success!"

${OUT_DIR}/%.o: ${SRC_DIR}/%.cpp ${OUT_DIR}/lex.yy.cpp
	@echo -e "${COLOR_CXX}[CLANG]${COLOR_RESET} Compiling ${COLOR_CXX}${COLOR_BOLD}$<${COLOR_RESET} (${COLOR_BOLD}${OUT_DIR}/$(@F)${COLOR_RESET})"
	@mkdir -p $(@D)  # Ensure subdirectories are created
	@${CXX} ${CXXFLAGS} -o $@ $< -c

${OUT_DIR}/lex.yy.cpp: ${SRC_DIR}/lexer.ll ${OUT_DIR}/parser.tab.h
	@echo -e "${COLOR_FLEX}[FLEX ]${COLOR_RESET} Generating ${COLOR_FLEX}${COLOR_BOLD}$@${COLOR_RESET} from ${COLOR_BOLD}$<${COLOR_RESET}"
	@flex -o $@ $<

${OUT_DIR}/parser.tab.cpp ${OUT_DIR}/parser.tab.h: ${SRC_DIR}/parser.yy
	@echo -e "${COLOR_MKDIR}[OTHER]${COLOR_RESET} Creating directory ${COLOR_MKDIR}${OUT_DIR}${COLOR_RESET}"
	@mkdir -p ${OUT_DIR}
	@echo -e "${COLOR_BISON}[BISON]${COLOR_RESET} Generating ${COLOR_BISON}${COLOR_BOLD}$@${COLOR_RESET} from ${COLOR_BOLD}$<${COLOR_RESET}"
	@bison ${BISONFLAGS} $< -o ${OUT_DIR}/parser.tab.cpp

${OUT_DIR}/parser.tab.o: ${OUT_DIR}/parser.tab.cpp
	@echo -e "${COLOR_CXX}[CLANG]${COLOR_RESET} Compiling ${COLOR_CXX}${COLOR_BOLD}$<${COLOR_RESET} (${COLOR_BOLD}${OUT_DIR}/parser.tab.o${COLOR_RESET})"
	@${CXX} ${CXXFLAGS} -o $@ $< -c

clean:
	@echo -e "${COLOR_CLEAN}[OTHER]${COLOR_RESET} Cleaning up ${COLOR_CLEAN}${COLOR_BOLD}${OUT_DIR}${COLOR_RESET}"
	@rm -fr ${OUT_DIR}

.PHONY: clean
