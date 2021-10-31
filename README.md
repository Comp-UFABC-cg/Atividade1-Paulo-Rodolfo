# ATIVIDADE 1

Este repositório contém o código fonte e link da aplicação rodando em <br>
WebAssembly.

A aplicação foi feita utilizando a seguinte arquitetura: C++, OpenGL e a biblioteca ABCG. <br>

__Membros:__

__Nome:__ Paulo Alexander Simões, <b>RA:</b>11084915 <br>
__Nome:__ Rodolfo Azevedo dos Santos, <b>RA:</b> 11093514


__Nome do Jogo:__ Starship Run <br>

<br>__Demonstração do Jogo:__ <br>

![image](https://user-images.githubusercontent.com/30665585/139577778-4c272a8a-3f76-4998-89d4-ad74d4fdee03.png)

<br>__Objetivo:__ Percorrer o mapa sem colidir com os asteróides. <br>

<br>__Funcionalidades e comandos:__
- Controle da nave através das setas do teclado (direita, esquerda, cima, baixo). Cada comando realiza as seguintes ações na nave, respectivamente: Move para a direita, move para a esquerda, acelera e freia.
- Tiros: A nave atira balas a partir de seus dois canhões. Os tiros são acionados a partir do botão esquerdo do mouse.
- Velocímetro: A velocidade é medida em metros/s (apresentada no visor superior esquerda) e aumenta/diminui conforme os comandos são realizados na nave

<br>__Estados possíveis durante o jogo:__
- Jogando: Quando a nave não colidiu com nenhum asteróide
- Perdeu: Quando a nave colide com algum asteróide
- Ganhou: Quando a nave destruiu todos asteróides sem ser atingida

<br>__Implementação:__

