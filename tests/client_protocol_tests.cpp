#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../client/client_protocol.h"
#include "../common/socket.h"
#include "../common/lobby.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

namespace {
TEST(CreateGameTests, Check) {
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
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
