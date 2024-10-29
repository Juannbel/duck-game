#include "receiver.h"
#include <iostream>
#include "common/blocking_queue.h"
#include "common/commands.h"
#include "common/liberror.h"

#include "action.h"

#define CREATE 1
#define JOIN 2

ServerReceiver::ServerReceiver(ServerProtocol& protocol, GamesMonitor& games_monitor, Queue<Snapshot>& sender_q, uint8_t duck_id):
        protocol(protocol),
        games_monitor(games_monitor),
        duck_id(duck_id),
        sender_q(sender_q),
        sender(protocol, sender_q, duck_id) {}

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
        } catch (const LibError& le) { // Catchear excepcion de socket cerrado
            std::cout << "LibError en receiver id: " << (int) duck_id << " " << le.what() << std::endl;
        }

        struct action action;
        action.duck_id = duck_id;  // Agregar el n de pato
        action.command = cmd;

        try {
            gameloop_q->push(action);
        } catch (const ClosedQueue& e) {
            std::cout << "ClosedQueue en receiver id: " << (int) duck_id << " " << e.what() << std::endl;
        }
    }
}

// Protocolo de inicio de juego
void ServerReceiver::setup_game() {
    int gameId;
    int cmd = protocol.receive_cmd();
    if (cmd == CREATE) {
        gameId = games_monitor.player_create_game(duck_id, sender_q);
        //Espero un input para iniciar el juego
        protocol.receive_cmd();
        games_monitor.start_game(gameId);
    } else if (cmd == JOIN) {
        std::vector<int> lobbies = games_monitor.list_lobbies();
        for (int lobby : lobbies) {
            protocol.send_lobby_info(lobby);
        }
        if (lobbies.size() == 1) {
            // volver a ejecutar todo (asi me manejo desde cliente), llamado recursivo
            setup_game();
        }
        gameId = protocol.receive_cmd();
        games_monitor.player_join_game(duck_id, gameId, sender_q);
    }
    gameloop_q = games_monitor.get_gameloop_q(gameId);
    MatchInfo match_info = games_monitor.get_match_info(gameId);
    // TODO: Ver si modificando el protocolo evito hacer el match info
    match_info.duck_id = duck_id;
    sender.send_match_info(match_info);
}

ServerReceiver::~ServerReceiver() {
    sender.join();
}


