#include "receiver.h"

#include <iostream>
#include <string>
#include <vector>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/liberror.h"
#include "common/lobby.h"
#include "common/snapshot.h"
#include "common/socket.h"

ServerReceiver::ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor,
                               Queue<Snapshot>& sender_q, int playerId,
                               std::atomic<bool>& is_alive):
        protocol(protocol),
        gameloop_q(nullptr),
        games_monitor(games_monitor),
        playerId(playerId),
        gameId(INVALID_GAME_ID),
        duck_id(-1),
        sender_q(sender_q),
        is_alive(is_alive),
        sender(protocol, sender_q, playerId, is_alive) {}

// Me quedo trabado en recibir_msg (hasta tener algo) y lo mando a queue de gameloop
void ServerReceiver::run() {
    try {
        setup_game();
    } catch (const SocketWasClosed& se) {
        is_alive = false;
        std::cout << "Client disconnected when setuping game" << std::endl;
        return;
    } catch (const LibError& le) {
        std::cout << "LibError when setuping game" << std::endl;
        is_alive = false;
        return;
    }

    sender.start();

    while (_keep_running && is_alive) {
        action action;
        try {
            action = protocol.recv_player_action();
        } catch (const LibError& le) {  // Catchear excepcion de socket cerrado
            std::cout << "LibError en receiver player id: " << playerId << " " << le.what()
                      << std::endl;
            break;
        } catch (const SocketWasClosed& e) {
            // TODO: Ver donde va ubicado esto (remove_player)
            if (is_alive) {
                games_monitor.remove_player(gameId, playerId);
            }
            std::cout << "Client dissconected" << std::endl;
            break;
        }

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
            int32_t num_players = protocol.receive_cmd();
            std::vector<std::string> players;
            players.push_back(protocol.recv_string());
            if (num_players == 2) {
                players.push_back(protocol.recv_string());
            }
            GameInfo game_info = games_monitor.player_create_game(playerId, sender_q, players);
            protocol.send_game_info(game_info);
            if (game_info.game_id == INVALID_GAME_ID) {
                continue;
            }
            gameId = game_info.game_id;

            while (true) {
                int32_t cmd = protocol.receive_cmd();
                if (cmd == START_GAME) {
                    break;
                } else if (cmd == GET_INFO) {
                    std::vector<LobbyInfo> lobbies(1);
                    lobbies[0] = games_monitor.get_lobby_info(gameId);
                    protocol.send_lobbies_info(lobbies);
                }
            }

            games_monitor.start_game(gameId);
            break;
        } else if (cmd == LIST_GAMES) {
            std::vector<LobbyInfo> lobbies = games_monitor.list_lobbies();
            protocol.send_lobbies_info(lobbies);
            continue;
        } else if (cmd == JOIN_GAME) {
            gameId = protocol.receive_cmd();
            int32_t num_players = protocol.receive_cmd();
            std::vector<std::string> players;
            players.push_back(protocol.recv_string());
            if (num_players == 2) {
                players.push_back(protocol.recv_string());
            }
            GameInfo game_info =
                    games_monitor.player_join_game(playerId, gameId, sender_q, players);
            protocol.send_game_info(game_info);
            // si no se pudo unir, game_id es INVALID_GAME_ID
            if (game_info.game_id != INVALID_GAME_ID) {
                break;
            }
        }
    }

    gameloop_q = games_monitor.get_gameloop_q(gameId);
}

void ServerReceiver::join_sender() {
    is_alive = false;
    sender.join();
}

ServerReceiver::~ServerReceiver() {}
