.PHONY: all test clean editor client common server build

NAME := duck_game

BIN_DIR := /usr/bin
CONFIG_DIR_INSTALL := /etc/$(NAME)
DATA_DIR_INSTALL := /var/$(NAME)
MAPS_DIR_INSTALL := $(or $(XDG_DATA_HOME)/$(NAME),$(HOME)/.local/share/$(NAME))
WHOAMI := $(shell whoami)

CMAKE_OPTIONS :=

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

compile-release:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Release $(CMAKE_OPTIONS) $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

dependencies:
	@echo "Installing dependencies"
	@sudo apt-get update
	@echo "Installing build dependencies"
	@sudo apt-get install -y cmake g++
	@echo "Installing SDL2 dependencies"
	@sudo apt-get install -y libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev libsdl2-mixer-2.0-0 libsdl2-mixer-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev
	@echo "Setting up SDL2 cmake config files"
	@if [ ! -e "/usr/lib/x86_64-linux-gnu/cmake/SDL2_image" ]; then \
	    sudo mkdir -p /usr/lib/x86_64-linux-gnu/cmake/SDL2_image; \
	    sudo cp ./cmake/sdl2/sdl2_image-config.cmake /usr/lib/x86_64-linux-gnu/cmake/SDL2_image/; \
	fi
	@if [ ! -e "/usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer" ]; then \
	    sudo mkdir -p /usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer; \
	    sudo cp ./cmake/sdl2/sdl2_mixer-config.cmake /usr/lib/x86_64-linux-gnu/cmake/SDL2_mixer/; \
	fi
	@if [ ! -e "/usr/lib/x86_64-linux-gnu/cmake/SDL2_ttf" ]; then \
	    sudo mkdir -p /usr/lib/x86_64-linux-gnu/cmake/SDL2_ttf; \
	    sudo cp ./cmake/sdl2/sdl2_ttf-config.cmake /usr/lib/x86_64-linux-gnu/cmake/SDL2_ttf/; \
	fi
	@echo "Installing QT dependencies"
	@sudo apt-get install -y qtbase5-dev
	@echo "Installing yaml-cpp dependencies"
	@sudo apt-get install -y libyaml-cpp-dev

install-no-deps: CMAKE_OPTIONS := -DCUSTOM_CONFIG_PATH=$(CONFIG_DIR_INSTALL)/config.yaml -DCUSTOM_DATA_PATH=$(DATA_DIR_INSTALL)/data -DCUSTOM_MAPS_PATH=$(MAPS_DIR_INSTALL)/maps
install-no-deps: compile-release run-tests
	@echo "Copying binaries to $(BIN_DIR)/$(NAME)_client, $(BIN_DIR)/$(NAME)_server, $(BIN_DIR)/$(NAME)_editor"
	@sudo mkdir -p $(CONFIG_DIR_INSTALL) $(DATA_DIR_INSTALL) $(MAPS_DIR_INSTALL)

	@sudo cp ./build/$(NAME)_client $(BIN_DIR)/$(NAME)_client
	@sudo cp ./build/$(NAME)_server $(BIN_DIR)/$(NAME)_server
	@sudo cp ./build/$(NAME)_editor $(BIN_DIR)/$(NAME)_editor

	@echo "Copying data to $(DATA_DIR_INSTALL) and maps to $(MAPS_DIR_INSTALL)"
	sudo cp -r ./client/data $(DATA_DIR_INSTALL)
	sudo cp -r ./server/maps $(MAPS_DIR_INSTALL)
	@echo "Setting permissions for $(WHOAMI) on $(MAPS_DIR_INSTALL)"
	sudo chown -R $(WHOAMI):$(WHOAMI) $(MAPS_DIR_INSTALL)
	sudo chmod -R u+rw $(MAPS_DIR_INSTALL)

	@echo "Copying config file to $(CONFIG_DIR_INSTALL)"

	@sudo cp ./config.yaml $(CONFIG_DIR_INSTALL)

install: dependencies install-no-deps

uninstall:
	@echo "Removing $(NAME)_client, $(NAME)_server and $(NAME)_editor binaries from $(BIN_DIR)"
	@sudo rm -f $(BIN_DIR)/$(NAME)_client
	@sudo rm -f $(BIN_DIR)/$(NAME)_server
	@sudo rm -f $(BIN_DIR)/$(NAME)_editor
	@echo "Removing config file from $(CONFIG_DIR_INSTALL)"
	@sudo rm -rf $(CONFIG_DIR_INSTALL)
	@echo "Removing data from $(DATA_DIR_INSTALL) and maps from $(MAPS_DIR_INSTALL)"
	@sudo rm -rf $(DATA_DIR_INSTALL)
	@sudo rm -rf $(MAPS_DIR_INSTALL)

run-tests:
	./build/$(NAME)_server_tests &
	./build/$(NAME)_client_tests

all: clean install

clean:
	rm -Rf ./build
