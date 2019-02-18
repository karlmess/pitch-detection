//
//  main.c
//  FinalProject
//
//  Created by Karl Messerschmidt on 4/25/17.
//  Copyright (c) 2017 Karl. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include <math.h>
#include <stdbool.h>
#include "main.h"
#include "calculations.h"
#include "noteDef.h"

#define MIN_LAG 15
#define MAX_LAG 800

#define WRITE_OUT 0

#ifndef N
#define N 2048
#endif

int usage(int argc, const char * argv[]);

bool loadBuffer(SNDFILE *sfile, SF_INFO sfinfo, float * input);

int main(int argc, const char * argv[])
{

	/* VARIABLE DECLARATIONS */
    
    /* Set up libsndfile data structures */
	SNDFILE *sfile;
	SF_INFO sfinfo;
    
    /* Other variables */
    float * input;
    int numBlocks;
    
    /* PARSE COMMAND LINE */
    if (usage(argc, argv)) {
        return -1;
    }
    const char * filename = argv[1];
    
    /* OPEN FILE, ALLOCATE BUFFER, READ INTO BUFFER */
    
    /* Open file */
    memset(&sfinfo, 0, sizeof(sfinfo));                     // Zero out header info
    if(!(sfile = sf_open(filename, SFM_READ, &sfinfo))){
        printf("Unable to open file %s!\n", filename);
    }
    
    /* Allocate buffer */
    input = (float *)malloc(sfinfo.frames * sfinfo.channels * sizeof(float));
    //input = (float *)malloc(sfinfo.frames * sizeof(float));
    
    /* Read input file into buffer */
    if(!loadBuffer(sfile, sfinfo, input)){
        printf("Failed to read file contents.\n");
        return -1;
    }
    
    /*** RUN AUTOCORRELATION ***/
    numBlocks = ceil(sfinfo.frames/(N*1.0));
    float rxx[numBlocks][N];            // Declare autocorrelation matrix
    memset(rxx, 0.0, sizeof(float));    // Zero out autocorrelation matrix
    autocorr(input, numBlocks, rxx);    // Autocorrelation function
    
    /* PICK PEAKS */
    int maxPeaks[numBlocks];            // Holds the index of the max peak of each block of rxx
    float rxxPower[numBlocks];          // Holds power in dB of the max peak of each block, used to filter weak detections
    pickPeaks(maxPeaks, numBlocks, rxx, rxxPower);
    
    /* CALCULATE F0's FROM MAX PEAKS */
    float frequency[numBlocks];         // Holds calculated frequencies after max peaks are returned
    for (int i = 0; i < numBlocks; i++) {
        frequency[i] = sfinfo.samplerate / (maxPeaks[i]*1.0);
    }
    
    /* CORRELATE WITH NOTES */
    findNotes(frequency, numBlocks, rxxPower);
    
    /* FINISH UP */
    sf_close (sfile);    // Close ya dang files
    return 0;
}


int usage(int argc, const char * argv[]){
    /* Error checking */
    if (argc != 2) {
        printf("Usage: %s filename.wav\n", argv[0]);
        return -1;
    }
    else return 0;
}

bool loadBuffer(SNDFILE *sfile, SF_INFO sfinfo, float * input){
    float frameBuf[sfinfo.channels];
    sf_count_t count;
    for (int i=0; i<sfinfo.frames; i++) {
		if ( (count = sf_read_float (sfile, frameBuf, sfinfo.channels)) != sfinfo.channels) {
			fprintf(stderr, "Error: on sample frame %d\n", i);
			return false;
		}
        input[i] = frameBuf[0]; // Keep only first channel
	}
    return true;
}

