from os import walk

strg = "fb_H"

f = []
for (dirpath, dirnames, filenames) in walk('./'+strg):
    f.extend(filenames)
    break    
f1 = open(strg+".txt","w")
for x in f:
	f1.write(x)
	f1.write("\n")
	
