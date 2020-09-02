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
	int in = 1119, in2 = 1196, in3 = 85;//Initializing size for train and test images and the deleted images
    //Initializing other vairables required
	double lst[in], corr = 0;
    int vec = 9;
    string foln = "fa2_H", foln2 = "fb_H", fla[in], flb[in2], flrem[in3], snames[in];
    ifstream ifp3(foln+".txt"), ifp, ifp2, rfp(foln+"_rem.txt");
    int i, j, k, N, perf, M, fl, Q, p;
    double  v1, v2, v3;
    string fname1, fname2, fname3, fname4, fname5;
     //Dimensions of the image
    M = 48;
    N = 60;
    Q = 255;
    

    k = 0;
     //Retrieving removed filenames
    while(getline(rfp,fname1))
    {
    	flrem[k] = fname1;
    	++k;
    }	

    MatrixXd difin(vec,1), res(vec,1), dif(vec,1);
    VectorXd egval(in), omega(vec), omegai(vec), val(1);

    ifp.open(foln+"_eig_val.txt");

    //Retrieving eigenvalues
    k = 0;
    while(getline(ifp, fname1))
    {
    	egval(k) = stod(fname1);
    	++k;
    }

    ifp.close();
    //Retrieving the training filenames
    k = 0; 	
    while(getline(ifp3, fname1))
    {
    	fla[k] = fname1;
    	++k;
    }	

    ifp3.close();

    
    ifp3.open(foln2+".txt");

    k = 0;
    //Retrieving the testing filenames
    while(getline(ifp3,fname1))
    {
    	flb[k] = fname1;
    	++k;
    }	

    ifp3.close();
    //File to calculate and store True positive rate and False Positive rate
    ofstream fpi, tpi;
    fpi.open(foln+"_fp.txt");
    tpi.open(foln+"_tp.txt");
    int pos, gi;
    //Variables to calculate TP, FP, TN, FN and to vary the threshold
    double tp, fp, tn, fn, tpr, fpr;
    double sgval = 1000, bgval = 0.00001, incr = 0.25656628, low = 0.178886;
    
    //Calculating the Mahalanobis distance between training and testing omega values 
    for(perf=1;perf<=50;++perf)
    {

 		tp = fp = tn = fn = 0;
    	for(i=0;i<in2;++i)
    	{
    		gi = 1;
    		cout<<i<<endl;
    		fname1 = flb[i];	

			pos = fname1.find('.');
			fname1.erase(pos);
    		
    		ifp.open("./"+foln2+"_omega/"+fname1+"omega.txt");
    		
    		pos = fname1.find('_');
			fname1.erase(pos);


    		for(k=0;k<in3;++k)
    		{
    			fname5 = flrem[k];
    			pos = fname5.find('_');
			    fname5.erase(pos);

    			if(fname1 == fname5)
    			{	
    				gi = 0;
    				break;
    			}	
    		}

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
				if(v1 < sgval)
					sgval = v1;
				if(v1 > bgval)
					bgval = v1;
				lst[j] = v1;
				snames[j] = fname3;
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

		    //Calculating TP, FP, TN and FN
		    if(lst[0] < low)
		    {
		    	if(gi==1){
		    		++ tp;
		    	}
		    	else
		    	{
		    		++ fp;
		    	}	
		    }	
		    else
		    {
		    	if(gi==1){
		    		++ fn;
		    	}
		    	else
		    	{
		    		++ tn;
		    	}
		    }

			ifp.close();

    	}

    	low = low + incr;
    	//Displaying the values for a given threshold
    	cout<<"True Positive: "<<tp<<endl;
    	cout<<"False Positive: "<<fp<<endl;
 		cout<<"True Negative: "<<tn<<endl;
    	cout<<"False Negative: "<<fn<<endl;    	
    	
    	//Calculating the True Positive rate and False Positive
    	tpr = tp/(tp + fn);
		fpr = fp/(fp + tn);

		//Writing to file
		fpi << fpr;
		fpi <<endl;

		tpi << tpr;
		tpi <<endl;

    }

    fpi.close();
    tpi.close();
    

   return 0; 	
}    
















