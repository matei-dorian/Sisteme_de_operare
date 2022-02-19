#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>



int **A;
int **B;
int **C;
int n, m, p;

struct matrix_element {
	int x; int y;
};

int** init_matrix(int n, int m){ /// function to initialize a matrix of n rows and m columns

	int **M = (int**)malloc(n * sizeof(int*));
	for(int i = 0; i < n; i++)
		*(M + i) = (int*)malloc(m * sizeof(int));
	return M;
}

void free_matrix(int **M, int n){ /// function to free the memory from a matrix of n rows and m columns
	for(int i = 0; i < n; i++)
		free(*(M + i));
	free(M);
}

void print_matrix(int **M, int n, int m){ /// function that prints a matrix
	for(int i = 0; i < n; i++){
		for(int j = 0; j < m; j ++)
			printf("%d ", M[i][j]);
		printf("\n");
	}
}


void* make_product(void *arg){
	struct matrix_element *e = (struct matrix_element*)arg;
	int s = 0;

	//printf("%d %d\n",e->x, e->y);
	for(int i = 0; i < p; i ++)
		s += A[e->x][i] * B[i][e->y];

	C[e->x][e->y] = s;
	return NULL;
}


int main()
{
	/// ---SAMPLE TEST---
	m = 3;
	p = 1;
	n = 2;

	A = init_matrix(m, p);
	B = init_matrix(p, n);
	C = init_matrix(m, n);

	A[0][0] = 1;
	A[1][0] = 2;
	A[2][0] = 3; /// A = [[1], [2], [3]]

	B[0][0] = 1;
	B[0][1] = 2; /// B = [[1, 2]]

	/// print_matrix(A, m, p);
	/// print_matrix(B, p, n);
	/// ---SAMPLE TEST---

	pthread_t thr[n * m];

	struct matrix_element **e = (struct matrix_element**)malloc(m*n*sizeof(struct matrix_element*));

    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++){ /// initialize the matrix of indexes to send as arguments for the threads
                e[i*n+j] = (struct matrix_element*)malloc(sizeof(struct matrix_element));
                e[i*n+j]->x = i;
                e[i*n+j]->y = j;
        }

	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++){
			int k = i * n + j;
			if(pthread_create(&thr[k], NULL, make_product, e[k])){
				perror("Error at create");
				return errno;
			}

		}


	for(int i = 0; i < n * m; i++){
		if(pthread_join(thr[i], NULL)){
			perror("Error at join");
			return errno;
		}
	}

	print_matrix(C, m, n);

    free(e);
	free_matrix(A, m);
	free_matrix(B, p);
	free_matrix(C, m);

	return 0;
}
