//
//  noteDef.h
//  FinalProject
//
//  Created by Karl Messerschmidt on 4/26/17.
//  Copyright (c) 2017 Karl. All rights reserved.
//

#ifndef FinalProject_noteDef_h
#define FinalProject_noteDef_h

struct tagNote{
    int octave;
    float frequency;
    char * name;
};
typedef struct tagNote Note;

void findNotes(float * frequency, int numBlocks, float * rxxPower);

#endif
