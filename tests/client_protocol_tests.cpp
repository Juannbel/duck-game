#include "../client/client_protocol.h"
#include "../common/lobby.h"
#include "../common/socket.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "utils.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

TEST(CLIENT_PROTOCOL, _CREATE_GAME_AND_START_GAME) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    Socket socket("localhost", "8080");
    ClientProtocol protocol(std::move(socket));
    EXPECT_NO_THROW(protocol.send_option(CREATE_GAME))
            << "send_option should not throw any exception";
    int32_t num_players(1);
    EXPECT_NO_THROW(protocol.send_option(num_players))
            << "send_option should not throw any exception";
    std::string name = "Facundo88";
    EXPECT_NO_THROW(protocol.send_string(name)) << "send_string should not throw any exception";
    GameInfo gameInfo = protocol.recv_game_info();
    ASSERT_TRUE(check_game_info(gameInfo, 0, 0, INVALID_DUCK_ID));

    EXPECT_NO_THROW(protocol.send_option(GET_INFO));
    std::vector<LobbyInfo> lobbies_info = protocol.recv_lobbies_info();
    EXPECT_EQ(lobbies_info.size(), 1) << "lobbies_info size should be equal to the expected size";
    ASSERT_TRUE(check_lobby_info(lobbies_info[0], gameInfo.game_id, 2, name));
    EXPECT_NO_THROW(protocol.send_option(START_GAME))
            << "send_option should not throw any exception";
    int32_t game_created = protocol.recv_option();
    EXPECT_EQ(game_created, CREATE_OK);
    protocol.shutdown();
}

TEST(CLIENT_PROTOCOL, _LIST_GAMES_AND_JOIN_GAME) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    Socket socket("localhost", "8080");
    ClientProtocol protocol(std::move(socket));
    EXPECT_NO_THROW(protocol.send_option(LIST_GAMES))
            << "send_option should not throw any exception";
    std::vector<LobbyInfo> lobbies_info = protocol.recv_lobbies_info();
    EXPECT_EQ(lobbies_info.size(), 1) << "lobbies_info size should be equal to the expected size";
    ASSERT_TRUE(check_lobby_info(lobbies_info[0], 0, 1, "Facundo88"));
    EXPECT_NO_THROW(protocol.send_option(JOIN_GAME))
            << "send_option should not throw any exception";
    uint32_t game_id = lobbies_info[0].game_id;
    uint32_t num_players = 1;
    EXPECT_NO_THROW(protocol.send_option(game_id)) << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.send_option(num_players))
            << "send_option should not throw any exception";
    EXPECT_NO_THROW(protocol.send_string("New player"))
            << "send_string should not throw any exception";
    GameInfo gameInfo = protocol.recv_game_info();
    EXPECT_NE(gameInfo.game_id, INVALID_GAME_ID)
            << "game_id should be equal to the expected game_id";
    protocol.shutdown();
}

TEST(CLIENT_PROTOCOL, _RECV_INITIAL_SNAPSHOT) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    Socket socket("localhost", "8080");
    ClientProtocol protocol(std::move(socket));
    Snapshot snapshot = protocol.recv_snapshot();

    EXPECT_EQ(snapshot.round_finished, false)
            << "round_finished should be equal to the expected match_finished";
    EXPECT_EQ(snapshot.show_stats, false)
            << "show_stats should be equal to the expected match_finished";
    EXPECT_EQ(snapshot.game_finished, false)
            << "game_finished should be equal to the expected match_finished";

    EXPECT_EQ(snapshot.ducks.size(), 1) << "ducks size should be equal to the expected size";
    ASSERT_TRUE(check_ducks(snapshot.ducks));

    EXPECT_EQ(snapshot.guns.size(), 1) << "guns size should be equal to the expected size";
    ASSERT_TRUE(check_guns(snapshot.guns));

    EXPECT_EQ(snapshot.bullets.size(), 1) << "bullets size should be equal to the expected size";
    ASSERT_TRUE(check_bullets(snapshot.bullets));

    EXPECT_EQ(snapshot.boxes.size(), 1) << "boxes size should be equal to the expected size";
    ASSERT_TRUE(check_boxes(snapshot.boxes));

    EXPECT_EQ(snapshot.maps.size(), 1) << "maps size should be equal to the expected size";
    ASSERT_TRUE(check_maps(snapshot.maps));

    protocol.shutdown();
}


TEST(CLIENT_PROTOCOL, _SEND_COMMANDS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    Socket socket("localhost", "8080");
    ClientProtocol protocol(std::move(socket));
    action action0 = {0, StartMovingRight};
    EXPECT_NO_THROW(protocol.send_player_action(action0));
    action action1 = {0, StartMovingLeft};
    EXPECT_NO_THROW(protocol.send_player_action(action1));
    action action2 = {0, StopMoving};
    EXPECT_NO_THROW(protocol.send_player_action(action2));
    action action3 = {0, StartShooting};
    EXPECT_NO_THROW(protocol.send_player_action(action3));
    action action4 = {0, StopShooting};
    EXPECT_NO_THROW(protocol.send_player_action(action4));
    action action5 = {0, StartLookup};
    EXPECT_NO_THROW(protocol.send_player_action(action5));
    action action6 = {0, StopLookup};
    EXPECT_NO_THROW(protocol.send_player_action(action6));
    action action7 = {0, PickUp};
    EXPECT_NO_THROW(protocol.send_player_action(action7));
    action action8 = {0, Jump};
    EXPECT_NO_THROW(protocol.send_player_action(action8));
    action action9 = {0, StopJump};
    EXPECT_NO_THROW(protocol.send_player_action(action9));
    action action10 = {0, LayDown};
    EXPECT_NO_THROW(protocol.send_player_action(action10));
    action action11 = {0, StandUp};
    EXPECT_NO_THROW(protocol.send_player_action(action11));
    action action12 = {0, FlyMode};
    EXPECT_NO_THROW(protocol.send_player_action(action12));
    action action13 = {0, InfiniteAmmo};
    EXPECT_NO_THROW(protocol.send_player_action(action13));
    action action14 = {0, KillEveryone};
    EXPECT_NO_THROW(protocol.send_player_action(action14));
    action action15 = {0, InfiniteHP};
    EXPECT_NO_THROW(protocol.send_player_action(action15));
    action action16 = {0, GetDeathLaser};
    EXPECT_NO_THROW(protocol.send_player_action(action16));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
