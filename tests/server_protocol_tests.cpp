#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../server/server_protocol.h"
#include "../common/socket.h"
#include "../common/lobby.h"
#include <cstring>

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

TEST(SERVER_PROTOCOL,_CREATE_GAME) {
    Socket socket("8080");
    Socket peer(socket.accept());
    ServerProtocol protocol(peer);
    int32_t cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, CREATE_GAME) << "cmd should be equal to the expected cmd";
    int32_t num_players = protocol.receive_cmd();
    EXPECT_EQ(num_players, 1) << "num_players should be 1";
    std::vector<std::string> players;
    std::string name = protocol.recv_string();
    EXPECT_EQ(name, "Facundo88") << "name should be equal to the expected name";
    GameInfo game_info = {0, 0, INVALID_DUCK_ID};
    EXPECT_NO_THROW(protocol.send_game_info(game_info)) << "send_game_info should not throw any exception";
    EXPECT_EQ(protocol.receive_cmd(),0)<< "cmd should be equal to the expected cmd";
    
    protocol.shutdown();
    socket.shutdown(2);
    socket.close();
    
}

TEST(SERVER_PROTOCOL,_LIST_GAMES_AND_JOIN_GAME) {
    Socket socket("8080");
    Socket peer(socket.accept());
    ServerProtocol protocol(peer);
    int32_t cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, LIST_GAMES) << "cmd should be equal to the expected cmd";
    LobbyInfo info;
    info.game_id = 0;
    info.connected_players = 1;
    std::strncpy(info.creator, "Facundo88", MAX_PLAYER_NAME - 1);
    info.creator[MAX_PLAYER_NAME - 1] = '\0';
    std::vector<LobbyInfo> lobbies = {info};
    EXPECT_NO_THROW(protocol.send_lobbies_info(lobbies)) << "send_lobbies_info should not throw any exception";
    cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, JOIN_GAME) << "game_id should be equal to the expected game_id";
    int32_t game_id = protocol.receive_cmd();
    EXPECT_EQ(game_id, 0) << "game_id should be equal to the expected game_id";
    int32_t num_players = protocol.receive_cmd();
    EXPECT_EQ(num_players, 1) << "num_players should be 1";
    std::vector<std::string> players;
    std::string name = protocol.recv_string();
    EXPECT_EQ(name, "New player") << "name should be equal to the expected name";
    GameInfo game_info = {0, 0, INVALID_DUCK_ID};
    EXPECT_NO_THROW(protocol.send_game_info(game_info)) << "send_game_info should not throw any exception";

    protocol.shutdown();
    socket.shutdown(2);
    socket.close();
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
