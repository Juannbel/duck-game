#include "receiver.h"

#include <iostream>
#include <vector>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/liberror.h"
#include "common/snapshot.h"
#include "common/socket.h"

#include "action.h"

#define CREATE 1
#define JOIN 2

ServerReceiver::ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor,
                               Queue<Snapshot>& sender_q, int playerId):
        protocol(protocol),
        games_monitor(games_monitor),
        playerId(playerId),
        duck_id(-1),
        sender_q(sender_q),
        sender(protocol, sender_q, playerId) {}

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

        struct action action;
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
}

// Protocolo de inicio de juego
void ServerReceiver::setup_game() {
    int cmd = protocol.receive_cmd();
    if (cmd == CREATE) {
        gameId = games_monitor.player_create_game(playerId, sender_q, std::ref(duck_id));
        // Espero un input para iniciar el juego
        protocol.receive_cmd();
        games_monitor.start_game(gameId);
        //} else if (cmd == JOIN) {
    } else {  // Para evitar el warning
        std::vector<int> lobbies = games_monitor.list_lobbies();
        for (int lobby: lobbies) {
            protocol.send_lobby_info(lobby);
        }
        if (lobbies.size() == 1) {
            // volver a ejecutar todo (asi me manejo desde cliente), llamado recursivo
            setup_game();
            return;  // Cuando un jugador listaba partidas y no habia, rompia porque se desapilaban
                     // las llamadas y seguian
        }
        gameId = protocol.receive_cmd();
        duck_id = games_monitor.player_join_game(playerId, gameId, sender_q);
    }
    gameloop_q = games_monitor.get_gameloop_q(gameId);
    sender.send_duck_id(duck_id);
}

ServerReceiver::~ServerReceiver() { sender.join(); }
