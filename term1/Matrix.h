#pragma once
#include <math.h>

class MATRIX {
public:
	MATRIX Add(MATRIX m);
	MATRIX Subtract(MATRIX m);
	MATRIX Multiply(MATRIX m);
	MATRIX Transpose();

	float ele[4][4];
	int rownum;
	int colnum;
	
	MATRIX() {

	}
	MATRIX(int row, int col) {
		rownum = row;
		colnum = col;
		for (int i = 0; i < rownum; i++)
			for (int j = 0; j < colnum; j++)
				ele[i][j] = 0.0;
	}
};

MATRIX MATRIX::Add(MATRIX m) {
	MATRIX result;
	for (int i = 0; i < rownum; i++) {
		for (int j = 0; j < colnum; j++) {
			result.ele[i][j] = ele[i][j] + m.ele[i][j];
		}
	}
	return result;
}

MATRIX MATRIX::Subtract(MATRIX m) {
	MATRIX result;
	for (int i = 0; i < rownum; i++) {
		for (int j = 0; j < colnum; j++) {
			result.ele[i][j] = ele[i][j] - m.ele[i][j];
		}
	}
	return result;
}

MATRIX MATRIX::Multiply(MATRIX m) {
	int i, j, k;
	MATRIX result;
	for (i = 0; i < rownum; i++)
		for (j = 0; j < colnum; j++)
			result.ele[i][j] = 0.0;

	if (colnum == m.rownum) {
		result.rownum = rownum;
		result.colnum = m.colnum;
		for (i = 0; i < rownum; i++) 
			for (j = 0; j < m.colnum; j++) 
				for (k = 0; k < colnum; k++) 
					result.ele[i][j] += ele[i][k] * m.ele[k][j];
		return result;
	}
}

MATRIX MATRIX::Transpose() {
	MATRIX result;
	result.rownum = colnum;
	result.colnum = rownum;

	for (int i = 0; i < rownum; i++)
		for (int j = 0; j < colnum; j++)
			result.ele[j][i] = ele[i][j];

	return result;
}

