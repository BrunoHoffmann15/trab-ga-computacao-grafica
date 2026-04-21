# TO-DO List

## MVP1

**Estrutura Inicial**

[x] Estruturar o trabalho;

**Leitura do .obj**

[ ] Buscar um `.obj` para usar;

[ ] Fazer o parse do arquivo `.obj` - Atributos: deve recuperar obrigatoriamente vértices, faces (apenas triângulos), normais (vn) e coordenadas de textura (vt);

[ ] Gerenciamento de Buffers: criação e utilização correta de VAO e VBOs (EBO opcional);

**Controle dos Objetos**

[ ] Mostrar objetos em tela;

[ ] Possibilitar a adição de mais de um objeto;

[ ] Seleção: interface via teclado para alternar o objeto selecionado;

[ ] Possibilitar a rotação nos eixos x, y, z do objeto selecionado;

[ ] Possibilitar a Translação nos eixos X, Y e Z do objeto selecionado;

[ ] Possibilitar a escala uniforme e por eixos (X, Y, Z) para o objeto selecionado;

**Câmera**

[ ] Possibilitar a troca das visões da câmera entre perspectiva e ortográfica;

[ ] Possibilitar a movimentação da câmera - frente, trás, esquerda, direita;

[ ] Possibilitar a movimentação da câmera - rotacionar para direita, para esquerda, para cima e para baixo;

**Iluminação**

[ ] Modelo de Iluminação: implementação do modelo de Phong (Ambiente, Difusa e Especular) nos
Shaders

[ ] Fonte de Luz: pelo menos uma fonte de luz pontual configurável na cena.

[ ] Superfície: as propriedades de refletância do material (ka, kd e ks) devem ser configuráveis
manualmente.

[ ] Modos de visualização: sólido (com iluminação aplicada) e desenho de wireframe sobreposto à
geometria sólida, quando o usuário optar.

## MVP2

[ ] Desenho de uma grid de chão; 

[ ] Desenho dos eixos;

[ ] Interface gráfica para ajuste de parâmetros;

[ ] Interface gráfica para ajuste transformações;