import glob, os
import matplotlib.pyplot as plt
import numpy

def createGraph(testName, first, second, v):
    x = []
    for num in numpy.logspace(1, 13, 13, True, 2):
        x.append(num)

    print(x)
    print(v)
    assert isinstance(v, list)
    assert isinstance(x, list)

    fig, ax = plt.subplots()
    ax.plot(x, v)
    ax.plot([2, 8192], [first, first])

    ax.plot([2, 8192], [second, second])
    ax.set_title(testName)
    ax.set_xscale('log')
    ax.set_xlabel("permutations")
    ax.set_ylabel("H0 rejected")

    fig.savefig(os.getcwd() +  testName[0:-4] + ".png")

def readFile(file):
    print(file)
    ns1 = 0;
    ns2 = 0;

    success = {}
    a = 2

    while a < 10000:
        success[a] = 0.0;
        a = a * 2

    with open(file) as f:
        content = f.readlines()
        for line in content:
            v = line.split()
            success[int(v[0])] = success[int(v[0])] + float(v[1])
            ns1 = ns1 + int(v[2])
            ns2 = ns2 + int(v[3])

    f1 = float(ns1) / len(content)
    f2 = float(ns2) / len(content)

    print (file)
    print (f1)
    print (f2)
    print (success[8192] / 1000)
    createGraph(file, f1, f2, list(success.values()))

os.chdir("./outputs/")
for file in glob.glob("*.txt"):
    readFile(file)

