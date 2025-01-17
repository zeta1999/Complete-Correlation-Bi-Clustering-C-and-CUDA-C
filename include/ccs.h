#ifndef _CCS_H_
#define _CCS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <math.h>
#include<time.h>

#define SUCCESS                  0
#define TRUE                     1
#define FALSE                    0
#define FAIL                    -1


#define mingene 10    //minimum number of genes in a bicluster 
#define min 10        //minimum number of samples in a bicluster
#define MAXB 1000     //default number of base gene (outer loop) in the decreasing order of SD to consider for forming the biclusters. Overide if -m number>1000 is used 


#define BUFFER_MAXSIZE 1048576


struct gn{
  char *id;
  int indx;
  double *x;
};

struct bicl{
  char *sample,*data;
  int samplecount,datacount; 
  double score;
};


void printUsage();
void getmatrixsize(FILE *,int *,int *);
void readgene(char *,struct gn *,char **,int,int);
void computebicluster(struct gn *,int,int,double,int,struct bicl *);
void printbicluster(FILE *,struct gn *,char **,int,int,int,double,struct bicl *,int);
double between_bicluster_correlation(struct gn *,struct bicl *, int,int,int,int,double);
void mergebcl(struct bicl *, int,int,int,int,double);

#endif  // _CCS_H_
