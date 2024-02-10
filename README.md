# Sobre o jogo:

Keep It in Sight é jogo de um terror curto com objetivo em estilo de PAC-MAN (colete enquanto foge).

Em um labirinto ambientado na história urbana americana "Backrooms", é preciso coletar esferas metálicas enquanto é perseguido por um monstro.

Para afugentá-lo, você está equipado de uma câmera fotográfica cujo flash ao tirar fotos faz com que o monstro fique pacífico por um tempo.

Mentenha o monstro à vista e fotografe-o enquanto coleta todas as esferas para vencer!

# Como jogar

## Compilação

O jogo pode ser compilado tanto para Windows quanto para Linux utilizando um arquivo de configuração de CMake.

Para compilar no `Windows`, rode o comando:

```shell
cmake --build {PATH_DO_JOGO}/build --config Debug --target main -j 10 --
```

Para compilato no `Linux`, rode o comando:
```shell
cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=${PATH_TO_GCC} -DCMAKE_CXX_COMPILER:FILEPATH=${PATH_TO_G++} -S${PATH_TO_PROJECT_ROOT} -B${PATH_TO_PROJECT_BUILD_FOLDER} -G "Unix Makefiles"
```

## Jogabilidade

- O personagem se move com as teclas `WASD`
- O personagem corre com `SHIFT esquerdo`
  - Tome cuidado, pois a estamina do jogador é **limitada**
  - Se a estamina acabar, você não conseguirá correr por algum tempo
- A tecla `F` dispara o flash da câmera
  - Estando suficientemente próximo dele, o flash fará com que o monstro fuja de você
  - Após um breve momento, ele volta a te perseguir, ficando imune por pouco tempo aos flashes da câmera
- A tecla `E` serve para o usuário interagir com o ambiente. Só existe uma porta, a qual o usuário pode abrir e fechar
- A tecla `M` faz a troca entre as câmeras livre e look-at

# Informações do projeto

## Contribuição de cada membro

Vale ressaltar que a dupla colaborou em quase todas as partes do desenvolvimento do trabalho, então no caso, serão destacados os pontos que tiveram mais contribuição por parte do membro em específico.

### Henrique Carniel da Silva (00335626)

- Colisões
- Mecânicas do player (estamina, uso da câmera)
- Curvas de bezier
- Comportamento da porta e interação com a mesma
- Objetos coletáveis
- Comportamento do monstro principal
- Áudio

### Léo Hernandes de Vasconcelos (00323961)

- Organização inicial do código
- Abstração de cenas, objetos e entidades
- Câmera livre e look-at
- Movimentação
- Iluminações
- Entidade câmera e posicionamento da mesma
- Carregamento de texturas e definição de coordenadas de textura nos arquivos `.obj`

## Uso de ferramentas baseadas em IA

Não fizemos o uso de ferramentas desse tipo em nenhum processo do desenvolvimento desse trabalho.

## Processo de desenvolvimento do trabalho

### Malhas poligonais complexas

A aplicação possui modelos geométricos bem complexos com milhares de vértices:

- A entidade da câmera fotográfica é construída a partir de um arquivo `.obj` de `2.43mb`
- A entidade do monstro é construída a partir de um arquivo `.obj` de `2.01mb`

### Transformações geométricas controladas pelo usuário

Implementado através da porta.
Se o usuário estiver perto o suficiente e pressionar a tecla `E`, a porta rotaciona em um ângulo de 90º. O processo de desenvolvimento não foi muito complicado, porém precisava das colisões implementadas.

### Câmera livre e câmera look-at

Os dois tipos de câmeras foram implementadas seguindo o que foi aprendido no `Laboratório 2`, com a única exceção de que o ponto fixo ao qual a câmera look-at aponta se modifica de acordo com a posição do jogador após se mover no modo de câmera livre.

### Instâncias de objetos

Na aplicação isso é feito através dos objetos virtuais de esferas e cubos, esses últimos utilizados para fazer as paredes.

Na imagem abaixo nota-se que são criadas quatro intâncias diferentes que usam o mesmo objeto, no caso a esfera.

![Imagem mostrando o código](https://github.com/LeoHernandes/keep-it-in-sight/assets/72423032/64059173-cdba-446d-9219-821fdbddeb06)

### Três tipos de testes de intersecção

No projeto representamos o jogador como um ponto, e também utilizamos duas estruturas de dados para representar áreas de colisão, sendo elas uma Hit Sphere e uma Hit Box AABB.

1. **Ponto-cubo:** teste realizado para detectar quando o player bate na parede ou no monstro, assim impedindo ele de atravessar esses objetos.
2. **Ponto-esfera:** teste realizado para detectar quando o player encosta em uma moeda, fazendo com que ela desapareça.
3. **Raio-cubo:** teste realizado para detectar se um raio, com comprimento máximo definido, intersecta uma Hit Box AABB. Utilizado para abrir a porta e também para tirar a foto do monstro.

Essa etapa do desenvolvimento foi um pouco demorada e complicada, o problema não foi os testes realizados, pois eles são relativamente simples, e sim como manter as informações da Hit Box com as transformações geométricas sofridas. Isso demandou um tempo de estudo para resolver, principalmente por conta da rotação.

### Modelos de Iluminação Difusa e Blinn-Phong

Usado em todos os objetos da aplicação. A implementação foi baseada nos laboratórios 4 e 5, adicionando as pequenas alterações da iluminação `Blinn-Phong` com o cálculo do `half vector`.

### Modelos de Interpolação de Phong e Gouraud

O modelo de interpolação de Phong foi aplicado em todos os objetos com exceção das esferas (moedas) que utilizam interpolação de Gouraud.

Cada objeto, ao chamar a função para enviar seus dados à GPU para ser renderizado, pode escolher qual tipo de iluminação quer:

- Phong: iluminação interpolada por `pixel`
- Gouraud: iluminação interpolada por `vértice`
- Sem especularidade: apenas a iluminação difusa com sombreamento, como no caso do chão e das paredes

![Interpolação de Phong na porta](https://i.ibb.co/Vgq9CqP/door.png)
![Interpolação de Gouraud na moeda](https://i.ibb.co/WK2Jpp3/coin.png)

### Mapeamento de texturas em todos os objetos

Todos os objetos da aplicação possuem mapeamento de texturas.

Cada objeto pode escolher qual textura e qual o tipo de cálculo de coordenadas de texturas que irá utilizar ao ser renderizado.

Mantemos controle dos `ids` de cada textura carregada no código por meio de um dicionário que mapeia `nome` para `id`. Cada objeto pode encontrar o identificador de sua textura por meio desse dicionário e enviá-lo para GPU.

Além disso, é possível escolher como a textura será mapeada no objeto:

- Projeção esférica: como no caso da skybox e nas moedas, as coordenadas de textura são calculadas dessa forma
- Projeção em plano: passou a não ser usada mais em nenhum objeto, mas optamos deixar como opção para futuras decisões
- Projeção de acordo com arquivo `.obj`: no restante dos objetos da aplicação, armazenamos em memória as coordenadas de textura definidas no arquibo `.obj` e utilizamos na GPU. Caso as coordenadas extrapolem os limites, utilzamos o paramêtro `GL_REPEAT` para deixar a aplicação mais agradável.

![Imagem mostrando as texturas sendo usadas na aplicação](https://i.ibb.co/NN8qTGW/jogo.png)

### Movimentação com curva Bézier cúbica

Dois objetos utilizam movimentação com curva de Bézier cúbica:

- A câmera livre é movimentada pra cima e pra baixo quando o jogador corre, simulando o movimento da cabeça do personagem
- As moedas fazem um movimento de espiral enquanto não são coletadas.

Não tivemos dificuldade nessa etapa, pois nos guiamos pelas fórmulas fornecidas durante as aulas.

### Animações baseadas no tempo ($\Delta t$)

Tudo os objetos animados são baseados no $\Delta t$, para assim, independente da velocidade da máquina, elas ocorram na mesma velocidade. Implementado o valor do $\Delta t$ logo no inicio do projeto, e sem maiores dificuldades para utilizar nas animações.

### Funcionalidades adicionais

- `Áudio`: a geração de diversos sons para a aplicação deixa o jogo bem mais dinâmico. Para isso foi utilizada a biblioteca miniaudio, por conta da sua baixa complexidade para ser utilizada, resultando num processo relativemente tranquilo.
- `Movimentação`: utilizamos vetores de aceleração, velocidade e atrito para simular uma movimentação mais suave para o jogador. Tudo é baseado em animações no tempo, incluindo interpolação no aumento do valor de `FOV` (field of view) da câmera quando o personagem corre, dando a sensação de rapidez.
- `Posicionamento da entidade da câmera`: para simular o personagem segurando a câmera fotográfica, foi necessário utilizar rotações mais complexas (fórmula de Rodrigues) para orientá-la de acordo com a direção de visão do jogador.
