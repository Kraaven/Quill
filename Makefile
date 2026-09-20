# Quill - Makefile convenience commands
#
# Wraps CMake so common tasks are one word. Requires CMake on PATH.
# (The default generator is Visual Studio Build Tools, which auto-detects MSVC.)
#
# Usage:
#   make              # or: make build  -> configure (once) + build + run
#   make run          # run the existing build, no rebuild
#   make configure    # run CMake configure only
#   make clean        # delete the Build/ folder
#   make rebuild      # clean + configure + build + run
#   make help         # show this message
#
#   Config override: make build CONFIG=Debug

SHELL := cmd.exe          # keep cmd semantics regardless of sh.exe on PATH
BUILD_DIR := Build
CONFIG ?= Release

.PHONY: all build run configure clean rebuild help

all: build

build:
	@if not exist "$(BUILD_DIR)\CMakeCache.txt" cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR) --config $(CONFIG) --target build-and-run

run:
	@cmake --build $(BUILD_DIR) --config $(CONFIG) --target run

configure:
	@cmake -S . -B $(BUILD_DIR)

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@echo [make] Build folder removed.

rebuild: clean build

help:
	@echo Quill - make targets:
	@echo   make [build]    configure + build + run
	@echo   make run        run the existing build
	@echo   make configure  CMake configure only
	@echo   make clean      delete the Build/ folder
	@echo   make rebuild    clean + build + run
	@echo   CONFIG=Debug    switch build configuration