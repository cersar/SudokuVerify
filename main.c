#include <pthread.h>
#include <stdio.h>

typedef struct {
	int x;
	int y;
} Point;

typedef struct {
	Point left_top;
	Point right_buttom;
	int thread_index;
} Data;

int array[9][9]; /* this data is shared by the thread(s) */
int flag = 0;
Data data[27]; /* data passed to each thread */
void *valid(void *); /* threads call this function */
void initSudokuArray(); /* init sudu array */
void initData(); /* init data passed to each thread */

int main() {
	pthread_t tid[27]; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */


	initSudokuArray();
	/* init data passed to each thread */
	initData();

	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the thread */
	for (int i = 0; i < 27; ++i) {
		pthread_create(&tid[i], &attr, valid, &data[i]);
	}
	/* wait for the thread to exit */
	for (int i = 0; i < 27; ++i) {
		pthread_join(tid[i], NULL);
	}
	if (flag == 0) {
		printf("valid!\n");
	} else {
		printf("invalid!\n");
	}

}

/* init sudoku array */
void initSudokuArray(){
	FILE *fp;
	fp = fopen("input", "r");

	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			fscanf(fp, "%d", &array[i][j]);
			printf("%d\t", array[i][j]);
		}
		printf("\n");
	}
	fclose(fp);
}


/* init data passed to each thread */
void initData() {
	for (int i = 0; i < 9; ++i) {
		int index = i;
		data[index].left_top.x = i;
		data[index].left_top.y = 0;
		data[index].right_buttom.x = 8;
		data[index].right_buttom.y = 0;
		data[index].thread_index = index;
	}

	for (int i = 0; i < 9; ++i) {
		int index = i + 9;
		data[index].left_top.x = 0;
		data[index].left_top.y = i;
		data[index].right_buttom.x = 0;
		data[index].right_buttom.y = 8;
		data[index].thread_index = index;
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			int index = i * 3 + j + 18;
			data[index].left_top.x = i * 3;
			data[index].left_top.y = j * 3;
			data[index].right_buttom.x = i * 3 + 2;
			data[index].right_buttom.y = j * 3 + 2;
			data[index].thread_index = index;
		}
	}
}

/* Valid each sudoku array block. The thread will begin control in this function */
void *valid(void *param) {
	Data *data = (Data *) param;
	int count[9] = { 0 };
	for (int i = data->left_top.x; i <= data->right_buttom.x; ++i) {
		for (int j = data->left_top.y; j <= data->right_buttom.y; ++j) {
			if (++count[array[i][j] - 1] > 1) {
				flag = flag | (1 << data->thread_index);
				pthread_exit(0);

			}
		}
	}
	pthread_exit(0);
}
