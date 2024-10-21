#include "client/constant_looper.h"
#include "common/snapshot.h"

#include <cmath>
#include <iostream>
#include <exception>

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>


int main() try {

	// Habria que conectar con el servidor
	// arrancar los hilos de envio y recepcion de mensajes
	// recibir el mapa y el id del pato y el primer snapshot

	// ConstantLooper looper(commands, snapshots, last_snapshot, duck_id);
	Snapshot last_snapshot;
	last_snapshot.ducks[0] = Duck(0, "", 0, 100, DuelingPistol, false, false, false, false, false, true, true, false, false, false, false, 10, 70);
	last_snapshot.ducks[1] = Duck(0, "", 0, 100, DuelingPistol, false, false, false, false, false, true, false, false, false, false, false, 150, 70);
	last_snapshot.ducks[2] = Duck(0, "", 0, 100, Ak47, false, false, false, false, false, true, false, false, false, false, false, 180, 70);
	last_snapshot.ducks[3] = Duck(0, "", 0, 100, None, false, false, false, false, false, true, false, false, false, false, false, 240, 70);

	last_snapshot.players_quantity = 4;

	last_snapshot.guns[0] = Gun{0,  Ak47, 60, 70};
	last_snapshot.guns_quantity = 1;

	ConstantLooper looper(0, last_snapshot);
	looper.run();
	return 0;
} catch (std::exception& e) {
	// If case of error, print it and exit with error
	std::cerr << e.what() << std::endl;
	return 1;
}
