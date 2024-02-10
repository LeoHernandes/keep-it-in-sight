# Sobre o jogo:

Keep It in Sight é jogo de um terror curto com objetivo em estilo de PAC-MAN (colete enquanto foge).

Em um labirinto ambientado na história urbana americana "Backrooms", é preciso coletar esferas metálicas enquanto é perseguido por um monstro.

Para afugentá-lo, você está equipado de uma câmera fotográfica cujo flash ao tirar fotos faz com que o monstro fique pacífico por um tempo.

Mentenha o monstro à vista e fotografe-o enquanto coleta todas as esferas para vencer!

# Como jogar

## Compilação

O jogo pode ser compilado tanto para Windows quanto para Linux utilizando um arquivo de configuração de CMake.

Para compilar no Windows, rode o comando

```shell
cmake --build {PATH_DO_JOGO}/build --config Debug --target main -j 10 --
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

A aplicação possui um modelo geométrico da câmera do player, a qual possui complexidade bem próxima ao modelo "cow.obj". [blablabla].

### Transformações geométricas controladas pelo usuário

Implementado através da porta, a qual se o usuário chegar perto o suficiente, e apertar E, a porta rotaciona em um ângulo de 90º. O processo de desenvolvimento não foi muito complicado, porém precisava das colisões implementadas.

### Câmera livre e câmera look-at

O jogo inicia com a câmera livre, porém pode haver a troca através da tecla M. [blablabla].

### Instâncias de objetos

Na aplicação isso é feito através dos objetos virtuais de moedas e cubos, esses últimos utilizados para fazer as paredes. [blablabla]. Na imagem abaixo nota-se que são criadas duas intâncias diferentes que usam o mesmo objeto, no caso o cubo.

![Imagem mostrando o código](https://i.ibb.co/473YCPn/codigo1.png)

### Três tipos de testes de intersecção

No projeto representamos o jogador como um ponto, e também utilizamos duas estruturas de dados para representar áreas de colisão, sendo elas uma Hit Sphere e uma Hit Box AABB.

1. **Ponto-cubo:** teste realizado para detectar quando o player bate na parede ou no monstro, assim impedindo ele de atravessar esses objetos.
2. **Ponto-esfera:** teste realizado para detectar quando o player encosta em uma moeda, fazendo com que ela desapareça.
3. **Raio-cubo:** teste realizado para detectar se um raio, com comprimento máximo definido, intersecta uma Hit Box AABB. Utilizado para abrir a porta e também para tirar a foto do monstro.

Essa etapa do desenvolvimento foi um pouco demorada e complicada, o problema não foi os testes realizados, pois eles são relativamente simples, e sim como manter as informações da Hit Box com as transformações geométricas sofridas. Isso demandou um tempo de estudo para resolver, principalmente por conta da rotação.

### Modelos de Iluminação Difusa e Blinn-Phong

Usado em todos os objetos da aplicação. A implementação foi relativamente simples, pois os laboratórios 4 e 5 foram uma base boa para tal.

### Modelos de Interpolação de Phong e Gouraud

O modelo de interpolação de Phong foi aplicado em todos os objetos, tirando as moedas que utilizam interpolação de Gourand. [blablabla].

![Interpolação de Phong na porta](https://i.ibb.co/Vgq9CqP/door.png)
![Interpolação de Gouraud na moeda](https://i.ibb.co/WK2Jpp3/coin.png)

### Mapeamento de texturas em todos os objetos

Todos os objetos da aplicação possuem mapeamento de texturas. [blablabla].

![Imagem mostrando as texturas sendo usadas na aplicação](https://i.ibb.co/NN8qTGW/jogo.png)

### Movimentação com curva Bézier cúbica

Dois objetos utilizam movimentação com curva de Bézier cúbica, a câmera livre que é movimentada pra cima e pra baixo quando o jogador corre, e as moedas que fazem um movimento de espiral. Não tivemos dificuldade nessa etapa, pois era só se guiar pelas fórmulas fornecidas durante as aulas.

### Animações baseadas no tempo ($\Delta t$)

Tudo os objetos animados são baseados no $\Delta t$, para assim, independente da velocidade da máquina, elas ocorram na mesma velocidade. Implementado o valor do $\Delta t$ logo no inicio do projeto, e sem maiores dificuldades para utilizar nas animações.

### Funcionalidades adicionais

Uma funcionalidade adicional implementada foi a geração de diversos sons para a aplicação, o que acaba dando bem mais dinâmismo. Foi utilizada a biblioteca miniaudio.h para tal, [blablabla].
