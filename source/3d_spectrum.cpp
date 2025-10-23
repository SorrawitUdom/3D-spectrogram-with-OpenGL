#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include "includes/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "includes/learnopengl/shader_m.h"
#include "includes/learnopengl/camera.h"

#include <iostream>


#include "audioprocessing.h"
#include <stdlib.h>
#include <cstring>
#include <cmath>


#define INTENSITY_RANGE_SIZE 20
#define BASE_INTENSITY       0.4

static int audioStreamCallback (
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
);

static float inline min(float a, float b){
    return (a < b) ? a : b;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.8f, 0.3f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    printf("initiated\n");

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.4.camera.vs", "7.4.camera.fs");
    printf("Shadder Created!\n");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
        -0.005f, -0.005f, -0.005f,  0.0f, 0.0f,
         0.005f, -0.005f, -0.005f,  1.0f, 0.0f,
         0.005f,  0.005f, -0.005f,  1.0f, 1.0f,
         0.005f,  0.005f, -0.005f,  1.0f, 1.0f,
        -0.005f,  0.005f, -0.005f,  0.0f, 1.0f,
        -0.005f, -0.005f, -0.005f,  0.0f, 0.0f,

        -0.005f, -0.005f,  0.005f,  0.0f, 0.0f,
         0.005f, -0.005f,  0.005f,  1.0f, 0.0f,
         0.005f,  0.005f,  0.005f,  1.0f, 1.0f,
         0.005f,  0.005f,  0.005f,  1.0f, 1.0f,
        -0.005f,  0.005f,  0.005f,  0.0f, 1.0f,
        -0.005f, -0.005f,  0.005f,  0.0f, 0.0f,

        -0.005f,  0.005f,  0.005f,  1.0f, 0.0f,
        -0.005f,  0.005f, -0.005f,  1.0f, 1.0f,
        -0.005f, -0.005f, -0.005f,  0.0f, 1.0f,
        -0.005f, -0.005f, -0.005f,  0.0f, 1.0f,
        -0.005f, -0.005f,  0.005f,  0.0f, 0.0f,
        -0.005f,  0.005f,  0.005f,  1.0f, 0.0f,

         0.005f,  0.005f,  0.005f,  1.0f, 0.0f,
         0.005f,  0.005f, -0.005f,  1.0f, 1.0f,
         0.005f, -0.005f, -0.005f,  0.0f, 1.0f,
         0.005f, -0.005f, -0.005f,  0.0f, 1.0f,
         0.005f, -0.005f,  0.005f,  0.0f, 0.0f,
         0.005f,  0.005f,  0.005f,  1.0f, 0.0f,

        -0.005f,  0.005f, -0.005f,  1.0f, 1.0f,
         0.005f,  0.005f, -0.005f,  1.0f, 1.0f,
         0.005f,  0.005f,  0.005f,  1.0f, 1.0f,
         0.005f,  0.005f,  0.005f,  1.0f, 1.0f,
        -0.005f,  0.005f,  0.005f,  1.0f, 1.0f,
        -0.005f,  0.005f, -0.005f,  1.0f, 1.0f
    };


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();

    printf("Texture used!\n");

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    PaError errorCode;
    PaDeviceIndex deviceCount;
    PaStreamParameters inputParameters, outputParameters;
    const PaDeviceInfo *activeDevice;

    userData = (USER_DATA_T*)calloc(1,sizeof(USER_DATA_T));

    userData->leftLevel    = (double *)calloc(FRAME_PER_BUFFER,sizeof(double));
    userData->rightLevel   = (double *)calloc(FRAME_PER_BUFFER,sizeof(double));
    userData->done         = (unsigned char*)calloc(1,sizeof(unsigned char));

    *(userData->done) = 0;

    //--Initialize PortAudio Application
    errorCode = Pa_Initialize();
    errorCheck(errorCode);

    deviceCount = Pa_GetDeviceCount();
    if(deviceCount == 0){
        std::cout << "No, audio device !" << std::endl;
    }

    std::cout << std::endl;
    printAudioDeviceInformation(10);

    activeDevice = Pa_GetDeviceInfo(10);

    //Setting the parameter of input streaam
    memset(&inputParameters,0,sizeof(inputParameters));
    inputParameters.channelCount = 2;
    inputParameters.device = 10;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = activeDevice->defaultLowInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field
    
    //Setting parameter of output stream
    memset(&outputParameters,0,sizeof(outputParameters));
    outputParameters.channelCount = 2;
    outputParameters.device = 10;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = activeDevice->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field


    //---------------------------------------------------------
    //--Initializing the FFTW
    //--Note: Since FFT operation is going to be part 
    //        of the call back calculation so it need to
    //        initialize before start pa stream
    //---------------------------------------------------------

    //--Allocating memory for FFTW structure
    //---------------------------------------

    userData->fft          = (FFTW_DATA_T *)calloc(1,sizeof(FFTW_DATA_T));
    if(userData->fft == NULL){
        std::cout << "Cannot allocate memory FFTW structure" << std::endl;
        exit(EXIT_FAILURE);
    }

    //--Allocating memory for FFTW input and output buffer
    //----------------------------------------------------------
    userData->fft->in = (double *)calloc(FRAME_PER_BUFFER,sizeof(double));
    userData->fft->out = (double *)calloc(FRAME_PER_BUFFER,sizeof(double));
    if(userData->fft->in == NULL || userData->fft->out == NULL){
        std::cout << "Cannot allocate memory FFTW buffer" << std::endl;
        exit(EXIT_FAILURE);
    }

    //--Creating FFTW plan
    //------------------------------------------------------------
    userData->fft->plan = fftw_plan_r2r_1d(FRAME_PER_BUFFER,userData->fft->in,userData->fft->out,FFTW_R2HC,FFTW_ESTIMATE);
    
    //-- Initialize audio stream
    errorCode = Pa_OpenStream(
        &audioStream,
        &inputParameters,
        &outputParameters,
        DEFAULT_SAMPLE_RATE,
        FRAME_PER_BUFFER,
        paNoFlag,
        audioStreamCallback,
        (void *)userData
    );
    errorCheck(errorCode);

    errorCode =  Pa_StartStream(audioStream);
    errorCheck(errorCode);

    //Make the pa audio sampling data for every 30 ms
    Pa_Sleep(100UL);



    double sampleRatio = FRAME_PER_BUFFER / DEFAULT_SAMPLE_RATE;
    int startIndex = std::ceil(sampleRatio * START_SPECTRO_FREQ);
    int spectroSize = min(
        std::ceil(sampleRatio * END_SPECTRO_FREQ),
        FRAME_PER_BUFFER / 2.0
    ) - startIndex;

    
    int freqBin = 256;
    int timeBin = 256;
    double dataBuffer[timeBin][FRAME_PER_BUFFER]; 
    double red,green,blue;

    for(int i = 0; i < timeBin;i++){
        for(int j = 0; j < FRAME_PER_BUFFER;j++){
            dataBuffer[i][j] = 0.0f;
        }
    }
    float scalefact = 1.0f;
    double shiftTime = 0.0f;


    GLuint normalizedScaleLoc = glGetUniformLocation(ourShader.ID,"normalizedScale");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        ourShader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        for (unsigned int i = 0; i < timeBin; i++)
        {
            for(unsigned int j = 0; j < freqBin;j++){
                // calculate the model matrix for each object and pass it to shader before drawing
                double prop =  std::pow(j / (double)freqBin,2) ;
                double freq = dataBuffer[i][(int)(startIndex + prop*spectroSize)];
                float scalefact = freq*0.5;
                if(scalefact < 0.0f)
                    scalefact = 0.0f;
                else if(scalefact > 20.0f)
                    scalefact = 20.0f;
                double normalizedScale = scalefact/20.0f;
                
                glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model = glm::translate(model, glm::vec3(0.01f*j,0.0f,-0.01f*i));
                red = 0.0f; green = 0.0f; blue =0.0f;
                glUniform1f(normalizedScaleLoc,normalizedScale);
                model = glm::translate(model, glm::vec3(0.0f,0.005*(scalefact-1),0.0f));
                model = glm::scale(model,glm::vec3(1.0f,scalefact,1.0f));
                ourShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 30);
            }
        }

        if(glfwGetTime()- shiftTime >= 0.00125){
            memcpy(dataBuffer+1,dataBuffer,FRAME_PER_BUFFER*(timeBin-1)*sizeof(double));
            if(*(userData->done) == 1){
                memcpy(dataBuffer[0],userData->fft->out,FRAME_PER_BUFFER*sizeof(double));
            *(userData->done) == 0;
            }
            shiftTime = glfwGetTime();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    audioProcessingCleanUp(audioStream,userData);


    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

static int audioStreamCallback (
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData
){
    USER_DATA_T *data = (USER_DATA_T*)userData;
    FFTW_DATA_T *fft = data->fft;
    float *in = (float*) inputBuffer;
    (void) outputBuffer;
    int i = 0;

    float cumLeft = 0, cumRight = 0;

    int j = 0;
    for(i = 0; i < framesPerBuffer*2; i+=2){
        data->leftLevel[j] = in[i];
        fft->in[j] = in[i];
        j++;
    }
    fftw_execute(fft->plan);
    *(data->done) = 1;
    return 0;
}