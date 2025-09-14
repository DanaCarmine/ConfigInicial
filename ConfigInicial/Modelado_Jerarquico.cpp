/*

Práctica 5								   Hernández Rubio Dana Valeria
Fecha de entrega: 14 de Septiembre del 2025		     	      317345153

*/
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

// --- Rangos en grados  ---
constexpr float SHOULDER_MIN = -130.0f;
constexpr float SHOULDER_MAX = 110.0f;

constexpr float ELBOW_MIN = 0.0f;
constexpr float ELBOW_MAX = 145.0f;

constexpr float WRIST_MIN = -70.0f;
constexpr float WRIST_MAX = 80.0f;

constexpr float MCP_MIN = -10.0f;  // Dedos A
constexpr float MCP_MAX = 90.0f;

constexpr float IP_MIN = 0.0f;     // Dedos B
constexpr float IP_MAX = 100.0f;

// Clamp helper (compatible con C++11)
inline float clampf(float v, float lo, float hi) {
	return (v < lo) ? lo : (v > hi ? hi : v);
}


//For model
float	hombro = 0.0f,
codo = 0.0f,
muneca = 0.0f,
dedo3ArribaA,
dedo3ArribaB,
dedo3AbajoA,
dedo3AbajoB;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado jerarquico - Hernandez Rubio Dana Valeria", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection


	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		0.5f,  0.5f, 0.5f,  
		-0.5f,  0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		
	    -0.5f, -0.5f,-0.5f, 
		 0.5f, -0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
		 0.5f,  0.5f,-0.5f, 
	    -0.5f,  0.5f,-0.5f, 
	    -0.5f, -0.5f,-0.5f, 
		
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,
      
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color= glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	

		glBindVertexArray(VAO);
		
		//Model Bicep
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//Model Antebrazo
		modelTemp = model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 1.0, 0.0f)); //codo
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//B

		//Model Palma
		modelTemp = model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0, 0.0f)); //muñeca
		modelTemp2 = modelTemp = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//C

		// =================== DEDO 1 =================== Arriba
		// Model Dedo A (1)
		glm::mat4 baseA1 = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.375f));
		glm::mat4 rotA1 = glm::rotate(baseA1, glm::radians(-dedo3ArribaA), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midA1 = glm::translate(rotA1, glm::vec3(0.375f, 0.0f, 0.0f));
		glm::mat4 endA1 = glm::translate(rotA1, glm::vec3(0.75f, 0.0f, 0.0f));

		model = glm::scale(midA1, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Model Dedo B (1)
		modelTemp = endA1; // pivote de B1 = extremo de A1
		glm::mat4 rotB1 = glm::rotate(modelTemp, glm::radians(-dedo3ArribaB), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midB1 = glm::translate(rotB1, glm::vec3(0.25f, 0.0f, 0.0f));

		model = glm::scale(midB1, glm::vec3(0.5f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// =================== DEDO 2 =================== Arriba
		// Model Dedo A (2)
		glm::mat4 baseA2 = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.375f));
		glm::mat4 rotA2 = glm::rotate(baseA2, glm::radians(-dedo3ArribaA), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midA2 = glm::translate(rotA2, glm::vec3(0.375f, 0.0f, 0.0f));
		glm::mat4 endA2 = glm::translate(rotA2, glm::vec3(0.75f, 0.0f, 0.0f));

		model = glm::scale(midA2, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Model Dedo B (2)
		modelTemp = endA2; // pivote de B2
		glm::mat4 rotB2 = glm::rotate(modelTemp, glm::radians(-dedo3ArribaB), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midB2 = glm::translate(rotB2, glm::vec3(0.25f, 0.0f, 0.0f));

		model = glm::scale(midB2, glm::vec3(0.5f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// =================== DEDO 3 =================== Arriba
		// Model Dedo A (3)
		glm::mat4 baseA3 = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.0f));
		glm::mat4 rotA3 = glm::rotate(baseA3, glm::radians(-dedo3ArribaA), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midA3 = glm::translate(rotA3, glm::vec3(0.375f, 0.0f, 0.0f));
		glm::mat4 endA3 = glm::translate(rotA3, glm::vec3(0.75f, 0.0f, 0.0f));

		model = glm::scale(midA3, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Model Dedo B (3)
		modelTemp = endA3; // pivote de B3
		glm::mat4 rotB3 = glm::rotate(modelTemp, glm::radians(-dedo3ArribaB), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midB3 = glm::translate(rotB3, glm::vec3(0.25f, 0.0f, 0.0f));

		model = glm::scale(midB3, glm::vec3(0.5f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// =================== DEDO 4 =================== Abajo
		// Model Dedo A (4)
		glm::mat4 baseA4 = glm::translate(modelTemp2, glm::vec3(0.25f, -0.35f, 0.1875f));
		glm::mat4 rotA4 = glm::rotate(baseA4, glm::radians(dedo3AbajoA), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midA4 = glm::translate(rotA4, glm::vec3(0.375f, 0.0f, 0.0f));
		glm::mat4 endA4 = glm::translate(rotA4, glm::vec3(0.75f, 0.0f, 0.0f));

		model = glm::scale(midA4, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Model Dedo B (4)
		modelTemp = endA4; // pivote de B4
		glm::mat4 rotB4 = glm::rotate(modelTemp, glm::radians(dedo3AbajoB), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midB4 = glm::translate(rotB4, glm::vec3(0.25f, 0.0f, 0.0f));

		model = glm::scale(midB4, glm::vec3(0.5f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// =================== DEDO 5 =================== Abajo
		// Model Dedo A (5)
		glm::mat4 baseA5 = glm::translate(modelTemp2, glm::vec3(0.25f, -0.35f, -0.1875f));
		glm::mat4 rotA5 = glm::rotate(baseA5, glm::radians(dedo3AbajoA), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midA5 = glm::translate(rotA5, glm::vec3(0.375f, 0.0f, 0.0f));
		glm::mat4 endA5 = glm::translate(rotA5, glm::vec3(0.75f, 0.0f, 0.0f));

		model = glm::scale(midA5, glm::vec3(0.75f, 0.3f, 0.25f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Model Dedo B (5)
		modelTemp = endA5; // pivote de B5
		glm::mat4 rotB5 = glm::rotate(modelTemp, glm::radians(dedo3AbajoB), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 midB5 = glm::translate(rotB5, glm::vec3(0.25f, 0.0f, 0.0f));

		model = glm::scale(midB5, glm::vec3(0.5f, 0.3f, 0.25f));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);

		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

	void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Cámara / vista
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Left
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // Right
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Camara arriba
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // Camara abajo
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Camara adelante
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Camara atras
		movZ += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Rotacion derecha
		rot += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // Rotacion izquierda
		rot -= 0.18f;

	// Articulaciones
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // Hombro arriba
		hombro += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // Hombro abajo
		hombro -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // Codo derecha (flexiona +)
		codo += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // Codo izquierda (flexiona -)
		codo -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // Muñeca adelante
		muneca += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) // Muñeca atras
		muneca -= 0.18f;

	// Dedos A (grupo superior e inferior) 
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // 3 arriba A -> adelante
		dedo3ArribaA += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // 3 arriba A -> atrás
		dedo3ArribaA -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // 2 abajo A -> adelante
		dedo3AbajoA += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // 2 abajo A -> atrás
		dedo3AbajoA -= 0.18f;

	// Dedos B (distales)
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // 3 arriba B -> adelante
		dedo3ArribaB += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // 3 arriba B -> atrás
		dedo3ArribaB -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // 2 abajo B -> adelante
		dedo3AbajoB += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) // 2 abajo B -> atrás
		dedo3AbajoB -= 0.18f;

	// ----------------- LIMITES (CLAMPS) -----------------
	hombro = clampf(hombro, SHOULDER_MIN, SHOULDER_MAX);
	codo = clampf(codo, ELBOW_MIN, ELBOW_MAX);
	muneca = clampf(muneca, WRIST_MIN, WRIST_MAX);


	dedo3ArribaA = clampf(dedo3ArribaA, MCP_MIN, MCP_MAX);
	dedo3AbajoA = clampf(dedo3AbajoA, MCP_MIN, MCP_MAX);
	dedo3ArribaB = clampf(dedo3ArribaB, IP_MIN, IP_MAX);
	dedo3AbajoB = clampf(dedo3AbajoB, IP_MIN, IP_MAX);

	if (rot > 360.f || rot < -360.f) {
		rot = std::fmod(rot, 360.f);
	}
}

