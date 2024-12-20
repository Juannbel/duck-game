#include "receiver.h"

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
        sender(protocol, sender_q, playerId, is_alive),
        in_lobby(true) {}

void ServerReceiver::run() {
    try {
        setup_game();
        in_lobby = false;
        sender.start();
    } catch (const SocketWasClosed& se) {
        is_alive = false;
    } catch (const LibError& le) {
        is_alive = false;
    }


    while (_keep_running && is_alive) {
        action action;
        try {
            action = protocol.recv_player_action();
        } catch (const LibError& le) {
            break;
        } catch (const SocketWasClosed& e) {
            break;
        }

        try {
            gameloop_q->push(action);
        } catch (const ClosedQueue& e) {
            break;
        }
    }
    is_alive = false;
    games_monitor.remove_player(gameId, playerId);
}

void ServerReceiver::handle_list_games() const {
    std::vector<LobbyInfo> lobbies = games_monitor.list_lobbies();
    protocol.send_lobbies_info(lobbies);
}

GameInfo ServerReceiver::handle_join_game() {
    gameId = protocol.receive_cmd();
    const int32_t num_players = protocol.receive_cmd();
    std::vector<std::string> players;
    players.push_back(protocol.recv_string());
    if (num_players == 2) {
        players.push_back(protocol.recv_string());
    }
    const GameInfo game_info = games_monitor.player_join_game(playerId, gameId, sender_q, players);
    protocol.send_game_info(game_info);
    return game_info;
}

void ServerReceiver::wait_for_start() const {
    while (true) {
        int32_t cmd = protocol.receive_cmd();
        if (cmd == START_GAME) {
            LobbyInfo info(games_monitor.get_lobby_info(gameId));
            if (info.connected_players >= 2) {
                protocol.send_option(CREATE_OK);
                break;
            }
            protocol.send_option(CREATE_ERROR);
        } else if (cmd == GET_INFO) {
            std::vector<LobbyInfo> lobbies(1);
            lobbies[0] = games_monitor.get_lobby_info(gameId);
            protocol.send_lobbies_info(lobbies);
        }
    }
}

void ServerReceiver::handle_create_game() {
    int32_t num_players = protocol.receive_cmd();
    std::vector<std::string> players;
    players.push_back(protocol.recv_string());
    if (num_players == 2) {
        players.push_back(protocol.recv_string());
    }
    GameInfo game_info = games_monitor.player_create_game(playerId, sender_q, players);
    protocol.send_game_info(game_info);
    if (game_info.game_id == INVALID_GAME_ID) {
        setup_game();
    }
    gameId = game_info.game_id;

    wait_for_start();

    games_monitor.start_game(gameId);
}


// Protocolo de inicio de juego
void ServerReceiver::setup_game() {
    while (true) {
        int32_t cmd = protocol.receive_cmd();
        if (cmd == CREATE_GAME) {
            handle_create_game();
            break;
        } else if (cmd == LIST_GAMES) {
            handle_list_games();
        } else if (cmd == JOIN_GAME) {
            GameInfo game_info = handle_join_game();
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
    if (!in_lobby) {
        sender.join();
    }
}

ServerReceiver::~ServerReceiver() {}
