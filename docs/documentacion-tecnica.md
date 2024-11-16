# Documentación técnica

---
Explicacion general de la arquitectura del proyecto, como se manejan las partidas, la comunicación entre cliente y servidor.

## Arquitectura de Server

---

### Estructura para manejar partidas
El server lanza un hilo aceptador encargado de aceptar clientes y crear dos hilos de comunicación por cada cliente (Receiver y Sender).

Para manejar múltiples partidas se utiliza el objeto `GamesMonitor` que tiene una referencia a cada partida en curso.
La clase `Game` contiene toda la información de la partida, como los IDs de jugadores, el mapa, y lo necesario para realizar la comuncacion con los ServerClient.

![arquitectura server](images/doc-tecnica/arquitectura_server.png)

### Estructura Game Loop
Game Loop es el encargado de leer las acciones de los jugadores, las procesa y envía al Game Operator lo que se recibió, luego se obtiene las actualizaciones del estado del juego (snapshots) y se envían a las queues de los jugadores

![game loop](images/doc-tecnica/estructura_gameloop.png)

### Estructura de GameOperator
Game Operator se encarga de la lógica del juego. Maneja la inicialización de los elementos del mapa, control de colisiones, administración de collectables y el estado de cada entidad. 

![game operator](images/doc-tecnica/game_operator.png)

### Secuencia de inicio de una partida
Se muestra la secuencia de inicio de una partida, donde se crea una nueva partida, se inicializan los elementos del game, se envía la informacion del juego a los jugadores y se inicia el game loop.

![secuencia game](images/doc-tecnica/secuencia_inicio_game.png)

## Arquitectura de Cliente

---

### Estructura de cliente
La estructura del cliente se divide en dos partes, el constant looper y el cliente.
El constant looper se encarga de procesar las acciones del jugador y de recibir los snapshots del estado del juego para mostrarlo por pantalla.
El cliente cuanta con una arquitectura basada en hilos, se tiene in hilo receiver y otro sender que se encargan de la comunicacion, hasta que el cliente o el server se cierre.

![arq cliente](images/doc-tecnica/estructura_cliente.png)

### Secuencia de accion hacia servidor
Caso de uso de la secuencia de acción hacia el servidor, donde el cliente envía una acción al servidor.

![secuencia hacia server](images/doc-tecnica/cliente-accion.png)

## Comunicación

---

### Comunicación Server - Cliente
Se muestra la comunicación entre el servidor y el cliente, donde el cliente envía las acciones al servidor y el servidor envía los snapshots del estado del juego al cliente.
Toda la comunicacion se realiza mediante socket previamente serializada/deserializada por el protocolo.

![comunicacion](images/doc-tecnica/comunicacion-server-cliente.png)
