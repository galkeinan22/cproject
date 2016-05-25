/*
 * SPPoint.c
 *
 *  Created on: May 17, 2016
 *      Author: alonfrydberg
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPPoint.h"

struct sp_point_t{
	double * data;
	int dim;
	int index;
};

SPPoint spPointCreate(double* data, int dim, int index){
	int i;
	//check validity of input
	if(index<0 || dim<=0 || !data)
		return 0;

	//allocate  memory
	double * temp = (double*)malloc(dim*sizeof(double));
	SPPoint point =(SPPoint)malloc (sizeof(struct sp_point_t));

	//check allocation failure
	if(!point || !temp)//if NULL
		return 0;//return NULL

	//copy data into temp
	for(i = 0; i<dim; i++){
		temp[i] = data[i];
	}

	//assign fields
	point->data = temp;
	point->dim = dim;
	point->index = index;

	return point;
}

SPPoint spPointCopy(SPPoint source){
	assert(source);
	return spPointCreate(source->data, source->dim , source->index);
}

void spPointDestroy(SPPoint point){
	if (point){
		free(point->data);
		free(point);
	}
}

int spPointGetDimension(SPPoint point){
	assert(point);
	return point->dim;
}

int spPointGetIndex(SPPoint point){
	assert(point);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	assert(point);
	assert(axis < point->dim);
	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p && q);
	assert(p->dim == q->dim);
	int i;
	double d, res=0;
	for (i = 0; i < p->dim; i++) {
		d= p->data[i] - q->data[i];
		res += d*d;
	}
	return res;
}
