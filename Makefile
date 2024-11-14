.PHONY: all test clean editor client common server build

NAME := duck_game

BIN_DIR := /usr/bin
CONFIG_DIR_INSTALL := /etc/$(NAME)
ASSETS_DIR_INSTALL := /var/$(NAME)

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
	sudo apt-get update
	sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev

install-no-deps: CMAKE_OPTIONS := -DCUSTOM_CONFIG_PATH=$(CONFIG_DIR_INSTALL)/config.yaml -DCUSTOM_DATA_PATH=$(ASSETS_DIR_INSTALL)/data -DCUSTOM_SERVER_DATA_PATH=$(ASSETS_DIR_INSTALL)/maps
install-no-deps: compile-release
	@echo "Copying binaries to $(BIN_DIR)/$(NAME)_client, $(BIN_DIR)/$(NAME)_server, $(BIN_DIR)/$(NAME)_editor"
	@sudo mkdir -p $(CONFIG_DIR_INSTALL) $(ASSETS_DIR_INSTALL)

	@sudo cp ./build/taller_client $(BIN_DIR)/$(NAME)_client
	@sudo cp ./build/taller_server $(BIN_DIR)/$(NAME)_server
	@sudo cp ./build/taller_editor $(BIN_DIR)/$(NAME)_editor

	@echo "Copying assets to $(ASSETS_DIR_INSTALL)"
	@sudo cp -r ./client/data $(ASSETS_DIR_INSTALL)
	@sudo cp -r ./server/maps $(ASSETS_DIR_INSTALL)

	@echo "Copying config file to $(CONFIG_DIR_INSTALL)"

	@sudo cp ./config.yaml $(CONFIG_DIR_INSTALL)

install: dependencies install-no-deps

uninstall:
	@echo "Removing binaries from $(BIN_DIR)/$(NAME)_client and $(BIN_DIR)/$(NAME)_server, config from $(CONFIG_DIR_INSTALL) and assets from $(ASSETS_DIR_INSTALL)"
	@sudo rm -f $(BIN_DIR)/$(NAME)_client
	@sudo rm -f $(BIN_DIR)/$(NAME)_server
	@sudo rm -f $(BIN_DIR)/$(NAME)_editor
	@sudo rm -rf $(CONFIG_DIR_INSTALL)
	@sudo rm -rf $(ASSETS_DIR_INSTALL)

run-tests: compile-debug
	./build/taller_server_tests &
	./build/taller_client_tests

all: clean run-tests

clean:
	rm -Rf build-*/
