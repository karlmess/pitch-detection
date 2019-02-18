//
//  main.h
//  FinalProject
//
//  Created by Karl Messerschmidt on 4/25/17.
//  Copyright (c) 2017 Karl. All rights reserved.
//

#ifndef FinalProject_main_h
#define FinalProject_main_h

#define N 2048
#define MAX_CHN		2

struct BUF_tag {
	float *buf[MAX_CHN];
};

typedef struct BUF_tag Buf;

#endif
