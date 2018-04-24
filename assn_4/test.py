fname="inputLarge.txt"
num=10000
f=open(fname,"w")

for i in range(num):
    line="add "+str(i)+"\n"
    f.write(line)
for i in range(num):
    line="find "+str(i)+"\n"
    f.write(line)
f.write("end\n")
f.close()

fname="outputLarge.txt"
output=open(fname,"w")
for i in range(num):
    line="Entry with key="+str(i)+" exists\n"
    output.write(line);

output.close()
