#include "StdAfx.h"
#include <stdio.h>
#include <memory.h>
#include <math.h>

 

#define DIM  (3)

 

/* determinant를 구하는 함수 */

double determinant(double *A, int dim);

/* 전치 행렬을 구하는 함수 */
void transpose_matrix(double *A);

/* 역행렬을 구하는 함수 */
void inverse_matrix(double *A, double det);

void mul_matrix(double* m1, double* m2, double* m3, int row);

 
#if 0
void main (void)
{
  int i, j, tmp;
  double p[DIM*DIM] = {0};
  double *q, *r, D, temp;

 

  /* 행렬의 값을 입력받는다 */
  for(i = 0; i < DIM; i++)
  {
    for(j = 0; j < DIM; j++)
    {
      printf("A[%d][%d] : ", i+1, j+1);
      scanf("%f", &temp);
      p[DIM*i+j] = temp;
    }
    printf("\n");
  }

 

  /* determinant를 구한다 */
  D = determinant(p, DIM);

  if(D != 0)  /* 역행렬이 존재하는 경우 */
  {
    inverse_matrix(p, D);
    printf("inverse matrix A :\n");

  

    /* 결과를 화면 출력한다. */
    for(i = 0; i < DIM; i++)
    {
      for(j = 0; j < DIM; j++)
      {
        temp = p[i*DIM+j];
        tmp = (int)temp;
        if(!tmp)
        {
          temp -= (double)tmp;
          temp *= 100.0;
          tmp = temp;


          if(!tmp)
          {
            printf("%8.2f ", 0.0);
          }
          else
          {
            printf("%8.2f ", p[i*DIM+j]);
          }
        }
        else
        {
          printf("%8.2f ", p[i*DIM+j]);
        }
      }
      printf("\n");
    }
  }
  else  /* 역행렬이 존재하지 않는 경우 */
  {
    printf("Determinant is %-8.2f.\n", D);
    printf("So, it is impossible to inverse this matrix...!\n");
  }
}
#endif
 

void mul_matrix(double* m1, double* m2, double* m3, int row)
{
 
    int r, c, k;
 
    for(r=0;r<row;r++){
  
        m3[r]=0;
 
        for(k=0;k<row;k++)
			m3[r]+=m1[k+(r*3)]*m2[k];
 
     }
}






/* determinant를 구하는 함수 */
double determinant(double *A, int dim)
{
  int col, row, i, j, k, imax, icol, irow, flag = 0;
  double result = 0, temp;

 

  if(dim == 2)
  {
    imax = 1;
  }
  else if(dim == 1)
  {
    return *A;
  }
  else if(dim > 2)
  {
    imax = dim;
  }
  else
  {
    return 0;
  }

 

  for(k = 1; k > -2; k-=2)
  {
    for(i = 0; i < imax; i++)
    {
      irow = k;
      icol = 1;
      col = 0;


      if(k < 0)
      {
        row = dim - i - 1;
      }
      else
      {
        row = i;
      }


      temp = 1;

      for(j = 0; j < dim; j++)
      {
        temp *= A[dim*col+row];
        if(k > 0 && row == dim-1)
        {
          /* row = (dim-2) - (row + col - (dim-1)); */
          row = 2 * dim - row - col - 3;
          flag = 1;
        }
        else if(k < 0 && row == 0)
        {
          /* row = (row+1) + (col+1) - 1; */
          row = row + col + 1;
          flag = 1;
        }
        else
        {
          col += icol;
          row += irow;
        }

 

        if(flag == 1)
        {
          col = dim - 1;
          icol = -1;
          irow = -1*k;
          flag = 0;
        }
      }
      result += (temp*k);
    }
  }
  return result;
}

 

/* 전치행렬을 구하는 함수 */
void transpose_matrix(double *A)
{
  int i, j;
  double temp;

 

  for(i = 0; i < DIM - 1; i++)
  {
    for(j = 1 + i; j < DIM; j++)
    {
      temp = A[DIM*i+j];
      A[DIM*i+j] = A[DIM*j+i];
      A[DIM*j+i] = temp;
    }
  }
}

 

/* 역행렬을 구하는 함수 */
void inverse_matrix(double *A, double det)
{
  int i, j, k, l, col, row;
  double tp[DIM*DIM] = {0};
  double rp[DIM*DIM] = {0};

 

  memcpy(rp, A, DIM*DIM*sizeof(double));

  for(i = 0; i < DIM; i++)
  {
    for(j = 0; j < DIM; j++)
    {
      col = row = 0;
      for(k = 0; k < DIM; k++)
      {
        if(k == i)
        {
          continue;
        }

 

        for(l = 0; l < DIM; l++)
        {
          if(j == l)
          {
            continue;
          }
          tp[col*(DIM-1)+row] = rp[k*DIM+l];
          row++;
        }
        col++;
        row = 0;
      }

 

      A[i*DIM+j] = (double)pow(-1.0, (double)(i+j+2))
                   * determinant(tp, DIM-1) * (1/det);
    }
  }

  transpose_matrix(A);
}
