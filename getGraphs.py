import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import math

def extractDataFrame(df, typeOfGraph):
    ret = df.loc[df["type_of_graph"] == typeOfGraph].iloc[1:]
    # group by the type of the graph and the number of nodes, find the mean
    ret = ret.groupby(["number_of_nodes"]).mean()
    return ret


# load data into a df
df = pd.read_csv("measurments.csv")

# extract data for a specific type of graph from df and get the mean
ringDf = extractDataFrame(df, "nestedSquaresGraph")

leda_ms = ringDf["leda_ms"]
myImpl_ms = ringDf["myImpl_ms"]

# create points to evaluate n+logn on
n = np.arange(1,100000)
big_o = (n + np.log2(n))*0.0001

# plot
# big_oLine = plt.plot(n, big_o)
leda_msLine = plt.plot(leda_ms, '-bo')
myImpl_msLine = plt.plot(myImpl_ms, '-gx')

# plt.legend(["leda_ms", "myImpl_ms", "n+logn"])
plt.legend(["leda_ms", "myImpl_ms"])
plt.xlabel("num_of_nodes")
plt.ylabel("exec time in ms")
plt.show()