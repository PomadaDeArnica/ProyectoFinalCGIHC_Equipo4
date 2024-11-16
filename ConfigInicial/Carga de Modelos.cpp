/*******************

PROYECTO FINAL: Entrega de Proyecto          Rodríguez Montes de Oca Andrés
Fecha de entrega: 15 de noviembre de 2024		     317035867

*******************/


// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>s

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"


// Properties
//const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint WIDTH = 1440, HEIGHT = 900;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void Animation();


// Camera
Camera camera( glm::vec3( -40.0f, 1.0f, 35.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

//ANIM
float rotBall = 0;
bool AnimBall = true;
float bounceBall = 0.0f;
bool bounce = true;
float radius = 6.0f;
float angleDog = 0.0f;   // Ángulo de rotación
float angleBall = 0.0f;


GLfloat deltaTime = 15.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Centro Familiar Medieval", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model boat2((char*)"Models/boat.obj");
    Model boat((char*)"Models/boat.obj");
    Model horse((char*)"Models/horse.obj");
    Model desert((char*)"Models/ProyectoCGHIC9.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        Animation();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
       /* glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);*/
        /*
        model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);
        */
        /*
        model = glm::translate(model, glm::vec3(1.0f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = glm::rotate(model, 4.71f, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        cat.Draw(shader);*/

        
        //model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
       // model = glm::rotate(model, 4.71f, glm::vec3(0.0f, 0.0f, 0.0f));
       // model = glm::translate(model, glm::vec3(0.0f, 150.0f, 250.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        desert.Draw(shader);
        model = glm::translate(model, glm::vec3(38.0f, 0.0f, -10.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        horse.Draw(shader);

        model = glm::mat4(1);
        float posXD = radius * cos(angleDog);  // Coordenada X basada en el ángulo
        float posZD = radius * sin(angleDog);  // Coordenada Z basada en el ángulo
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -12.0f));
        model = glm::translate(model, glm::vec3(posXD, 0.0f, posZD));
        float rotationAngleDog = glm::atan(posZD, posXD) + 180.0f;
        //float rotationAngle = glm::radians(tan(angle) );
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -(rotationAngleDog - 45), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        boat.Draw(shader);

        model = glm::mat4(1);
        //float posXD = radius * cos(angleDog);  // Coordenada X basada en el ángulo
        //float posZD = radius * sin(angleDog);  // Coordenada Z basada en el ángulo
        model = glm::translate(model, glm::vec3(5.0f, 0.5f, -12.0f));
        model = glm::translate(model, glm::vec3(-posXD, 0.0f, -posZD));
       // float rotationAngleDog = glm::atan(posZD, posXD) + 180.0f;
        //float rotationAngle = glm::radians(tan(angle) );
        //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, -(rotationAngleDog - 45), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        boat2.Draw(shader);

       


        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Movimiento de la cámara con las teclas W, A, S, D
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    // Limitamos el movimiento vertical de la cámara en el eje Y entre 0 y 2
    glm::vec3 front = camera.GetFront();
    if (front.x < 0.0f)
    {
        front.x = 0.0f;
    }
    else if (front.x > 2.0f)
    {
        front.x = 2.0f;
    }

   
}

void Animation() {
    if (AnimBall)
    {
        if (bounce)
        {
            bounceBall += 0.02f;
            if (bounceBall >= 1.5f)
            {
                bounce = false;
            }
        }
        else
        {
            bounceBall -= 0.02f;
            if (bounceBall <= 0.0f)
            {
                bounce = true;
            }
        }
    }
    angleBall -= 0.01;
    angleDog += 0.01;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }

    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

