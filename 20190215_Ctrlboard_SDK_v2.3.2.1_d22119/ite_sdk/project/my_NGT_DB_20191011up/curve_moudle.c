#include "project.h"

/*****Bézier curve********/
#define CTRLNUMBER  30

void calculate(float poss[][2], int precision, float result[][2])
{

	//维度，坐标轴数（二维坐标，三维坐标...）
	int dimersion = 2;

	//贝塞尔曲线控制点数（阶数）
	int number = CTRLNUMBER;

	//控制点数不小于 2 ，至少为二维坐标系
	if (number < 2 || dimersion < 2)
		return;

	//float[][] result = new float[precision][dimersion];

	//计算杨辉三角
	int mi[CTRLNUMBER];
	int lt[CTRLNUMBER - 1];
	//int lt = 0;
	mi[0] = mi[1] = 1;
	for (int i = 3; i <= number; i++) {

		//int[] t = new int[i - 1];
		//lt = i - 1;
		for (int j = 0; j < i - 1; j++) {
			lt[j] = mi[j];
		}

		mi[0] = mi[i - 1] = 1;
		for (int j = 0; j < i - 2; j++) {
			mi[j + 1] = lt[j] + lt[j + 1];
		}
	}

	//计算坐标点
	for (int i = 0; i < precision; i++) {
		float t = (float)i / ((float)precision);
		for (int j = 0; j < dimersion; j++) {
			float temp = 0.0f;
			for (int k = 0; k < number; k++) {
				temp += (float)pow(1 - t, number - k - 1) * poss[k][j] * (float)pow(t, k) * mi[k];
			}
			result[i][j] = temp;
		}
	}

	//return result;
}
