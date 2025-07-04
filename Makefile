# Use bash instead of default /bin/sh.
SHELL := /bin/bash
# Make bash fail and report the 1st error in ;-separated lists or pipes.
.SHELLFLAGS := -e -o pipefail -c 

# Directories
OUT_DIR=build
SRC_DIR=sources
INC_DIR=includes
INC_DIRS = $(addprefix -I, $(sort $(dir $(wildcard includes/*))))
INC_DIRS+= $(addprefix -I, $(sort $(dir $(wildcard includes/**/*))))
INC_DIRS+= $(addprefix -I, $(sort $(dir $(wildcard includes/**/**/*))))
INC_DIRS+= $(addprefix -I, $(sort $(dir $(wildcard includes/**/**/**/*))))
SRC_DIR_SHADERS=renderer/shaders
OUT_DIR_SHADERS=${OUT_DIR}/shaders

# Compilation setup
C=clang
CXX=clang++
CXXFLAGS=-std=c++20 -Wpedantic -Wall -Wextra -flto -I/usr/include/GLFW -I/usr/include/freetype2 -I${INC_DIR} -I${OUT_DIR} -I${SRC_DIR} ${INC_DIRS} -DGLFW_USE_WAYLAND=0 -DVK_PROTOTYPES
CXXLINKING=-lfl -lm -lglfw -lvulkan -lGLEW -ldl -lpthread -lfreetype
BISONFLAGS=-t -d --defines=${OUT_DIR}/parser.tab.h -Wconflicts-rr -Wcounterexamples

ifeq ($(MAKECMDGOALS), profile)
  CXXFLAGS   += -fprofile-arcs -ftest-coverage -g -fprofile-instr-generate -fcoverage-mapping
  CXXFLAGS   += -DTEST
  CXXLINKING += -lgcov
endif

ifeq ($(MAKECMDGOALS), test)
  CXXFLAGS +=-O3 -DTEST
endif

ifeq ($(MAKECMDGOALS), release)
  CXXFLAGS +=-O3 
endif

ifeq ($(MAKECMDGOALS), debug)
  CXXFLAGS +=-DDEBUG -g -O0
endif

# Files
SRC  = $(wildcard ${SRC_DIR}/*.cpp)
SRC += $(wildcard ${SRC_DIR}/**/*.cpp)
SRC += $(wildcard ${SRC_DIR}/**/**/*.cpp)
SRC += $(wildcard ${SRC_DIR}/**/**/**/*.cpp)
OBJ  = $(patsubst ${SRC_DIR}/%.cpp,${OUT_DIR}/%.o,${SRC})

# Shaders
SRC_SHADER = $(wildcard ${SRC_DIR_SHADERS}/*)
OBJ_SHADER = $(patsubst ${SRC_DIR_SHADERS}/%, ${OUT_DIR_SHADERS}/%,$(addsuffix .spv,${SRC_SHADER}))

# ANSI escape codes for pretty output
COLOR_RESET=\033[0m
COLOR_BOLD =\033[1m
COLOR_CLEAN=\033[1;31m
COLOR_LINK =\033[1;32m
COLOR_MKDIR=\033[1;33m
COLOR_CXX  =\033[1;34m
COLOR_FLEX =\033[1;35m
COLOR_BISON=\033[1;36m

release: lsystems
debug: lsystems

test: lsystems tests/run_tests.py
	@pytest tests/run_tests.py

profile: lsystems tests/run_tests.py
	@LLVM_PROFILE_FILE="build/lsystems.profraw" pytest tests/run_tests.py
	@llvm-profdata merge -output=build/lsystems.profdata build/lsystems.profraw
	@llvm-cov show build/lsystems -instr-profile=build/lsystems.profdata -format=html -output-dir=build/coverage_report
	@llvm-cov report build/lsystems -instr-profile=build/lsystems.profdata

lsystems: ${OUT_DIR}/lsystems ${OBJ_SHADER}

run: lsystems examples/default.lsy
	@${OUT_DIR}/lsystems examples/default.lsy

${OUT_DIR}/lsystems: ${OBJ} ${OUT_SHADERS} ${OUT_DIR}/lex.yy.cpp ${OUT_DIR}/parser.tab.o 
	@echo -e "${COLOR_LINK}[CLANG]${COLOR_RESET} Linking executable ${COLOR_LINK}${COLOR_BOLD}$(@F)${COLOR_RESET}"
	@${CXX} ${CXXFLAGS} ${CXXLINKING} -o $@ $^
	@echo -e "${COLOR_LINK}[OTHER]${COLOR_RESET} Building ${COLOR_LINK}${COLOR_BOLD}$(@F)${COLOR_RESET} success!"

${OUT_DIR}/%.o: ${SRC_DIR}/%.cpp ${OUT_DIR}/lex.yy.cpp
	@echo -e "${COLOR_CXX}[CLANG]${COLOR_RESET} Compiling ${COLOR_CXX}${COLOR_BOLD}$(@F)${COLOR_RESET} ($(notdir $<))"
	@mkdir -p $(@D)
	@${CXX} ${CXXFLAGS} -o $@ $< -c

${OUT_DIR}/lex.yy.cpp: ${SRC_DIR}/lexer.ll ${OUT_DIR}/parser.tab.h
	@echo -e "${COLOR_FLEX}[FLEX ]${COLOR_RESET} Generating ${COLOR_FLEX}${COLOR_BOLD}$(@F)${COLOR_RESET} ($(notdir $<))"
	@flex -o $@ $<

${OUT_DIR}/parser.tab.cpp ${OUT_DIR}/parser.tab.h: ${SRC_DIR}/parser.yy
	@echo -e "${COLOR_MKDIR}[OTHER]${COLOR_RESET} Creating directory ${COLOR_MKDIR}${OUT_DIR}${COLOR_RESET}"
	@mkdir -p ${OUT_DIR}
	@echo -e "${COLOR_BISON}[BISON]${COLOR_RESET} Generating ${COLOR_BISON}${COLOR_BOLD}$(@F)${COLOR_RESET} ($(notdir $<))"
	@bison ${BISONFLAGS} $< -o ${OUT_DIR}/parser.tab.cpp

${OUT_DIR}/parser.tab.o: ${OUT_DIR}/parser.tab.cpp
	@echo -e "${COLOR_CXX}[CLANG]${COLOR_RESET} Compiling ${COLOR_CXX}${COLOR_BOLD}$(@F)${COLOR_RESET} ($(notdir $<))"
	@${CXX} ${CXXFLAGS} -o $@ $< -c

${OUT_DIR_SHADERS}/%.spv: ${SRC_DIR_SHADERS}/%
	@echo -e "${COLOR_CXX}[GLSLC]${COLOR_RESET} Compiling shader ${COLOR_CXX}${COLOR_BOLD}$(@F)${COLOR_RESET} ($(notdir $<))"
	@mkdir -p $(@D)
	@glslc $< -o $@

clean:
	@echo -e "${COLOR_CLEAN}[OTHER]${COLOR_RESET} Cleaning up ${COLOR_CLEAN}${COLOR_BOLD}${OUT_DIR}${COLOR_RESET}"
	@rm -fr ${OUT_DIR}/*

.PHONY: clean
