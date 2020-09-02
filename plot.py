import math
import numpy as np
import random
import os
import sys
from scipy.spatial import distance
from matplotlib import pyplot as plt
import pandas as pd


def fextract(fname1, fname2):
	file1 = open(fname1,'r')
	file2 = open(fname2,'r')
	data1 = file1.readlines()
	data2 = file2.readlines()
	cnt = 50;
	tpr = np.zeros((1,cnt))
	fpr = np.zeros((1,cnt))
	cnt2 = 0
	for x in data1:
		if(cnt2 == 50):
			break
		tpr[0,cnt2] = float(x)
		cnt2 = cnt2 + 1

	cnt2 = 0
	for x in data2:
		if(cnt2 == 50):
			break
		fpr[0,cnt2] = float(x)
		cnt2 = cnt2 + 1

	return tpr, fpr		

def plotfig(x1,y1,x2=0,y2=0,x3=0,y3=0):
	plt.figure(figsize=(9,5))
	plt.title("Comparitive CMC Graph - High Resolution")
	plt.plot(x1[0,:], y1[0,:], label="Information", zorder=2)
	#plt.plot(x2[0,:], y2[0,:], label="90% Information", zorder=2)
	#plt.plot(x3[0,:], y3[0,:], label="95% Information", zorder=2)
	#plt.plot([0, 1], [0, 1],'r--', zorder=3)
	plt.legend(loc='best')
	plt.xlabel('Rank')
	plt.ylabel('Performance')
	plt.show()



tpr,fpr = fextract('fa_H_perf.txt','fa_H_index.txt')
#tpr2,fpr2 = fextract('fa_H_90_perf.txt','fa_H_90_index.txt')
#tpr3,fpr3 = fextract('fa_H_95_perf.txt','fa_H_95_index.txt')

plotfig(fpr,tpr)
#plotfig(fpr,tpr,fpr2,tpr2,fpr3,tpr3)
#plotfig(fpr4,tpr4)



