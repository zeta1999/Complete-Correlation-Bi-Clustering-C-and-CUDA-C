/**************************************************************************
 * Complete Correlation Subgroups (CCS) 
 * Description: Biclustering has been emerged as a powerful tool for 
                identification of a group of co-expressed genes under a subset 
                of experimental conditions (measurements) present in a gene 
                expression dataset.  In this program we implemented CCS biclustering. 
 * Developer: Dr. Anindya Bhattacharya and Dr. Yan Cui, UTHSC, Memphis, TN, USA
 * Email: anindyamail123@gmail.com; ycui2@uthsc.edu 

Note: The minimum number of genes and the samples per bicluster is 10. 
User can alter the minimum size by changing the values for 'mingene' 
and 'min' defined in "ccs.h" file for minimum number of genes and samples
respectively. 

****************************************************************************
*/

#include "../include/ccs.h"

int main(int argc, char *argv[])
{
	FILE   *in,*out;
	struct gn *gene;
	char **Hd;
	char *infile,*outfile;	
	int mydebug,c, errflag,maxbcn=MAXB,print_type=0;
	int i,n,D,count;
	extern char *optarg;
	double thr;
        struct bicl *bicluster;
          
	 clock_t start = clock() ;
	 in= out = NULL;

  	 n = D = 0;
	 thr = errflag = 0;
  
	 while ((c = getopt(argc, argv, "ht:m:i:p:o:?")) != -1)
  	 {
    		switch(c)
    		{
    			case 'h': // help
      				printUsage();
      				exit(0);
    			case 't': // threshold value
      				thr = atof(optarg);
      				break;
    			case 'm': // maximum number of bicluster search
      				maxbcn = atoi(optarg);
      				break;
    			case 'p': // output file format
      				print_type = atoi(optarg);
     				break;

    			case 'i': // the input expression file
      				infile = optarg;
      				break;
    			case 'o': // the output file
      				outfile = optarg;
      				break;
    			case ':':       /* -f or -o without operand */
      				printf("Option -%c requires an operand\n", optopt);
      				errflag++;
     				break;
    			case '?':
      				fprintf(stderr,"Unrecognized option: -%c\n", optopt);
      				errflag++;
    		}
 	  }



  	if (thr == 0)
  	{
    		fprintf(stderr,"***** WARNING: Threshold Theta (corr coeff) "
                   "value assumed to be ZERO (0)\n");
  	}


  	if (outfile[0] == '\0')
  	{
    		fprintf(stderr,"***** WARNING: Output file assumed to be STDOUT\n");
    		out = stdout;
  	}
  	else if ((out = fopen(outfile,"w")) == NULL) //write open bicluster file
  	{
    		fprintf(stderr,"***** ERROR: Unable to open Output file %s\n",outfile);
    		errflag++;
  	}

	if ((thr < 0) || (thr > 1))
  	{
    		fprintf(stderr,"***** ERROR: Threshold Theta (corr coeff) "
                   "must be between 0.0-1.0\n");
  	}

  	if (infile[0] == '\0')
  	{
    		fprintf(stderr,"***** ERROR: Input file not defined\n");
    		if (out) fclose(out);
    			errflag++;
  	}
 	else if ((in = fopen(infile,"r")) == NULL)  //open gene file
  	{
    		fprintf(stderr,"***** ERROR: Unable to open Input %s\n", infile);
    		if (out) fclose(out);
    			errflag++;
  	}

  	if (errflag)
  	{
    		printUsage();
    		exit(1);
  	}

	getmatrixsize(in,&n,&D);
	printf("Number of rows=%d\tNumber of columns=%d\n",n,D);

	if(maxbcn>n)
		maxbcn=n;

 	gene = (struct gn *)calloc(n,sizeof(struct gn));
 	Hd = (char **)calloc(D+1,sizeof(char *));

  	for (i = 0; i < n; i++)
	    gene[i].x = (double *)calloc(D+2,sizeof(double));

  	readgene(infile,gene,Hd,n,D);	

  	count=0;

	bicluster = (struct bicl *)calloc(maxbcn,sizeof(struct bicl));

  	for (i = 0; i < maxbcn; i++)
  	{
   	       bicluster[i].sample = (char *)calloc(D,sizeof(char));
 	       bicluster[i].data = (char *)calloc(n,sizeof(char));
	       bicluster[i].samplecount=0;
    	       bicluster[i].datacount=0;
	       computebicluster(gene,n,D,thr,i,bicluster);
  	}

        printbicluster(out,gene,Hd,n,D,maxbcn,thr,bicluster,print_type);
  
 	clock_t end = clock() ;
 	double elapsed_time = (end-start)/(double)CLOCKS_PER_SEC ;
 	printf("Ellapsed time=%lf\n",elapsed_time);
        if(print_type==0)     
        	fprintf(out,"\n\nEllapsed time=%lf\n",elapsed_time);

  	if (out) fclose(out);

  	for (i = 0; i < n; i++)
		free(gene[i].x);
	for (i = 0; i < D+1; i++)   {   
		free(Hd[i]);
	}
	free(Hd);
	free(gene);
   
	for (i = 0; i < maxbcn; i++)
  	{
           free(bicluster[i].sample);
           free(bicluster[i].data);
           
        }
        free(bicluster);
   	return SUCCESS;
}

void printUsage()
{
printf("\n\t\tUsage: affyAddGene\n"
         "\t\t         -h [display this help message]\n"
         "\t\t         -t threshold theta in a range 0.0 - 1.0\n"
         "\t\t         -m maximum expected biclusters in a range 1 - number_of_rows_in_input_data_matrix\n"
         "\t\t         -i <input microarray expression file (processed data)>\n"
         "\t\t         -p Output file format : 0 for standard format and 1 for BiBench bicluster format\n"
         "\t\t         -o <Output file>\n"
        );
}

