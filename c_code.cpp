#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "c_code.h"

using namespace std;

Bin4DNN::Bin4DNN(std::vector< std::vector<bool> > X, std::vector< std::vector<bool> > Y){
	mat1 = conv2intRep(X);
	printf(".........\n");
	mat2 = conv2intRep(transpose(Y));
	pad_size = 32 - (((X[0].size()-1)%32)+1);
}

std::vector< std::vector<bool> > Bin4DNN::transpose(std::vector< std::vector<bool> >X){
	vector< std::vector<bool> > res(X[0].size(), vector<bool>(X.size(),0));
	for (int i=0; i<X.size(); i++){
		for (int j=0; j<X[0].size(); j++){
			res[j][i] = X[i][j];
		}
	}
	return res;
}

std::vector< std::vector<unsigned int> > Bin4DNN::conv2intRep(std::vector< std::vector<bool> >X){
	
	// initialize a new matrix of appropriate size
	int nrow = int(X.size());
	int ncol = int(X[0].size());

	int nrow2 = nrow;
	int ncol2 = int(ceil(ncol*1.0/32.0));
	printf("%d %d\n", nrow2, ncol2);

	std::vector< std::vector<unsigned int> > res(nrow2, std::vector<unsigned int>(ncol2,0));

	// plant the bits
	int b=31, idx=0;
	unsigned int n=0;
	for (int r=0; r<nrow; r++){
		idx = 0;
		for (int c=0; c<ncol; c++){
			// set appropriate bit in n
			
			// printf("start: n-%u b-%d X[r][c]-%d\n", n, b, int(X[r][c]));
			// int k = (X[r][c]) ? 1:0;
			// n |= (k<<b);
			
			n = (X[r][c]) ? n|(1<<b):n;
			b--;
			// printf("end: %u\n", n);

			// check if all bits in current int have been set
			if (b<0 || c==ncol-1){
				// reset bit position
				b = 31;
				// store the int at index & increment it
				res[r][idx] = n;
				idx++;
				// reset n
				n = 0;
			}
		}
	}
	for (int r=0; r<nrow2; r++){
		for (int c=0; c<ncol2; c++){
			cout<<res[r][c]<<"\t";}
			cout<<endl;
		}
	return res;

}

std::vector< std::vector<bool> > Bin4DNN::binMult(){
	std::vector< std::vector<bool> > res(mat1.size(), std::vector<bool>(mat2.size(),0));
	for (int i=0; i<mat1.size(); i++){
		for (int j=0; j<mat2.size(); j++){
			int sum = 0;
			for (int k=0; k<mat1[0].size(); k++){
				unsigned int temp = ~(mat1[i][k]^mat2[j][k]);
				printf("### temp:%u ", temp);
				sum += (2*__builtin_popcount(temp) - 32);
				printf("%d %d\n", sum, pad_size);
			}
			sum += pad_size;
			res[i][j] = (sum > 0) ? 1:0;
		}
	}
	return res;
}

void aJCanCode(){
	int r1 = 4, c1 = 62, r2 = 62, c2 = 5;
	std::vector< std::vector<bool> >  matrix1(r1, std::vector<bool>(c1,0));
	printf("!!!!!!! Matrix1 !!!!!!!\n");
	for (int i = 0; i < r1; i++)
	{
		for (int j = 0; j < c1; j++)
		{
			matrix1[i][j]=rand()%2;
			string s = (((j+1)%32)==0) ? ".":"|";
			cout << matrix1[i][j] << s;
		}
		cout<<endl;
	}
	
	std::vector< std::vector<bool> >  matrix2(r2, std::vector<bool>(c2,0));
	printf("!!!!!!! Matrix2 !!!!!!!\n");
	for (int i = 0; i < c2; i++)
	{
		for (int j = 0; j < r2; j++)
		{
			matrix2[j][i]=rand()%2;
			string s = (((j+1)%32)==0) ? ".":"|";
			cout << matrix2[j][i] << s;
		}
		cout<<endl;
	}

	printf("!!!!!!! Intermediate Computation !!!!!!!\n");
	Bin4DNN* multiplier = new Bin4DNN(matrix1,matrix2);
	std::vector< std::vector<bool> > output = multiplier->binMult();

	printf("!!!!!!! Result !!!!!!!\n");
	for (int i = 0; i < output.size(); i++)
	{
		for (int j = 0; j < output[i].size(); j++)
		{
			cout << output[i][j] << "\t";
		}
		cout<<endl;
	}
	delete multiplier;

}

int main(){
	aJCanCode();
}
