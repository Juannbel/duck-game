# Manual de usuario

## 🖧 Servidor

Antes de poder conectar clientes es necesario iniciar un servidor, para esto, una vez instalado el juego, basta con correr el siguiente comando:
```bash
duck_game_server <puerto>
```

Donde `<puerto>` es el puerto donde estará escuchando el mismo.

## 💻 Clientes

Para iniciar un cliente, se ejecuta el siguiente comando:

```bash
duck_game_client
```

### **Conexión al servidor**

- Al iniciar un cliente se muestra una ventana que permite conectarse al servidor. Para ello, es necesario ingresar la dirección IP del mismo (en el caso de ejecutar en red local, se usaría la IP de la computadora donde se está ejecutando el mismo) y el puerto en el que está escuchando.

![Pre Lobby](images/manual-usuario/pre-lobby.png)

### 1. **Lobby inicial:**

- Una vez conectado al servidor, se mostrará un lobby con las opciones para Crear o Unirse a una partida.

![lobby inicial](images/manual-usuario/lobby_inicial.png)

#### 1.1. **Crear partida:**

- Al seleccionar la opción de *Crear partida* (Create Game), se mostrará un menú para seleccionar la cantidad de jugadores que se conectarán en este cliente gráfico, y la opción para ingresar sus nombres de usuario

![ingreso partida](images/manual-usuario/ingreso-partida.png)

##### 1.1.1. **Menu de inicio de Partida:**

- Una vez ingresados los datos, se mostrará un menú de inicio de partida, donde se esperará a que se unan los jugadores.
 El creador decide cuando iniciar la partida.
- Se puede ver tanto el ID del juego creado, como la cantidad de jugadores conectados actualmente
- Se puede refrescar la cantidad de jugadores conectados con el botón *Refresh*.

![esperando jugadores](images/manual-usuario/lobby-creador.png)

#### 1.2. **Unirse a partida:**

- Al seleccionar la opción de *Unirse a partida* (Join Game) se listan los juegos disponibles, se puede seleccionar cualquiera de estos y presionar *Join*.
- Se puede refrescar la lista de juegos disponibles con el botón *Refresh*.
- Por cada juego se muestra su ID, la cantidad de jugadores conectados actualmente, y quien es el creador del mismo.

![listar lobbys](images/manual-usuario/lobby-join.png)

#### 1.2.1. **Ingreso a partida:**

- Una vez elegida la partida, aparecen las mismas opciones que al crear un juego, para seleccionar la cantidad de jugadores a conectarse en el cliente gráfico y se pide sus nombres.

![ingreso partida](images/manual-usuario/unirse-partida.png)

#### 1.2.2. **Menu de inicio de Partida:**

- Una vez ingresados a la partida, se mostrará una pantalla de espera, donde se esperará a que el creador inicie el juego.
- Se muestra el color y ID del jugador.

![esperando jugadores](images/manual-usuario/esperando-jugadores.png)

### 2. **Juego:**

- El objetivo del juego es ser el último jugador en pie.
- Una partida consta de 5 rounds, al final de los cuales se mostrará un resumen de las puntuaciones de los jugadores.
- Si tras 5 rounds nadie ha ganado, se vuelven a jugar otros 5 rounds mas.

#### 2.1 **Inicio de partida:**

- Una vez iniciada la partida, se muestra que pato fue asignado a cada jugador, y empieza la primera ronda.

![inicio juego](images/manual-usuario/inicio-juego.png)

- Cada cinco rounds (configurable, ver sección [configuración](#-configuración)) se mostrará un resumen de las puntuaciones de los jugadores y se verificará si alguno de ellos ganó la partida.

![Puntuacion](images/manual-usuario/puntuaciones-media.png)

#### 2.2 **Condición de victoria:**

- El jugador que obtenga 10 (nuevamente configurable, ver sección [configuración](#-configuración)) o mas puntos será el ganador. En caso de empate, se vuelven a jugar 5 rondas hasta que pueda declararse un único ganador.
- Como la partida se desarrolla en tiradas de 5 rondas, y se busca el ganador unicamente al finalizar estas tiradas, puede pasar que un jugador llegue primero a 10 rondas y otro jugador termine siendo el ganador, 

![victoria](images/manual-usuario/victoria.png)

- Una vez terminado el juego, con `ESC` puede cerrase el cliente, o puede decidirse volver a jugar presionando la tecla `r`, lo que lleva de nuevo al [lobby](#1-lobby-inicial)

### 3. 🎮 **Mecanicas:**
- Estando en el aire y presionando repetidamente la tecla de salto ( `w` o `↑` según el jugador ), se puede aletear y caer mas despacio.

<p align="center">
  <img src="./images/manual-usuario/aleteando.gif" alt="pato aleteando" />
</p>

- **CAJAS:** Al disparar a una caja, esta se rompe, puede contener equipamiento (armas y armaduras), **explotar** o estar vacía.

<p align="center">
  <img src="./images/manual-usuario/cajas.gif" alt="Rompiendo cajas" />
</p>

### 4. 🕹️ **Controles:**

- `1` Toggle mute
- `2` Toggle fullscreen

#### Jugador 1:
- `w` `a` `s` `d` para moverse.
- `c` para recoger/soltar equipamientos (armas y armaduras).
- `v` para disparar (caso arma), para equipar (caso armadura).
- `e` para mirar arriba.

#### Jugador 2:
- `↑` `↓` `←` `→` para moverse.
- `k` para recoger/soltar equipamientos (armas y armaduras).
- `l` para disparar (caso arma), para equipar (caso armadura).
- `shift derecho` para mirar arriba.

### 🛡️ Cheats
#### Jugador 1
- `F1` Modo vuelo
- `F2` Munición infinita
- `F3` Matar a todos los otros jugadores
- `F4` Vida infinita
- `F5` Obtener Death Laser

#### Jugador 2
- `F8` Modo vuelo
- `F9` Munición infinita
- `F10` Matar a todos los otros jugadores
- `F11` Vida infinita
- `F12` Obtener Death Laser

## 🗺️️️️️️ **Editor de niveles:**

Para inciar el editor de niveles:

```bash
duck_game_editor
```

- Una vez iniciado el editor de niveles, se mostrará este pantalla inicial
- Se pueden utilizar las opciones para `Guardar`, `Cargar` o `Borrar` el mapa.

![editor inicial](images/manual-usuario/inicial-editor.png)

- Una vez seleccionado el tipo de bloque, con `Click Izquierdo` se puede colocar y con `Click Derecho` se puede eliminar.
- Si se desea hacer que el bloque sea traspasable, se puede hacer con `Click Izquierdo` sobre el bloque (teniendo seleccionado el mismo tipo de bloque colocado actualmente).
- También es necesario seleccionar los 4 spawns para los patos, spawns para armas, y ubicaciones de cajas
- Para aumentar el zoom de la grilla hacer `Ctrl + wheel` (ctrl + "ruedita") sobre la misma. 

## 🔧 **Configuración**

Gran parte de la configuración del juego puede modificarse a traes del archivo `config.yaml`, al instalarse el juego, el mismo se ubica en `/etc/duck_game/config.yaml`.
Algunos de los aspectos que pueden modificarse son:
- Vida inicial y velocidad del pato
- Rondas necesarias para ganar el juego
- Cantidad de rondas entre que se muestran las estadísticas y se busca un ganador
- Daño, rango, y munición de las armas