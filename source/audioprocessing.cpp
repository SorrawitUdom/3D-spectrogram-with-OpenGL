#include "audioprocessing.h"

void errorCheck(PaError errorCode){
    if(errorCode != paNoError){
        std::cout << "Exiting application with error" << Pa_GetErrorText(errorCode) << std::endl;
        exit(EXIT_FAILURE);
    }
} 

void printAudioDeviceInformation(PaDeviceIndex device){
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo(device);

    std::cout << "device ID : " << device << std::endl;
    std::cout << " Name                  : " << deviceInfo->name << std::endl;
    std::cout << " HostAPI               : " << deviceInfo->hostApi << std::endl;
    std::cout << " maxInputChannel       : " << deviceInfo->maxInputChannels << std::endl;
    std::cout << " maxOutputChannel      : " << deviceInfo->maxInputChannels  << std::endl;
    std::cout << " maxInputLatency       : " << deviceInfo->defaultHighInputLatency  << std::endl;
    std::cout << " maxOutputLatency      : " << deviceInfo->defaultHighOutputLatency  << std::endl;
    std::cout << " Default Sampling rate : " << deviceInfo->defaultSampleRate  << std::endl;
}

void audioProcessingCleanUp(PaStream *stream, USER_DATA_T* userData){
    PaError errorCode;
    std::cout << "\nStoping stream...\n";

    //--Cleanup
    errorCode = Pa_StopStream(stream);
    errorCheck(errorCode);

    std::cout << "Closing stream...\n" ;

    errorCode = Pa_CloseStream(stream);
    errorCheck(errorCode);

    std::cout << "Cleaning up..\n" ;
    free(userData->leftLevel);
    free(userData->rightLevel);
    free(userData->done);
    free(userData->fft->in);
    free(userData->fft->out);
    fftw_destroy_plan(userData->fft->plan);
    free(userData);

    std::cout << "Exiting..\n" ;
    errorCode = Pa_Terminate();
    errorCheck(errorCode);
}