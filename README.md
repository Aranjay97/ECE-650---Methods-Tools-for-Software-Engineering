# ECE 650 - Methods & Tools for Software Engineering
Python & C++

The project was completed as part of the ECE650 Graduate Coursework at the University of Waterloo.

The goal of the project is to assist the local police force in installing surveillance cameras at busy junctions. In this context, we'll solve a specific type of optimization issue known as the Vertex Cover problem. The goal is for the police to be able to reduce the number of cameras they need to place while still monitoring as effectively as feasible. You must: i)Take a series of commands that describe streets as input for this task. ii)Construct a certain type of undirected graph using the data. To summarise the project, it is divided into four assignments.

**Assignment 1:** Gathering GPS coordinates of streets and calculating intersections (using an undirected graph's vertices and edges concept) (Python)

**Assignment 2:** Given a set of street coordinates in the form of vertices and edges, solve the problem. To identify the shortest path between two vertices in the graph, we used the BFS Algorithm. (C++)

**Assignment 3:** Develop a random generate programme in Python to produce street coordinates as input to our assignment 1 Python software. To compute the shortest path between any two vertices, the Output from Assignment1 was fed into the Input of Assignment2. We use Multi-threaded programming and IPC (Inter-Process Communication) to connect the Standard Output and Input of multiple programmes utilising Pipes. (C++,Python)

**Assignment 4:** We use the CNF-SAT solver to solve the Vertex Cover issue.

For more information about the project and how to implement it, please refer to the individual PDFs in the folder.
