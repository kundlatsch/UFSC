from MST import MST

class Graph:
    def __init__(self, graph_dictionary=None):
        self.graph_dictionary = graph_dictionary or {}

    def get_vertices(self):
        return list(self.graph_dictionary.keys())

    def get_edges(self):
        edges = []
        for vertex in self.graph_dictionary:
            for neighbor in self.graph_dictionary[vertex]:
                if {neighbor, vertex} not in edges:
                    edges.append((vertex, neighbor[0], neighbor[1]))
        return edges

    def get_neighbors(self, vertex):
        neighbor_list = []
        for neighbor in self.graph_dictionary[vertex]:
            neighbor_list.append(neighbor[0])

        return neighbor_list

    def insert_vertex(self, vertex):
        if vertex not in self.graph_dictionary:
            self.graph_dictionary[vertex] = []

    def build_file(self, file_in):
        data = open(file_in, 'r').read()
        data = data.split('*')
        vertices = data[1].strip().split('\n')[1:]
        edges = data[2].strip().split('\n')[1:]

        for v in vertices:
            v = v.split()
            self.insert_vertex(v[0])

        for e in edges:
            e = e.split()
            self.insert_edge((e[0], e[1]), e[2])

    def remove_vertex(self, vertex):
        if vertex in self.graph_dictionary:
            neighbor_list = self.get_neighbors(vertex)

            for neighbor in neighbor_list:
                self.remove_edge(vertex, neighbor)
                self.remove_edge(neighbor, vertex)

            self.graph.pop(vertex)
        else:
            print("not a vertex")

    def insert_edge(self, edge, w=1):
        edge = set(edge)
        (v1, v2) = tuple(edge)
        v1 = int(v1)
        v2 = int(v2)
        w = float(w)
        if v1 not in self.graph_dictionary:
            self.graph_dictionary[v1] = []

        if v2 not in self.graph_dictionary:
            self.graph_dictionary[v2] = []

        self.graph_dictionary[v2].append((v1, w))
        self.graph_dictionary[v1].append((v2, w))


    def remove_edge(self, edge):
        for node in self.graph_dictionary:
            for edge in self.graph_dictionary[node]:
                self.graph_dictionary[node].remove(edge)
                print('edge removida')



    def order(self, vertex):
        return len(self.graph_dictionary[vertex])

    def get_weight(self, v1, v2):
        for vertex in self.get_edges():
            if ((vertex[0] == v2) and vertex[1] == v1):
                return vertex[2]
        return float('inf')


if __name__ == '__main__':
    graph = Graph()
    graph.build_file('agm_tiny.net')

    n_vertices = len(graph.get_vertices())

    mst = MST(n_vertices, graph)
    mst.KruskalMST()
