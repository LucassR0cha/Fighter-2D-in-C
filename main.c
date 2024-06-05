#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>

const int larguraTela = 800;          // Define a largura da tela do jogo
const int alturaTela = 400;           // Define a altura da tela do jogo

const float velocidadeCavaleiro = 4.5;  // Define a velocidade de movimento do cavaleiro
const float velocidadeGoblin = 1.5;     // Define a velocidade de movimento do goblin (ajustada para ser menor)
const int gravidade = 1;              // Define a força da gravidade aplicada ao cavaleiro
const int posicaoCoordenadaY = (2 * larguraTela) / 5;  // Define a posição Y onde o cavaleiro fica (cerca de 2/9 da largura da tela)

const int puloFrameSuperior = 3;      // Define o índice do frame quando o cavaleiro está no pulo superior
const int puloFrameInferior = 10;     // Define o índice do frame quando o cavaleiro está no pulo inferior

// Verifica se o cavaleiro está no chão
bool personagemNoChao(Texture2D *personagem, Vector2 *posicaoPersonagem)
{
    return posicaoPersonagem->y + personagem->height >= posicaoCoordenadaY;
}

// Verifica se as imagens são válidas
bool imagemValida(const Texture2D *img)
{
    return img->id > 0;
}

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

// Função para desenhar a barra de HP
void drawHpBar(Vector2 position, int hp, int maxHp, Color color)
{
    float barWidth = 200.0f;
    float barHeight = 20.0f;
    float hpPercentage = (float)hp / maxHp;

    DrawRectangle(position.x, position.y, barWidth, barHeight, LIGHTGRAY);
    DrawRectangle(position.x, position.y, barWidth * hpPercentage, barHeight, color);
    DrawRectangleLines(position.x, position.y, barWidth, barHeight, BLACK);
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
    const char *arquivoGoblinCorreDir = "imagens/goblin/goblinCorreEsq.png";
    Texture2D goblinCorreDir = LoadTexture(arquivoGoblinCorreDir);
    const char *arquivoGoblinCorreEsq = "imagens/goblin/goblinCorreDir.png";
    Texture2D goblinCorreEsq = LoadTexture(arquivoGoblinCorreEsq);
    const char *arquivoGoblinAtaqueDir = "imagens/goblin/ataqueGoblinDir.png";
    Texture2D goblinAtaqueDir = LoadTexture(arquivoGoblinAtaqueDir);
    const char *arquivoGoblinAtaqueEsq = "imagens/goblin/ataqueGoblinEsq.png";
    Texture2D goblinAtaqueEsq = LoadTexture(arquivoGoblinAtaqueEsq);

    if (!imagemValida(&goblinParadoDir)) erroCarregarArquivo(arquivoGoblinParadoDir);
    if (!imagemValida(&goblinParadoEsq)) erroCarregarArquivo(arquivoGoblinParadoEsq);
    if (!imagemValida(&goblinCorreDir)) erroCarregarArquivo(arquivoGoblinCorreDir);
    if (!imagemValida(&goblinCorreEsq)) erroCarregarArquivo(arquivoGoblinCorreEsq);
    if (!imagemValida(&goblinAtaqueDir)) erroCarregarArquivo(arquivoGoblinAtaqueDir);
    if (!imagemValida(&goblinAtaqueEsq)) erroCarregarArquivo(arquivoGoblinAtaqueEsq);

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
    bool movendo = false;

    // Variáveis para o goblin
    unsigned qtdFramesParadoGoblin = 4;
    unsigned qtdFramesCorrendoGoblin = 8;
    unsigned qtdFramesAtaqueGoblin = 8;
    int tamanhoDoFrameParadoGoblin = goblinParadoDir.width / qtdFramesParadoGoblin;
    int tamanhoDoFrameCorrendoGoblin = goblinCorreDir.width / qtdFramesCorrendoGoblin;
    int tamanhoDoFrameAtaqueGoblin = goblinAtaqueDir.width / qtdFramesAtaqueGoblin;

    Rectangle movimentoFrameGoblin = {0.0f, 0.0f, (float)tamanhoDoFrameParadoGoblin, (float)goblinParadoDir.height};
    Vector2 posicaoGoblin = {larguraTela * 4 / 5.0f, posicaoCavaleiro.y - 20.0f};

    bool goblinViradoParaDireita = false;
    bool goblinAtacando = false;
    bool goblinMovendo = false;

    // Variáveis para HP
    int hpCavaleiro = 100;
    int maxHpCavaleiro = 100;
    int hpGoblin = 100;
    int maxHpGoblin = 100;

    while (!WindowShouldClose())
    {
        movendo = false;  // Reseta o estado de movimento do cavaleiro
        goblinMovendo = false;  // Reseta o estado de movimento do goblin

        // Atualiza o movimento do cavaleiro
        if (IsKeyDown(KEY_RIGHT))
        {
            posicaoCavaleiro.x += velocidadeCavaleiro;
            viradoParaDireita = true;
            movendo = true;
        }
        else if (IsKeyDown(KEY_LEFT))
        {
            posicaoCavaleiro.x -= velocidadeCavaleiro;
            viradoParaDireita = false;
            movendo = true;
        }

        // Verifica se o cavaleiro está no chão para permitir pular
        if (personagemNoChao(&cavaleiroAndandoDir, &posicaoCavaleiro))
        {
            if (IsKeyDown(KEY_SPACE))
            {
                aceleracaoCavaleiro.y = -15.0f;
            }
        }
        else
        {
            aceleracaoCavaleiro.y += gravidade;
        }

        // Atualiza a posição vertical do cavaleiro
        posicaoCavaleiro.y += aceleracaoCavaleiro.y;

        // Ajusta a posição do cavaleiro para que ele não saia do chão
        if (posicaoCavaleiro.y > posicaoCoordenadaY - cavaleiroAndandoDir.height)
        {
            posicaoCavaleiro.y = posicaoCoordenadaY - cavaleiroAndandoDir.height;
            aceleracaoCavaleiro.y = 0;
        }

        // Atualiza o estado de ataque do cavaleiro
        atacando = IsKeyPressed(KEY_A);

        // Atualiza a lógica de movimento do goblin
        if (posicaoGoblin.x > posicaoCavaleiro.x + 10)
        {
            posicaoGoblin.x -= velocidadeGoblin;
            goblinViradoParaDireita = false;
            goblinMovendo = true;
        }
        else if (posicaoGoblin.x < posicaoCavaleiro.x - 10)
        {
            posicaoGoblin.x += velocidadeGoblin;
            goblinViradoParaDireita = true;
            goblinMovendo = true;
        }

        // Atualiza o frame de movimento do cavaleiro
        if (++frameDelayCounter >= frameDelay)
        {
            frameDelayCounter = 0;
            frameIndex++;
            if (movendo)
            {
                if (frameIndex >= qtdFramesAndando) frameIndex = 0;
                movimentoFrameCavaleiro.x = (float)frameIndex * tamanhoDoFrameAndando;
            }
            else if (atacando)
            {
                if (frameIndex >= qtdFramesAtaque) frameIndex = 0;
                movimentoFrameCavaleiro.x = (float)frameIndex * tamanhoDoFrameAtaque;
            }
            else
            {
                if (frameIndex >= qtdFramesParado) frameIndex = 0;
                movimentoFrameCavaleiro.x = (float)frameIndex * tamanhoDoFrameParado;
            }

            if (goblinMovendo)
            {
                if (frameIndex >= qtdFramesCorrendoGoblin) frameIndex = 0;
                movimentoFrameGoblin.x = (float)frameIndex * tamanhoDoFrameCorrendoGoblin;
            }
            else if (goblinAtacando)
            {
                if (frameIndex >= qtdFramesAtaqueGoblin) frameIndex = 0;
                movimentoFrameGoblin.x = (float)frameIndex * tamanhoDoFrameAtaqueGoblin;
            }
            else
            {
                if (frameIndex >= qtdFramesParadoGoblin) frameIndex = 0;
                movimentoFrameGoblin.x = (float)frameIndex * tamanhoDoFrameParadoGoblin;
            }
        }

        // Lógica de ataque do cavaleiro
        if (atacando && CheckCollisionRecs(
                (Rectangle){posicaoCavaleiro.x, posicaoCavaleiro.y, tamanhoDoFrameAtaque, cavaleiroAtaqueDir.height},
                (Rectangle){posicaoGoblin.x, posicaoGoblin.y, tamanhoDoFrameParadoGoblin, goblinParadoDir.height}))
        {
            hpGoblin -= 10;
        }

        // Lógica de ataque do goblin
        goblinAtacando = CheckCollisionRecs(
            (Rectangle){posicaoGoblin.x + (goblinViradoParaDireita ? -10 : 10), posicaoGoblin.y, tamanhoDoFrameAtaqueGoblin - 30, goblinAtaqueDir.height - 20},
            (Rectangle){posicaoCavaleiro.x, posicaoCavaleiro.y, tamanhoDoFrameAndando, cavaleiroAndandoDir.height});

        if (goblinAtacando)
        {
            hpCavaleiro -= 1;
        }

        // Desenho dos elementos na tela
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenho da barra de HP do cavaleiro
        drawHpBar((Vector2){20, 20}, hpCavaleiro, maxHpCavaleiro, RED);
        DrawText("Cavaleiro", 20, 40, 20, BLACK);

        // Desenho da barra de HP do goblin
        drawHpBar((Vector2){larguraTela - 220, 20}, hpGoblin, maxHpGoblin, GREEN);
        DrawText("Goblin", larguraTela - 220, 40, 20, BLACK);

        Texture2D *texturaCavaleiro;
        if (atacando)
        {
            texturaCavaleiro = viradoParaDireita ? &cavaleiroAtaqueDir : &cavaleiroAtaqueEsq;
            movimentoFrameCavaleiro.width = tamanhoDoFrameAtaque;
        }
        else if (movendo)
        {
            texturaCavaleiro = viradoParaDireita ? &cavaleiroAndandoDir : &cavaleiroAndandoEsq;
            movimentoFrameCavaleiro.width = tamanhoDoFrameAndando;
        }
        else
        {
            texturaCavaleiro = viradoParaDireita ? &cavaleiroParadoDir : &cavaleiroParadoEsq;
            movimentoFrameCavaleiro.width = tamanhoDoFrameParado;
        }
        DrawTextureRec(*texturaCavaleiro, movimentoFrameCavaleiro, posicaoCavaleiro, WHITE);

        Texture2D *texturaGoblin;
        if (goblinAtacando)
        {
            texturaGoblin = goblinViradoParaDireita ? &goblinAtaqueDir : &goblinAtaqueEsq;
            movimentoFrameGoblin.width = tamanhoDoFrameAtaqueGoblin;
        }
        else if (goblinMovendo)
        {
            texturaGoblin = goblinViradoParaDireita ? &goblinCorreDir : &goblinCorreEsq;
            movimentoFrameGoblin.width = tamanhoDoFrameCorrendoGoblin;
        }
        else
        {
            texturaGoblin = goblinViradoParaDireita ? &goblinParadoDir : &goblinParadoEsq;
            movimentoFrameGoblin.width = tamanhoDoFrameParadoGoblin;
        }
        DrawTextureRec(*texturaGoblin, movimentoFrameGoblin, posicaoGoblin, WHITE);

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
    UnloadTexture(goblinAtaqueDir);
    UnloadTexture(goblinAtaqueEsq);

    CloseWindow();
    return 0;
}
