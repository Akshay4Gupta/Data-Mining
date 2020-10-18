import timeit

sh_fileName = "2019MCS2556.sh"
datafile = "retail.dat"
support_list = [1, 5, 10, 25, 50, 90]
trees = ["apriori", "fptree"]

def notAparallel():
    time_collection = []
    SETUP_CODE = "import os"
    for tree in trees:
        treetime = []
        for support in support_list:
            PROGRAM = 'os.system("./"+sh_fileName+" "+datafile+" "+str(support)+" -"+tree+" "+tree+str(support))'
            treetime.append(timeit.timeit(setup = SETUP_CODE, stmt = PROGRAM, number = 1))
        time_collection.append(treetime)
    print(time_collection)

def parallel():
    return

if __name__ == '__main__':
    notAparallel()
