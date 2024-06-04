#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>

const int larguraTela = 800;          // Define a largura da tela do jogo
const int alturaTela = 400;           // Define a altura da tela do jogo

const float velocidadeCavaleiro = 4.5;  // Define a velocidade de movimento do cavaleiro
const float velocidadeGoblin = 3.0;     // Define a velocidade de movimento do goblin
const int gravidade = 1;              // Define a força da gravidade aplicada ao cavaleiro
const int posicaoCoordenadaY = (2 * larguraTela) / 5;  // Define a posição Y onde o cavaleiro fica (cerca de 2/9 da largura da tela)

const int puloFrameSuperior = 3;      // Define o índice do frame quando o cavaleiro está no pulo superior
const int puloFrameInferior = 10;     // Define o índice do frame quando o cavaleiro está no pulo inferior

// Verifica se o cavaleiro está no chão
bool personagemNoChao(Texture2D *personagem, Vector2 *posicaoPersonagem)
{
    return posicaoPersonagem->y + personagem->height >= posicaoCoordenadaY;
}

// Verifica se a imagem é válida
bool imagemValida(const Texture2D *img)
{
    return img->id > 0;
}

// Mostra uma mensagem de erro e encerra o programa
void mostrarErroESair(const char *msgErro)
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(msgErro, 20, 20, 20, BLACK);
        EndDrawing();
    }
    exit(EXIT_FAILURE);
}

// Mostra uma mensagem de erro específica para falha ao carregar um arquivo
void erroCarregarArquivo(const char *arquivo)
{
    mostrarErroESair(TextFormat("ERRO: NÃO FOI POSSÍVEL CARREGAR O ARQUIVO %s.", arquivo));
}

void finalizar()
{
    CloseWindow();
}

// Função principal
int main()
{
    InitWindow(larguraTela, alturaTela, "Teste Personagem 2D");  // Inicializa a janela do jogo
    SetTargetFPS(60);  // Define a taxa de quadros por segundo
    atexit(finalizar);  // Chama a função finalizar() na saída do programa

    // Carregamento das texturas do cavaleiro
    const char *arquivoAndandoDir = "imagens/cavaleiro/andandoDir.png";
    Texture2D cavaleiroAndandoDir = LoadTexture(arquivoAndandoDir);
    const char *arquivoAndandoEsq = "imagens/cavaleiro/andandoEsq.png";
    Texture2D cavaleiroAndandoEsq = LoadTexture(arquivoAndandoEsq);
    const char *arquivoParadoDir = "imagens/cavaleiro/paradoDir.png";
    Texture2D cavaleiroParadoDir = LoadTexture(arquivoParadoDir);
    const char *arquivoParadoEsq = "imagens/cavaleiro/paradoEsq.png";
    Texture2D cavaleiroParadoEsq = LoadTexture(arquivoParadoEsq);
    const char *arquivoAtaqueDir = "imagens/cavaleiro/ataqueDir.png";
    Texture2D cavaleiroAtaqueDir = LoadTexture(arquivoAtaqueDir);
    const char *arquivoAtaqueEsq = "imagens/cavaleiro/ataqueEsq.png";
    Texture2D cavaleiroAtaqueEsq = LoadTexture(arquivoAtaqueEsq);

    if (!imagemValida(&cavaleiroAndandoDir)) erroCarregarArquivo(arquivoAndandoDir);
    if (!imagemValida(&cavaleiroAndandoEsq)) erroCarregarArquivo(arquivoAndandoEsq);
    if (!imagemValida(&cavaleiroParadoDir)) erroCarregarArquivo(arquivoParadoDir);
    if (!imagemValida(&cavaleiroParadoEsq)) erroCarregarArquivo(arquivoParadoEsq);
    if (!imagemValida(&cavaleiroAtaqueDir)) erroCarregarArquivo(arquivoAtaqueDir);
    if (!imagemValida(&cavaleiroAtaqueEsq)) erroCarregarArquivo(arquivoAtaqueEsq);

    // Carregamento das texturas do goblin
    const char *arquivoGoblinParadoDir = "imagens/goblin/goblinParadoDir.png";
    Texture2D goblinParadoDir = LoadTexture(arquivoGoblinParadoDir);
    const char *arquivoGoblinParadoEsq = "imagens/goblin/goblinParadoEsq.png";
    Texture2D goblinParadoEsq = LoadTexture(arquivoGoblinParadoEsq);
    const char *arquivoGoblinCorreDir = "imagens/goblin/goblinCorreDir.png";
    Texture2D goblinCorreDir = LoadTexture(arquivoGoblinCorreDir);
    const char *arquivoGoblinCorreEsq = "imagens/goblin/goblinCorreEsq.png";
    Texture2D goblinCorreEsq = LoadTexture(arquivoGoblinCorreEsq);

    if (!imagemValida(&goblinParadoDir)) erroCarregarArquivo(arquivoGoblinParadoDir);
    if (!imagemValida(&goblinParadoEsq)) erroCarregarArquivo(arquivoGoblinParadoEsq);
    if (!imagemValida(&goblinCorreDir)) erroCarregarArquivo(arquivoGoblinCorreDir);
    if (!imagemValida(&goblinCorreEsq)) erroCarregarArquivo(arquivoGoblinCorreEsq);

    // Variáveis para o cavaleiro
    unsigned qtdFramesAndando = 10;
    unsigned qtdFramesParado = 10;
    unsigned qtdFramesAtaque = 4;
    int tamanhoDoFrameAndando = cavaleiroAndandoDir.width / qtdFramesAndando;
    int tamanhoDoFrameParado = cavaleiroParadoDir.width / qtdFramesParado;
    int tamanhoDoFrameAtaque = cavaleiroAtaqueDir.width / qtdFramesAtaque;

    Rectangle movimentoFrameCavaleiro = {0.0f, 0.0f, (float)tamanhoDoFrameAndando, (float)cavaleiroAndandoDir.height};
    Vector2 posicaoCavaleiro = {larguraTela / 20.0f, posicaoCoordenadaY - cavaleiroAndandoDir.height};
    Vector2 aceleracaoCavaleiro = {0.0f, 0.0f};

    unsigned frameDelay = 5;
    unsigned frameDelayCounter = 0;
    unsigned frameIndex = 0;
    bool viradoParaDireita = true;
    bool atacando = false;

    // Variáveis para o goblin
    unsigned qtdFramesParadoGoblin = 4;
    unsigned qtdFramesCorrendoGoblin = 8;
    int tamanhoDoFrameParadoGoblin = goblinParadoDir.width / qtdFramesParadoGoblin;
    int tamanhoDoFrameCorrendoGoblin = goblinCorreDir.width / qtdFramesCorrendoGoblin;

    Rectangle movimentoFrameGoblin = {0.0f, 0.0f, (float)tamanhoDoFrameParadoGoblin, (float)goblinParadoDir.height};
    Vector2 posicaoGoblin = {larguraTela * 4 / 5.0f, posicaoCavaleiro.y - 20.0f};


    bool goblinViradoParaDireita = false;

    // Variável para a contagem regressiva
    int countdown = 180;

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        // Atualização do cavaleiro
        if (personagemNoChao(&cavaleiroAndandoDir, &posicaoCavaleiro))
        {
            if (IsKeyPressed(KEY_A))
            {
                atacando = true;
                frameIndex = 0;
            }

            if (!atacando)
            {
                if (IsKeyDown(KEY_UP) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
                {
                    aceleracaoCavaleiro.y = -2 * velocidadeCavaleiro;
                }
                if (IsKeyDown(KEY_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
                {
                    aceleracaoCavaleiro.x = velocidadeCavaleiro;
                    viradoParaDireita = true;
                }
                else if (IsKeyDown(KEY_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
                {
                    aceleracaoCavaleiro.x = -velocidadeCavaleiro;
                    viradoParaDireita = false;
                }
                else
                {
                    aceleracaoCavaleiro.x = 0;
                }
            }
            else
            {
                aceleracaoCavaleiro.x = 0;
            }
        }

        bool cavaleiroMovendo = aceleracaoCavaleiro.x != 0.0f || aceleracaoCavaleiro.y != 0.0f;
        bool cavaleiroEstavaNoChao = personagemNoChao(&cavaleiroAndandoDir, &posicaoCavaleiro);
        posicaoCavaleiro = Vector2Add(posicaoCavaleiro, aceleracaoCavaleiro);
        bool cavaleiroEstaNoChao = personagemNoChao(&cavaleiroAndandoDir, &posicaoCavaleiro);

        if (cavaleiroEstaNoChao)
        {
            aceleracaoCavaleiro.y = 0;
            posicaoCavaleiro.y = posicaoCoordenadaY - cavaleiroAndandoDir.height;
        }
        else
        {
            aceleracaoCavaleiro.y += gravidade;
        }

        // Atualização do goblin após a contagem regressiva
        if (countdown <= 0)
        {
            if (posicaoGoblin.x < posicaoCavaleiro.x)
            {
                goblinViradoParaDireita = false;
                posicaoGoblin.x += velocidadeGoblin;
            }
            else if (posicaoGoblin.x > posicaoCavaleiro.x)
            {
                goblinViradoParaDireita = true;
                posicaoGoblin.x -= velocidadeGoblin;
            }

            bool goblinMovendo = posicaoGoblin.x != larguraTela * 4 / 5.0f;
        }

        ++frameDelayCounter;
        if (frameDelayCounter > frameDelay)
        {
            frameDelayCounter = 0;

            if (atacando)
            {
                frameIndex++;
                if (frameIndex >= qtdFramesAtaque)
                {
                    frameIndex = 0;
                    atacando = false;
                }
                movimentoFrameCavaleiro.width = tamanhoDoFrameAtaque;
                movimentoFrameCavaleiro.x = frameIndex * tamanhoDoFrameAtaque;
            }
            else if (cavaleiroMovendo)
            {
                if (cavaleiroEstaNoChao)
                {
                    frameIndex = (frameIndex + 1) % qtdFramesAndando;
                    movimentoFrameCavaleiro.width = tamanhoDoFrameAndando;
                    movimentoFrameCavaleiro.x = frameIndex * tamanhoDoFrameAndando;
                }
                else
                {
                    frameIndex = aceleracaoCavaleiro.y < 0 ? puloFrameSuperior : puloFrameInferior;
                    movimentoFrameCavaleiro.width = tamanhoDoFrameAndando;
                    movimentoFrameCavaleiro.x = frameIndex * tamanhoDoFrameAndando;
                }
            }
            else
            {
                frameIndex = (frameIndex + 1) % qtdFramesParado;
                movimentoFrameCavaleiro.width = tamanhoDoFrameParado;
                movimentoFrameCavaleiro.x = frameIndex * tamanhoDoFrameParado;
            }

            if (countdown <= 0)
            {
                frameIndex = (frameIndex + 1) % qtdFramesCorrendoGoblin;
                movimentoFrameGoblin.width = tamanhoDoFrameCorrendoGoblin;
                movimentoFrameGoblin.x = frameIndex * tamanhoDoFrameCorrendoGoblin;
            }
            else
            {
                frameIndex = (frameIndex + 1) % qtdFramesParadoGoblin;
                movimentoFrameGoblin.width = tamanhoDoFrameParadoGoblin;
                movimentoFrameGoblin.x = frameIndex * tamanhoDoFrameParadoGoblin;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenho do cavaleiro
        if (atacando)
        {
            if (viradoParaDireita)
            {
                DrawTextureRec(cavaleiroAtaqueDir, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);
            }
            else
            {
                DrawTextureRec(cavaleiroAtaqueEsq, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);
            }
        }
        else if (cavaleiroMovendo)
        {
            if (viradoParaDireita)
            {
                DrawTextureRec(cavaleiroAndandoDir, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);
            }
            else
            {
                DrawTextureRec(cavaleiroAndandoEsq, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);
            }
        }
        else
        {
            if (viradoParaDireita)
            {
                DrawTextureRec(cavaleiroParadoDir, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);
            }
            else
            {
                DrawTextureRec(cavaleiroParadoEsq, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);
            }
        }

        // Desenho do goblin
        if (countdown <= 0)
        {
            if (goblinViradoParaDireita)
            {
                DrawTextureRec(goblinCorreDir, movimentoFrameGoblin, posicaoGoblin, WHITE);
            }
            else
            {
                DrawTextureRec(goblinCorreEsq, movimentoFrameGoblin, posicaoGoblin, WHITE);
            }
        }
        else
        {
            if (goblinViradoParaDireita)
            {
                DrawTextureRec(goblinParadoDir, movimentoFrameGoblin, posicaoGoblin, WHITE);
            }
            else
            {
                DrawTextureRec(goblinParadoEsq, movimentoFrameGoblin, posicaoGoblin, WHITE);
            }

            int countdownNumber = countdown / 60 + 1;
            DrawText(TextFormat("%d", countdownNumber), larguraTela / 2 - 20, alturaTela / 2 - 20, 40, RED);
            countdown--;
        }

        EndDrawing();
    }

    UnloadTexture(cavaleiroAndandoDir);
    UnloadTexture(cavaleiroAndandoEsq);
    UnloadTexture(cavaleiroParadoDir);
    UnloadTexture(cavaleiroParadoEsq);
    UnloadTexture(cavaleiroAtaqueDir);
    UnloadTexture(cavaleiroAtaqueEsq);
    UnloadTexture(goblinParadoDir);
    UnloadTexture(goblinParadoEsq);
    UnloadTexture(goblinCorreDir);
    UnloadTexture(goblinCorreEsq);

    return 0;
}
