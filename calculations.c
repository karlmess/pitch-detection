//
//  calculations.c
//  FinalProject
//
//  Created by Karl Messerschmidt on 4/27/17.
//  Copyright (c) 2017 Karl. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "main.h"
#include "calculations.h"

void normalize(float *rxx);

void autocorr(float * input, int numBlocks, float rxx[numBlocks][N]){
    
    /*
     Runs time-domain autocorrelation on the input signal.
     Signal is broken into numBlocks blocks of length N:
     N is a precompiler constant defined in main.h
     numBlocks is defined in main() by dividing the number of frames by N
     The block-by-block autocorrelation is returned in rxx.
     */
    
    int firstFrame = 0;
    
    for (int i = 0; i < numBlocks; i++) {       // For each block
        normalize(rxx[i]);
        for(int lag = 0; lag < N; lag++){       // For each lag value
            for (int n = 0; n < N-lag; n++) {   // For each sample in the block
                rxx[i][lag] += (1.0/(N-lag)) * input[firstFrame+n]*input[firstFrame+n+lag];
            }
        }
        firstFrame += N;
    }
}

void pickPeaks(int * maxPeaks, int numBlocks, float rxx[numBlocks][N], float * rxxPower){
    
    /*
     Finds the indices of all peaks in each block of rxx,
     then finds the index of the max peak in each block.
     maxPeaks[] is returned with the index of the max peak of the autocorrelation
     in each block, which the program then uses to calculate the f0.
     */
    
    for (int i = 0; i < numBlocks; i++) {                       // For each block
        int peaksInBlock[N];                                    // Holds the index of all the peaks found in the current block
        int j = 0;
        for (int n = 1; n < N-1; n++) {                         // For each value
            if (
                (rxx[i][n] > rxx[i][n-1]) &&                    // If the value is greater
                (rxx[i][n] > rxx[i][n+1])                       // than the value oneither side of it
                ) {
                peaksInBlock[j] = n;                            // Store the index
                j++;
            }
        }
        maxPeaks[i] = peaksInBlock[0];                          // Set the max peak for the current block to the first one
        for(int n = 0; n < j; n++){                             // For each value in the block
            int currentIndex = peaksInBlock[n];                 // Current index of rxx[] is the value in peaksInBlock
            int currentMax = maxPeaks[i];                       // Index of the current max is maxPeaks[i]
            if (rxx[i][currentIndex] > rxx[i][currentMax]) {    // If peak indexed by current > peak indexed by max
                maxPeaks[i] = currentIndex;                     // then set the max index to the current index
            }
        }
        int k = maxPeaks[i];
        rxxPower[i] = 20*log10(rxx[i][k]);
    }
}
void normalize(float *rxx){
    
    float rms, gain;
    
    for (int i = 0; i < N; i++) {
        rms += rxx[i]*rxx[i];
    }
    rms = rms / N;
    rms = sqrt(rms);
    
    gain = 0.707/rms;
    
    for (int i = 0; i < N; i++) {
        rxx[i] = rxx[i] * gain;
    }
}
