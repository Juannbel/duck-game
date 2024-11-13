#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../server/server_protocol.h"
#include "../common/socket.h"
#include "../common/lobby.h"

using ::testing::AllOf;
using ::testing::HasSubstr;
using ::testing::ThrowsMessage;

namespace {
TEST(CreateGameTests, Check) {
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

    //EXPECT_NO_THROW(protocol.shutdown()) << "shutdown should not throw any exception";
}
}  // namespace


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}