#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../client/client_protocol.h"
#include "../common/socket.h"
#include "../common/lobby.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

TEST(CLIENT_PROTOCOL,_CREATE_GAME_AND_START_GAME) {
    Socket socket("localhost","8080");
    ClientProtocol protocol(std::move(socket));
    EXPECT_NO_THROW(protocol.send_option(CREATE_GAME)) << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.send_option(1)) << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.send_string("Facundo88")) << "send_string should not throw any exception";
    GameInfo gameInfo = protocol.recv_game_info();
    EXPECT_EQ(gameInfo.game_id, 0) << "game_id should be equal to the expected game_id";
    EXPECT_EQ(gameInfo.duck_id_1, 0) << "duck_id_1 should be equal to the expected duck_id_1";
    EXPECT_EQ(gameInfo.duck_id_2, INVALID_DUCK_ID) << "duck_id_2 should be equal to the expected duck_id_2";
    EXPECT_NO_THROW(protocol.send_option(0)) << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.shutdown()) << "shutdown should not throw any exception";
}

TEST(CLIENT_PROTOCOL,_LIST_GAMES_AND_JOIN_GAME) {
    Socket socket("localhost","8080");
    ClientProtocol protocol(std::move(socket));
    EXPECT_NO_THROW(protocol.send_option(LIST_GAMES)) << "send_option should not throw any exception";
    std::vector<LobbyInfo> lobbies_info = protocol.recv_lobbies_info();
    EXPECT_EQ(lobbies_info.size(), 1) << "lobbies_info size should be equal to the expected size";
    EXPECT_EQ(lobbies_info[0].game_id, 0) << "game_id should be equal to the expected game_id";
    EXPECT_EQ(lobbies_info[0].connected_players, 1) << "connected_players should be equal to the expected connected_players";
    EXPECT_EQ(std::string(lobbies_info[0].creator), "Facundo88") << "creator should be equal to the expected creator";
    EXPECT_NO_THROW(protocol.send_option(JOIN_GAME)) << "send_option should not throw any exception";
    uint32_t game_id = lobbies_info[0].game_id;
    uint32_t num_players = 1;
    EXPECT_NO_THROW(protocol.send_option(game_id)) << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.send_option(num_players)) << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.send_string("New player")) << "send_string should not throw any exception";
    GameInfo gameInfo = protocol.recv_game_info();
    EXPECT_NE(gameInfo.game_id, INVALID_GAME_ID) << "game_id should be equal to the expected game_id";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
