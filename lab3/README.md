# AIMN91 algorithm implementation
<i>Incremental Algorithms for Minimal Length Paths</i>

<i>Ausiello - Italiano - Spaccamela - Nanni - Journal of algorithms 1991</i>

AIMN91 provides a solution to the <u><i>All Pairs Shortes Path Problem</u></i>.

It does this under the assumptions below:
1. The graph has a fixed amount of vertices n.
2. The graph is directed.
3. Each edge should have an integer cost in a fixed range [1...C]
4. Eacb edge may be inserted at most once, but you may decrease its cost at any point.

Space complexity: O(n<sup>2</sup>)

Time complexity: O(Cn<sup>3</sup>log(nC))

Operations implemented:
- add(x,y,w)
- decrease(x,y,Δ)
- length(x,y)
- minimal_path(x,y)

## Dependancies
If you want to run the code in a UNIX environment you will need:
<i>The boost library</i>
1. sudo apt-get install libboost-all-dev

<i>Library used by a python script, in case you set VISUALIZATION true</i>

2. sudo apt-get install graphviz

<i>Python library, in case you set VISUALIZATION true</i>

3. pip install graphviz

<i>Command line utility, used when VISUALIZATION true</i>

4. sudo apt-get install xdotool


## Acknowledgements
I could have used the graphviz library directly in C++, since it is written in C, but it would be a lot more effort than writing the python script.
If I ended up using graphviz directly I would most likely avoid the use of the xdotool utilities, but that was not the point of this project.
