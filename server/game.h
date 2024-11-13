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
    std::map<int, std::pair<uint8_t, uint8_t>> player_to_duck_ids;
    bool open = true;
    int id;

public:
    Game(int id, const std::string& creator);

    Queue<action>& get_gameloop_queue();

    void start();

    GameInfo add_player(int player_id, Queue<Snapshot>& player_sender_queue,
                        const std::vector<std::string>& players_names);

    void delete_player(int id_player);

    bool is_open() { return open; }

    LobbyInfo get_info() {
        LobbyInfo info;
        info.game_id = id;
        info.connected_players = cant_players;
        std::strncpy(info.creator, creator.c_str(), MAX_PLAYER_NAME - 1);
        info.creator[MAX_PLAYER_NAME - 1] = '\0';

        return info;
    }

    void set_on_game_end_callback(const std::function<void(int)>& callback);

    ~Game();
};

#endif
