# Space Invaders

Jogo desktop 2D em C17 com OpenGL, GLFW e GLAD. A lógica da partida é
independente da renderização e pode ser testada sem abrir uma janela.

## Pré-requisitos

- CMake 3.21 ou superior
- Ninja
- Git
- Compilador C17 (GCC, Clang ou MSVC)
- Python 3 e pip
- OpenGL de desenvolvimento

O CMake baixa GLFW 3.4 e GLAD 2.0.8 durante a primeira configuração. A geração
do GLAD também requer Jinja2; instale as dependências Python do projeto:

```powershell
py -3 -m pip install -r requirements-build.txt
```

No Ubuntu, instale também os pacotes do compilador, do Ninja, do OpenGL e do
GLFW:

```sh
sudo apt-get install build-essential cmake git ninja-build python3 python3-pip \
  libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
python3 -m pip install -r requirements-build.txt
```

## Configurar, compilar e executar

Execute a partir da raiz do repositório. Os comandos abaixo funcionam no
Windows (PowerShell) e no Linux:

```sh
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

### Windows: conflito de gerador ou NMake indisponível

Um diretório configurado com `NMake Makefiles` não pode ser reutilizado com
`-G Ninja`. Use um diretório novo, sem apagar o build anterior. O erro ao
executar `nmake` indica que essa ferramenta não está no PATH; com MSVC,
execute os comandos no Developer PowerShell do Visual Studio.

Para usar o GCC do MSYS2 UCRT64, execute no PowerShell:

```powershell
$env:PATH = "C:\msys64\ucrt64\bin;$env:PATH"
cmake -S . -B build-ninja -G Ninja -DCMAKE_C_COMPILER=C:\msys64\ucrt64\bin\gcc.exe -DBUILD_TESTING=ON
cmake --build build-ninja --parallel
ctest --test-dir build-ninja --output-on-failure --no-tests=error
.\build-ninja\bin\space_invaders.exe
```

Se CMake e Ninja foram instalados pelo pip e não são encontrados, adicione
também o diretório de scripts Python ao PATH da sessão:

```powershell
$pythonScripts = py -3 -c "import sysconfig; print(sysconfig.get_path('scripts', sysconfig.get_preferred_scheme('user')))"
$env:PATH = "$pythonScripts;$env:PATH"
```

Execute cada etapa somente após a anterior terminar com sucesso. Uma
configuração incompleta não registra os testes; `--no-tests=error` evita que
essa situação seja confundida com uma execução bem-sucedida.

Inicie o jogo:

```powershell
.\build\bin\space_invaders.exe
```

```sh
./build/bin/space_invaders
```

## Controles

| Tecla | Ação |
| --- | --- |
| ENTER | Iniciar no menu, retomar após pausa ou reiniciar após o fim |
| A ou seta esquerda | Mover a nave para a esquerda |
| D ou seta direita | Mover a nave para a direita |
| SPACE | Disparar |
| ESC | Pausar a partida |

## Campanha

A campanha tem cinco ondas por padrão. Para escolher outra quantidade, defina
`SPACE_INVADERS_FINAL_WAVE` ao configurar o build:

```sh
cmake -S . -B build -G Ninja -DSPACE_INVADERS_FINAL_WAVE=8
```

O jogador começa com três vidas. Invasores eliminados concedem pontos; perder
todas as vidas mostra a tela Game Over. Limpar a última onda configurada mostra
a tela Victory.

## Testes

Os testes CTest cobrem a configuração da campanha, a lógica do jogo, a
proporção do viewport e o texto apresentado em cada estado. Eles não exigem
uma janela OpenGL:

```sh
ctest --test-dir build --output-on-failure
```

Para registrar a média de quadros renderizados em janelas de cinco segundos,
defina `SPACE_INVADERS_LOG_FPS` antes de iniciar o jogo. O resultado é escrito
em stderr; inicie uma partida com ENTER para medir uma onda:

```powershell
$env:SPACE_INVADERS_LOG_FPS = "1"
.\build\bin\space_invaders.exe
```

```sh
SPACE_INVADERS_LOG_FPS=1 ./build/bin/space_invaders
```
