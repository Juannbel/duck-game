#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../common/blocking_queue.h"
#include "common/lobby.h"
#include "common/snapshot.h"

#include "game_loop.h"

class Game {
private:
    QueueListMonitor sv_msg_queues;
    Queue<action> gameloop_q;
    GameLoop gameloop;
    int cant_players = 0;
    std::string creator;
    int owner_id;
    std::map<int, std::pair<uint8_t, uint8_t>> player_to_duck_ids;
    bool open = true;
    int game_id;

public:
    Game(int id, const std::string& creator, int owner_id);

    Queue<action>& get_gameloop_queue();

    void start();

    GameInfo add_player(int player_id, Queue<Snapshot>& player_sender_queue,
                        const std::vector<std::string>& players_names);

    bool delete_player(int id_player);

    bool is_open() const { return open; }

    LobbyInfo get_info() const {
        LobbyInfo info{};
        info.game_id = game_id;
        info.connected_players = cant_players;
        std::strncpy(info.creator, creator.c_str(), MAX_PLAYER_NAME - 1);
        info.creator[MAX_PLAYER_NAME - 1] = '\0';

        return info;
    }


    ~Game();
};

#endif
