#include "client.h"
#include "../common/snapshot.h"
#include "constant_looper.h"

Client::Client(const char *hostname, const char *servname) : 
        protocol(Socket(hostname, servname)), 
        receiver(protocol, snapshot_q), 
        sender(protocol, command_q){}

void Client::run() {
    receiver.start();
    sender.start();
    //Snapshot last_snapshot;
	//last_snapshot.ducks[0] = {0, "", 0, 100, DuelingPistol, false, false, false, false, false, true, true, false, false, false, false, 10, 70};
	//last_snapshot.ducks[1] = {0, "", 0, 100, DuelingPistol, false, false, false, false, false, true, false, false, false, false, false, 150, 70};
	//last_snapshot.ducks[2] = {0, "", 0, 100, Ak47, false, false, false, false, false, true, false, false, false, false, false, 180, 70};
	//last_snapshot.ducks[3] = {0, "", 0, 100, None, false, false, false, false, false, true, false, false, false, false, false, 240, 70};
//
	//last_snapshot.players_quantity = 4;
//
	//last_snapshot.guns[0] = Gun{0,  Ak47, 60, 70};
	//last_snapshot.guns_quantity = 1;

	ConstantLooper looper(0, snapshot_q, command_q);
	looper.run();
}

Client::~Client() {}