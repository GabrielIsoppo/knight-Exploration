# Trabalho A3(Programação 2 - 2026/1)
- Jogo platformer 2D usando a biblioteca ALLEGRO;

- Composição:
    - 1 personagem principal:
        - Possui aceleração;
        - Apenas corre, não tem opção de andar;
        - Pode agachar e andar agachado;
        - Pulo ajustável ao tempo que o botão de pular está pressionado;


    - 6 tipos de obstáculos:
        - Inimigo móvel;
        - Inimigo Imóvel;
        - Inimigo voador;
        - Lava;
        - Espinho;
        - Buracos;
        OBS: Todos os inimigos, exceto a lava, causam 1 de dano. A lava mata instantaneamente.

    - 2 tipos de platformas:
        - Chão (plataformas voadoras usam a mesma mecânica que o chão);
        - Escadas (escaléveis);

- Janela do jogo tem tamanho fixo de 1024x768 (4:3)

- Menu:
    - 3 opções iniciais:
        - Começar Jogo;
        - Opções (dificuldades);
        - Fechar Jogo;
    
    - Ao selecionar Opções, abre o menu de dificuldade, com 3 opções:
        - Fácil;
        - Médio;
        - Difícil;
        OBS: a dificuldade altera a velocidade dos inimigos, fácil->lento, difícil->rápido.

- Apenas 1 fase:
    - 9 inimigos:
        - 2 Imóveis;
        - 3 Móveis;
        - 1 Lava;
        - 1 Espinho;
        - 2 Voadores;
    - 1 Buraco;
    - 11 Plataformas;
    - 1 Escada;

    - Para que a fase seja terminada basta o jogador alcançar o limite direito da tela;
        - Ao chegar lá é mostrada a tela de vitória;
    
    - Caso o jogador morra, a tela de derrota é mostrada;
    OBS: Ambas as telas levam ao menu novamente.

## Créditos das sprites:
1 - Jogador:        artista: https://jumpbutton.itch.io/;       link: https://jumpbutton.itch.io/girlknightasset
2 - Inimigo Imóvel: artista: https://free-game-assets.itch.io/; link: https://free-game-assets.itch.io/free-tiny-hero-sprites-pixel-art?download
3 - Inimigo Móvel:  artista: https://free-game-assets.itch.io/; link: https://free-game-assets.itch.io/free-tiny-hero-sprites-pixel-art?download
4 - Inimigo Voador: artista: https://rekaalgames.itch.io/;      link: https://rekaalgames.itch.io/2d-flying-enemy
5 - Espinho:        artista: https://froggu999.itch.io/;        link: https://froggu999.itch.io/stabby-spikes
6 - Lava:           artista: https://martixs.itch.io/;          link: https://martixs.itch.io/lava
7 - Plataforma:     artista: https://edermunizz.itch.io/;       link: https://edermunizz.itch.io/free-pixel-art-forest
8 - Escada:         artista: https://nyknck.itch.io/;           link: https://nyknck.itch.io/wood-set
9 - Background:     artista: https://free-game-assets.itch.io/; link: https://free-game-assets.itch.io/nature-landscapes-free-pixel-art

OBS: Todas as licenças podem ser encontradas na pasta licenses.
---

## ⚠️ Aviso de Integridade Acadêmica

* **Fins Educacionais e de Portfólio:** Este repositório foi tornado público estritamente como demonstração de aprendizado e portfólio pessoal de desenvolvimento. 
* **Uso por outros estudantes:** Se você é um estudante atual da **UFPR** (Universidade Federal do Paraná) ou de outra instituição e está cursando uma matéria com projeto semelhante, **recomendo fortemente que não copie este código**. A cópia parcial ou total deste projeto sem autorização da sua coordenação pode violar as políticas de integridade acadêmica da sua instituição, resultando em penalidades por plágio.
