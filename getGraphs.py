import pandas as pd
import matplotlib.pyplot as plt

def extractDataFrame(df, typeOfGraph):
    ret = df.loc[df["type_of_graph"] == typeOfGraph].iloc[1:]
    # group by the type of the graph and the number of nodes, find the mean
    ret = ret.groupby(["number_of_nodes"])
    return ret

def plotDataframe(subplot_ax, df, typeOfGraph):
    # extract data for a specific type of graph from df and get the mean
    graphDf = extractDataFrame(df, typeOfGraph)
    graphDf = graphDf.mean()
    leda_ms = graphDf["leda_ms"]
    myImpl_ms = graphDf["myImpl_ms"]

    # create points to evaluate n+logn on
    # n = np.arange(1,100000)
    # big_o = (n + np.log2(n))*0.0001

    # plot on subplot
    # big_oLine = plt.plot(n, big_o)
    subplot_ax.plot(leda_ms, '-bo')
    subplot_ax.plot(myImpl_ms, '-gx')
    subplot_ax.set_title(typeOfGraph)
    subplot_ax.set(xlabel="num_of_nodes", ylabel="exec time in ms")
    subplot_ax.ticklabel_format(axis='x', style="scientific", scilimits=(10000,100000), useMathText=True)


def printTimes_ofGraphType(df, typeOfGraph):
    graphDf = extractDataFrame(df, typeOfGraph)
    print("\n############### ", typeOfGraph, " ###############")
    for name, group in graphDf:
        print("Number of Nodes: ", name)
        print("\tLEDA: ", group["leda_ms"].mean(), "ms")
        print("\tmyImpl: ", group["myImpl_ms"].mean(), "ms")


# load data into a df
df = pd.read_csv("measurments.csv")

# plot
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2,2, figsize=(7,7))

plotDataframe(ax1, df, "nestedSquaresGraph")
plotDataframe(ax2, df, "ringGraph")
plotDataframe(ax3, df, "fourLevelsGraph")
plotDataframe(ax4, df, "generalizedFibonacciCubeGraph")

# plt.legend(["leda_ms", "myImpl_ms", "n+logn"])
fig.legend(["leda_ms", "myImpl_ms"])
plt.tight_layout()
plt.show()

printTimes_ofGraphType(df, "nestedSquaresGraph")
printTimes_ofGraphType(df, "ringGraph")
printTimes_ofGraphType(df, "fourLevelsGraph")
printTimes_ofGraphType(df, "generalizedFibonacciCubeGraph")