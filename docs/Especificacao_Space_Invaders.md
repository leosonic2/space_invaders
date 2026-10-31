# Especificação de Software -- Space Invaders

**Versão:** 1.0\
**Tipo de sistema:** Jogo eletrônico 2D\
**Nome do projeto:** Space Invaders\
**Plataforma:** Desktop (Windows/Linux)\
**Linguagem de programação:** C\
**Biblioteca gráfica:** OpenGL

------------------------------------------------------------------------

# 1. Introdução

## 1.1 Objetivo

Este documento apresenta a especificação funcional e técnica para o
desenvolvimento de um jogo inspirado no clássico *Space Invaders*,
utilizando a linguagem C e a biblioteca gráfica OpenGL.

O objetivo do jogo é controlar uma nave espacial que deve eliminar ondas
de invasores alienígenas antes que eles alcancem a parte inferior da
tela.

O jogador deverá movimentar a nave horizontalmente, disparar projéteis
contra os inimigos, acumular pontos e sobreviver ao maior número
possível de ondas de invasores.

## 1.2 Escopo do projeto

O sistema deverá implementar as seguintes funcionalidades:

-   Exibição de uma tela inicial.
-   Controle de movimentação da nave do jogador.
-   Disparo de projéteis.
-   Geração e movimentação de inimigos.
-   Detecção de colisões.
-   Sistema de pontuação.
-   Sistema de vidas.
-   Progressão de dificuldade.
-   Condição de vitória e derrota.
-   Reinicialização da partida.
-   Exibição da pontuação final.

## 1.3 Público-alvo

O jogo será destinado a usuários que desejam jogar um jogo de ação
arcade, com mecânica simples, controles intuitivos e dificuldade
progressiva.

# 2. Tecnologias e ferramentas

  Tecnologia           Finalidade
  -------------------- -----------------------------------
  C                    Linguagem principal
  OpenGL               Renderização gráfica 2D
  GLFW                 Gerenciamento de janela e eventos
  GLAD                 Carregamento de funções OpenGL
  CMake                Gerenciamento da compilação
  GCC / Clang / MSVC   Compilação do código

## 2.1 Arquitetura tecnológica

O sistema utilizará OpenGL para desenhar os elementos gráficos do jogo.

A biblioteca GLFW será responsável pela criação da janela, gerenciamento
do teclado e controle do ciclo de eventos.

A biblioteca GLAD será utilizada para carregar as funções da API OpenGL.

Recomenda-se utilizar OpenGL 3.3 Core Profile, com renderização 2D por
meio de shaders e primitivas gráficas.

# 3. Requisitos funcionais

Os requisitos funcionais descrevem as funcionalidades que o sistema
deverá disponibilizar.

  -----------------------------------------------------------------------
  ID                      Requisito               Descrição
  ----------------------- ----------------------- -----------------------
  RF001                   Tela inicial            O sistema deverá
                                                  apresentar uma tela
                                                  inicial com o título do
                                                  jogo.

  RF002                   Iniciar partida         O jogador deverá
                                                  conseguir iniciar uma
                                                  nova partida.

  RF003                   Movimentar nave         O jogador deverá
                                                  movimentar a nave
                                                  horizontalmente.

  RF004                   Disparar projéteis      O jogador deverá
                                                  conseguir disparar
                                                  projéteis contra os
                                                  inimigos.

  RF005                   Gerar inimigos          O sistema deverá gerar
                                                  uma formação de
                                                  inimigos
                                                  automaticamente.

  RF006                   Movimentar inimigos     Os inimigos deverão se
                                                  movimentar
                                                  horizontalmente e mudar
                                                  de direção ao atingir
                                                  as bordas da tela.

  RF007                   Aproximar inimigos      Os inimigos deverão
                                                  descer gradativamente
                                                  em direção à nave do
                                                  jogador.

  RF008                   Detectar colisões       O sistema deverá
                                                  identificar colisões
                                                  entre projéteis e
                                                  inimigos.

  RF009                   Pontuação               O sistema deverá
                                                  adicionar pontos quando
                                                  um inimigo for
                                                  eliminado.

  RF010                   Sistema de vidas        O jogador deverá
                                                  iniciar com três vidas.

  RF011                   Perder vida             O jogador deverá perder
                                                  uma vida quando os
                                                  invasores atingirem a
                                                  região inferior da tela
                                                  ou ocorrer uma colisão
                                                  prevista pelas regras
                                                  do jogo.

  RF012                   Progressão              O sistema deverá
                                                  aumentar a dificuldade
                                                  conforme as ondas forem
                                                  concluídas.

  RF013                   Vitória                 O sistema deverá
                                                  identificar a conclusão
                                                  de uma onda ou
                                                  campanha, conforme a
                                                  configuração do jogo.

  RF014                   Game Over               O sistema deverá
                                                  finalizar a partida
                                                  quando as vidas
                                                  chegarem a zero.

  RF015                   Reiniciar               O jogador deverá
                                                  conseguir iniciar uma
                                                  nova partida após o
                                                  Game Over.

  RF016                   Pausar                  O jogador deverá
                                                  conseguir pausar e
                                                  retomar a partida.
  -----------------------------------------------------------------------

# 4. Requisitos não funcionais

  -----------------------------------------------------------------------
  ID                      Requisito               Descrição
  ----------------------- ----------------------- -----------------------
  RNF001                  Linguagem               O sistema deverá ser
                                                  desenvolvido em
                                                  linguagem C.

  RNF002                  Renderização            A renderização gráfica
                                                  deverá utilizar OpenGL.

  RNF003                  Desempenho              O jogo deverá buscar
                                                  uma taxa de atualização
                                                  de 60 FPS.

  RNF004                  Resolução               O jogo deverá suportar
                                                  resolução mínima de 800
                                                  × 600 pixels.

  RNF005                  Compatibilidade         O software deverá
                                                  funcionar em sistemas
                                                  operacionais Windows e
                                                  Linux compatíveis com
                                                  as dependências
                                                  utilizadas.

  RNF006                  Usabilidade             Os comandos deverão ser
                                                  simples e intuitivos.

  RNF007                  Manutenibilidade        O código deverá ser
                                                  organizado em módulos
                                                  independentes.

  RNF008                  Portabilidade           O projeto deverá
                                                  utilizar bibliotecas
                                                  multiplataforma sempre
                                                  que possível.

  RNF009                  Estabilidade            O jogo deverá tratar
                                                  corretamente eventos de
                                                  encerramento e falhas
                                                  de inicialização
                                                  gráfica.

  RNF010                  Temporização            A movimentação deverá
                                                  ser independente da
                                                  taxa de quadros,
                                                  utilizando delta time.
  -----------------------------------------------------------------------

# 5. Regras de negócio

As regras de negócio definem o comportamento esperado durante uma
partida.

  -----------------------------------------------------------------------
  ID                      Regra                   Descrição
  ----------------------- ----------------------- -----------------------
  RN001                   Vidas iniciais          O jogador inicia a
                                                  partida com 3 vidas.

  RN002                   Pontuação               Cada inimigo eliminado
                                                  adiciona pontos ao
                                                  jogador.

  RN003                   Pontuação básica        Inimigo comum vale 10
                                                  pontos.

  RN004                   Movimento               A nave não poderá
                                                  ultrapassar os limites
                                                  horizontais da tela.

  RN005                   Disparos                O projétil deverá se
                                                  movimentar
                                                  verticalmente em
                                                  direção aos inimigos.

  RN006                   Colisão                 Um inimigo atingido por
                                                  um projétil deverá ser
                                                  removido da formação.

  RN007                   Movimento dos inimigos  Quando a formação
                                                  atingir uma das bordas,
                                                  deverá inverter a
                                                  direção e descer uma
                                                  posição.

  RN008                   Nova onda               Quando todos os
                                                  inimigos forem
                                                  eliminados, uma nova
                                                  onda deverá ser
                                                  iniciada.

  RN009                   Dificuldade             A velocidade dos
                                                  inimigos deverá
                                                  aumentar a cada nova
                                                  onda.

  RN010                   Game Over               A partida deverá
                                                  terminar quando o
                                                  jogador perder todas as
                                                  vidas.
  -----------------------------------------------------------------------

## 5.1 Sistema de pontuação sugerido

  Tipo de inimigo           Pontuação
  ----------------------- -----------
  Inimigo básico                   10
  Inimigo intermediário            20
  Inimigo avançado                 30
  Nave especial                   100

## 5.2 Sistema de dificuldade

A dificuldade deverá aumentar progressivamente.

-   Onda 1: velocidade inicial dos inimigos.
-   Onda 2: velocidade dos inimigos aumentada em 15%.
-   Onda 3: velocidade aumentada em mais 15%.
-   Onda 4: aumento da quantidade de inimigos.
-   Ondas seguintes: combinação de velocidade e quantidade.

# 6. Casos de uso

## 6.1 Diagrama geral de casos de uso

**Ator principal:** Jogador.

**Casos de uso identificados:**

-   UC001 -- Iniciar partida.
-   UC002 -- Movimentar nave.
-   UC003 -- Disparar projéteis.
-   UC004 -- Eliminar inimigos.
-   UC005 -- Visualizar pontuação.
-   UC006 -- Pausar partida.
-   UC007 -- Retomar partida.
-   UC008 -- Finalizar partida.
-   UC009 -- Reiniciar partida.

## 6.2 Descrição detalhada dos casos de uso

### UC001 -- Iniciar partida

  Campo          Descrição
  -------------- --------------------------------------
  Ator           Jogador
  Objetivo       Iniciar uma nova partida.
  Pré-condição   O jogo deverá estar na tela inicial.
  Pós-condição   A partida será iniciada.

**Fluxo principal:**

1.  O jogador abre o jogo.
2.  O sistema apresenta a tela inicial.
3.  O jogador pressiona ENTER.
4.  O sistema inicializa a nave.
5.  O sistema inicializa a pontuação em zero.
6.  O sistema configura três vidas.
7.  O sistema gera a primeira formação de inimigos.
8.  O jogo começa.

**Fluxo alternativo:**

-   Caso o jogador feche a janela, o sistema deverá encerrar a
    aplicação.

### UC002 -- Movimentar nave

  Campo          Descrição
  -------------- ------------------------------------
  Ator           Jogador
  Objetivo       Movimentar a nave horizontalmente.
  Pré-condição   A partida deverá estar ativa.
  Pós-condição   A posição da nave será atualizada.

**Fluxo principal:**

1.  O jogador pressiona a tecla A ou seta para esquerda.
2.  O sistema identifica o comando.
3.  O sistema atualiza a posição da nave para a esquerda.
4.  O jogador pressiona a tecla D ou seta para direita.
5.  O sistema atualiza a posição da nave para a direita.
6.  O sistema verifica os limites da tela.

**Fluxo alternativo:**

-   Caso a nave alcance a extremidade esquerda, não poderá continuar se
    movimentando nessa direção.
-   Caso a nave alcance a extremidade direita, não poderá continuar se
    movimentando nessa direção.

### UC003 -- Disparar projéteis

  Campo          Descrição
  -------------- ----------------------------------------
  Ator           Jogador
  Objetivo       Disparar contra os invasores.
  Pré-condição   A partida deverá estar ativa.
  Pós-condição   Um projétil será criado e movimentado.

**Fluxo principal:**

1.  O jogador pressiona a tecla ESPAÇO.
2.  O sistema verifica se o disparo está permitido.
3.  O sistema cria um projétil na posição da nave.
4.  O projétil é movimentado verticalmente.
5.  O sistema verifica colisões.
6.  Caso o projétil ultrapasse o limite superior da tela, será removido.

**Fluxo alternativo:**

-   Caso exista um limite de disparos simultâneos, o sistema deverá
    impedir a criação de novos projéteis até que exista capacidade
    disponível.

### UC004 -- Eliminar inimigos

  Campo          Descrição
  -------------- ----------------------------------------------------
  Ator           Sistema
  Objetivo       Remover inimigos atingidos por projéteis.
  Pré-condição   Existir pelo menos um inimigo e um projétil ativo.
  Pós-condição   O inimigo será removido e a pontuação atualizada.

**Fluxo principal:**

1.  O sistema verifica a posição dos projéteis.
2.  O sistema verifica a posição dos inimigos.
3.  O sistema identifica uma colisão.
4.  O sistema remove o projétil.
5.  O sistema remove o inimigo.
6.  O sistema adiciona pontos ao jogador.
7.  O sistema atualiza a pontuação exibida.

**Fluxo alternativo:**

-   Caso não exista colisão, o projétil continuará se movimentando.

### UC005 -- Visualizar pontuação

  Campo          Descrição
  -------------- ----------------------------------------
  Ator           Jogador
  Objetivo       Consultar a pontuação atual.
  Pré-condição   A partida deverá estar ativa.
  Pós-condição   A pontuação será exibida na interface.

**Fluxo principal:**

1.  O sistema consulta a pontuação atual.
2.  O sistema atualiza o componente gráfico.
3.  O jogador visualiza a pontuação na parte superior da tela.

### UC006 -- Pausar partida

  Campo          Descrição
  -------------- -------------------------------------
  Ator           Jogador
  Objetivo       Interromper temporariamente o jogo.
  Pré-condição   A partida deverá estar ativa.
  Pós-condição   O jogo ficará pausado.

**Fluxo principal:**

1.  O jogador pressiona ESC.
2.  O sistema altera o estado para PAUSADO.
3.  A movimentação dos inimigos é interrompida.
4.  A movimentação dos projéteis é interrompida.
5.  O sistema apresenta a mensagem PAUSADO.

### UC007 -- Retomar partida

  Campo          Descrição
  -------------- ------------------------------------
  Ator           Jogador
  Objetivo       Retomar uma partida pausada.
  Pré-condição   O jogo deverá estar pausado.
  Pós-condição   A partida voltará a ser executada.

**Fluxo principal:**

1.  O jogador pressiona ENTER.
2.  O sistema altera o estado para JOGANDO.
3.  O sistema retoma a movimentação dos elementos.
4.  O jogo continua do ponto em que foi pausado.

### UC008 -- Finalizar partida

  Campo          Descrição
  -------------- ----------------------------------------------------------
  Ator           Sistema
  Objetivo       Encerrar a partida quando todas as vidas forem perdidas.
  Pré-condição   O jogador deverá estar com zero vidas.
  Pós-condição   A tela de Game Over será apresentada.

**Fluxo principal:**

1.  O sistema identifica que as vidas chegaram a zero.
2.  O sistema altera o estado para GAME OVER.
3.  O sistema interrompe a movimentação dos elementos.
4.  O sistema apresenta a pontuação final.
5.  O sistema disponibiliza a opção de reiniciar.

### UC009 -- Reiniciar partida

  Campo          Descrição
  -------------- -------------------------------------------
  Ator           Jogador
  Objetivo       Iniciar uma nova partida.
  Pré-condição   O jogo deverá estar na tela de Game Over.
  Pós-condição   Uma nova partida será iniciada.

**Fluxo principal:**

1.  O jogador pressiona ENTER.
2.  O sistema limpa os projéteis existentes.
3.  O sistema reinicializa a pontuação.
4.  O sistema restaura as três vidas.
5.  O sistema recria a formação de inimigos.
6.  O jogo começa novamente.

# 7. Arquitetura do software

O software deverá ser organizado em módulos para facilitar a manutenção,
os testes e a evolução do projeto.

## 7.1 Organização dos módulos

  Módulo                 Responsabilidade
  ---------------------- ---------------------------------------------
  GAME / MAIN            Inicialização e controle do ciclo principal
  PLAYER                 Controle da nave e disparos
  ENEMY                  Criação e movimentação dos inimigos
  COLLISION              Detecção e tratamento de colisões
  GAME STATE             Controle dos estados da partida
  RENDERER               Renderização com OpenGL
  UI / HUD               Pontuação, vidas e mensagens
  OPENGL + GLFW + GLAD   Camada gráfica, janela e eventos de entrada

## 7.2 Estrutura de diretórios

``` text
space-invaders/
│
├── CMakeLists.txt
│
├── include/
│   ├── game.h
│   ├── player.h
│   ├── enemy.h
│   ├── projectile.h
│   ├── collision.h
│   ├── renderer.h
│   └── game_state.h
│
├── src/
│   ├── main.c
│   ├── game.c
│   ├── player.c
│   ├── enemy.c
│   ├── projectile.c
│   ├── collision.c
│   ├── renderer.c
│   └── game_state.c
│
├── shaders/
│   ├── vertex.glsl
│   └── fragment.glsl
│
├── assets/
│   ├── images/
│   └── fonts/
│
└── tests/
    ├── test_player.c
    ├── test_enemy.c
    ├── test_collision.c
    └── test_game.c
```

## 7.3 Principais estruturas de dados

### Estrutura do jogador

``` c
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
    int lives;
} Player;
```

### Estrutura do inimigo

``` c
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
    int active;
    int points;
} Enemy;
```

### Estrutura do projétil

``` c
typedef struct {
    float x;
    float y;
    float speed;
    int active;
} Projectile;
```

### Estado do jogo

``` c
typedef enum {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY
} GameState;
```

# 8. Fluxo de execução do jogo

O jogo deverá utilizar um ciclo principal (*Game Loop*) responsável por
atualizar a lógica e renderizar os elementos gráficos.

``` mermaid
flowchart TD
    A([Iniciar aplicação]) --> B[Inicializar GLFW e OpenGL]
    B --> C[Carregar shaders e recursos]
    C --> D[Exibir menu inicial]
    D --> E{Jogador iniciou?}
    E -- Não --> D
    E -- Sim --> F[Inicializar partida]
    F --> G[Capturar entradas]
    G --> H[Atualizar jogador e inimigos]
    H --> I[Atualizar projéteis]
    I --> J[Verificar colisões]
    J --> K[Atualizar pontuação e vidas]
    K --> L{Fim de partida?}
    L -- Não --> M[Renderizar cena]
    M --> G
    L -- Sim --> N[Exibir resultado]
    N --> O{Reiniciar?}
    O -- Sim --> F
    O -- Não --> P[Encerrar aplicação]
```

## 8.1 Pseudocódigo do Game Loop

``` c
while (!glfwWindowShouldClose(window)) {

    float deltaTime = calculateDeltaTime();

    processInput();

    if (gameState == PLAYING) {

        updatePlayer(deltaTime);

        updateEnemies(deltaTime);

        updateProjectiles(deltaTime);

        checkCollisions();

        updateScore();

        checkGameOver();

        checkVictory();
    }

    renderGame();

    glfwSwapBuffers(window);

    glfwPollEvents();
}
```

O parâmetro `deltaTime` deverá representar o tempo transcorrido entre
dois quadros, garantindo que a movimentação dos objetos não dependa
diretamente do FPS.

# 9. Especificação gráfica

## 9.1 Elementos visuais

O jogo deverá apresentar os seguintes elementos:

  -----------------------------------------------------------------------
  Elemento                            Descrição
  ----------------------------------- -----------------------------------
  Nave do jogador                     Representação gráfica da nave
                                      controlada pelo jogador.

  Inimigos                            Aliens organizados em formação.

  Projéteis                           Elementos gráficos que representam
                                      os disparos.

  Fundo                               Cenário espacial com estrelas.

  HUD                                 Área de informações com pontuação,
                                      vidas e onda atual.

  Menu                                Interface para iniciar ou reiniciar
                                      a partida.
  -----------------------------------------------------------------------

## 9.2 Layout da tela

Resolução de referência: 1024 × 768 pixels.

Distribuição sugerida:

-   Parte superior: pontuação, vidas e onda atual.
-   Região central: formação de inimigos.
-   Região inferior: nave do jogador.
-   Fundo: cenário espacial.

# 10. Plano de testes

Os testes deverão validar o funcionamento dos requisitos funcionais, as
regras de negócio e a estabilidade do sistema.

## 10.1 Testes unitários

Os testes unitários deverão validar individualmente os módulos do
software.

  -----------------------------------------------------------------------
  ID                Módulo            Cenário           Resultado
                                                        esperado
  ----------------- ----------------- ----------------- -----------------
  TU001             Player            Movimentar nave   Coordenada X
                                      para esquerda.    diminui.

  TU002             Player            Movimentar nave   Coordenada X
                                      para direita.     aumenta.

  TU003             Player            Movimentar nave   Nave permanece
                                      além do limite    dentro da tela.
                                      esquerdo.         

  TU004             Player            Movimentar nave   Nave permanece
                                      além do limite    dentro da tela.
                                      direito.          

  TU005             Projectile        Criar projétil.   Projétil recebe
                                                        posição inicial
                                                        correta.

  TU006             Projectile        Atualizar posição Coordenada Y é
                                      do projétil.      atualizada
                                                        conforme a
                                                        velocidade.

  TU007             Enemy             Criar inimigo.    Inimigo recebe
                                                        posição e
                                                        atributos
                                                        iniciais.

  TU008             Enemy             Atingir limite    Formação muda de
                                      horizontal.       direção.

  TU009             Collision         Projétil colide   Colisão
                                      com inimigo.      identificada.

  TU010             Collision         Projétil não      Nenhuma colisão
                                      atinge inimigo.   identificada.

  TU011             Score             Eliminar inimigo  Pontuação aumenta
                                      básico.           em 10 pontos.

  TU012             Game              Perder última     Estado passa para
                                      vida.             GAME_OVER.
  -----------------------------------------------------------------------

## 10.2 Testes de integração

  -----------------------------------------------------------------------
  ID                Cenário           Procedimento      Resultado
                                                        esperado
  ----------------- ----------------- ----------------- -----------------
  TI001             Integração        Movimentar nave e Projétil é criado
                    jogador e         disparar.         na posição da
                    projétil                            nave.

  TI002             Integração        Disparar contra   Inimigo é
                    projétil e        inimigo.          eliminado.
                    inimigo                             

  TI003             Integração        Eliminar inimigo. Pontuação é
                    colisão e                           atualizada.
                    pontuação                           

  TI004             Integração        Eliminar todos os Nova onda é
                    inimigos e        inimigos.         criada com
                    dificuldade                         dificuldade
                                                        maior.

  TI005             Integração vidas  Perder todas as   Tela de Game Over
                    e Game Over       vidas.            é apresentada.

  TI006             Integração pausa  Pausar e retomar  O jogo interrompe
                    e retomada        a partida.        e retoma
                                                        corretamente.
  -----------------------------------------------------------------------

## 10.3 Testes funcionais

  -----------------------------------------------------------------------
  ID                      Funcionalidade          Resultado esperado
  ----------------------- ----------------------- -----------------------
  TF001                   Abrir aplicação         Tela inicial
                                                  apresentada.

  TF002                   Iniciar jogo            Primeira onda
                                                  apresentada.

  TF003                   Movimentar nave         Nave movimenta conforme
                                                  o comando.

  TF004                   Disparar                Projéteis são
                                                  apresentados na tela.

  TF005                   Eliminar inimigos       Inimigos desaparecem
                                                  após colisão válida.

  TF006                   Pontuação               Pontos são
                                                  contabilizados
                                                  corretamente.

  TF007                   Perder vida             Quantidade de vidas é
                                                  decrementada.

  TF008                   Game Over               Mensagem de fim de jogo
                                                  apresentada.

  TF009                   Reiniciar               Nova partida iniciada
                                                  com atributos
                                                  reinicializados.

  TF010                   Pausar                  Movimentação dos
                                                  elementos é
                                                  interrompida.
  -----------------------------------------------------------------------

## 10.4 Testes não funcionais

  -----------------------------------------------------------------------
  ID                Tipo              Cenário           Critério de
                                                        aceitação
  ----------------- ----------------- ----------------- -----------------
  TNF001            Desempenho        Executar jogo com Manter
                                      quantidade padrão aproximadamente
                                      de inimigos.      60 FPS no
                                                        ambiente de
                                                        referência.

  TNF002            Desempenho        Executar jogo com Não apresentar
                                      100 projéteis     travamentos
                                      ativos.           significativos.

  TNF003            Estabilidade      Jogar             Não ocorrer
                                      continuamente por encerramento
                                      30 minutos.       inesperado.

  TNF004            Compatibilidade   Executar no       Inicialização e
                                      Windows.          execução
                                                        corretas.

  TNF005            Compatibilidade   Executar no       Inicialização e
                                      Linux.            execução
                                                        corretas.

  TNF006            Usabilidade       Utilizar os       Comandos
                                      controles         respondem
                                      disponíveis.      corretamente.

  TNF007            Recursos          Encerrar a        Recursos de
                                      aplicação.        memória e
                                                        contexto gráfico
                                                        são liberados.
  -----------------------------------------------------------------------

# 11. Critérios de aceite

O projeto será considerado concluído quando atender aos seguintes
critérios:

1.  O software estiver desenvolvido em linguagem C.
2.  A renderização gráfica utilizar OpenGL.
3.  O jogador conseguir movimentar a nave horizontalmente.
4.  O jogador conseguir disparar projéteis.
5.  Os inimigos forem criados e movimentados automaticamente.
6.  O sistema identificar corretamente as colisões.
7.  A pontuação for atualizada corretamente.
8.  O sistema controlar as vidas do jogador.
9.  O jogo apresentar dificuldade progressiva.
10. O sistema apresentar corretamente os estados de pausa, vitória e
    Game Over.
11. O jogador conseguir reiniciar a partida.
12. Os testes unitários, de integração e funcionais forem executados com
    sucesso.

# 12. Considerações sobre implementação

Alguns aspectos técnicos deverão ser observados durante o
desenvolvimento:

**Renderização OpenGL**

Recomenda-se utilizar Vertex Buffer Objects (VBO), Vertex Array Objects
(VAO) e shaders para desenhar os elementos 2D.

**Detecção de colisões**

Uma abordagem simples é utilizar AABB (*Axis-Aligned Bounding Box*),
comparando os retângulos que representam os objetos.

**Gerenciamento de memória**

Como a linguagem C não possui gerenciamento automático de memória, o
projeto deverá controlar explicitamente a alocação e liberação de
estruturas dinâmicas, principalmente para projéteis e inimigos.

**Separação de responsabilidades**

A lógica do jogo deverá ser independente da renderização gráfica. Dessa
forma, será possível testar movimentação, colisões e pontuação sem
depender da inicialização do OpenGL.

**Temporização**

Todas as movimentações deverão considerar o delta time, evitando
diferenças de velocidade entre computadores com diferentes capacidades
gráficas.

# 13. Evoluções futuras

Após a implementação da versão inicial, poderão ser desenvolvidas as
seguintes funcionalidades:

-   Ranking de melhores pontuações.
-   Salvamento de recordes em arquivo.
-   Diferentes tipos de inimigos.
-   Chefes (*Bosses*) ao final de determinadas ondas.
-   Power-ups e armas especiais.
-   Efeitos sonoros e música de fundo.
-   Animações de explosão.
-   Tela de configurações.
-   Suporte a joystick ou controle USB.
-   Diferentes níveis de dificuldade.

# 14. Definição de conclusão do projeto

A versão 1.0 do Space Invaders deverá entregar um jogo 2D funcional,
desenvolvido em C e OpenGL, contemplando movimentação, disparos,
inimigos, colisões, pontuação, vidas, progressão de dificuldade e
gerenciamento dos estados da partida.

A arquitetura modular permitirá a evolução do sistema sem comprometer as
funcionalidades já implementadas.

**Próximo passo recomendado:** elaborar o projeto técnico detalhado, com
interfaces das funções C, estruturas de dados completas e implementação
dos módulos `player`, `enemy`, `projectile`, `collision` e `renderer`.
