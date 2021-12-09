# Sandan

Sandan é um jogo desenvolvido seguindo o estilo de jogo [*bullet hell*](https://en.wikipedia.org/wiki/Danmaku) (ou também conhecido como *danmaku*) em um ambiente 3D, na qual o objetivo é desviar de todos os projéteis sendo liberados pelo inimigo, enquanto atira nele para derrotá-lo com a menor quantidade de mortes possíveis. A movimentação do jogador é feita numa espécie de órbita ao redor do inimigo, podendo transladar para as quatro direções (cima, baixo, esquerda, direita) e podendo também se aproximar ou afastar dele (dentro de limites pré-determinados). O jogo desenvolvido pode ser acessado [aqui](https://hirusora.github.io/abcg/sandan/).

## Controles do jogo

- `W`: mover a nave para cima
- `S`: mover a nave para baixo
- `A`: mover a nave para a esquerda
- `D`: mover a nave para a direita
- `↑`: mover a nave para frente (em direção ao inimigo)
- `↓`: mover a nave para trás (em direção oposta ao inimigo)
- `←`: diminuir zoom da visão (FoV)
- `→`: aumentar zoom da visão (FoV)
- `SPACE`: atirar
- `LSHIFT` ou `RSHIFT`: entrar no modo "foco". Neste modo, a velocidade da nave se reduz pela metade, e o *hitbox* da nave aparece (esfera vermelha).

## Técnicas de desenvolvimento

Sobre a renderização de todos os objetos na cena, foram utilizados três conjuntos de *vertex / fragment shaders* diferentes: o conjunto *model*, principal dos *shaders*, responsável por realizar a renderização de objetos 3D, com suporte a iluminação (modelo de reflexão de Blinn-Phong), texturização e mapeamento de normais; o conjunto *model2d*, reutilizado do projeto `Danmaku` e responsável por renderizar elementos 2D do jogo; e o conjunto *skybox*, responsável por renderizar o *skybox* para simular o ambiente de universo. Desta forma, a nave, o UFO e todos os modelos esféricos (projéteis do inimigo, tiros da nave e *hitbox* da nave) foram renderizados utilizando o conjunto *model*, em que a nave e o UFO foram obtidos com texturização por mapeamento UV e mapeamento de normais (em arquivo `mtl`), enquanto a esfera apenas possui a definição do objeto (sem arquivo `mtl`), e então as texturizações dela são carregadas de forma separada utilizando mapeamento esférico; a barra de vida do UFO é renderizada utilizando o conjunto *model2d*, com as configurações para renderização (VAO, VBO, variáveis uniformes, etc.) sendo praticamente similares ao do projeto `Danmaku`; e o elemento *skybox* é renderizado usando o conjunto *skybox*, com configurações para que a renderização da textura esteja nas faces internas do cubo (principalmente `glFrontFace(GL_CW)`), já que a câmera ficará na parte interna dele. Além disso, com relação aos modelos esféricos, todos aqueles que são da mesma classificação / mesmo propósito utilizam a mesma VAO / VBO / EBO, mas os que possuem propósitos diferentes utilizam VAO / VBO / EBO distintas (e.g. todos os projéteis do inimigo compartilham do mesmo VAO, mas este VAO não é compartilhado com o *hitbox* da nave), e isto foi programado visando a possível troca de modelos, e.g. para que a troca do modelo dos tiros não influencie o modelo dos projéteies. Sobre a iluminação, é utilizada uma luz direcional `(0,0,1,0)` para que o jogador inicie a visão no lado iluminado dos objetos (a posição inicial da nave é `(0,0,-0.8)`), podendo ver o lado sombreado dos objetos ao orbitar horizontalmente o suficiente. Por último, em questão de codificação, todos os modelos 3D utilizam como base a classe `Model`, que possui vários métodos genéricos para carregamento dos dados da modelagem do objeto, texturização, renderização com definição de variáveis uniformes, entre outros aspectos, enquanto que o modelo da barra de vida e do *skybox* não herdam de nenhuma classe e possuem suas próprias definições para possibilitar suas respectivas renderizações.

Para modelar o padrão de ataque *Wave / Particle*, foi reutilizado o padrão de ataque desenvolvido na atividade anterior (`Sandan (Preview)`). Desta forma, tratando-se de um ambiente 3D, a dinamicidade do padrão é realizada randomizando-se dentre 1 a 3 velocidades angulares alvos, sendo que cada uma é relacionada a cada eixo `x`, `y` e `z` da base, respectivamente. De forma similar à atividade anterior, enquanto a velocidade angular atual não atingir o alvo para cada um dos eixos, esta velocidade angular é acrescentada em `π/4` por segundo. Apenas quando as velocidades angulares de todos os eixos alcançarem o alvo é considerado que o alvo foi atingido como um todo, e então são calculados novas velocidades angulares alvos e o ciclo se repete (não há randomização de tempo desta vez). Além disso, para rotacionar o padrão de ataque pelas velocidades angulares foi utilizada uma matriz de rotação, que a cada *frame* é rotacionada pela velocidade angular em `x`, depois `y` e por fim `z`, e então aplicada no vetor base que é utilizado para construir o padrão de ataque. Além disso, vale a pena citar que a classe de padrão de tiros é a responsável por definir os vetores de velocidade e escala de cada projétil gerado, enquanto a classe que agrega todos os projéteis é apenas responsável por atualizar a translação deles com o tempo e de renderizá-los utilizando transformações de translação e escalamento na matriz de modelo.

Para modelar a movimentação de órbita da nave em torno do inimigo (que está sempre em `(0,0,0)`), primeiramente com relação às direções cima, baixo, esquerda e direta: é armazenado no objeto um vetor `r` de 2 posições para armazenar os valores de rotação *yaw* e *pitch* respectivamente (ângulos de rotação nos eixos `y` e `x`) e um vetor `t` que é o vetor base de translação iniciado com os valores `(0,0,-0.8)`. Quando é detectada uma entrada do teclado referente às movimentações direcionais (`W`, `A`, `S` ou `D`), é atualizado o valor dos ângulos em `r` na respectiva direção de entrada, e.g. se a direção desejada é para a direita, adiciona-se um valor de `velocidade angular * tempo delta` em `r[0]`, já que a rotação é para ser realizada no eixo `y`. Desta forma, na renderização e cálculo da matriz de modelo, é utilizada, nesta exata ordem: a transformação de translação pelo vetor `t`; rotação utilizando os valores em `r`, os eixos `y` e `x`, e usando o ponto `(0,0,0)` como referência; e a transformação de escala, o que resulta no movimento de órbita da nave em torno do UFO. Com relação aos movimentos de aproximação e afastamento do inimigo, apenas é alterado a componente `z` de `t` (a aproximação adiciona valor positivo em `t` e o afastamento adiciona valor negativo), pois o cálculo da matriz de modelo na renderização já se encarregará de rotacionar `t` para a posição necessária.

Com o objetivo da câmera acompanhar a movimentação da nave, as lógicas de movimentação de nave previamente explicadas se aplicam igualmente para a movimentação da câmera, com dois diferenciais: o vetor inicial `t` da câmera é `(0,0.1,-1.0)`, ou seja, ela está sempre um pouco acima e um pouco para trás com relação à nave, para que seja possível ter visão da nave na parte inferior da tela; e a cada momento que há atualização em `r` da câmera, a posição `eye` da câmera é calculada utilizando uma matriz de rotação sobre `t` e então é recalculada a matriz de visão, para atualizar a visão da câmera para o jogador. Além disso, a câmera também possui a funcionalidade de realizar *zoom in* ou *zoom out*, que é atingida apenas alterando-se o valor de FoV da câmera por um valor de `velocidade de FoV * delta time` e recalculando a matriz de projeção, desta forma dando a impressão de efeitos de *zoom* ao usuário.

Com relação aos tiros da nave, estes são criados a partir da posição `p` atual da nave, dada pela multiplicação da matriz de rotação com o vetor base de translação da nave, e o vetor velocidade deles são definidos pela normalização de `(0,0,0) - p`, para que eles se movimentem em direção ao UFO. Para a renderização, são utilizadas apenas transformações de translação e de escalamento, e é também utilizado um temporizador entre tiros para limitar uma taxa de tiros por segundo.

Para implementar a colisão dos objetos da cena, foi utilizada a lógica de colisão entre esferas, na qual é calculada a distância entre o centro de dois objetos da cena, e caso essa distância seja menor que a soma dos raios desses dois objetos, então há a colisão. Este modo de detecção foi escolhida pois a maior parte dos elementos são esferas (projéteis do inimigo, tiros da nave, *hitbox* da nave), e também porque não haveria impacto significativo na jogabilidade ao aproximar a área de colisão de certos objetos para uma esfera (e.g. UFO).

Para implementar a animação de *hit* quando a nave recebe um dano por projétil, esta é feita habilitando a mesclagem do OpenGL e configurando-a com a parametrização `GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA`. Desta forma, enquanto a nave estiver no estado invulnerável, o valor *alpha* de `glBlendColor` varia entre 0.2 e 0.7 a cada 0.2 segundos, dando o efeito de "piscagem" da nave ao colidir com um projétil.

A implementação da barra de vidas é uma reutilização do projeto `Danmaku`, onde é renderizado um retângulo simples verde e foi utilizado um escalamento horizontal que é diminuído a cada vez que o inimigo é acertado, simulando assim a vida restante dele.

Por fim, para com relação ao *skybox*, vale a pena citar que este utiliza das matriz de projeção e visão que provém da câmera, simulando assim a movimentação do plano de fundo ao realizar a movimentação da nave (e, consequentemente, da câmera).

## Considerações

Caso o programa esteja sendo executado no Google Chrome com baixa *performance*, é recomendável utilizar uma guia nova **como visitante** ou **como anônimo** para executar o jogo, pois neste caso a guia estará limpa de outras abas, e o código consseguirá executar em sua máxima *performance*. Isto foi testado e validado com o *browser* do Google Chrome, mas é também recomendado para qualquer outro *browser* em que o jogo for aberto e que esteja apresentado baixa *performance*.

## Créditos

Aplicação desenvolvida por Jonathan Takeshi Ywahashi.

Modelo da esfera desenvolvido por thequinneffect ([link](https://skfb.ly/6XEn7)).

Texturas da esfera gerados por Jonathan Takeshi Ywahashi.

Modelo e textura da nave desenvolvido por 3dhaupt ([link](https://free3d.com/3d-model/intergalactic-spaceship-in-blender-28-eevee-394046.html)).

Modelo e textura do UFO desenvolvido por printable_models ([link](https://free3d.com/3d-model/ufo-saucer-v1--190141.html)).

Textura do *skybox* desenvolvido por StumpyStrust ([link](https://opengameart.org/content/space-skyboxes-0)).

"S01C01 Low Poly Sphere" (https://skfb.ly/6XEn7) by thequinneffect is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
