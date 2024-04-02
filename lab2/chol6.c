
#include <assert.h>
#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <immintrin.h>

#define IDX(i, j, n) (((j)+ (i)*(n)))
#define BLKSIZE 16 // Change this to 16 for 16 iterations
#define max(a,b) (((a)>(b))?(a):(b))

static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */

double dclock(){
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday( &tv, NULL );
    if ( gtod_ref_time_sec == 0.0 )
        gtod_ref_time_sec = ( double ) tv.tv_sec;
    norm_sec = ( double ) tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;
    return the_time;
}



int chol(double * A, unsigned int n){
    register unsigned int i, j, k;
    register double tmp;
    register __m256d tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10, tmp11, tmp12, tmp13, tmp14, tmp15;

    for (j = 0; j < n; j++) {
        for (i = j; i < n; i++) {
            tmp = A[IDX(i, j, n)];
            for (k = 0; k < j; ) {
                if (k < max(j - BLKSIZE, 0)){
                    tmp0 = _mm256_loadu_pd(A+IDX( i , k , n ) ) ;
                    tmp1 = _mm256_loadu_pd(A+IDX( j , k , n ) ) ;
                    tmp0 = _mm256_mul_pd(tmp0, tmp1);
                    // tmp -= A[IDX(i, k, n)] * A[IDX(j, k, n)];
                    //tmp -= A[IDX(i, k+1, n)] * A[IDX(j, k+1, n)];
                    //tmp -= A[IDX(i, k+2, n)] * A[IDX(j, k+2, n)];
                    //tmp -= A[IDX(i, k+3, n)] * A[IDX(j, k+3, n)];

                    tmp4 = _mm256_loadu_pd(A+IDX( i , k+4 , n ) ) ;
                    tmp5 = _mm256_loadu_pd(A+IDX( j , k+4 , n ) ) ;
                    tmp4 = _mm256_mul_pd(tmp4, tmp5);
                    //tmp -= A[IDX(i, k+4, n)] * A[IDX(j, k+4, n)];
                    //tmp -= A[IDX(i, k+5, n)] * A[IDX(j, k+5, n)];

                    //tmp -= A[IDX(i, k+6, n)] * A[IDX(j, k+6, n)];
                    //tmp -= A[IDX(i, k+7, n)] * A[IDX(j, k+7, n)];

                    tmp8 = _mm256_loadu_pd(A+IDX( i , k+8 , n ) ) ;
                    tmp9 = _mm256_loadu_pd(A+IDX( j , k+8 , n ) ) ;
                    tmp8 = _mm256_mul_pd(tmp8, tmp9);
                    //tmp -= A[IDX(i, k+8, n)] * A[IDX(j, k+8, n)];
                    //tmp -= A[IDX(i, k+9, n)] * A[IDX(j, k+9, n)];

                    //tmp -= A[IDX(i, k+10, n)] * A[IDX(j, k+10, n)];
                    //tmp -= A[IDX(i, k+11, n)] * A[IDX(j, k+11, n)];

                    tmp12 = _mm256_loadu_pd(A+IDX( i , k+12 , n ) ) ;
                    tmp13 = _mm256_loadu_pd(A+IDX( j , k+12 , n ) ) ;
                    tmp12 = _mm256_mul_pd(tmp12, tmp13);
                    //tmp -= A[IDX(i, k+12, n)] * A[IDX(j, k+12, n)];
                    //tmp -= A[IDX(i, k+13, n)] * A[IDX(j, k+13, n)];

                    //tmp -= A[IDX(i, k+14, n)] * A[IDX(j, k+14, n)];
                    //tmp -= A[IDX(i, k+15, n)] * A[IDX(j, k+15, n)];

                    tmp0 = _mm256_add_pd(tmp0, tmp4);
                    tmp0 = _mm256_add_pd(tmp0, tmp8);
                    tmp0 = _mm256_add_pd(tmp0, tmp12);

                    tmp -= tmp0[0] + tmp0[1] + tmp0[2] + tmp0[3];
                    k += BLKSIZE;
                }
                else{
                    tmp -= A[IDX(i, k, n)] * A[IDX(j, k, n)];
                    k++;
                }
            }
            A[IDX(i, j, n)] = tmp;
        }
        

        if (A[IDX(j, j, n)] < 0.0) {
            return (1);
        }

        tmp = sqrt(A[IDX(j, j, n)]);
        for (i = j + 1; i < n; i++){
            A[IDX(i, j, n)] /= tmp; 
        }
    }

    return (0);
}

int main(int argc, char ** argv){
    double* A;
    double t1, t2;
    int i, j, n, ret;
    double dtime;

    n = atoi(argv[1]);
    A = malloc(n * n * sizeof(double));
    assert(A != NULL);

    for (i = 0; i < n; i++) {
        A[IDX(i, i, n)] = 1.0;
    }

    dtime = dclock();
    if (chol(A, n)) {
      fprintf(stderr,"Matrix is not symmetric or not positive definite\n");
    }
    dtime = dclock()-dtime;
    printf( "Time: %le \n", dtime);

    fflush( stdout );

    free(A);
    return 0;
}
