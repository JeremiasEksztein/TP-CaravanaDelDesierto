# Caravana del Desierto 🏜️🐪

**Juego de tablero por consola desarrollado en C** — un trabajo práctico de Algoritmos y Estructuras de Datos. El jugador avanza por un tablero lineal esquivando bandidos, recolectando premios y vidas extra, y enfrentando tormentas y oasis, con el objetivo de llegar a la meta antes de quedarse sin vidas.

---

## 📋 Tabla de contenidos

- [Caravana del Desierto 🏜️🐪](#caravana-del-desierto-️)
  - [📋 Tabla de contenidos](#-tabla-de-contenidos)
  - [🎮 Cómo jugar](#-cómo-jugar)
    - [Objetivo](#objetivo)
    - [Mecánica](#mecánica)
    - [Casillas especiales](#casillas-especiales)
    - [Bandidos](#bandidos)
  - [⚙️ Compilación y ejecución](#️-compilación-y-ejecución)
    - [Compilación manual](#compilación-manual)
    - [Ejecución](#ejecución)
  - [🏗️ Arquitectura del proyecto](#️-arquitectura-del-proyecto)
    - [Estructura de archivos](#estructura-de-archivos)
    - [Diagrama de módulos](#diagrama-de-módulos)
  - [📦 Estructuras de datos](#-estructuras-de-datos)
    - [Cola genérica (FIFO)](#cola-genérica-fifo)
    - [Lista circular doblemente enlazada](#lista-circular-doblemente-enlazada)
    - [Árbol binario de búsqueda (ABB)](#árbol-binario-de-búsqueda-abb)
  - [💾 Persistencia y base de datos](#-persistencia-y-base-de-datos)
    - [Archivos de datos](#archivos-de-datos)
    - [Índice en árbol binario](#índice-en-árbol-binario)
  - [⚙️ Configuración](#️-configuración)
  - [📖 Documentación técnica](#-documentación-técnica)
    - [Doxygen](#doxygen)
    - [Documentación adicional](#documentación-adicional)
  - [🧑‍💻 Autores](#-autores)

---

## 🎮 Cómo jugar

### Objetivo

Llegar a la **última casilla del tablero** (la meta) sin perder todas las vidas. Avanzas casilla por casilla según el resultado de un dado de 6 caras.

### Mecánica

1. **Menú principal**: al iniciar el juego se presenta un menú con tres opciones: Jugar, Ver Ranking o Salir.
2. **Registro**: ingresás tu nombre. Si ya jugaste antes, se recupera tu historial.
3. **Turnos**: en cada ronda, el jugador tira el dado y elige dirección (avanzar o retroceder). Luego los bandidos se mueven automáticamente.
4. **El orden de los turnos se mezcla aleatoriamente** en cada ronda para mayor imprevisibilidad.
5. **Victoria**: llegás a la última casilla → ganaste.
6. **Derrota**: te quedás sin vidas → perdiste.

### Casillas especiales

| Símbolo | Tipo        | Efecto                                                      |
|---------|-------------|-------------------------------------------------------------|
| `.`     | Normal      | No pasa nada.                                               |
| `O`     | Oasis       | Te volvés **invulnerable** a los bandidos por un turno.     |
| `T`     | Tormenta    | **Perdés el próximo turno** (no podés moverte).             |
| `P`     | Premio      | Ganás **1 punto**. La casilla se consume y vuelve a normal. |
| `V`     | Vida extra  | Ganás **1 vida extra**. La casilla se consume.              |
| `B`     | Bandido     | Casilla donde nace un bandido (se ve como normal después).  |
| `I`     | Inicio      | El jugador empieza acá. Vuelve acá tras ser capturado.      |
| `S`     | Meta        | ¡Llegar acá es ganar el juego!                              |

### Bandidos

- Aparecen al azar en el tablero al inicio de la partida.
- **Persiguen al jugador** moviéndose en la dirección más corta.
- Si un bandido te alcanza:
  - **Estando invulnerable**: perdés la invulnerabilidad, pero no las vidas.
  - **Sin invulnerabilidad**: perdés **1 vida**, volvés a la casilla de inicio y el bandido es eliminado.
- La invulnerabilidad se pierde al moverte, y se recupera al caer en un oasis.

---

## ⚙️ Compilación y ejecución

El proyecto está escrito en **C89 pedantic** y no requiere librerías externas. Se compila con `gcc` en Linux, macOS o Windows (MinGW/Cygwin).

### Compilación manual

```bash
# Desde el directorio CaravanaDelDesierto/
gcc -std=c89 -Wall -Wextra -pedantic -o bin/caravana \
    main.c \
    AdministradorDeJuego.c \
    arbolBinBusq.c \
    bandido.c \
    cola.c \
    common.c \
    interfaz.c \
    juego.c \
    jugador.c \
    listaCircularDoble.c \
    Ranking.c \
    tablero.c \
    turnos.c \
    usuarios_db.c \
    -I.
```

> El proyecto incluye un directorio `bin/` y `obj/` pre-creados para los binarios y objetos respectivamente.

### Ejecución

```bash
./bin/caravana
```

Asegurate de que el archivo `config.txt` esté en el mismo directorio que el ejecutable (o en el directorio de trabajo).

---

## 🏗️ Arquitectura del proyecto

### Estructura de archivos

| Archivo                      | Rol                                                                 |
|------------------------------|----------------------------------------------------------------------|
| `main.c`                     | Punto de entrada. Bucle principal del menú y orquestación.           |
| `AdministradorDeJuego.c/.h`  | Orquestador de partidas: ciclo jugar, ranking, persistencia.         |
| `juego.c/.h`                 | Motor del juego: inicialización, turnos, detección de colisiones.    |
| `tablero.c/.h`               | Gestión del tablero: casillas, distribución, visualización.          |
| `jugador.c/.h`               | Entidad jugador: vidas, puntos, movimiento, invulnerabilidad.        |
| `bandido.c/.h`               | Entidad bandido: creación, movimiento, búsqueda por ID.              |
| `turnos.c/.h`                | Lógica de turnos: creación y mezcla de turnos de jugador y bandidos. |
| `interfaz.c/.h`              | Interfaz de usuario: menús, mensajes, entrada de datos.              |
| `Ranking.c/.h`               | Visualización del ranking de jugadores.                              |
| `usuarios_db.c/.h`           | **Sistema de persistencia ISAM**: ABB como índice de archivos.       |
| `common.c/.h`                | Tipos comunes, macros y utilidades generales.                        |
| `cola.c/.h`                  | **Cola genérica FIFO** con nodos enlazados simples.                  |
| `listaCircularDoble.c/.h`    | **Lista circular doblemente enlazada** genérica.                     |
| `arbolBinBusq.c/.h`          | **Árbol binario de búsqueda (ABB)** genérico.                        |

### Diagrama de módulos

```
                    ┌──────────────┐
                    │    main.c    │
                    └──────┬───────┘
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
    ┌─────────────────┐ ┌──────┐ ┌──────────┐
    │AdministradorDe  │ │inter-│ │usuarios  │
    │Juego.c/.h       │ │faz   │ │_db.c/.h  │
    └────────┬────────┘ │.c/.h │ └────┬─────┘
             │          └──────┘      │
             ▼                        ▼
    ┌─────────────────┐      ┌────────────────┐
    │   juego.c/.h    │      │  arbolBinBusq  │
    └────┬────────┬───┘      │  .c/.h (índice)│
         │        │          └────────────────┘
    ┌────▼───┐ ┌──▼──────┐
    │tablero │ │turnos   │
    │.c/.h   │ │.c/.h    │
    └─┬──┬───┘ └────┬────┘
      │  │          │
  ┌───▼┐ ┌▼────┐  ┌▼──────────┐
  │ju- │ │ban- │  │Estructuras│
  │gador│ │dido │  │de datos   │
  │.c/h │ │.c/h │  │genéricas  │
  └─────┘ └─────┘  │(cola,     │
                    │listaC,    │
                    │arbolBB)   │
                    └───────────┘
```

**Flujo de dependencias**:

- `main` → `AdministradorDeJuego` → `juego` → `tablero`, `turnos`, `jugador`, `bandido`
- Las estructuras de datos (`cola`, `listaCircularDoble`, `arbolBinBusq`) son usadas por `juego`, `tablero`, y `usuarios_db`
- `common.h` es incluido por todos los módulos (macros, tipos de callback)

---

## 📦 Estructuras de datos

Todas las estructuras son **genéricas** mediante punteros `void*` + tamaño en bytes.

### Cola genérica (FIFO)

- **Archivos**: `cola.c` / `cola.h`
- **Implementación**: nodos enlazados simples con punteros `pri` (frente) y `ult` (final).
- **Operaciones**: `crear`, `encolar`, `desencolar`, `frente`, `estaVacia`, `destruir`.
- **Uso**: registro de movimientos del jugador y cola de turnos por ronda.

### Lista circular doblemente enlazada

- **Archivos**: `listaCircularDoble.c` / `listaCircularDoble.h`
- **Implementación**: nodos con punteros `ant` y `sig` formando un anillo.
  La lista se representa con un puntero al nodo actual.
- **Operaciones**: `crear`, `empujar`, `buscar`, `actualizarEnPos`, `largo`, `mirarEnPos`,
  `buscarPos`, `mostrarLR`, `destruir`.
- **Uso**: almacenamiento del tablero como secuencia circular de casillas.

### Árbol binario de búsqueda (ABB)

- **Archivos**: `arbolBinBusq.c` / `arbolBinBusq.h`
- **Implementación**: árbol binario con datos genéricos. Sin balanceo.
- **Recorridos**: `PREORDEN`, `INORDEN`, `POSTORDEN`.
- **Operaciones**: `crear`, `poner`, `sacar`, `buscar`, `imprimir`, `visitar`,
  `destruir`, `contarNodos`.
- **Uso**: índice de la base de datos de jugadores (búsqueda por nombre).

---

## 💾 Persistencia y base de datos

### Archivos de datos

| Archivo          | Contenido                                                    |
|------------------|--------------------------------------------------------------|
| `jugadores.dat`  | Registros de jugadores (id, nombre, partidas jugadas).       |
| `jugadores.idx`  | Índice serializado del ABB para búsquedas rápidas por nombre.|
| `partidas.dat`   | Resultados de partidas (idJugador, puntos, movimientos).     |

### Índice en árbol binario

El módulo `usuarios_db` implementa un sistema de persistencia tipo **ISAM simplificado**:

1. Los jugadores se guardan secuencialmente en `jugadores.dat`.
2. Un **árbol binario de búsqueda** en memoria indexa cada registro por nombre de jugador.
3. El índice contiene entradas de tipo `tEntradaIndice`: `{clave, offset, id}`.
4. Al abrir la base de datos, el índice se carga desde `jugadores.idx` (o se reconstruye desde el archivo de datos si no existe).
5. El índice se persiste inmediatamente después de cada inserción.

---

## ⚙️ Configuración

El archivo `config.txt` permite ajustar los parámetros del juego:

```text
cantidad_posiciones: 25
vidas_inicio: 3
maximo_bandidos: 3
maximo_premios: 2
maximo_vidas_extra: 2
maximo_oasis: 2
maximo_tormentas: 1
```

Si el archivo no existe o está mal formado, el juego crea uno nuevo con valores por defecto.

---

## 📖 Documentación técnica

### Doxygen

Todos los archivos de cabecera (`.h`) están documentados con **Doxygen** en formato Javadoc (`/** ... */`), incluyendo:

- `@file` + `@brief` + `@details` para cada módulo.
- `@struct`, `@enum`, `@typedef` para tipos.
- `@def` para macros y constantes.
- `@brief`, `@param`, `@return` para todas las funciones públicas.

Para generar la documentación HTML:

```bash
# Instalar Doxygen
# sudo apt install doxygen   # Linux
# brew install doxygen        # macOS

# Generar configuración por defecto
doxygen -g Doxyfile

# Editar Doxyfile:
#   INPUT                  = ./
#   RECURSIVE              = YES
#   GENERATE_LATEX         = NO
#   HAVE_DOT               = NO

# Generar documentación
doxygen Doxyfile

# Abrir html/index.html en tu navegador
```

### Documentación adicional

En el directorio `Documentacion/` se encuentra el PDF original del trabajo práctico:
- `CaravanaDelDesierto-AyED.pdf`

---

## 🧑‍💻 Autores

- Doricich, Agustin Francisco
- Eksztein Rolón, Jeremías Octavio
- Miranda, Guillermo Damian 
- Nieva, Alejo Nicolas

Trabajo práctico de la materia **Algoritmos y Estructuras de Datos**.
