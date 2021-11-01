# Danmaku

Danmaku é um jogo desenvolvido do estilo [*bullet hell*](https://en.wikipedia.org/wiki/Danmaku) (ou também conhecido como *danmaku*), na qual o objetivo é desviar de todos os projéteis sendo liberados pelo inimigo, enquanto atira nele para derrotá-lo com a menor quantidade de mortes possíveis. O jogo desenvolvido pode ser acessado [aqui](https://hirusora.github.io/abcg/danmaku).

Um diferencial desse estilo de jogo em relação ao *Shoot 'em up* é que os projéteis normalmente cobrem bastante parte da dela, procurando formar um padrão visual e nem sempre sendo direcionados ao jogador em si. Além disso, normalmente os jogadores sempre possuem uma *hitbox* bem menor que o modelo aparenta, o que auxilia ao desviar dos padrões de ataques.

## Controles do jogo

- `W` ou `↑`: mover a nave para cima
- `A` ou `←`: mover a nave para a esquerda
- `S` ou `↓`: mover a nave para baixo
- `D` ou `→`: mover a nave para a direita
- `SPACE`: atirar
- `LSHIFT` ou `RSHIFT`: entrar no modo "foco". Neste modo, a velocidade da nave se reduz pela metade, e o *hitbox* da nave aparece (círculo vermelho).

## Técnicas de desenvolvimento

Para todos os objetos do jogo, exceto as estrelas do *background*, foi utilizado um mesmo *vertex shader* e *fragment shader*, tentando torná-lo o mais genérico possível para ser utilizado. Desta forma, o *vertex shader* possui como parâmetros por vértice o seu posicionamento e sua coloração, enquanto possui como variáveis globais a rotação, a translação e a escala a ser aplicada, sendo que este último é um vetor para diferenciar a escala horizontal e vertical a serem aplicadas. O *fragment shader* apenas é responsável por fazer a saída da cor que é recebida de entrada.

Para modelar as formas das naves, que são mais complexas, foi utilizado o conceito de geomtria indexada, e para ser possível pintá-las sem realizar o gradiente, os vértices que participam de dois triângulos de cores distintos foram duplicados, para que cada vértice possua uma cor diferente e evite a aparição de gradientes. A exemplo, a cabeça da nave do jogador, que é azul, e o corpo da nave, que é branca, poderiam ser modeladas utilizando apenas 5 vértices (3 para a cabeça, e as outras 2 para o final do corpo), mas desta forma ocorreria uma gradiente do azul para o branco do corpo; então foram criados 7 vértices, 3 para a cabeça e 4 para o corpo, duplicando-se os vértices que ligam a cabeça ao corpo. Desta forma, os 3 vértices da cabeça foram pintadas de azul e os outros 4 de branco, evitando-se o gradiente.

Para implementar o padrão de ataque do inimigo, foram utilizados os conceitos de velocidade angular, aceleração angular e algumas randomizações. Utilizando $ω$ como velocidade angular atual e $ω_t$ como velocidade angular alvo, o padrão de ataque funciona conforme o seguinte: é randomizada um $ω_t$ alvo, e enquanto este alvo não é atingido, $ω$ é acrescentado em $0.1*π/2$ por segundo (positivo ou negativo, dependendo do valor de $ω_t$ e $ω$). Quando a diferença $|ω_t-ω|<0.1*π/2$, é considerado que o alvo foi atingido, e então é randomizado um tempo para manter o valor de $ω$; após passar esse tempo, é calculado um novo valor de $ω_t$ e o ciclo se repete, dando uma dinamicidade para o padrão de ataque do inimigo. De forma similar, para dar ainda mais dinamicidade ao inimigo, também são randomizadas a posição dele na tela (que limita-se para a metade superior da tela) e o tempo que ele vai utilizar para atingir a posição alvo, além de também randomizar o tempo em que ele permanece em cada posição.

Com relação aos tiros do inimigo, foi utilizado um polígono regular de 30 lados para esse assemelhar-se a um círculo, com intesidade alfa em 0 no centro e coloração randômica nas bordas (sendo estas as cores de um arco-íris) para o efeito de bolha nos tiros.

Para a barra de vida do inimigo, foi utilizado um escalamento horizontal (por isso a necessidade do *vertex shader* possuir duas escalas, uma para cada eixo) que é diminuído a cada vez que o inimigo é acertado, simulando assim a vida restante dele.

Por último, como as estrelas do *background* foram modeladas como sendo pontos, foi utilizado um *vertex shader* e *fragment shader* diferente do restante, na qual cada vértice do *vertex shader* recebe o posicionamento, coloração e tamanho do ponto, enquanto que o *fragment shader* realiza uma transformação de gradiente na intensidade alfa de cada fragmento, na qual a intensidade é 1 quando no centro do tamanho do ponto, e torna-se 0 quando na borda do tamanho do ponto.

## Créditos

Desenvolvido por Jonathan Takeshi Ywahashi.
