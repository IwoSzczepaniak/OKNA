#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


static double gtod_ref_time_sec = 0.0;

#define __first__(i, j) first[ j * SIZE + i ]
#define __second__(i, j) second[ j * SIZE + i ]
#define __multiply__(i, j) multiply[ j * SIZE + i ]
/* Adapted from the bl2_clock() routine in the BLIS library */

double dclock()
{
  double the_time, norm_sec;
  struct timeval tv;
  gettimeofday( &tv, NULL );
  if ( gtod_ref_time_sec == 0.0 )
    gtod_ref_time_sec = ( double ) tv.tv_sec;
  norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
  the_time = norm_sec + tv.tv_usec * 1.0e-6;
  return the_time;
}


//subroutine for optimization
int mm(double * first, double * second, double * multiply, int SIZE_)
{
  register unsigned int i,j,k; 
  register unsigned int SIZE = SIZE_;
  register double sum = 0;
  for (i = 0; i < SIZE; i++) { //rows in multiply
    for (j = 0; j < SIZE; j++) { //columns in multiply
      for (k = 0; k < SIZE; ) { //columns in first and rows in second
        if(k<SIZE - 4){
           sum = sum + __first__(i,k)*__second__(j,k);
           sum = sum + __first__(i,k+1)*__second__(j,k+1);
           sum = sum + __first__(i,k+2)*__second__(j,k+2);
           sum = sum + __first__(i,k+3)*__second__(j,k+3);
           k+=4;
        }else{
           sum = sum + __first__(i,k)*__second__(k,j);
           k++;
        }   
      } 
      __multiply__(i,j) = sum;
      sum = 0;
    }
  }
  return 0;
}



int main( int argc, const char* argv[] ){
  int i,j,iret;
  double ** first;
  double ** second;
  double ** multiply;

  double * first_;
  double * second_;
  double * multiply_;
  
  double dtime;

  int SIZE = 1500;

//allocate blocks of continous memory
  first_ = (double*) malloc(SIZE*SIZE*sizeof(double));
  second_ = (double*) malloc(SIZE*SIZE*sizeof(double));
  multiply_ = (double*) malloc(SIZE*SIZE*sizeof(double));

//allocate 2D matrices
  first = (double**) malloc(SIZE*sizeof(double*));
  second = (double**) malloc(SIZE*sizeof(double*));
  multiply = (double**) malloc(SIZE*sizeof(double*));

//set pointers to continous blocks
  for (i = 0; i < SIZE; i++) {
    first[i] = first_ + i*SIZE;
    second[i] = second_ + i*SIZE;
    multiply[i] = multiply_ + i*SIZE;
  }

//fill matrices with test data
  for (i = 0; i < SIZE; i++) { //rows in first
    for (j = 0; j < SIZE; j++) { //columns in first
      first[i][j]=i+j;
      second[i][j]=i-j;
    }
  }

//measure mm subroutine computation time
  dtime = dclock();
  iret = mm(first_,second_,multiply_,SIZE); 
  dtime = dclock()-dtime;
  printf( "Time: %le \n", dtime);


  fflush( stdout );

//cleanup
  free(first_);
  free(second_);
  free(multiply_);

  free(first);
  free(second);
  free(multiply);

  return iret;
}
 
