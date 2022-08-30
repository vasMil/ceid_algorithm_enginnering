# READ INSTALLATION GUIDE: https://pypi.org/project/graphviz/
# You will also need to install the graphviz library before running this script
# Step1: sudo apt install graphviz
# Step2: pip install graphviz
import graphviz
import sys

def renderGraph(path):
    G = graphviz.Source.from_file(path, format='png')
    G.view()

if __name__ == '__main__':
    renderGraph(sys.argv[1])