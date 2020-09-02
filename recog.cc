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
	int in = 1204/*1119*/, in2 = 1196; //Initializing size for train and test images
    //Initializing other vairables required
	double lst[in], corr = 0;
    int vec = 40;
    string foln = "fa_H", foln2 = "fb_H", fla[in], flb[in2], snames[in];
    ifstream fp(foln+".txt"), ifp, ifp2;
    int i, j, k, N, perf, M, fl, Q, p, pvar=0;
    double  v1, v2, v3;
    string fname1, fname2, fname3, fname4, fname5, fname6;
   //Dimensions of the image
    M = 48;
    N = 60;
    Q = 255;
    //Declaring vectors to store and add up images

    MatrixXd difin(vec,1), res(vec,1), dif(vec,1);
    VectorXd egval(in), omega(vec), omegai(vec), val(1);

    //Retrieving eigenvalues
    ifp.open(foln+"_eig_val.txt");
    k = 0;
    while(getline(ifp, fname1))
    {
    	egval(k) = stod(fname1);
    	++k;
    }

    ifp.close();

    //Retrieving filenames from training
    k = 0; 	
    while(getline(fp, fname1))
    {
    	fla[k] = fname1;
    	++k;
    }	

    fp.close();

    
    fp.open(foln2+".txt");

    //Retrieving filenames from testing
    k = 0;
    while(getline(fp,fname1))
    {
    	flb[k] = fname1;
    	++k;
    }	

    fp.close();

    ofstream cmi, cmp;
    cmi.open(foln+"_index.txt"); To store the values for plotting
    cmp.open(foln+"_perf.txt");  To store the values for plotting
    int pos, corr2;
    
    //Calculating the Mahalanobis distance between training and testing omega values for 
    for(perf=1;perf<=1;++perf)
    {
    	corr = corr2 = 0;
    	for(i=0;i<in2;++i)
    	{
    		cout<<i<<endl;
    		fname1 = flb[i];
			pos = fname1.find('.');
			fname1.erase(pos);
    		fname5 = fname1;

    		ifp.open("./"+foln2+"_omega/"+fname1+"omega.txt");
    		
    		pos = fname1.find('_');
			fname1.erase(pos);

    		for(j=0;j<vec;++j)
    		{

    			getline(ifp, fname2);
    			omega(j) = stod(fname2);
    		}	

    		for(j=0;j<in;++j)
    		{
    			fname3 = fla[j];
				pos = fname3.find('.');
				fname3.erase(pos);
	    		fname6 = fname3;

		        val << VectorXd::Zero(1);
		        difin.resize(vec,1);

	    		ifp2.open("./"+foln+"_omega/"+fname3+"omega.txt");
	    		
	    		pos = fname3.find('_');
				fname3.erase(pos);

	    		for(k=0;k<vec;++k)
	    		{
	    			getline(ifp2, fname4);
	    			omegai(k) = stod(fname4);
	    		}

	    		difin = omega - omegai;
		        dif = omega - omegai;
		        difin.transposeInPlace();
		        for(k=0;k<vec;++k)
		        {
		        	difin(0,k) = difin(0,k)/egval(k);
		        }

		        val.noalias() += difin * dif;
				v1 = val(0);
				v1 = sqrt(v1);
				lst[j] = v1;
				snames[j] = fname6;

                //cout<<fname6<<endl;
				ifp2.close();

    		}	



			for (k = 0; k < in-1; k++) 
            {   
		      for (j = 0; j < in-k-1; j++) 
		      { 
		         if (lst[j] > lst[j+1]) 
		         { 
		         	v1 = lst[j];
		         	lst[j] = lst[j+1];
		         	lst[j+1] = v1;

		         	fname3 = snames[j];
		         	snames[j] = snames[j+1];
		            snames[j+1] = fname3; 
		         } 
		      }

		    }


			for(j=0;j<perf;++j)
			{
				fname3 = fname6 = snames[j];
    
                pos = fname3.find('_');
                fname3.erase(pos);
				if(fname1==fname3)
				{
                    //For a.2 - 5
                    cout<<"Correctly:"<<endl;
                    cout<<fname5<<endl<<fname6<<endl<<endl;
			        ++pvar;
                    ++corr;
					break;
				}	
			}

            //cout<<fname1<<"\n"<<fname3<<endl<<endl;

            /*if(corr == corr2)
            {
                //For a.2 - 5
                 ++pvar;
                 cout<<"incorrectly:"<<endl;
                 cout<<fname5<<endl<<fname6<<endl<<endl;
            }    
            else
            {
                corr2 = corr;
            }*/

			ifp.close();

            /*if(pvar==3)
                exit(1);*/

    	}

        //cout<<corr<<endl;

	    corr = corr/in2;
	    //Writes values to files
		cmi << perf;
		cmi <<endl;

		cmp << corr;
		cmp << endl;

    }

    cmp.close();
    cmi.close();
    

   return 0; 	
}    
















