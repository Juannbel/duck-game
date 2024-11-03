#ifndef GAMES_MONITOR_H
#define GAMES_MONITOR_H

#include <cstdint>
#include <map>
#include <mutex>
#include <vector>

#include "game.h"

class Game;

class GamesMonitor {
private:
    std::mutex m;
    std::map<int, Game*> map_games;
    int id = 0;

public:
    // Devuelve el game id y asigna el duck_id a la variable pasada por referencia
    int player_create_game(int id_player, Queue<Snapshot>& player_sender_queue, uint8_t& duck_id);

    uint8_t player_join_game(int id_player, int id_game, Queue<Snapshot>& player_sender_queue);

    void start_game(int id);

    Game* create_game();
    // TODO: caso que se desconecte uno o termina la partida
    // void delete_game(int id);

    std::vector<int32_t> list_lobbies();

    Queue<action>* get_gameloop_q(int id_game);

    void remove_player(int id_game, int id_player);

    ~GamesMonitor();
};

#endif
