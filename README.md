# ATIVIDADE 1

Este repositório contém o código fonte e link da aplicação rodando em <br>
WebAssembly.

A aplicação foi implementada utilizando a seguinte arquitetura: C++, OpenGL e a biblioteca ABCG. <br>

__Membros:__

__Nome:__ Paulo Alexander Simões, <b>RA:</b>11084915 <br>
__Nome:__ Rodolfo Azevedo dos Santos, <b>RA:</b> 11093514

__Link para o jogo:__ https://comp-ufabc-cg.github.io/Atividade1-Paulo-Rodolfo/public/

__Nome do Jogo:__ Starship Run <br>

<br>__Demonstração do Jogo:__ <br>

![image](https://user-images.githubusercontent.com/30665585/139603272-f5a60399-ac66-44fc-b4f5-7023f0406ac9.png)

<br>__Objetivo:__ Percorrer o mapa sem colidir com os asteróides. <br>

<br>__Funcionalidades e comandos:__
- Controle da nave através das setas do teclado (direita, esquerda, cima, baixo). Cada comando realiza as seguintes ações na nave, respectivamente: Move para a direita, move para a esquerda, acelera e freia.
- Tiros: A nave atira balas a partir da sua parte superior central. Os tiros são acionados a partir do botão esquerdo do mouse ou tecla espaço do teclado.
- Velocímetro: A velocidade é medida em quilômetros/s (apresentada no visor superior esquerda) e aumenta/diminui conforme os comandos são realizados na nave

<br>__Estados possíveis durante o jogo:__
- Jogando: Quando a nave não colidiu com nenhum asteróide
- Perdeu: Quando a nave colide com algum asteróide
- Ganhou: Quando a nave destruiu todos asteróides sem ser atingida

<br>__Implementação:__

O velocímetro é implementado através do ImGui a partir da exibição da variável que armazena a velocidadade vertical da nave (m_velocity.y originada e atualizada em ship.cpp - sua implementação será mostrada ao longo do relatório). <br>
![image](https://user-images.githubusercontent.com/30665585/139600983-082bc03e-7056-4f13-a2b9-8b72298293d3.png)


As mensagens ao jogador são implementadas através do ImGui no qual, a partir da vitória ou derrota são exibidos os textos na tela: "Perdeu!", "Venceu!": <br>
<br>a)
 ![image](https://user-images.githubusercontent.com/30665585/139578338-33ca0ee6-611f-4a3d-ba9b-f593481fd17e.png)
<br>b)
 ![image](https://user-images.githubusercontent.com/30665585/139578344-14be6064-4a2a-4fd9-bc05-8a4d21a290cf.png)
 <br><br>Para que a mensagem seja exibida, o programa tem como entrada o m_state (implementado em gamedata.hpp):<br>
 ![image](https://user-images.githubusercontent.com/30665585/139578478-95c93a57-c9d3-4abe-a275-1b106559968e.png)

<br> Os comandos são captados através do OpenGLWindow::handleEvent (dentro de openglwindow.cpp), no qual cada evento do mouse e teclado são tratados e configurados para que uma ação seja executada de modo coerente com a proposta do jogo: <br>
<br>![image](https://user-images.githubusercontent.com/30665585/139578945-86ea72b8-e150-4030-8cd0-105f55d25a80.png)

A movimentação para os lados e para a cima da aeronave consiste em deslocamentos em cada eixo (x para direita e esquerda, y para cima). Já para freiar é necessário aplicar um deslocamento negativo no eixo y e adicionalmente fazer uma validação para que seja verificado se a velocidade já é zero (para não ficar negativa e fazer a nave "cair para trás"):<br>
Os deslocamentos são implementados no ship.cpp dentro da função Ship::update. O deslocamento para a esquerda é negativo (-4f) e para direita é positivo (+4f) dado a orientação do nosso eixo. Nosso jogo não possui rotação devido a sua natureza de movimentar na vertical.
<br>
![image](https://user-images.githubusercontent.com/30665585/139579179-f287196c-aef3-4f38-a71b-f3cb14e8fcd1.png)

Abaixo temos a implementação da aceleração vertical, no qual além de deslocarmos positivamente em Y exibimos o propulsor da nave. <br>
![image](https://user-images.githubusercontent.com/30665585/139579443-1a8f6612-ada6-4d88-b81c-dcc1ec1da169.png)

Por fim, para implementar o freio utilizamos a implementação abaixo: <br>
![image](https://user-images.githubusercontent.com/30665585/139579508-07c1f52e-1401-471b-9d1f-68aa9916fcb2.png)

Para todas as ações do jogo é necessário verificar se ainda é possível tomar alguma ação, ou seja, é necessário que o estado seja "Playing", pois de modo contrário o jogador teria ou perdido ou vencido, que em ambos cenários resultam em finalização do jogo.


 

