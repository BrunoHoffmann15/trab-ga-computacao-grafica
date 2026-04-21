/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 * e https://antongerdelan.net/opengl/
 * Adaptado por Rossana Baptista Queiroz
 * para as disciplinas de Processamento Gráfico/Computação Gráfica - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 03/03/2026
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Camera
#include "Camera.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupWireframeShader();
int loadSimpleOBJ(string filePATH, int &nVertices, int r, int g, int b);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 600, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = R"glsl(#version 450
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
out vec4 finalColor;
void main()
{
gl_Position = projection * view * model * vec4(position, 1.0);
finalColor = vec4(color, 1.0);
}
)glsl";

// Código fonte do Fragment Shader (em GLSL)
const GLchar* fragmentShaderSource = R"glsl(#version 450
in vec4 finalColor;
out vec4 color;

void main()
{
    color = finalColor;
}
)glsl";

// Wireframe Vertex Shader
const GLchar* wireframeVertexShaderSource = R"glsl(#version 450
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
void main()
{
gl_Position = projection * view * model * vec4(position, 1.0);
}
)glsl";

// Wireframe Fragment Shader (black lines)
const GLchar* wireframeFragmentShaderSource = R"glsl(#version 450
out vec4 color;

void main()
{
    color = vec4(0.0, 0.0, 0.0, 1.0);
}
)glsl";

// Definindo variáveis globais para controle de transformações.
bool axisX=true, axisY=false, axisZ=false, rotateEnabled = true, scale = false, translade = false;
int active_mesh = 0; //mesh selecionado para transformação (0 ou 1)

//Instanciação da Camera
Camera camera(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0.0,1.0,0.0),90.0,0.0);
float deltaTime = 0.0;
float lastFrame = 0.0; 

// Estrutura Mesh para controle de objeto;
struct Mesh 
{
    GLuint VAO;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale; 
	int nVertices;
};

// Definição da função de renderização dos meshes.
void renderMeshes(std::vector<Mesh> &meshes, GLuint shaderID);

// Definição das funções de transformação.
void applyTransform(Mesh &mesh, bool shouldGoUp);

void rotateMesh(Mesh &mesh, bool clockwise);

void scaleMesh(Mesh &mesh, bool up);

void transladeMesh(Mesh &mesh, bool up);

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercício 2 - Bel Cogo e Bruno Hoffmann", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();
	GLuint wireframeShaderID = setupWireframeShader();

	glUseProgram(shaderID);

    // Matriz de modelo - Transformações nos objetos
	glm::mat4 model = glm::mat4(1); //matriz identidade;
  	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    //-----------------
    // Matriz de projeção
    //glm::mat4 projection = glm::ortho(-3.0, 3.0, -3.0, 3.0, 0.1, 100.0);
    //glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)WIDTH/(float)HEIGHT,0.1f,100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
    
    // Matriz de view
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    
    glEnable(GL_DEPTH_TEST);

	// Definição dos objetos e seus atributos.
	std::vector<Mesh> meshes;

	Mesh m1;

	m1.VAO = loadSimpleOBJ("../assets/Suzanne.obj",m1.nVertices, 1, 0, 1);
	m1.position = glm::vec3(-0.8, 0.0, 0.0);
	m1.rotation = glm::vec3(0.0, 180.0, 0.0);
	m1.scale = glm::vec3(0.5, 0.5, 0.5);

	meshes.push_back(m1);

	Mesh m2;

	m2.VAO = loadSimpleOBJ("../assets/Suzanne.obj",m2.nVertices, 0, 1, 1);
	m2.position = glm::vec3(0.8, 0.0, 0.0);
	m2.rotation = glm::vec3(0.0, 180.0, 0.0);
	m2.scale = glm::vec3(0.5, 0.5, 0.5);

	meshes.push_back(m2);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Controle do tempo entre frames
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Essas funções estão depreciadas, só vão funcionar se usarmos o 
        // OpenGL em modo "Compability" -- eu as deixo pra facilitar a visualização
		glLineWidth(10);
		glPointSize(10);

		// Configuração da projeção.
		projection = glm::perspective(glm::radians(45.0f),(float)WIDTH/(float)HEIGHT,0.1f,100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Configuração da movimentação da câmera via teclado (IKJL).
		if(glfwGetKey(window,GLFW_KEY_I) == GLFW_PRESS) camera.processKeyboard("FORWARD",deltaTime);
		if(glfwGetKey(window,GLFW_KEY_K) == GLFW_PRESS) camera.processKeyboard("BACKWARD",deltaTime);
		if(glfwGetKey(window,GLFW_KEY_J) == GLFW_PRESS) camera.processKeyboard("LEFT",deltaTime);
		if(glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS) camera.processKeyboard("RIGHT",deltaTime);

		// Configurações das transformação dos objetos.
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			applyTransform(meshes[active_mesh], true);
		}

		// Configurações das transformação dos objetos.
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			applyTransform(meshes[active_mesh], false);
		}
        
        glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		// Atualização da matriz de view de acordo com as mudanças que ela sofreu via input
		// de mouse e/ou teclado
		view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Chamada de Desenho - DRAWCALL
		// Primeiro: renderizar solido
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		renderMeshes(meshes, shaderID); // Renderiza cada mesh da cena.
		
		// Segundo: renderizar wireframe por cima
		glUseProgram(wireframeShaderID);
		glUniformMatrix4fv(glGetUniformLocation(wireframeShaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(wireframeShaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5f);

		renderMeshes(meshes, wireframeShaderID); // Renderiza cada mesh da cena.

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUseProgram(shaderID);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}


	// Desaloca o VAO do buffer.
	for (auto& mesh : meshes) 
	{
		glDeleteVertexArrays(1, &mesh.VAO);
	}

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void applyTransform(Mesh &mesh, bool shouldGoUp)
{
	if (rotateEnabled) { // Caso de rotação.
		rotateMesh(mesh, shouldGoUp);
	}
	else if (scale) { // Caso de escalar.
		scaleMesh(mesh, shouldGoUp);
	} else { // Caso de translação.
		transladeMesh(mesh, shouldGoUp);
	}
}

// Translada o mesh no sentido positivo ou negativo do eixo selecionado (X, Y ou Z)
void transladeMesh(Mesh &mesh, bool up)
{
	float delta = 0.01f * (up ? 1 : -1);

	if (axisX)
		mesh.position.x += delta;
	if (axisY)
		mesh.position.y += delta;
	if (axisZ)
		mesh.position.z += delta;
}

// Rotaciona o mesh em torno do eixo selecionado (X, Y ou Z) e no sentido horário ou anti-horário
void rotateMesh(Mesh &mesh, bool clockwise)
{
    float delta = clockwise ? -1.0f : 1.0f;

	if (axisX)
		mesh.rotation.x += delta;
	if (axisY)
		mesh.rotation.y += delta;
	if (axisZ)
		mesh.rotation.z += delta;
}

// Escala o mesh para mais próximo ou mais distante do ponto de origem, dependendo do valor de "up"
void scaleMesh(Mesh &mesh, bool up)
{
	float delta = up ? 0.01f : -0.01f;

	if (axisX)
		mesh.scale.x += delta;
	if (axisY)
		mesh.scale.y += delta;
	if (axisZ)
		mesh.scale.z += delta;
}

// Renderiza os meshes com base no shader e nas transformações aplicadas a ele.
void renderMeshes(std::vector<Mesh> &meshes, GLuint shaderID)
{
	for (auto& mesh : meshes) 
	{
		glm::mat4 modelMesh = glm::mat4(1);
		
		// Aplicar transformações do mesh
		modelMesh = glm::translate(modelMesh, mesh.position);
		modelMesh = glm::rotate(modelMesh, glm::radians(mesh.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMesh = glm::rotate(modelMesh, glm::radians(mesh.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMesh = glm::rotate(modelMesh, glm::radians(mesh.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMesh = glm::scale(modelMesh, mesh.scale);
		
		glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, glm::value_ptr(modelMesh));
		
		glBindVertexArray(mesh.VAO);
		glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices);
		glBindVertexArray(0);
	}
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		scale = true;
		rotateEnabled = false;
		translade = false;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		scale = false;
		rotateEnabled = true;
		translade = false;
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		scale = false;
		rotateEnabled = false;
		translade = true;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		axisX = true;
		axisY = false;
		axisZ = false;
	}
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		axisX = false;
		axisY = true;
		axisZ = false;
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		axisX = false;
		axisY = false;
		axisZ = true;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		axisX = true;
		axisY = true;
		axisZ = true;
	}
	if ((key == GLFW_KEY_1 || key == GLFW_KEY_KP_1) && action == GLFW_PRESS)
	{
		active_mesh = 0;
	}
	if ((key == GLFW_KEY_2 || key == GLFW_KEY_KP_2) && action == GLFW_PRESS)
	{
		active_mesh = 1;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		active_mesh = (active_mesh + 1) % 2;
	}
}

//Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupWireframeShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &wireframeVertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::WIREFRAME_VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &wireframeFragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::WIREFRAME_FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::WIREFRAME_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int loadSimpleOBJ(string filePATH, int &nVertices, int r, int g,int b)
 {
    std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
    std::vector<GLfloat> vBuffer;

    std::ifstream arqEntrada(filePATH.c_str());
    if (!arqEntrada.is_open()) 
	{
        std::cerr << "Erro ao tentar ler o arquivo " << filePATH << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(arqEntrada, line)) 
	{
        std::istringstream ssline(line);
        std::string word;
        ssline >> word;

        if (word == "v") 
		{
            glm::vec3 vertice;
            ssline >> vertice.x >> vertice.y >> vertice.z;
            vertices.push_back(vertice);
        } 
        else if (word == "vt") 
		{
            glm::vec2 vt;
            ssline >> vt.s >> vt.t;
            texCoords.push_back(vt);
        } 
        else if (word == "vn") 
		{
            glm::vec3 normal;
            ssline >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } 
        else if (word == "f")
		 {
            while (ssline >> word) 
			{
				int vi = 0;
                std::istringstream ss(word);
                std::string index;

                if (std::getline(ss, index, '/')) vi = !index.empty() ? std::stoi(index) - 1 : 0;
				if (std::getline(ss, index, '/')) { }
				if (std::getline(ss, index)) { }

                vBuffer.push_back(vertices[vi].x);
                vBuffer.push_back(vertices[vi].y);
                vBuffer.push_back(vertices[vi].z);
                vBuffer.push_back(r);
                vBuffer.push_back(g);
                vBuffer.push_back(b);
            }
        }
    }

    arqEntrada.close();

    std::cout << "Gerando o buffer de geometria..." << std::endl;
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	nVertices = vBuffer.size() / 6;  // x, y, z, r, g, b (valores atualmente armazenados por vértice)

    return VAO;
}
