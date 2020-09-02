# Face Recognition

## Installation

- Install Python3 and C++ in your system.

- Download and extract the 'Eigen' library for C++ from [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download)

- Copy the directory named 'Eigen' within the extracted directory and paste it in your C++ installation directory along with the header files. In linux this will be in /usr/include.


## Data

images from the FERET face database. FERET contains a large number of images acquired during different photo sessions and has a good variety of gender, ethnicity and age groups. The lighting conditions, face orientation and time of capture vary.

fa_H - **High** resolution **training** images (Frontal images)<br>
fb_H - **High** resolution **testing** images (Alternate frontal images)<br>

fa_L - **Low** resolution **training** images (Frontal images)
fb_L - **Low** resolution **testing** images (Alternate frontal images)

All faces have been normalized with regards to orientation, position, and size. Also, they have been masked to include only the face region (i.e., upper body and background were cropped out). The first subset (fa) contains 1203 images from 867 subjects while the second subset (fb)contains 1196 images from the 866 subjects (i.e., there is one subject in fa who is not in fb). You have been provided with two different sizes.

for each image: low resolution (16 x 20) and high resolution (48 x 60).


The file naming convention for the FERET database is as follows:

->**nnnnn_yymmdd_xx_q.pgm**<-

where **nnnnn** is a five digit integer that uniquely identifies the subject, **yymmdd** indicates the year, month, and date when the photo was taken, **xx** is a lowercase character string (i.e., either fa or fb), and q is a flag (e.g., indicating whether the subject wears glasses - not always present).


## Execution

Clone the github project and navigate to the directory. Inside the cloned "Face Recognition" directory, **delete all the files within all the sub directories except for the fa_H, fb_H, fa_L, fb_L and "ROC test directories" directories.** 

Do not change the name of the folders. Once you are familiar with the code, then you may change the folder names since you can alter the names within the .cpp and .py files as well.


The files should be executed in this order - 

**For higher resolution (fa_H and fb_H)**

- cpfile.py: Creates a '.txt' file (fa_H or fb_H) and copies all the filenames from the fa_H/ or fb_H/ to this file

- test.cc: Creates a file avg.pgm which containcs the average face calculated by adding all the image pixels from /fa_H and dividing it by the total no: of images

- covar.cc: Creates a file fa_H_covar.txt, calculates the covariance matrix from the average face and all the image faces and writes it to the file

- eigcomp.py: Computes the eigenvectors and the corresponding eigenvalues of the images, stores the eigenvalues to a new file fa_H_eig_val.txt and writes each eigenvector to the directory /fa_H_part_eig in files 0p_eig.txt, 1p_eig.txt, 2p_eig.txt...etc.

- eigvec.cc: Retreives and calculates the eigenvectors (See theory), normalizes and maps the eigenvectors to the range [0,255] and then writes each of the eigenvectors to the directory /fa_H_et in files 0et.txt, 1et.txt, 2et.txt...etc. The eigenvectors are also projected as eigenfaces, written to the directory /fa_H_eigfaces in files 0ef.pgm, 1ef.pgm, 2ef.pgm....etc.

**Execute cpfile.py with fb_H instead of fa_H**

- dimred.cc: Maps the eigenvectors of the training images to a lower dimensional eigenspace and writes these 'omega' vectors to the directory /fa_H_omega in files (filename)omega.txt (filename corresponds to the name of the **training images**). Retreives the testing images from fb_H, projects it into the eigenspace (lower dimensionality) and writes the 'omega' vectors to the directory /fb_H_omega in files (filename)omega.txt (filename corresponds to the name of the **testing** images).

**Please Note: You may change the percentage of information retained by the eigenspace by changing the value of the variable 'perc' (currently = 80) in the above file.**

- recog.cc: Calculates the Mahalanobis distance between training and testing omega vectors and if the difference is less than a certain threshold (see Theory) then the two images are classified as the same. The rank and the performance is recorded in the files fa_H_index.txt and fa_H_perf.txt respectively. 

**Please Note: The variable 'vec' in the above file should be set as the length of one of the omega vectors (Changes with the percentage of information set in dimred2.cc).**

- plot.py: Plots the Comparitive CMC Graph based on the rank and the performance stored


**You may now repeat the same steps mentioned above, for lower resolution images by changing, in each file, the string filenames from 'fa_H' to 'fa_L' as well as 'fb_H' to 'fb_L' respectively and setting the resolution variables - M = 16 and N = 20.**


## Additional Tests

In order to compute and plot ROC curves, navigate to the directory 'ROC test directories' and delete files in all the sub directories except for fa2_H, fb2_H, fa2_L, fb2_L directories and copy the sub directories and files to the outer directory. 

**fa2_H, fa2_L - contains the fa_H, fa_L images of only the first 140 test subjects**  

Run the files

**For higher resolution (fa2_H and fb2_H)**

- filextract.py: creates a new file fa2_H_rem.txt and stores all the filenames in /fa2_H

- Now run the files from test.cc to eigvec.cc in the same order mentioned previously. Make sure to change the number of images from '1204' to '1119'.

- dimred2.cc: Same as dimred.cc 

- recog2.cc: Almost as same as recog.cc except that it calculates True Positive, False Positice, True Negative and False Negative
	- True Positive: recognized or classified images in fb_H **present** in fa2_H
	- False Positive: unrecognized images in fb_H **present** in fa2_H 
	- True Negative: unrecognized images in fb_H **not present** in fa2_H
	- False Negative: unrecognized images in fb_H **present** in fa2_H


**You may now repeat the same steps mentioned above, for lower resolution images by changing, in each file, the string filenames from 'fa2_H' to 'fa2_L' as well as 'fb_H' to 'fb_L' respectively and setting the resolution variables - M = 16 and N = 20.**


## Theory


**Feature Extraction** is used in order to reduce dimensionality. Now there are two methods for linear feature extraction -

- **Principal Component Analysis (PCA)** - This method seeks a projection that preserves as much information as possible in the data.

- **Linear Discriminant Analysis (LDA)** - This <meter></meter>thod seeks a projection that best discriminates the class.

Feature Extraction was implemented using the first method - PCA. In this method we represent each image or dataset as a **linear combination** of **orthonormal** set of basis vectors -

<img src='pat1.PNG' width=500px><br><br>

PCA basically maps or represents x in a new **lower dimensional** space -

<img src='pat2.PNG' width=500px><br><br>

The optimal lower dimensional basis vectors u 1, u 2, ...... u k are basically the **eigenvectors** of the covariance
matrix of the data.

<img src='pat3.PNG' width=500px><br><br>

The **K ‘Largest’ eigenvectors** corresponding to the **K ‘Largest’ eigenvalues** are chosen as the optimal basis vectors.

Dimensionality is reduced for the training Images through the following steps - 

Training images - x1 , x2 , x3 ,......xM vectors of size N

Step 1: Compute the **sample mean**

<img src='pat4.PNG' width=500px><br><br>

Step2: Subtract mean from the training data

<img src='pat5.PNG' width=500px><br><br>

Step 3: Compute the sample **Covariance matrix**

<img src='pat6.PNG' width=500px><br><br>

Step 4: Compute the eigenvalues and the eigenvectors of the covariance matrix using eq(3)

<img src='pat7.PNG' width=500px><br><br>

Step 5: Approximate x using the first largest K eigenvectors K<<N.

<img src='pat8.PNG' width=500px><br><br>


### Eigenface Implementation

We were given M images of size N1 × N2. Each image is represented as a vector by stacking up all the rows, one below the other. Forming an (N1 * N2) × 1 vector. 

Steps - 1 and 2 are followed as explained in detail above. 

For Step 3 - the covariance matrix Σ X calculated using eq(6) is very large as it forms an (N1*N2) × (N1*N2) matrix. In order to make this easier, instead of taking A x Transpose(A) we took Transpose(A) x A. Giving us an M × Mmatrix.

Step 4 - Now let us assume the eigenvalues and eigenvectors for the newly formed M × M matrix are - μ i , v i . These are obtained **substituting (Transpose(A) x A)** in eq(3).

Step 4b - After Multiplying both sides with by A and simplifying we get the eigenvectors and eigenvalues as follows -

<img src='pat9.PNG' width=500px><br><br>

M eigenvalues and eigenvectors obtained are also the M largest eigenvalues and eigenvectors of A x Transpose(A). Hence by using eq(11) we get the N eigenvectors.

The eigenfaces were produced by taking each eigenvector and normalizing and mapping their values to the range [0,255] and writing them to their corresponding newly created .pgm files.


### Experiments

The average face was computed by adding up each image in it’s vector form and dividing it by the total no: of images as mentioned earlier. The values were then written to a .pgm file to produce the face.

The eigenfaces were produced by taking each eigenvector and normalizing and mapping each of the values to the range [0,255]. The values were then written to a .pgm image file.

The eigenvectors were sorted according to their respective eigenvalues sorted in descending order, so that the eigenfaces produced at the beginning will be the largest and of higher variance and the ones towards the end will be the smallest and of lower variance

Used eq(9) and computed Ω = y1 , y2 , y3 ,.....yK  using eq(10).
For the testing set the following steps were implemented -
For each unknown test image Γ ,

Step 1 - The mean face Ψ from the training was subtracted

<img src='pat10.PNG' width=500px><br><br>

Step 2 - The unknown face was projected in the eigenspace -

<img src='pat11.PNG' width=500px><br><br>

Step 3 - Image from the training set that matches the given image is selected by finding the **Mahalanobis Distance** between the two vectors Ω and Ω i . For each pair (Ω, Ω i ), where i = 1,2,3....M.

<img src='pat12.PNG' width=500px><br><br>

<img src='pat13.PNG' width=500px><br><br>

This distance is called **distance in face space (difs).**

The threshold or the no: of eigenvalues to preserve 80% of the information was computed as follows -

<img src='pat14.PNG' width=500px><br><br>

After calculating k, we ran the test from N = 1 to 50 inorder to get the values to plot the CMC curves.


The images corresponding to 50 subjects were removed, their file names written to a different file. The True Positive, False Positive, True Negative and False Negative values were computed for 50 thresholds between the smallest and the largest recorded distance between the image pairs.


## Visualized Results

### High Resolution

<img src='pat15.PNG' width=500px><br><br>

<img src='pat16.PNG' width=500px><br><br>

Three correctly classified images for 80% information: 

<img src='pat17.PNG' width=500px><br><br>

<img src='pat18.PNG' width=500px><br><br>


### Low Resolution

<img src='pat19.PNG' width=500px><br><br>

<img src='pat20.PNG' width=500px><br><br>

Three correctly classified images for 80% information:

<img src='pat21.PNG' width=500px><br><br>

<img src='pat22.PNG' width=500px><br><br>



















































