import pandas as pd
import matplotlib.pyplot as plt

def extractDataFrame(df, typeOfGraph):
    ret = df.loc[df["type_of_graph"] == typeOfGraph].iloc[1:]
    # group by the type of the graph and the number of nodes, find the mean
    ret = ret.groupby(["number_of_nodes"])
    return ret

def plotDataframe(subplot_ax_time, subplot_ax_nodesVisited, df, typeOfGraph):
    # extract data for a specific type of graph from df and get the mean
    graphDf = extractDataFrame(df, typeOfGraph)
    graphDf = graphDf.mean()
    time_dijkstra_SP = graphDf["time_Dijkstra_SP"]
    time_a_star = graphDf["time_A*"]
    nodesVisited_dijkstra_SP = graphDf["nodesVisited_Dijkstra"]
    nodesVisited_a_star = graphDf["nodesVisited_A*"]

    subplot_ax_time.plot(time_dijkstra_SP, '-bo')
    subplot_ax_time.plot(time_a_star, '-go')

    subplot_ax_time.set_title("Graph: " + typeOfGraph)
    subplot_ax_time.set(xlabel="Number of nodes in graph", ylabel="Execution time in ms")
    subplot_ax_time.ticklabel_format(axis='x', style="scientific", scilimits=(10000,100000), useMathText=True)

    subplot_ax_nodesVisited.plot(nodesVisited_dijkstra_SP, '-bo')
    subplot_ax_nodesVisited.plot(nodesVisited_a_star, '-go')

    subplot_ax_nodesVisited.set_title("Graph: " + typeOfGraph)
    subplot_ax_nodesVisited.set(xlabel="Number of nodes in graph", ylabel="Nodes visited")
    subplot_ax_nodesVisited.ticklabel_format(axis='x', style="scientific", scilimits=(10000,100000), useMathText=True)


def plotLedaTimes(ax, ledaDf, typeOfGraph):
    # extract data for a specific type of graph from df and get the mean
    graphDf = extractDataFrame(ledaDf, typeOfGraph)
    graphDf = graphDf.mean()
    time_LEDA = graphDf["time_LEDA"]
    
    # plot
    ax.plot(time_LEDA, "-ro")


def printTimes_ofGraphType(df, typeOfGraph):
    graphDf = extractDataFrame(df, typeOfGraph)
    print("\n############### ", typeOfGraph, " ###############")
    for name, group in graphDf:
        print("Number of Nodes: ", name)
        print("\tDijkstra_SP: ", group["time_Dijkstra_SP"].mean(), "ms")
        print("\tA*: ", group["time_A*"].mean(), "ms")
        print("\tLEDA: ", group["time_LEDA"].mean(), "ms")


# load data into a df
df = pd.read_csv("boost_times.csv")
ledaDf = pd.read_csv("leda_times.csv")

# plot
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2,2, figsize=(7,7))
plotDataframe(ax1, ax2, df, "random")
plotLedaTimes(ax1, ledaDf, "random")
plotDataframe(ax3, ax4, df, "grid")
plotLedaTimes(ax3, ledaDf, "grid")

fig.legend(["Dijkstra_SP", "A*", "leda::DIJKSTRA_T"])
plt.tight_layout()
plt.show()

# Merge the two dataframes
print("\n\nIf boost_time.csv and leda_times.csv are not parallel, joining the two dataframes on the index will result to undefined behaviour!\n\n")
df = df.join(ledaDf["time_LEDA"])
printTimes_ofGraphType(df, "random")
printTimes_ofGraphType(df, "grid")