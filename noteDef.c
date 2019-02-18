//
//  noteDef.c
//  FinalProject
//
//  Created by Karl Messerschmidt on 4/26/17.
//  Copyright (c) 2017 Karl. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sndfile.h>
#include "noteDef.h"

#define MAX_NOTES 96
#define A0 27.5
#define MIN_PITCH 32.7
#define MAX_PITCH 1047

#define LEFT -1
#define RIGHT 1

#define MIN_DB -70

void initNote(Note note[MAX_NOTES]);

int getName(float target, Note *note, int numBlocks);

bool writeAnalysis(SNDFILE *outfile, SF_INFO sfinfo, float * frequency);

void findNotes(float * frequency, int numBlocks, float * rxxPower){
    /* Match up the detected pitches to the nearest note */
    
    Note note[MAX_NOTES];
    
    initNote(note);
    
    printf("Detected    Estimated   Note\n");
    
    float lastNote = 0.0;
    int n = 0;
    
    for (int i = 0; i < numBlocks; i++) {
        // Filter weak detections or out of the desired range
        if (frequency[i] > MIN_PITCH && frequency[i] < MAX_PITCH  && rxxPower[i] > MIN_DB) {
            n = getName(frequency[i], note, numBlocks);
            if (note[n].frequency != lastNote) {    // filter duplicate notes
                printf("%04.2f      %04.2f      %s%d\n", frequency[i], note[n].frequency, note[n].name, note[n].octave);
            }
        }
        lastNote = note[n].frequency;
    }
}

void initNote(Note note[MAX_NOTES]){
    /* Function to fill up the array of notes with note data */
    
    char *letterNames[12] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    
    for (int i = 0; i < MAX_NOTES; i++) {
        note[i].octave = (i > 2) ? (i+9)/12 : i/12;
        note[i].frequency = A0 * powf(2.0, i/12.0);
        note[i].name = letterNames[i%12];
    }
    return;
}

int getName(float target, Note *note, int numBlocks) {
    /*
     Binary search through all frequencies in note[] trickles down to the closest value to
     the target note. Compares to the note directly to the left or right depending on 
     direction of approach just to verify, returns the closer of the two notes.
     */
    
    int index, low, mid, high, from;
    low = 0;
    high = MAX_NOTES;
    mid = (low+high)/2;
    index = 0;
    from = 0;
    while ( low <= high ) {
        if(target < note[mid].frequency){
            index = mid;
            from = RIGHT;
            high = mid-1;
            mid = (low+high)/2;
        }
        else if(target > note[mid].frequency){
            index = mid;
            from = LEFT;
            low = mid+1;
            mid = (low+high)/2;
        }
        else{
            index = mid;
            from = 0;
            break;
        }
    }
    
    float lower = abs(target - note[index+LEFT].frequency);
    float upper = abs(target - note[index+RIGHT].frequency);
    float middle = abs(target - note[index].frequency);
    int answer = (lower<upper) ? index+LEFT : index+RIGHT;
    answer = (middle<abs(target - note[answer].frequency)) ? index : answer;
    
    return answer;
}

/*
    This was used for writing out the output to a sound file to be graphically analyzed in Matlab
    - DOES NOT WORK.  Don't bother with this.
 */
//bool writeAnalysis(SNDFILE *outfile, SF_INFO sfinfo, float * frequency){
//    //float * output = (float *)malloc(sfinfo.frames * sfinfo.channels * sizeof(float));
//    sf_count_t count;
//    
//    char * ofilename = "pitch_transcript.wav";
//    outfile = sf_open(ofilename, SFM_WRITE, &sfinfo);
//    
//	float frameBuf[sfinfo.channels]; /* to hold one sample frame of audio data */
//	for (int i = 0; i < sfinfo.frames; i++) {
//		/* for each frame */
//		//ToDo: interleave separate channel buffers buf->buf[j][i] into a frame_buf[j]
//        for(int j = 0; j < sfinfo.channels; j++){
//            frameBuf[j] = frequency[i];
//        }
//		if ( (count = sf_write_float (outfile, frameBuf, sfinfo.channels)) != sfinfo.channels) {
//			fprintf(stderr, "Error: on sample frame %d\n", i);
//			return false;
//		}
//	}
//    sf_close (outfile);
//    return true;
//}
