from cProfile import label
import pandas as pd
import matplotlib.pyplot as plt

# Given typeOfGraph and algorithm extract useful info from dataframe
def extractDataFrame(df, typeOfGraph):
    ret = df.loc[df["typeOfGraph"] == typeOfGraph].iloc[1:]
    # group by the type of the graph and the number of nodes, find the mean
    # ret = ret.groupby(["numberOfNodes"])
    return ret

# Given typeOfGraph and a df
# Plot all operations (add, minpath, length, decrease)
# each plot should contain the algorithms: DIJKSTRA, AIMN91
def plotOperations(df: pd.DataFrame, typeOfGraph: str):
    print("\n\n***************** TIMES *******************")
    graphDf = extractDataFrame(df, typeOfGraph)
    # Drop unecessary columns
    graphDf = graphDf.drop(columns="numberOfEdges")

    # Print the data
    print(typeOfGraph)
    print(graphDf.groupby(["numberOfNodes", "algorithm", "operation"]).mean())

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
        

# Plot one of the graph types (random, aimn91_synth, complete)
# the plot should contain times of the algorithms Dijkstra, Floyd_Warshall and AIMN91
def plot_Floyd_AIMN91(df: pd.DataFrame, typeOfGraph: str):
    print("\n\n***************** BATCH *******************")
    graphDf = extractDataFrame(df, typeOfGraph)
    # Drop unecessary columns
    graphDf = graphDf.drop(columns=["numberOfEdges", "operation"])

    # Create a figure and configure it
    fig, (ax) = plt.subplots(1,1, figsize=(15,15))
    ax.set_title(typeOfGraph)
    ax.set(xlabel="Number of nodes", ylabel="Time (ns)")
    ax.ticklabel_format(axis='x', style="scientific", useMathText=True)
    print(typeOfGraph)
    print(graphDf.groupby(["numberOfNodes", "algorithm"]).mean())
    # Create a line for each algorithm
    graphDf = graphDf.groupby("algorithm")
    for alg in ["AIMN91", "FLOYD_WARSHALL", "DIJKSTRA"]:
        algDf = graphDf.get_group(alg).reset_index(drop=True)
        # Get the mean of all the measurments that have the same amount of nodes
        algDf = algDf.groupby(["numberOfNodes"])
        algDf = algDf.mean().reset_index()
        algDf.plot(x="numberOfNodes", y="time", ax=ax, label=alg, style='o-')
    ax.legend()
    fig.savefig("../out/plot4_" + typeOfGraph + "_batch")


pd.set_option('display.max_rows', None)

df = pd.read_csv("../out/times.csv")
plotOperations(df, "aimn91_synth")
plotOperations(df, "random")
plotOperations(df, "complete")

df = pd.read_csv("../out/times_batch.csv")
plot_Floyd_AIMN91(df, "aimn91_synth")
plot_Floyd_AIMN91(df, "random")
plot_Floyd_AIMN91(df, "complete")