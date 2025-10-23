#ifndef AUDIO_PROCESS_H
#define AUIDO_PROCESS_H


#include <portaudio.h>
#include <fftw3.h>
#include <iostream>
#include <stdlib.h>

#ifndef DEFAULT_SAMPLE_RATE
#define DEFAULT_SAMPLE_RATE 44100.0
#endif

#ifndef FRAME_PER_BUFFER
#define FRAME_PER_BUFFER    2048
#endif

#ifndef START_SPECTRO_FREQ
#define START_SPECTRO_FREQ  20
#endif

#ifndef END_SPECTRO_FREQ
#define END_SPECTRO_FREQ    12000
#endif

//auido level datatype 

typedef struct fftData_t{
    double *in; 
    double *out;
    fftw_plan plan;
}FFTW_DATA_T;

typedef struct userData_t{
    double *leftLevel;
    double *rightLevel;
    unsigned char *done;
    FFTW_DATA_T *fft;
}USER_DATA_T;



void errorCheck(PaError errorCode);
void printAudioDeviceInformation(PaDeviceIndex device);
void audioProcessingCleanUp(PaStream *stream, USER_DATA_T* userData);



static USER_DATA_T *userData; 
static PaStream *audioStream;


// FFTW parameters



#endif