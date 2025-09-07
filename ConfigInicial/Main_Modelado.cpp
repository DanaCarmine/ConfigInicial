/*

Práctica 4								   Hernández Rubio Dana Valeria
Fecha de entrega: 7 de Septiembre del 2025		     	      317345153

*/

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modelado geometrico - Hernandez Rubio Dana Valeria", nullptr, nullptr);

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
	
	// usando un color uniforme en el shader para cambiar el color de los objetos
	GLint faceColorsLoc = glGetUniformLocation(ourShader.Program, "uFaceColors");
	//Paletas de colores

	glm::vec3 paletteWhite[6] = {
		{0.92f, 0.92f, 0.92f},   // +X blanco puro
		{0.95f, 0.95f, 0.95f},// -X gris muy claro
		{0.9f, 0.9f, 0.9f},   // +Y gris claro
		{0.85f, 0.85f, 0.85f},// -Y gris un poco más oscuro
		{1.0f, 1.0f, 1.0f},// +Z gris suave
		{0.8f, 0.8f, 0.8f}    // -Z gris claro tirando a sombra
	};

	glm::vec3 paletteBlack[6] = {
		{0.05f, 0.05f, 0.05f}, // +X negro casi puro
		{0.1f, 0.1f, 0.1f},    // -X negro un poco levantado
		{0.15f, 0.15f, 0.15f}, // +Y gris muy oscuro
		{0.2f, 0.2f, 0.2f},    // -Y gris oscuro
		{0.12f, 0.12f, 0.12f}, // +Z gris medio oscuro
		{0.25f, 0.25f, 0.25f}  // -Z gris más claro (reflejo simulado)
	};

	glm::vec3 paletteBlue[6] = {
		 {0.18f, 0.49f, 0.73f}, // +X base
		{0.15f, 0.43f, 0.66f}, // -X
		{0.12f, 0.36f, 0.58f}, // +Y
		{0.10f, 0.30f, 0.50f}, // -Y
		{0.22f, 0.56f, 0.80f}, // +Z un poco más brillante
		{0.08f, 0.25f, 0.45f}  // -Z más oscuro
	};

	glm::vec3 paletteTongue[6] = {
		{0.78f, 0.27f, 0.27f}, // +X base
		{0.70f, 0.23f, 0.23f}, // -X
		{0.62f, 0.20f, 0.20f}, // +Y
		{0.55f, 0.17f, 0.17f}, // -Y
		{0.85f, 0.32f, 0.32f}, // +Z highlight
		{0.45f, 0.14f, 0.14f}  // -Z sombra
	};

	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
	    -0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,
      
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
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
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.95f, 0.64f, 0.23f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
		//Cabeza
	    model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.3f, 1.1f, 1.3f));//ancho, grasor , profundidad
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Torso
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.5f)); // Tamaño
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f)); // Posición
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pantalones
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.5f));//Tamaño	
		model = glm::translate(model, glm::vec3(0.0f, 0.07f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Brazo 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.23f, 0.4f, 0.23f));//Tamaño 	
		model = glm::translate(model, glm::vec3(-3.0f, -0.2f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Brazo 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.23f, 0.4f, 0.23f));//Tamaño 	
		model = glm::translate(model, glm::vec3(3.0f, -0.2f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Mano 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));//Tamaño 	
		model = glm::translate(model, glm::vec3(-1.6f, -0.5f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Mano 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));//Tamaño 	
		model = glm::translate(model, glm::vec3(1.6f, -0.5f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Hombro 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.23f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Hombro 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.23f, 0.23f));//Tamaño 
		model = glm::translate(model, glm::vec3(1.5f, 1.0f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pantalon 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));//Tamaño 	
		model = glm::translate(model, glm::vec3(-0.8f, -0.9f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pantalon 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));//Tamaño 	
		model = glm::translate(model, glm::vec3(0.8f, -0.9f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.6f, 0.25f));//Tamaño 	
		model = glm::translate(model, glm::vec3(-1.0f, -0.9f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.6f, 0.25f));//Tamaño 	
		model = glm::translate(model, glm::vec3(1.0f, -0.9f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Zapato 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.39f, 0.37f, 0.39f));//Tamaño 	
		model = glm::translate(model, glm::vec3(-0.6f, -2.5f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Zapato 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.39f, 0.37f, 0.39f));//Tamaño 	
		model = glm::translate(model, glm::vec3(0.6f, -2.5f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Punta de Zapato 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.39f, 0.28f, 0.4f));//Tamaño 	
		model = glm::translate(model, glm::vec3(-0.6f, -3.45f, 0.7f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Punta de Zapato 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.39f, 0.28f, 0.39f));//Tamaño 	
		model = glm::translate(model, glm::vec3(1.3f, -3.45f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Agarradera 1 
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.66f, 0.2f));//Tamaño 	
		model = glm::translate(model, glm::vec3(4.8f, 1.5f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Agarradera 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));//Tamaño 	
		model = glm::translate(model, glm::vec3(3.8f, 3.8f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Agarradera 3
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));//Tamaño 	
		model = glm::translate(model, glm::vec3(3.8f, 6.1f, 0.0f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Nariz
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.23f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(0.0f ,4.0f, 3.0));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Boca 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.45f, 0.1f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 2.5));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Boca 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(0.0f, 7.0f, 2.5));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Lengua
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.28f, 0.07f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(0.0f, 8.7f, 2.8));
		glUniform3fv(faceColorsLoc, 6, &paletteTongue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ojo 1 izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(4.0f, 3.76f, 2.5));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//ojo 2 izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.35f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(1.9f, 3.3f, 2.5));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//ojo 1 der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(-2.5f, 3.76f, 2.5));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//ojo 2 der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.15f, 0.35f, 0.23f));//Tamaño
		model = glm::translate(model, glm::vec3(-2.5f, 3.3f, 2.5));
		glUniform3fv(faceColorsLoc, 6, &paletteBlack[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Popote 1 
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.66f, 0.2f));//Tamaño 	
		model = glm::translate(model, glm::vec3(2.0f, 2.5f,- 1.5f));
		glUniform3fv(faceColorsLoc, 6, &paletteWhite[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Popote 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.21f, 0.2f, 0.22f));//Tamaño 	
		model = glm::translate(model, glm::vec3(1.9f, 8.3f, -1.4f));
		glUniform3fv(faceColorsLoc, 6, &paletteBlue[0].x);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Swap the screen buffers
		glfwSwapBuffers(window);


	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		 movY += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		 movY -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.01f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
 }


