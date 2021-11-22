# Sandan (Preview)

Sandan (preview) é um teste desenvolvido seguindo o estilo de jogo [*bullet hell*](https://en.wikipedia.org/wiki/Danmaku) (ou também conhecido como *danmaku*) em um ambiente 3D, algo raramente realizado, para a partir deste validar a viabilidade de desenvolver o jogo em si. O teste desenvolvido pode ser acessado [aqui](https://hirusora.github.io/abcg/sandanpreview).

## Controles do teste

A partir da tela do teste há uma janela no canto superior direito que permite mexer em duas configurações:

- `Pattern`: Permite selecionar o padrão de ataque a ser visto para *preview*;
- `FOV`: Permite alterar o *Field of View* da câmera.

## Técnicas de desenvolvimento

Para todos os objetos do teste foi utilizado um mesmo *vertex shader* e *fragment shader*, assim como um mesmo VAO, já que têm-se apenas um modelo 3D de objeto utilizado para simular os padrões de ataques. Vale a pena notar que o modelo escolhido foi uma esfera classificada como *low poly*, ou seja, um objeto modelado com poucos polígonos / triângulos, o que foi de suma importância para estabilizar o FPS do programa em 60. Pela natureza do estilo de jogo, na maioria das vezes há vários elementos na tela do usuário e, com a utilização de menos polígonos, o trabalho de renderização se torna mais leve (antes foi utilizado um modelo de esfera com mais de 6000 polígonos, e o FPS do programa estabilizava em 10).

Por questões de simplificação, foi escolhido separar uma classe que representa todos os tiros que serão renderizados. Nesta classe encontra-se o VAO / VBO / EBO do modelo da esfera, além de também possuir métodos simples de atualização de translação do objeto e de definição da matriz de modelo (translação e escala) para cada objeto. Definida o comportamento padrão dos tiros, a codificação dos padrões de ataque ficam mais simples e não precisam se importar tanto com a renderização / comportamento dos tiros que serão utilizados.

Para modelar o padrão de ataque *Wave / Particle*, foi utilizado como base o padrão de ataque desenvolvido na atividade anterior (*Danmaku*). Como agora a ambientação é em 3D, desta vez a dinamicidade do padrão é realizada randomizando-se dentre 1 a 3 velocidades angulares alvos, sendo que cada uma é relacionada a cada eixo `x`, `y` e `z` da base, respectivamente. De forma similar à atividade anterior, enquanto a velocidade angular atual não atingir o alvo para cada um dos eixos, esta velocidade angular é acrescentada em `π/4` por segundo. Apenas quando as velocidades angulares de todos os eixos alcançarem o alvo é considerado que o alvo foi atingido como um todo, e então são calculados novas velocidades angulares alvos e o ciclo se repete (não há randomização de tempo desta vez). Além disso, para rotacionar o padrão de ataque pelas velocidades angulares foi utilizada uma matriz de rotação, que a cada *frame* é rotacionada pela velocidade angular em `x`, depois `y` e por fim `z`, e então aplicada no vetor base que é utilizado para construir o padrão de ataque.

Para o padrão de ataque *Nuclear*, a cada 1 segundo é randomizada uma posição `x` e `z` inicial para o objeto, e então ela é colocada em `y = 1.0`, que é o limite `y` de renderização escolhido, com uma velocidade constante `(0,-1.0,0)`. Cada um dos objetos renderizados que vêm de `y` possui dois estados possíveis, que são "encolhendo" ou "explodindo", e todos são inicializados com o estado "encolhendo". O efeito de "encolhendo" ou "explodindo" é realizado aplicando uma mudança na escala do objeto conforme o tempo `Δ` entre *frames*, sendo que a velocidade da escala na explosão é maior que o do encolhimento. Após atingir a escala alvo para o encolhimento, inicia-se o processo de explosão, que é similar ao reverso do encolhimento, mas com uma diferença na velocidade de mudança de escala e no final da explosão: no limite da explosão, ou seja, quando a escala da explosão estiver no máximo, são gerados 22 partículas ao redor desta explosão, cada uma seguindo seu próprio vetor de velocidade com direção e sentido `(P - O)`, sendo `P` o centro da partícula e `O` o centro da explosão. Com o fim da liberação de partículas, o processo de encolhimento inicia-se novamente, e o ciclo se repete. Por último, como nesse padrão de ataque é necessário um maior controle sobre o comportamento das esferas (e.g. mudança de escala com o tempo), para cada tiro foi gerado um ID e também foi utilizada a estrutura *HashMap* que, para cada ID mapeia um objeto, chamado de `BulletExtension`, que possui as propriedades adicionais para o tiro, como se fosse uma extensão sobre o `Bullet` original.

Por fim, a câmera utilizada na aplicação trata-se de uma que utiliza projeção em perspectiva sobre o espaço utilizado para renderização, que é um cubo `(2,2,2)` que possui coordenadas de -1 a 1 nos 3 eixos. Além disso, ela é rotacionada no eixo `y` a `π/4` radianos por segundo, para dar uma visão melhor sobre o padrão de ataque a ser realizado o *preview*.

## Considerações

Vale a pena citar que foram obtidos comportamentos diferentes dependendo do *browser* em que executou-se o programa em *WebAssembly*:

- Brave: O programa executou perfeitamente para os dois padrões (~60FPS);
- Microsoft Edge: O programa executou com ótimo FPS (~60FPS) para os dois padrões, mas com eventual  *exception* no padrão *Nuclear* quando ocorre a explosão;
- Google Chrome: O programa executou com péssimo FPS (~10FPS) para o padrão *Wave / Particle*, e FPS razoável (~30FPS) para o padrão *Nuclear*. Além disso, eventualmente acontece uma *exception* no padrão *Nuclear* quando ocorre a explosão;

Desta forma, dentre os navegadores utilizados, o navegador que apresentou melhores resultados foi o Brave, mas os problemas e *performance* relatados no Google Chrome podem ser por causa de sobrecarga no navegador com as abas pessoais.

## Créditos

Desenvolvido por Jonathan Takeshi Ywahashi.
Modelo de esfera desenvolvido por thequinneffect.

"S01C01 Low Poly Sphere" (https://skfb.ly/6XEn7) by thequinneffect is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).
