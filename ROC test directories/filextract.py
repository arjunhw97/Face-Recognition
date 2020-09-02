import numpy as numpy

foln = "fa_H";
foln2 = "fa2_H"

file = open(foln+".txt")
file2 = open(foln2+"_rem.txt", 'w')

lst = []
for x in file:
	lst.append(x)

lst.sort()

k = 0;
remlist = []
for x in lst:
	if(k==0):
		remlist.append(x)
	if('00140' in x):
		k = 1;

for x in remlist:
	file2.write(x)

