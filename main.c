#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>

const int larguraTela = 800;
const int alturaTela = 400;
const float velocidadeCavaleiro = 4.5;
const float velocidadeGoblin = 1.5;
const int gravidade = 1;
const int posicaoCoordenadaY = (2 * larguraTela) / 5;
const int puloFrameSuperior = 3;
const int puloFrameInferior = 10;

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

// Erro caso os arquivos ao sejam carregados corretamente
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

int main()
{
    InitWindow(larguraTela, alturaTela, "Teste Personagem 2D");
    SetTargetFPS(60);
    atexit(finalizar);

    // texturas cavaleiro
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

    // textura dos goblins
    const char *arquivoGoblinParadoDir = "imagens/goblin/goblinParadoDir.png";
    Texture2D goblinParadoDir = LoadTexture(arquivoGoblinParadoDir);
    const char *arquivoGoblinParadoEsq = "imagens/goblin/goblinParadoEsq.png";
    Texture2D goblinParadoEsq = LoadTexture(arquivoGoblinParadoEsq);
    const char *arquivoGoblinCorreDir = "imagens/goblin/goblinCorreDir.png";
    Texture2D goblinCorreDir = LoadTexture(arquivoGoblinCorreDir);
    const char *arquivoGoblinCorreEsq = "imagens/goblin/goblinCorreEsq.png";
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

    // morte do goblin/ PS NAO ESTA FUNCIONANDO AINDA
    const char *arquivoMorteGoblin = "imagens/goblin/morteGoblin.png";
    Texture2D goblinMorte = LoadTexture(arquivoMorteGoblin);
    if (!imagemValida(&goblinMorte)) erroCarregarArquivo(arquivoMorteGoblin);

    // variaveis do cavaleiro
    unsigned qtdFramesAndando = 10;
    unsigned qtdFramesParado = 10;
    unsigned qtdFramesAtaque = 4;
    int tamanhoDoFrameAndando = cavaleiroAndandoDir.width / qtdFramesAndando;
    int tamanhoDoFrameParado = cavaleiroParadoDir.width / qtdFramesParado;
    int tamanhoDoFrameAtaque = cavaleiroAtaqueDir.width / qtdFramesAtaque;

    Rectangle movimentoFrameCavaleiro = {0.0f, 0.0f, (float)tamanhoDoFrameAndando, (float)cavaleiroAndandoDir.height};
    //posicao inical do cavaleiro
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
    unsigned qtdFramesMorteGoblin = 4;
    int tamanhoDoFrameParadoGoblin = goblinParadoDir.width / qtdFramesParadoGoblin;
    int tamanhoDoFrameCorrendoGoblin = goblinCorreDir.width / qtdFramesCorrendoGoblin;
    int tamanhoDoFrameAtaqueGoblin = goblinAtaqueDir.width / qtdFramesAtaqueGoblin;
    int tamanhoDoFrameMorteGoblin = goblinMorte.width / qtdFramesMorteGoblin;

    Rectangle movimentoFrameGoblin = {0.0f, 0.0f, (float)tamanhoDoFrameParadoGoblin, (float)goblinParadoDir.height};
    //posicao inicial do goblin
    Vector2 posicaoGoblin = {larguraTela * 4 / 5.0f, posicaoCavaleiro.y - 20.0f};

    //iniciando as variaveis de movimento, ataque e morte do goblin
    bool goblinViradoParaDireita = false;
    bool goblinAtacando = false;
    bool goblinMovendo = false;
    bool goblinMorto = false;

    // Variáveis para HP
    int hpCavaleiro = 400;
    int maxHpCavaleiro = 400;
    int hpGoblin = 100;
    int maxHpGoblin = 100;

    //Variáveis para os níveis do personagem
    const int expNiveis[10] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
    int nivelCavaleiro = 1;
    int expCavaleiro = 0;


    //Aqui começa o loop principal da logica do jogo
    while (!WindowShouldClose())
    {
        movendo = false;
        goblinMovendo = false;


        for (int i = nivelCavaleiro - 1; i < 10; i++)
        {
            if (expCavaleiro >= expNiveis[i])
            {
                nivelCavaleiro++;
                expCavaleiro -= expNiveis[i]; // Remove a experiência necessária para passar de nível
            }
            else
            {
                break; // Para o loop se o cavaleiro não tiver experiência suficiente para o próximo nível
            }
        }
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
                aceleracaoCavaleiro.y = -12.0f;
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

        // Lógica de ataque do cavaleiro
        if (atacando && CheckCollisionRecs((Rectangle)
    {
        posicaoCavaleiro.x, posicaoCavaleiro.y, tamanhoDoFrameAtaque, cavaleiroAtaqueDir.height
    }, (Rectangle)
    {
        posicaoGoblin.x, posicaoGoblin.y, tamanhoDoFrameParadoGoblin, goblinParadoDir.height
    }))
        {
            hpGoblin -= 10;
        }

        // Lógica de ataque do goblin
        goblinAtacando = CheckCollisionRecs((Rectangle)
        {
            posicaoGoblin.x + (goblinViradoParaDireita ? -10 : 10), posicaoGoblin.y, tamanhoDoFrameAtaqueGoblin - 150, goblinAtaqueDir.height - 20
        }, (Rectangle)
        {
            posicaoCavaleiro.x - 80, posicaoCavaleiro.y, tamanhoDoFrameAndando, cavaleiroAndandoDir.height
        });
        if (goblinAtacando)
        {
            hpCavaleiro -= 1;
        }

        // Verifica se o goblin morreu
        if (hpGoblin <= 0 && !goblinMorto)
        {
            goblinMorto = true;
            frameIndex = 0;
            expCavaleiro += 5;
        }

        // Atualiza a animação de morte do goblin se ele estiver morto PS: NAO ESTA FUNCIONANDO AINDA
        if (goblinMorto)
        {
            // Atualiza o frame de morte do goblin
            if (++frameDelayCounter >= frameDelay)
            {
                frameDelayCounter = 0;
                frameIndex++;
                if (frameIndex >= qtdFramesMorteGoblin)
                {
                    // Se a animação de morte acabou, reinicie a batalha com um novo goblin
                    goblinMorto = false;
                    hpGoblin = maxHpGoblin; // Restaura a vida do novo goblin
                    posicaoGoblin = (Vector2)
                    {
                        larguraTela * 4 / 5.0f, posicaoCavaleiro.y - 20.0f
                    }; // Posiciona o novo goblin
                }
                movimentoFrameGoblin.x = (float)frameIndex * tamanhoDoFrameMorteGoblin;
            }
        }
        else
        {
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

        //Colisão do ataque do cavaleiro
        if (atacando && CheckCollisionRecs(
                    (Rectangle)
    {
        posicaoCavaleiro.x + (viradoParaDireita ? tamanhoDoFrameAndando - 30 : 0), posicaoCavaleiro.y, 30, cavaleiroAtaqueDir.height
        },
        (Rectangle)
        {
            posicaoGoblin.x, posicaoGoblin.y, tamanhoDoFrameParadoGoblin, goblinParadoDir.height
        }))
        hpGoblin -= 10;

        // Lógica de ataque do goblin
        goblinAtacando = CheckCollisionRecs(
                             (Rectangle)
        {
            posicaoGoblin.x + (goblinViradoParaDireita ? -10 : 10), posicaoGoblin.y, 10, goblinAtaqueDir.height - 20
        },
        (Rectangle)
        {
            posicaoCavaleiro.x - 40, posicaoCavaleiro.y, tamanhoDoFrameAndando, cavaleiroAndandoDir.height
        });
        if (goblinAtacando)
            hpCavaleiro -= 1;


        //Começa a desenhar os elementos na tela
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Barra de HP do cavaleiro
        drawHpBar((Vector2)
        {
            20, 20
        }, hpCavaleiro, maxHpCavaleiro, RED);
        DrawText(TextFormat("Cavaleiro: %i/%i", hpCavaleiro, maxHpCavaleiro), 22, 30, 10, BLACK);

        DrawRectangleLines(20, 50, 200, 10, BLACK);
        float nivelPercentage = (float)expCavaleiro / expNiveis[nivelCavaleiro - 1];
        DrawRectangle(20, 50, 200 * nivelPercentage, 10, BLUE);
        DrawText(TextFormat("Nivel %i", nivelCavaleiro), 22, 40, 5, BLACK);

        // Barra de HP do goblin
        drawHpBar((Vector2)
        {
            larguraTela - 220, 20
        }, hpGoblin, maxHpGoblin, GREEN);
        DrawText(TextFormat("Goblin: %i/%i", hpGoblin, maxHpGoblin), 705, 30, 10, BLACK);

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

    //Descarregando as texturas ( é como se fosse um FREE só que nas texturas dos sprites)
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

