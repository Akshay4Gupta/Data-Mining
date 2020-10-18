import time
import os
from matplotlib import pyplot as plt
import sys

sh_fileName = "MCS192556.sh"
datafile = sys.argv[1]
support_list = [90, 50, 25, 10, 5, 1]
trees = ["fptree", "apriori"]

def notAparallel():
    time_collection = []
    for tree in trees:
        treetime = []
        for support in support_list:
            start = time.time()
            os.system("sh "+sh_fileName+" "+datafile+" "+str(support)+" -"+tree+" "+tree+str(support))
            end = time.time()
            treetime.append(end - start)
        time_collection.append(treetime)
    return time_collection

def parallel():
    return

if __name__ == '__main__':
    time_fptree, time_apirori = notAparallel()
    plt.figure()
    plt.plot(support_list, time_apirori, label='Apriori')
    plt.plot(support_list, time_fptree, label='FP-tree')
    plt.title('Performance Comparison')
    plt.xlabel('Support threshold')
    plt.ylabel('Execution Time(s)')
    plt.legend()
    plt.savefig("Plot.png")
