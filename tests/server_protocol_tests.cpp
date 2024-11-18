#include <cstring>

#include "../common/lobby.h"
#include "../common/socket.h"
#include "../server/server_protocol.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "utils.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

TEST(SERVER_PROTOCOL, _CREATE_GAME_AND_START_GAME) {
    Socket socket("8080");
    Socket peer(socket.accept());
    ServerProtocol protocol(peer);
    int32_t cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, CREATE_GAME) << "cmd should be equal to the expected cmd";
    int32_t num_players = protocol.receive_cmd();
    EXPECT_EQ(num_players, 1) << "num_players should be 1";
    std::string name = protocol.recv_string();
    EXPECT_EQ(name, "Facundo88") << "name should be equal to the expected name";
    GameInfo game_info = {0, 0, INVALID_DUCK_ID};
    EXPECT_NO_THROW(protocol.send_game_info(game_info))
            << "send_game_info should not throw any exception";
    cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, GET_INFO);

    LobbyInfo info = initialize_lobby_info(game_info.game_id, 2, name);
    std::vector<LobbyInfo> lobbies = {info};

    EXPECT_NO_THROW(protocol.send_lobbies_info(lobbies));
    EXPECT_EQ(protocol.receive_cmd(), START_GAME) << "cmd should be equal to the expected cmd";
    EXPECT_NO_THROW(protocol.send_option(CREATE_OK));
}

TEST(SERVER_PROTOCOL, _LIST_GAMES_AND_JOIN_GAME) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    Socket socket("8080");
    Socket peer(socket.accept());
    ServerProtocol protocol(peer);
    int32_t cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, LIST_GAMES) << "cmd should be equal to the expected cmd";
    LobbyInfo info = initialize_lobby_info(0, 1, "Facundo88");
    std::vector<LobbyInfo> lobbies = {info};
    EXPECT_NO_THROW(protocol.send_lobbies_info(lobbies))
            << "send_lobbies_info should not throw any exception";
    cmd = protocol.receive_cmd();
    EXPECT_EQ(cmd, JOIN_GAME) << "game_id should be equal to the expected game_id";
    int32_t game_id = protocol.receive_cmd();
    EXPECT_EQ(game_id, 0) << "game_id should be equal to the expected game_id";
    int32_t num_players = protocol.receive_cmd();
    EXPECT_EQ(num_players, 1) << "num_players should be 1";
    std::string name = protocol.recv_string();
    EXPECT_EQ(name, "New player") << "name should be equal to the expected name";
    GameInfo game_info = {0, 0, INVALID_DUCK_ID};
    EXPECT_NO_THROW(protocol.send_game_info(game_info))
            << "send_game_info should not throw any exception";
}

TEST(SERVER_PROTOCOL, SEND_INITIAL_SNAPSHOT) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    Socket socket("8080");
    Socket peer(socket.accept());
    ServerProtocol protocol(peer);
    Snapshot snapshot = initialize_snapshot();
    EXPECT_NO_THROW(protocol.send_snapshot(snapshot))
            << "send_snapshot should not throw any exception";
}

TEST(SERVER_PROTOCOL, RECV_COMMANDS) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    Socket socket("8080");
    Socket peer(socket.accept());
    ServerProtocol protocol(peer);
    action action_check;
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StartMovingRight);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StartMovingLeft);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StopMoving);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StartShooting);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StopShooting);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StartLookup);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StopLookup);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == PickUp);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == Jump);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StopJump);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == LayDown);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == StandUp);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == FlyMode);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == InfiniteAmmo);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == KillEveryone);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == InfiniteHP);
    action_check = protocol.recv_player_action();
    ASSERT_TRUE(action_check.duck_id == 0 && action_check.command == GetDeathLaser);
    protocol.shutdown();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
