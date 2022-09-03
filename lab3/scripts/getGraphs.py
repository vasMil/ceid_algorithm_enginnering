from cProfile import label
import pandas as pd
import matplotlib.pyplot as plt

# Load the data from ../out/times.csv into a dataframe

# Group by the type of graph (random, aimn91_synth)

# Group by the operation (add, minpath, length, decrease)

# Group by the algorithm (AIMN91, DIJKSTRA)

# Plot for fixed amount of edges

# Plot for fixed amount of vertices

# Given typeOfGraph and algorithm extract useful info from dataframe
def extractDataFrame(df, typeOfGraph):
    ret = df.loc[df["typeOfGraph"] == typeOfGraph].iloc[1:]
    # group by the type of the graph and the number of nodes, find the mean
    # ret = ret.groupby(["numberOfNodes"])
    return ret

# Given typeOfGraph, fix_str ("edges" || "nodes")
# Plot all operations (add, minpath, length, decrease)
# each plot should contain both algorithms (DIJKSTRA, AIMN91)
def plotOperations(df: pd.DataFrame, typeOfGraph: str):
    graphDf = extractDataFrame(df, typeOfGraph)
    # Drop unecessary columns
    graphDf = graphDf.drop(columns="numberOfEdges")

    # Create a seperate plot for each operation
    # Setup the figure with 4 (2x2) subplots
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2,2, figsize=(15,15))
    axList = [ax1, ax2, ax3, ax4]
    graphDf = graphDf.groupby("operation")
    for i, oper in enumerate(["add", "decrease", "minpath", "length"]):
        # Choose a subplot and configure
        ax = axList[i]
        ax.set_title("Operation: " + oper)
        ax.set(xlabel="Number of nodes", ylabel="Time (ns)")
        ax.ticklabel_format(axis='x', style="scientific", useMathText=True)
        operDf = graphDf.get_group(oper).reset_index(drop=True)
        # Create a line for each algorithm
        operDf = operDf.groupby("algorithm")
        for alg in ["AIMN91", "DIJKSTRA"]:
            operAlgDf = operDf.get_group(alg).reset_index(drop=True)
            # Get the mean of all the measurments that have the same amount of nodes
            operAlgDf = operAlgDf.groupby(["numberOfNodes"])
            operAlgDf = operAlgDf.mean().reset_index()
            operAlgDf.plot(x="numberOfNodes", y="time", ax=ax, label=alg, style='o-')
        ax.legend()
    fig.savefig("../out/plots4_" + typeOfGraph + "")
        


df = pd.read_csv("../out/times.csv")
plotOperations(df, "aimn91_synth")
plotOperations(df, "random")