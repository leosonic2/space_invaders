# Space Invaders

[English](README.md) | [Português (Brasil)](README.pt-BR.md) | [Español](README.es.md)

**Este é um projeto de estudo com foco no aprendizado do OpenSpec.** O jogo Space Invaders é um exemplo prático para explorar como o fluxo spec-driven do OpenSpec transforma requisitos de comportamento em mudanças planejadas, implementadas e verificadas.

O jogo é uma aplicação arcade 2D para desktop, implementada em C17 com OpenGL 3.3, GLFW e GLAD. A simulação é separada da janela e da renderização, permitindo testar a lógica do jogo e alguns recursos do renderizador sem abrir uma janela.

## Aprendendo OpenSpec

Use este repositório para explorar como os artefatos do OpenSpec apoiam mudanças de software:

- As especificações em `openspec/specs/` descrevem comportamentos duradouros do jogo.
- Propostas, designs e listas de tarefas mostram como uma mudança é planejada antes da implementação.
- A execução das tarefas conecta requisitos escritos ao código e aos testes; mudanças concluídas podem ser revisadas e arquivadas.

O jogo oferece um projeto C concreto para estudar esse processo, incluindo instruções de compilação, testes automatizados e comportamento visível em execução.

## Recursos

- Movimento horizontal controlado pelo teclado e disparo de projéteis.
- Formações de inimigos em movimento, detecção de colisões, pontuação e vidas.
- Estados de menu, pausa, Game Over, vitória e reinício.
- Nave do jogador em pixel art, desenhada a partir da referência visual.
- Duração configurável da campanha, com cinco ondas por padrão.
- Campo de jogo lógico 4:3 redimensionável, com tamanho mínimo de janela de 800 x 600.
- Testes headless com CTest para configuração, lógica do jogo, recursos do renderizador e integração.

## Requisitos

- CMake 3.21 ou mais recente.
- Ninja e Git.
- Compilador C17: GCC, Clang ou MSVC.
- Python 3 e pip; a geração do GLAD usa a dependência Jinja2 fixada em `requirements-build.txt`.
- Bibliotecas de desenvolvimento do OpenGL e dependências da plataforma para GLFW.

Durante a configuração, o CMake baixa GLFW 3.4 e GLAD 2.0.8 com FetchContent. Para executar o aplicativo, é necessária uma sessão desktop compatível com OpenGL; a suíte CTest não precisa abrir uma janela do jogo.

### Linux (Ubuntu)

Instale o compilador, as ferramentas de build, os cabeçalhos do OpenGL e as dependências X11/Wayland do GLFW:

```sh
sudo apt-get update
sudo apt-get install -y build-essential cmake git ninja-build pkg-config \
  libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
  libwayland-dev wayland-protocols libxkbcommon-dev python3 python3-venv
```

Instale a dependência Python de build em um ambiente virtual:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements-build.txt
```

O `pkg-config` é usado pela configuração Linux do GLFW para verificar dependências da plataforma. `libwayland-dev` fornece o Wayland scanner necessário ao backend Wayland do GLFW.

### Windows (MSYS2 UCRT64)

Instale os pacotes GCC UCRT64, CMake e Ninja no MSYS2 e use um terminal MSYS2 UCRT64 com essas ferramentas no `PATH`:

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
  mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Instale a dependência Python de build pelo PowerShell:

```powershell
py -3 -m pip install -r requirements-build.txt
```

Como alternativa, compile com MSVC em um Visual Studio Developer PowerShell com CMake e Ninja disponíveis. Não reutilize um diretório de build configurado para outro gerador; escolha um diretório novo ao trocar de toolchain.

## Configurar e compilar

Execute estes comandos na raiz do repositório. Mantenha ativo o ambiente Python usado para instalar Jinja2 durante a configuração do GLAD.

```sh
cmake -S . -B build -G Ninja
cmake --build build --parallel
```

O CMake usa o padrão C17 e desativa extensões do compilador. O alvo do aplicativo é compilado com `/W4` no MSVC ou `-Wall -Wextra -Wpedantic` nos demais compiladores.

### Configurar a duração da campanha

`SPACE_INVADERS_FINAL_WAVE` é uma configuração positiva do cache CMake e tem valor padrão `5`:

```sh
cmake -S . -B build -G Ninja -DSPACE_INVADERS_FINAL_WAVE=8
```

## Executar

O aplicativo carrega os shaders usando caminhos relativos ao diretório de trabalho. Execute-o a partir da raiz do repositório, que contém `shaders/`, ou do diretório de build, onde o CMake copia os shaders:

```sh
./build/bin/space_invaders
```

No Windows, execute `.\build\bin\space_invaders.exe` no PowerShell a partir da raiz do repositório.

## Controles

| Tecla | Ação |
| --- | --- |
| ENTER | Inicia no menu, retoma durante a pausa ou reinicia após Game Over |
| A ou Seta para a esquerda | Move para a esquerda |
| D ou Seta para a direita | Move para a direita |
| SPACE | Dispara |
| ESC | Pausa |

O jogo começa com três vidas. Eliminar um inimigo comum vale 10 pontos. Um invasor que alcance a borda inferior da tela ou colida com a nave custa uma vida. Ao concluir a última onda configurada, o jogo exibe a tela de vitória.

## Testes

Compile o projeto e execute a suíte headless de testes a partir da raiz do repositório:

```sh
ctest --test-dir build --output-on-failure
```

Consulte [TESTING.md](TESTING.md) para detalhes sobre cobertura e limitações dos testes.

## Registro opcional da taxa de quadros

Defina `SPACE_INVADERS_LOG_FPS` com um valor não vazio antes de iniciar o aplicativo para imprimir a média de quadros por segundo em intervalos de cinco segundos no fluxo de erro padrão:

```powershell
$env:SPACE_INVADERS_LOG_FPS = "1"
.\build\bin\space_invaders.exe
```

```sh
SPACE_INVADERS_LOG_FPS=1 ./build/bin/space_invaders
```

Consulte [ARCHITECTURE.md](ARCHITECTURE.md) para conhecer as relações entre módulos e o fluxo de execução. Para as interfaces públicas em C, consulte [API_REFERENCE.md](API_REFERENCE.md).