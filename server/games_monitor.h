#ifndef GAMES_MONITOR_H
#define GAMES_MONITOR_H

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
    int player_create_game(uint8_t id_player, Queue<Snapshot>& player_sender_queue);

    int player_join_game(uint8_t id_player, int id_game, Queue<Snapshot>& player_sender_queue);

    void start_game(int id);

    Game* create_game();
    // TODO: caso que se desconecte uno o termina la partida
    // void delete_game(int id);

    std::vector<int> list_lobbies() ;

    Queue<action>* get_gameloop_q(int id_game);

    MatchInfo get_match_info(int id_game);

    ~GamesMonitor();
};

#endif