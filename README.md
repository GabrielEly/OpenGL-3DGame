# OpenGL_3DGame
## FPS_Game
Jogo feito em OpenGL para a disciplina de Processamento Gráfico de 2021.

Autores: Gabriel Ely Konrath e Guilherme Closs Fraga.

Vídeo sobre: https://youtu.be/S7Yv5O_p7Cw

### Instruções para compilar o jogo:
- Fazer o download ou clone do repositório;
- Abrir a solução `Sabertooth.sln` (pode ser encontrada dentro da pasta baixada ou clonada `OpenGL-3DGame-main/Sabertooth`) no Visual Studio 2019;
- Verificar o modo de Debug da solução, este deve estar em `x64` na barra de feramentas superior do Visual Studio. Caso não esteja, deverá ser mudado para `x64`;
- Clicar em `Depurar Local do Windows` na barra de ferramentas superior do Visual Studio ou depurar através do atalho `F5` para executar a aplicação.

OBS.: 
- Caso apareça algum erro de shader, modifique o número versão do core na primeira linha dos arquivos `core` e `sprite.vs`, para uma inferior, exemplo: `#version 400 core`, ou menor;
- Caso tenha problema com as dependências, deverá ser linkado todas as bibliotecas adicionais presentes na pasta `External`.

### Informações sobre o código:
Código usado é C++ juntamente com o OpenGL e GLSL, e as bibliotecas adicionais GLEW, GLFW, GLM, e SOIL.

* FPS_Game: Projeto do jogo 3D foi realizado com base no código de um projeto no github, que pode ser encontrado em: 
Também utilizamos alguns recursos presentes em: https://learnopengl.com
