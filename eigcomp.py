import numpy as np
import numpy.linalg as linalg

#fa_H_part_eig
strg = "fa_H" 	#High or low resolution files, currently for high
imn = 1204     #Initializing size

#Extracting covariance matrix 
dirc = strg+"_part_eig"
f = open(strg+"_covar.txt", "r")
i = 0;
j = 0;
C = np.zeros((imn,imn))
for i in range(imn):
	for j in range(imn):
		x = float(f.readline())
		C[i,j] = x;

f.close()

#Computing eigenvalues and their corresponding eigenvectors
eigenValues, eigenVectors = linalg.eig(C)

#Sorting eigenvectors/eigenvalues in descending order
idx = np.argsort(eigenValues)[::-1]  
eigenValues = eigenValues[idx]
eigenVectors = eigenVectors[:,idx]

#Writing the eigenvalues to a file
f = open(strg+"_eig_val.txt", "w")
for i in range(imn):
	f.write(str(eigenValues[i]))
	f.write("\n")

f.close()

#Writing the eigenvectors to files
for i in range(imn):
	f = open("./"+dirc+"/"+str(i)+"p_eig.txt", "w")
	for j in range(imn):
		f.write(str(eigenVectors[j,i]))
		f.write("\n")
	f.close()	

