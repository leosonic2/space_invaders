# Space Invaders

[English](README.md) | [Português (Brasil)](README.pt-BR.md) | [Español](README.es.md)

**Este es un proyecto de estudio centrado en aprender OpenSpec.** El juego Space Invaders es un ejemplo práctico para explorar cómo el flujo spec-driven de OpenSpec convierte los requisitos de comportamiento en cambios planificados, implementados y verificados.

El juego es una aplicación arcade 2D de escritorio, implementada en C17 con OpenGL 3.3, GLFW y GLAD. La simulación está separada de la ventana y el renderizado, lo que permite probar la lógica del juego y algunas funciones del renderizador sin abrir una ventana.

## Aprender OpenSpec

Usa este repositorio para explorar cómo los artefactos de OpenSpec respaldan los cambios de software:

- Las especificaciones de `openspec/specs/` describen el comportamiento duradero del juego.
- Las propuestas, los diseños y las listas de tareas muestran cómo se planifica un cambio antes de implementarlo.
- La aplicación de las tareas conecta los requisitos escritos con el código y las pruebas; los cambios completados pueden revisarse y archivarse.

El juego ofrece un proyecto C concreto para estudiar ese proceso, con instrucciones de compilación, pruebas automatizadas y comportamiento visible durante la ejecución.

## Características

- Movimiento horizontal controlado con el teclado y disparo de proyectiles.
- Formaciones de enemigos en movimiento, detección de colisiones, puntuación y vidas.
- Estados de menú, pausa, Game Over, victoria y reinicio.
- Nave del jugador en pixel art, dibujada a partir del diseño de referencia.
- Duración de campaña configurable, con cinco oleadas de forma predeterminada.
- Campo de juego lógico 4:3 redimensionable, con un tamaño mínimo de ventana de 800 x 600.
- Pruebas headless con CTest para configuración, lógica del juego, funciones del renderizador e integración.

## Requisitos

- CMake 3.21 o posterior.
- Ninja y Git.
- Un compilador C17: GCC, Clang o MSVC.
- Python 3 y pip; la generación de GLAD usa la dependencia Jinja2 fijada en `requirements-build.txt`.
- Bibliotecas de desarrollo de OpenGL y dependencias de GLFW para la plataforma.

Durante la configuración, CMake descarga GLFW 3.4 y GLAD 2.0.8 mediante FetchContent. Para ejecutar la aplicación se necesita una sesión de escritorio compatible con OpenGL; la suite CTest no necesita abrir una ventana del juego.

### Linux (Ubuntu)

Instala el compilador, las herramientas de build, los encabezados de OpenGL y las dependencias X11/Wayland de GLFW:

```sh
sudo apt-get update
sudo apt-get install -y build-essential cmake git ninja-build pkg-config \
  libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
  libwayland-dev wayland-protocols libxkbcommon-dev python3 python3-venv
```

Instala la dependencia Python de build en un entorno virtual:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-build.txt
```

`pkg-config` se usa en la configuración Linux de GLFW para comprobar las dependencias de la plataforma. `libwayland-dev` proporciona el Wayland scanner que necesita el backend Wayland de GLFW.

### Windows (MSYS2 UCRT64)

Instala los paquetes GCC UCRT64, CMake y Ninja en MSYS2 y usa una terminal MSYS2 UCRT64 con estas herramientas en el `PATH`:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Instala la dependencia Python de build desde PowerShell:

```powershell
py -3 -m pip install -r requirements-build.txt
```

Como alternativa, compila con MSVC desde un Visual Studio Developer PowerShell que tenga CMake y Ninja disponibles. No reutilices un directorio de build configurado con otro generador; elige uno nuevo al cambiar de toolchain.

## Configuración y compilación

Ejecuta estos comandos desde la raíz del repositorio. Mantén activo el entorno Python usado para instalar Jinja2 mientras configuras GLAD.

```sh
cmake -S . -B build -G Ninja
cmake --build build --parallel
```

CMake usa el estándar C17 y desactiva las extensiones del compilador. El objetivo de la aplicación se compila con `/W4` en MSVC o con `-Wall -Wextra -Wpedantic` en los demás compiladores.

### Configurar la duración de la campaña

`SPACE_INVADERS_FINAL_WAVE` es un ajuste entero positivo de la caché de CMake y su valor predeterminado es `5`:

```sh
cmake -S . -B build -G Ninja -DSPACE_INVADERS_FINAL_WAVE=8
```

## Ejecución

La aplicación carga los shaders mediante rutas relativas al directorio de trabajo. Ejecútala desde la raíz del repositorio, que contiene `shaders/`, o desde el directorio de build, donde CMake copia los shaders:

```sh
./build/bin/space_invaders
```

En Windows, ejecuta `.\build\bin\space_invaders.exe` desde PowerShell, en la raíz del repositorio.

## Controles

| Tecla | Acción |
| --- | --- |
| ENTER | Inicia desde el menú, reanuda durante la pausa o reinicia después de Game Over |
| A o Flecha izquierda | Se mueve a la izquierda |
| D o Flecha derecha | Se mueve a la derecha |
| SPACE | Dispara |
| ESC | Pausa |

El juego comienza con tres vidas. Eliminar un enemigo común otorga 10 puntos. Un invasor que llegue al borde inferior de la pantalla o choque con la nave cuesta una vida. Al completar la última oleada configurada, se muestra la pantalla de victoria.

## Pruebas

Compila el proyecto y ejecuta la suite de pruebas headless desde la raíz del repositorio:

```sh
ctest --test-dir build --output-on-failure
```

Consulta [TESTING.md](TESTING.md) para conocer la cobertura y las limitaciones de las pruebas.

## Registro opcional de la tasa de fotogramas

Asigna un valor no vacío a `SPACE_INVADERS_LOG_FPS` antes de iniciar la aplicación para imprimir la media de fotogramas por segundo en intervalos de cinco segundos en el flujo de error estándar:

```powershell
$env:SPACE_INVADERS_LOG_FPS = "1"
.\build\bin\space_invaders.exe
```

```sh
SPACE_INVADERS_LOG_FPS=1 ./build/bin/space_invaders
```

Consulta [ARCHITECTURE.md](ARCHITECTURE.md) para conocer las relaciones entre módulos y el flujo de ejecución. Para las interfaces públicas en C, consulta [API_REFERENCE.md](API_REFERENCE.md).