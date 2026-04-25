# trab-ga-computacao-grafica

Trabalho para atividade de computação gráfica dos alunos Bel Cogo e Bruno Hoffmann.

## Descrição

Este projeto é uma aplicação OpenGL que renderiza dois modelos 3D do macaco Suzanne com iluminação Phong, câmera controlável e transformações interativas. Desenvolvido em C++ usando GLFW para gerenciamento de janelas, GLM para matemática 3D e GLAD para carregamento de funções OpenGL.

## Pré-requisitos

- **Compilador C++**: GCC ou Clang com suporte a C++17
- **CMake**: Versão 3.10 ou superior
- **OpenGL**: Versão 4.6 ou superior (suportado pela placa gráfica)
- **Bibliotecas de Sistema**:
  - OpenGL development libraries
  - Threads library

## Dependências

As dependências externas (GLFW, GLM e stb_image) são baixadas automaticamente pelo CMake usando FetchContent. Não é necessário instalá-las manualmente.

## Construindo o Projeto

1. Clone ou baixe o repositório:
   ```bash
   git clone https://github.com/BrunoHoffmann15/trab-ga-computacao-grafica.git
   cd trab-ga-computacao-grafica
   ```

2. Crie o diretório de build e configure com CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Compile o projeto:
   ```bash
   make
   ```
   Ou usando CMake:
   ```bash
   cmake --build .
   ```

## Executando a Aplicação

Após a compilação, execute o binário:
```bash
./TrabalhoGA
```

A aplicação abrirá uma janela OpenGL exibindo dois modelos Suzanne com iluminação.

## Controles

### Câmera
- **WASD**: Movimentar a câmera para frente, esquerda, trás e direita
- **Mouse**: Girar a câmera (mantenha pressionado e mova)

### Transformações dos Objetos
- **Setas para cima/baixo**: Aplicar transformação no objeto ativo (rotação, escala ou translação)
- **1**: Modo rotação
- **2**: Modo escala
- **3**: Modo translação
- **X/Y/Z**: Selecionar eixo para transformação (X, Y ou Z)
- **C**: Selecionar todos os eixos
- **N**: Alternar entre os dois objetos (Suzanne esquerda e direita)

### Iluminação
- **4**: Modo movimento da luz
- **5**: Modo ajuste de ka (ambiente)
- **6**: Modo ajuste de kd (difusa)
- **7**: Modo ajuste de ks (especular)

### Visualização
- **P**: Alternar entre projeção perspectiva e ortográfica
- **O**: Ativar/desativar wireframe
- **ESC**: Sair da aplicação

## Estrutura do Projeto

- `src/TrabalhoGA/`: Código fonte principal
  - `main.cpp`: Arquivo principal da aplicação
  - `Camera.h` e `Camera.cpp`: Classe para controle da câmera
- `assets/`: Arquivos de recursos (modelos OBJ, materiais MTL)
- `include/`: Cabeçalhos (GLAD)
- `common/`: Código comum (GLAD)
- `CMakeLists.txt`: Configuração do build

## Solução de Problemas

- **Erro de compilação**: Certifique-se de que o CMake e o compilador C++ estão instalados e atualizados.
- **Erro ao executar**: Verifique se a placa gráfica suporta OpenGL 4.6. Se necessário, instale drivers atualizados.
- **Modelo não carrega**: Certifique-se de que o arquivo `assets/Suzanne.obj` existe e está no formato correto.

## Licença

Este projeto é parte de uma atividade acadêmica e não possui licença específica.
