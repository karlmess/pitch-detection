//
//  calculations.h
//  FinalProject
//
//  Created by Karl Messerschmidt on 4/27/17.
//  Copyright (c) 2017 Karl. All rights reserved.
//

#ifndef FinalProject_calculations_h
#define FinalProject_calculations_h

void autocorr(float * input, int numBlocks, float rxx[numBlocks][N]);

void pickPeaks(int * maxPeaks, int numBlocks, float rxx[numBlocks][N], float * rxxPower);

#endif
