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
    int in = 1204;  //Initializing sizw
    string foln = "fa_H"; //High or low resolution files, currently for high

    //Declaring required variables
    int i, j, k, N, M, Q; 
    double  v1, v2;
    unsigned char *charImage, *charImage_write; //Variables to read and write from images
    char header[100], *ptr, *ptr2;
    string fname1;
    //Dimensions of the image
    M = 48;
	N = 60;
    Q = 255;
    //Declaring vectors to store and add up images
    VectorXd s(N*M); 
    VectorXd av(N*M);
    k = 0; 
    //Initializing a vector to zero 
    for(i=0; i < N; i++){ 
		for(j=0; j < M; j+=1) {
			av(k) = 0;
			++k;
		}	
	}	
    //Opening file with filenames
    ifstream fp(foln+".txt"), ifp;
    ofstream ofp;
    int fl = 0;

    //Reading each image as a vector and adding them up
    while (getline(fp, fname1)) {
    	ifp.open("./"+foln+"/"+fname1, ios::in | ios::binary);
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
		       s(i*M+j) = v1;	//Retrieving image values	       
		    }  
		}

        av = av + s; //Sum up all images
        
		ifp.close();
    }

    av = av/in; //Dividing by the total no: of images thereby computing average
    charImage_write = (unsigned char *) new unsigned char [N*M];
    //Writing values to a .pgm file as average face
    k = 0;
    for(i=0;i<N;++i)
    {	
    	for(j=0;j<M;++j)
    	{
    		charImage_write[i*M+j]=(unsigned char)av(i*M+j);
        }
    }

    ofp.open(foln+"_avg.pgm", ios::out | ios::binary);
    ofp << header << endl;
    ofp.write( reinterpret_cast<char *>(charImage_write), (M*N)*sizeof(unsigned char));	
    ofp.close();
    return 0;    
}	

