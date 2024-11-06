#include "receiver.h"

#include <iostream>
#include <vector>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/liberror.h"
#include "common/lobby.h"
#include "common/snapshot.h"
#include "common/socket.h"

#include "action.h"

#define CREATE 1
#define JOIN 2

ServerReceiver::ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor,
                               Queue<Snapshot>& sender_q, int playerId,
                               std::atomic<bool>& is_alive):
        protocol(protocol),
        games_monitor(games_monitor),
        playerId(playerId),
        duck_id(-1),
        sender_q(sender_q),
        is_alive(is_alive),
        sender(protocol, sender_q, playerId, is_alive) {}

// Me quedo trabado en recibir_msg (hasta tener algo) y lo mando a queue de gameloop
void ServerReceiver::run() {
    // bool was_closed = false; // comento hasta que se use por cppcheck
    // Snapshot msg;

    setup_game();

    // Ya tengo todo, lanzo thread sender
    sender.start();

    while (_keep_running) {
        Command cmd;
        try {
            cmd = protocol.recv_player_command();
        } catch (const LibError& le) {  // Catchear excepcion de socket cerrado
            std::cout << "LibError en receiver player id: " << playerId << " " << le.what()
                      << std::endl;
            break;
        } catch (const SocketWasClosed& e) {
            // TODO: Ver donde va ubicado esto (remove_player)
            games_monitor.remove_player(gameId, playerId);
            std::cout << "Client dissconected" << std::endl;
            break;
        }

        action action;
        action.duck_id = duck_id;  // Agregar el n de pato
        action.command = cmd;

        try {
            gameloop_q->push(action);
        } catch (const ClosedQueue& e) {
            std::cout << "ClosedQueue en receiver player id: " << playerId << " " << e.what()
                      << std::endl;
            break;
        }
    }
    is_alive = false;
}

// Protocolo de inicio de juego
void ServerReceiver::setup_game() {
    while (true) {
        int32_t cmd = protocol.receive_cmd();
        if (cmd == CREATE_GAME) {
            gameId = games_monitor.player_create_game(playerId, sender_q, std::ref(duck_id));
            sender.send_game_info(gameId, duck_id);
            // esperamos comando para iniciar juego
            protocol.receive_cmd();
            games_monitor.start_game(gameId);
            break;
        } else if (cmd == LIST_GAMES) {
            std::vector<int32_t> lobbies = games_monitor.list_lobbies();
            protocol.send_lobbies_info(lobbies);
            continue;
        } else if (cmd == JOIN_GAME) {
            gameId = protocol.receive_cmd();
            duck_id = games_monitor.player_join_game(playerId, gameId, sender_q);
            if (duck_id == INVALID_DUCK_ID) {
                sender.send_game_info(INVALID_GAME_ID, duck_id);
                continue;
            }

            sender.send_game_info(gameId, duck_id);
            break;
        }
    }

    gameloop_q = games_monitor.get_gameloop_q(gameId);
}

ServerReceiver::~ServerReceiver() {
    is_alive = false;
    sender.join();
}
