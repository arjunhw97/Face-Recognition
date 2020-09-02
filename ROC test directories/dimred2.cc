#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<cstdio>
#include <algorithm> 
#include <bits/stdc++.h> 
#include <Eigen/Dense>
#include<string>

using namespace std;
using namespace Eigen;

int main()
{
	int in = 1119, in2 = 1196; //Initializing size for train and test images
	//Initializing other vairables required
	double perc = 80, lst[in], corr = 0; 
	ofstream fp1,fp2;
    int vec;
    string foln = "fa2_H", foln2 = "fb_H", fla[in], flb[in2], snames[in];
    ifstream fp(foln+".txt"), ifp, ifp2;
    int i, j, k, N, M, Q, fl, p;
    double  v1, v2, v3;
    unsigned char *charImage;
    char header[100], *ptr, *ptr2;
    string fname1, fname2, lf;
    //Dimensions of the image
    M = 48;
    N = 60;
    Q = 255;
    //Declaring vectors to store and add up images

    VectorXd egval(in);

    //Retrieving eigenvalues from file	
    ifp.open(foln+"_eig_val.txt");
    k = 0;
    while(getline(ifp, fname1))
    {
    	egval(k) = stod(fname1);
    	++k;
    }
    
    ifp.close();
    v1 = 0;

    //Computing the threshold for preserving information
    v2 = egval.sum();
    for(i=0;i<in;++i)
    {	
    	v1 = v1 + egval(i);
        v3 = v1/v2;

        if(v3 >= 0.95)
        {
        	break;
        }	
    }	 
  
    vec = i + 1;
    cout<<vec<<endl;			

    //Declaring vectors and matrices according to how much information you want to preserve
    MatrixXd A((N*M), in), B((N*M), in2), emat((N*M), vec), ematres((N*M), vec), temp1(vec,2), temp2(vec,2), temp3(vec,vec), temp4(vec,vec), difin(vec,1), res(vec,1), dif(vec,1), covar(2,2);
    VectorXd av(N*M), x(N*M), omega(vec), omegai(vec), eign(N*M), val(1);
    
    temp4 << MatrixXd::Zero(vec,vec);
    
    
    //Computing inverse of diagonal matrix containing eigenvalues
    for(k=0;k<vec;++k)
    {
    	temp4(k,k) = (1/egval(k));
    }	

    ifp.close();

    //Retrieving the average face values
    ifp.open(foln+"_avg.pgm", ios::in | ios::binary);
    if (!ifp) {
        cout << "Can't read1 image: " << endl;
        exit(1);
    }
    ifp.getline(header,100,'\n');
    charImage = (unsigned char *) new unsigned char [N*M];
    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    for(i=0; i < N; ++i)
    { 
        for(j=0; j < M; j+=1) {
           v1 = (double)charImage[i*M+j];
           av(i*M+j) = v1;
        }  
    }
    
    ifp.close();

    fl = 0;
    
    //Retrievinng the training data
	while (getline(fp, fname1)) 
	{
		fla[fl] = fname1;
    	ifp.open("./"+foln+"/"+fname1, ios::in | ios::binary);
        if (!ifp) {
	      cout << "Can't read2 image: " << endl;
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

    fp.close();

    fp.open(foln2+".txt");
    
    
    //Retrievinng the testing data
    fl = 0;
	while (getline(fp, fname1)) 
	{
		flb[fl] = fname1;
    	ifp.open("./"+foln2+"/"+fname1, ios::in | ios::binary);
        if (!ifp) {
	      cout << "Can't read3 image: " << endl;
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
		       B((i*M+j),fl) = v1 - av(i*M+j);
		    }  
		}
        ++fl;
		ifp.close();
    }
   
    fp.close();

    //Extracting the eigenvectors from their respective files
    for(i=0;i<vec;++i)
    {
        v2 = 0;
        ifp.open("./"+foln+"_et/"+to_string(i)+"et.txt"); 
        
        if (!ifp) {
          cout << "Can't read image: " << endl;
          exit(1);
        }

        
        k = 0;
        while (getline(ifp, fname1)) {
            eign(k) = stod(fname1);
            ++k;        
        }
        
        ifp.close();
        
        //Normalizing the eigenvectors
        for(j=0;j<(N*M);++j)
        {
          v1 = eign(j);  
          v2 = v2 + (v1 * v1);
        }    
        v2 = sqrt(v2);
        eign = eign/v2;
        	 
        for(j=0;j<(N*M);++j)
        {
            emat(j,i) = eign(j);
        } 

        eign << VectorXd::Zero(N*M);
    } 
  
    ematres = emat;

    emat.transposeInPlace();
    
    ofstream cmi, cmp;
    int pos;
   
    //Projecting the test image data to the eigenspace of lower dimensionality
	corr = 0;
	for(i=0;i<in2;++i)
	{
	  omega << VectorXd::Zero(vec);	
      fname2 = flb[i];
      pos = fname2.find('.');
      
      fname2.erase(pos);
	  //cout<<fname2<<endl;
	  

	  fp1.open("./"+foln2+"_omega/"+fname2+"omega.txt");
	  if (!fp1) {
          cout << "Can't read image: " << endl;
          exit(1);
      }

	  for(j=0;j<(N*M);++j)
	  {
		x(j) = B(j,i);
		
	  }


      omega.noalias() += emat * x;
      for(j=0;j<vec;++j)
      {
      	fp1 << omega(j);
      	fp1 <<endl;
      }	
      fp1.close();
    }

    //Computing the optimal features of lower dimensional spaces for training
	for(i=0;i<in;++i)
	{
	  omegai << VectorXd::Zero(vec);	
      fname2 = fla[i];
      pos = fname2.find('.');
      fname2.erase(pos);
	  
	  fp1.open("./"+foln+"_omega/"+fname2+"omega.txt");
	  for(j=0;j<(N*M);++j)
	  {
		x(j) = A(j,i);
		
	  }

      omegai.noalias() += emat * x;
      for(j=0;j<vec;++j)
      {
      	fp1 << omegai(j);
      	fp1 <<endl;
      }	

      fp1.close();
    }

	return 0;
	
}

