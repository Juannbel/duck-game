#include "receiver.h"

#include <iostream>
#include <stdexcept>

#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/liberror.h"
#include "common/snapshot.h"

#include "action.h"

#define CREATE 1
#define JOIN 2

ServerReceiver::ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor, Queue<Snapshot>& sender_q, int playerId):
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
        } catch (const std::runtime_error& e) {
            // was_closed = true; hago que protocolo largue runtime error,  queda hasta definir otra exc especifico
            // sino, queda loopeando ad infinitum porque manda el comando sin inicializar
            // el jugador se desconecto
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
    int gameId;
    MatchInfo match_info;
    int cmd = protocol.receive_cmd();
    if (cmd == CREATE) {
        gameId = games_monitor.player_create_game(playerId, sender_q);
        //Espero un input para iniciar el juego
        match_info = games_monitor.get_match_info(gameId);
        duck_id = match_info.duck_id;
        protocol.receive_cmd();
        games_monitor.start_game(gameId);
    //} else if (cmd == JOIN) {
    } else { // Para evitar el warning
        std::vector<int> lobbies = games_monitor.list_lobbies();
        for (int lobby : lobbies) {
            protocol.send_lobby_info(lobby);
        }
        if (lobbies.size() == 1) {
            // volver a ejecutar todo (asi me manejo desde cliente), llamado recursivo
            setup_game();
            return; // Cuando un jugador listaba partidas y no habia, rompia porque se desapilaban las llamadas y seguian
        }
        gameId = protocol.receive_cmd();
        games_monitor.player_join_game(playerId, gameId, sender_q);
        match_info = games_monitor.get_match_info(gameId);
        duck_id = match_info.duck_id;
    }
    gameloop_q = games_monitor.get_gameloop_q(gameId);
    // TODO: Ver si modificando el protocolo evito hacer el match info
    sender.send_match_info(match_info);
}

ServerReceiver::~ServerReceiver() {
    sender.join();
}
