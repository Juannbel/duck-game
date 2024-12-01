# Manual de proyecto

## División de trabajo

Al inicio del proyecto, decidimos dividirnos para trabajar cada uno en una parte distinta, tratamos de generar una división lo mas disjunta posible, para evitar depender de cambios que podía estar realizando otra vez otro integrante del equipo. De esta forma logramos evitar estar pisando trabajo realizado por otro.
La primera división que hicimos fue la siguiente:
- Lautaro Pedrozo: Encargado de la arquitectura de hilos y partidas, tanto del lado del servidor como del cliente (mayor trabajo del lado del servidor, con varios hilos para manejar multipartidas y multiclientes)
- Federico Letier: Encargado de toda la implementación del juego del lado del servidor; todo lo referido a las entidades del juego, físicas, etc
- Juan Bel: Encargado de la mayor parte de la implementación del lado del cliente; parte gráfica del juego, lobby para crear y unirse a partidas, entre otros
- Facundo Lescano: Implementación del protocolo para la comunicación cliente-servidor, editor de niveles, y testing con google tests

### Organización por semana
#### Semana 1 (15 al 22 de Octubre)
Empezamos con una reunion entre todos para definir las estructuras generales necesarias para iniciar el desarrollo, como snapshot y comandos
- Lautaro Pedrozo: Primeras implementaciones de clases para manejar los hilos y levantar el Server con comunicacion hacia el cliente.
- Federico Letier: Implementacion de constant rate loop para iterar las acciones del juego. Primeras funciones de los patos para moverlos y un sistema de colisiones. Se envia el estado del juego al cliente actualizando una snapshot en el game_loop.
- Juan Bel: primeras pruebas con SDL, creando clases para las animaciones y la camara, se implementa tambien un constant looper para manejar las iteraciones. Empiezo a usar un formato en YAML para poder generar animaciones mas facilmente.
- Facundo Lescano: Agregado de las herramientas para crear los protocolos de comunicación como la clase socket y creacion de los protocolos cliente y servidor. Envios de snapshots y de comandos del jugador(cliente).

#### Semana 2 (22 de Octubre al 29 de Octubre)
- Lautaro Pedrozo: Primera implementacion de la clase YAMLLoader. Y implementacion de clases para manejar una unica partida desde el server.
- Federico Letier: En cada iteración del game loop se crea una snapshot que los distintos objetos la llenan con su estado. Se implementan los spawns para que las armas aparezcan cada un tiempo constante mas un tiempo random extra. Implementación de aleteo de los patos y cambio a uso de coordenadas con floats del lado del server. Patos pueden agarrar armas, no pueden disparar. Primera idea para validar que si un solo jugador queda vivo, gano la ronda.
- Juan Bel: Se hacen modificaciones para poder recibir y usar el mapa enviado por el servidor. Se empieza a agregar música y sonidos. Se agregan collectables. Modularización de varias cosas, creando clase para obtener datos de animaciones y texturas y no tener que cargarlas cada vez que se piden. Mejora de controles.
- Facundo Lescano: Modificaciones en los vectores en el snapshot a vectores dinamicos y cambio de metodos para enviar y recibir los vectores por medio de los protocolos y tambien se implementa la comunicación para la información de la partida en que está cada cliente.

#### Semana 3 (29 de Octubre al 5 de Noviembre)
- Lautaro Pedrozo: Se implementa la comunicacion inicial para multiples partidas. Se aplican mutex a la clase gamesMonitor y me aseguro de que no haya memory leaks.
- Federico Letier: Se implementa la lógica de plataformas, lgunos bloques se pueden atravesar saltando por debajo. Patos ya pueden disparar y mueren por los disparos. Aun las balas no eran una clase con comportamiento, las movia el manager. Balas no atraviesan jugadores ni bloques.
- Juan Bel: Se agrega la pantalla de estadísticas para mostrar entre    rondas. Se arreglan varios bugs en algunas texturas y animaciones. Optimizaciones en el renderizado para objetos que no son visibles. Pruebas para crear un lobby con QT en lugar de manejarlo con CLI. Ahora se detecta el cierre del servidor y se desconectan los clientes.
- Facundo Lescano: Agregado de excepciones en el protocolo si se cierra el socket en las comunicaciones, un vector de mapas y un inidicador de que la partida termino en la snapshot y metodos para enviarlos en el protocolo.
Comienzo con el editor de niveles. Se pueden editar bloques en un mapa, guardar el mapa y cargar mapas anteriores.


#### Semana 4 (5 al 12 de Noviembre)
- Lautaro Pedrozo: Implementacion de eliminar las partidas cuando terminan.
- Federico Letier: Armas tienen fisicas y los spawns se empiezan a cargar de los yaml. Optimizacion de las colisiones, ahora verifica solo con los bloques cercanos, no con todo el mapa.  Casco y armadura salvan al pato de una bala. Las balas pasan a ser una clase y los laseres pueden rebotar. Se implementa la banana y la granada. Implementacion de disparos y mecanicas particulares del resto de armas (retroceso, tiro de recarga, angulos de disparo, etc). Implementacion de la clase Box.
- Juan Bel: Cambios necesarios para poder conectar 2 jugadores en el mismo cliente gráfico. Mejora y arreglo de varios bugs en el nuevo lobby con UI en QT. Se agregan los renderizables para las cajas. Varios avances en toda la parte de sonido.
- Facundo Lescano: Agregado de un vector de spawns de boxes en la snapshot y envio en el protocolo. Se agrega solidez en los bloques, colocar spawns de patos, de armas y de cajas en el editor y poder limpiar el mapa que se esta editando.

#### Semana 5 (12 al 19 de Noviembre) -> Primera entrega
- Lautaro Pedrozo: Documentacion tecnica y manual de usuario. Arreglos finales y correcciones de bugs/memory leaks en distintos casos del lobby.
- Federico Letier: Cheats de jugador. Cambios para usar la configuración del yaml. Se manda siempre una última snapshot para que del lado del cliente se maneje el final de partida. Fix de bugs y mejora de algunas mecanicas implementadas previamente.
- Juan Bel: Ultimos ajustes sobre armas por cambios en snapshot. Pasaje de configuraciones a un archivo YAML, implementando una clase estática encargada de cargarlo. Retoques para soportar los cheats. Trabajo sobre el CMake y el Makefile para tener un instalador. Ajustes finales sobre parte gráfica por bugs encontrados testeando mas a fondo todo lo hecho.
- Facundo Lescano: implementacion de los tests de los protocolos con google test.

#### Semana 6 (19 al 26 de Noviembre)
- Lautaro Pedrozo:
- Federico Letier: Mejoro movilidad en las plataformas. Ahora el game_loop entre rondas espera a que todos los clientes conectados reciban la snapshot y ellos le devuelvan un mensaje "ready", que simboliza que ya cargaron el siguiente mapa, evitando posibles desync si a alguno no le llega por problemas de red. 
- Juan Bel: Implementación de UI para realizar la conexión con el servidor (pre-lobby), y se permite al finalizar la partida, volver al lobby, sin necesidad de volver a ingresar los datos del servidor
- Facundo Lescano: mejora en la ui del editor de mapas, trabajo en la documentacion y agrego mapas para jugar.

#### Semana 7 (26 de Noviembre al 3 de Diciembre) -> Entrega final
- Lautaro Pedrozo:
- Federico Letier: Patos caen al morir. Lock al mapa de players adentro de gameloop porque habia una rc. Pre-lobby en el que los jugadores comienzan dentro de un mapa especial para esperar a que arranque la partida, cuando el owner da start game, deben romper la caja de su cuadrante para comenzar la partida. Ahora se puede bajar de una plataforma apretando la flecha para abajo.
- Juan Bel: Mejoras sobre el README y la documentación, soporte para joysticks, varios temas mas de mapas, cambios para pasar al nuevo lobby dinámico
- Facundo Lescano: Agrego aviso para saber si un mapa se guardó correctamente, a la hora de guardarlo sugiere el nombre del ultimo mapa cargado y validaciones con los spawns y los bloques para no generar bugs en el juego.
