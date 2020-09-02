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
    int i, j, k, N, M, Q, sz;
    double  v1, v2 = 0, v3;
    unsigned char *charImage, *charImage_write;
    char header[100], *ptr, *ptr2;
    string fname1, tmp;
     string fname1;
    //Dimensions of the image
    M = 48;
    N = 60;
    Q = 255;
    //Declaring vectors to store and add up images

    MatrixXd A((N*M), in), B((N*M), in), C((N*M), in);
    VectorXd av(N*M), v(in), eign(N*M);

    //Declaring file pointers
    ifstream fp(foln+".txt"), ifp;
    ofstream ofp, ofp1;

    //Extracting the average as vectors
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
           av(i*M+j) = v1;
        }  
    }

    
    ifp.close();

    //Extracting the images into a single matrix
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
               A((i*M+j),fl) = v1 - av(i*M+j);
            }  
        }
        ++fl;
        ifp.close();
    }

    //Retrieving the eigenvectors 
    for(i=0;i<in;++i)
    {
        v2 = 0;
        ifp.open("./"+foln+"_part_eig/"+to_string(i)+"p_eig.txt", ios::in | ios::binary);
        
        if (!ifp) {
          cout << "Can't read image: " << endl;
          exit(1);
        }

        k = 0;
        while (getline(ifp, fname1)) {
            v(k) = stod(fname1);
            ++k;        
        }
        ifp.close();
        
        //Calculating each of the n eigenvectors
        eign.noalias() += A * v;
        
        //Nromalizing the eigenvector and mapping it to the range [0,255]
        v1 = eign.minCoeff();
        v2 = eign.maxCoeff();
        for(j=0;j<(N*M);++j)
        {
            C(j,i) = eign(j);
            v3 = (eign(j) - v1)/(v2-v1);
            v3 = v3 * 255;
            B(j,i) = v3;
        } 

        //Reinitializing the vector to zero
        eign << VectorXd::Zero(N*M);
    }

    fp.close();

    fp.open(foln+".txt");

    //Writing the normalized and mapped eigenvectors to .pgm files 
    for(k=0;k<in;++k)
    {    
        
      ofp1.open("./"+foln+"_et/"+to_string(k)+"et.txt");  
      if (!ofp1) {
          cout <<"Can't Write Image"<<endl;
          exit(1);
        }
      charImage_write = (unsigned char *) new unsigned char [N*M];  
      
      for(i=0;i<N;++i){
        for(j=0;j<M;++j){
          charImage_write[i*M+j]=(unsigned char)B(i*M+j,k);        
          //Storing the eigenvectors in it's original form in text files 
          ofp1<<C(i*M+j,k);
          ofp1<<endl;
        }
      }
      ofp1.close();  
      ofp.open("./"+foln+"_eigfaces/"+to_string(k)+"ef.pgm", ios::out | ios::binary);
      if (!ofp) {
          cout << "Can't read image: " << endl;
          exit(1);
        }
      ofp << header << endl;
      ofp.write( reinterpret_cast<char *>(charImage_write), (M*N)*sizeof(unsigned char)); 
      ofp.close();
    }
    return 0;
}    