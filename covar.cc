#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<cstdio>
#include <bits/stdc++.h> 
#include <Eigen/Dense>
#include<string>

using namespace std;
using namespace Eigen;

int main()
{
    int in = 1204;  //Initializing size
    string foln = "fa_H"; //High or low resolution files, currently for high

    //Declaring required variables
    int i, j, k, N, M, Q;
    double  v1, v2;
    unsigned char *charImage, *charImage_write;
    char header[100], *ptr, *ptr2;
    string fname1;
    //Dimensions of the image
    M = 48;
    N = 60;
    Q = 255;
    //Declaring vectors to store and add up images
    MatrixXd A((N*M), in), B((N*M), in), C(in, in), D((N*M),1);
    VectorXd av(N*M);
    //Intitializing Matrix as zero
    C << MatrixXd::Zero(in,in);
    ifstream fp(foln+".txt"), ifp;
    
    //Opening and retrieving values from average face
    ifp.open(foln+"_avg.pgm", ios::in | ios::binary);
    if (!ifp) {
        cout << "Can't read image: " << endl;
        exit(1);
    }
    ifp.getline(header,100,'\n');
    charImage = (unsigned char *) new unsigned char [N*M];
    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    for(i=0; i < N; ++i){ 
        for(j=0; j < M; j+=1) {
           v1 = (double)charImage[i*M+j];
           av(i*M+j) = v1; //Reading from average face as vector
        }  
    }
    
    //Reading all the image values into one matrix
    ifp.close();
    ofstream ofp;
    int fl = 0;
    while (getline(fp, fname1)) {
    	ifp.open("./"+foln+"/"+fname1, ios::in | ios::binary);
	    //cout<<fname1<<" "<<fl<<endl;
        if (!ifp) {
	      cout << "Can't read image: " << endl; 
	      exit(1);
	    }
	    ifp.getline(header,100,'\n');
		while(header[0]=='#')
	      ifp.getline(header,100,'\n');
         
		charImage = (unsigned char *) new unsigned char [N*M];
		ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
		for(i=0; i < N; ++i){ 
		    for(j=0; j < M; j+=1) {  
		       v1 = (double)charImage[i*M+j];
		       A((i*M+j),fl) = v1 - av(i*M+j); //Subtracting the mean from each image  
		    }  
		}
        ++fl;
		ifp.close();
    }

    fp.close();
    
    //Taking A transpose 
    B = A;
    cout<<"Taking Transpose..."<<endl;
    A.transposeInPlace();
    cout<<"Multiplying A.transpose * A"<<endl;
    //Calculating Covariance matrix A^T * A
    C.noalias() += A * B;
    C = C/in; //Dividing the covariance matrix by the total no: of images

    //Writing the covariance matrix to a file
    cout<<"Proceeding to write"<<endl;
    ofp.open(foln+"_covar.txt");
    for(i=0;i<in;++i)
        for(j=0;j<in;++j)
        {
            ofp << C(i,j);
            ofp <<endl;
        }    
    ofp.close();    
    return 0;    
}	

