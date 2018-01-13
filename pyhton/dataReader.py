import scipy.stats
import numpy.random
import numpy
import matplotlib.pyplot as plt


def readFromFile(fileName):
    out = {}
    test = {}
    for a in numpy.logspace(0, 13, 14, True, 2):
        out[a] = 0.0
        test[a] = 0
    with open(fileName) as f:
        lines = f.readlines()
        for l in lines:
            t = l.split()
            iterations = t[0]
            permutations = t[1]
            result = t[2]

            out[int(permutations)] = out[int(permutations)] + float(result)
            test[int(permutations)] = test[int(permutations)] + 1

    for t in out.keys():
        out[t] = out[t] / 10000.0

    ls = []
    for a in numpy.logspace(0, 13, 14, True, 2):
        ls.append(1.0 - out[a])
        assert test[a] == 10000

    return ls


def evaluatePower(genFun1, genFun2, test):
    pos = 0.0
    for a in range(0, 10000):
        ls = []
        ls2 = []
        for a in range(0, 25):
            ls.append(genFun1())
        for a in range(0, 25):
            ls2.append(genFun2())

        pos = pos + test(ls, ls2)

    pos = 1.0 - (pos / 10000.0)
    return pos


def getpowers(n1, n2, g1, g2, g3, g4, c1, c2, test):
    d = {}
    d["uniform equal"] = evaluatePower(n1, n1, test)
    d["uniform 0.1"] = evaluatePower(n1, n2, test)
    d["normal equal"] = evaluatePower(g1, g1, test)
    d["normal 0.5"] = evaluatePower(g1, g2, test)
    d["mixture equal"] = evaluatePower(g3, g3, test)
    d["mixture disequal"] = evaluatePower(g3, g4, test)
    d["chi equal"] = evaluatePower(c1, c1, test)
    d["chi 1"] = evaluatePower(c1, c2, test)
    return d


def test(l1, l2):
    t, p = scipy.stats.ttest_ind(l1, l2, equal_var=False)
    #t, p = scipy.stats.mannwhitneyu(l1, l2, False)
    return int(p > 0.05);


def uniform1():
    return numpy.random.uniform(0, 1)


def uniform2():
    return numpy.random.uniform(0.1, 1.1)


def gaussian1():
    return numpy.random.normal(0, 1)


def gaussian2():
    return numpy.random.normal(0.5, 1)


def gaussian3():
    if numpy.random.uniform(0, 1) > 0.5:
        return numpy.random.normal(0, 1)
    return numpy.random.normal(0.5, 1)


def gaussian4():
    if numpy.random.uniform(0, 1) > 0.5:
        return numpy.random.normal(0.2, 1)
    return numpy.random.normal(0.7, 2)


def chi1():
    return numpy.random.chisquare(3.0)


def chi2():
    return numpy.random.chisquare(4.0)


def readFromAllFiles():
    d = {}
    d["uniform equal"] = readFromFile("uniform_equal.txt")
    d["uniform 0.1"] = readFromFile("uniform_0,1.txt")
    d["normal equal"] = readFromFile("normal_equal.txt")
    d["normal 0.5"] = readFromFile("normal_0,5.txt")
    d["mixture equal"] = readFromFile("mixed_equal.txt")
    d["mixture disequal"] = readFromFile("mixed_disequal.txt")
    d["chi 1"] = readFromFile("chi_squared_1.txt")
    d["chi equal"] = readFromFile("chi_squared_equal.txt")
    return d

def generateSingleGraph(v, v2, regFA, regPow, testName):

    x = []
    for num in numpy.logspace(0, 13, 14, True, 2):
        x.append(num)

    assert isinstance(v, list)
    assert isinstance(x, list)

    fig, ax = plt.subplots()
    ax.plot(x, v)
    ax.plot(x, v2)
    ax.plot([0, 8192], [regFA, regFA])

    ax.plot([0, 8192], [regPow, regPow])
    ax.set_title(testName)
    ax.set_xscale('log')
    ax.set_xlabel("permutations")
    ax.set_ylabel("H0 rejected")

    fig.savefig("pictures/"+testName+".png")


def generateGraphs():
    lines = getpowers(uniform1, uniform2, gaussian1, gaussian2, gaussian3, gaussian4, chi1, chi2, test)
    points = readFromAllFiles()
    generateSingleGraph(points["uniform equal"], points["uniform 0.1"], lines["uniform equal"], lines["uniform 0.1"], "uniform 0.1")
    generateSingleGraph(points["normal equal"], points["normal 0.5"], lines["normal equal"], lines["normal 0.5"], "normal 0.5")
    generateSingleGraph(points["mixture equal"], points["mixture disequal"], lines["mixture equal"], lines["mixture disequal"], "mixture")
    generateSingleGraph(points["chi equal"], points["chi 1"], lines["chi equal"], lines["chi 1"], "chi 1")

generateGraphs()
