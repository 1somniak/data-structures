class Bplustree:
    def __init__(self, order, values, childrens):
        self.order = order
        self.values = values
        self.childrens = childrens

    def __str__(self):
        return str(self.values)

    def __binary_search(self, array, value):
        left = 0
        right = len(array)
        while left < right:
            mid = left + (right - left) // 2
            if value > array[mid]:
                left = mid + 1
            elif value < array[mid]:
                right = mid
            elif value == array[mid]:
                return mid
        return right

    def export(self, filename, extension=""):
        s = "digraph Btree {\n"
        nodes = [self]
        index = 0
        while len(nodes) - index != 0:
            s += "    node" + extension + str(index) + " [shape=box][label=\""
            for v in nodes[index].values:
                s += str(v) + " "
            if len(nodes[index].values) >= 1:
                s = s[:-1]
            s += "\"]\n"
            for c in nodes[index].childrens:
                nodes.append(c)
                s += "    node" + extension + str(index) + " -> node" + extension + str(len(nodes) - 1)
                s += "\n"
            index += 1
        s += "}"
        file = open(filename, "w")
        file.write(s)
        file.close()

    """
    return type (int, Bplustree, Bplustree)
    represents the values and the tree that can come back and must be inserted.
    are represented at none if everything appends good.
    """

    def aux_insert(self, value):
        if len(self.childrens) == 0:
            if len(self.values) + 1 < self.order:
                index = self.__binary_search(self.values, value)
                self.values.insert(index, value)
                return (None, None, None)
            else:  # la feuille est pleine
                index = self.__binary_search(self.values, value)
                self.values.insert(index, value)
                middle = self.order // 2
                left = Bplustree(self.order, self.values[:middle], [])
                right = Bplustree(self.order, self.values[middle:], [])
                return (right.values[0], left, right)

        else:  # il y a des enfants
            index = self.__binary_search(self.values, value)
            (val, left, right) = self.childrens[index].aux_insert(value)

            if len(self.values) + 1 < self.order:  # le noeud n'est pas plein
                if val != None:
                    self.values.insert(index, val)
                    self.childrens[index] = left
                    self.childrens.insert(index + 1, right)
                return (None, None, None)
            else:  # le noeud est plein
                if val != None:
                    self.values.insert(index, val)
                    self.childrens[index] = left
                    self.childrens.insert(index + 1, right)
                    middle = self.order // 2
                    one = Bplustree(self.order, self.values[:middle],
                                    self.childrens[:(middle + 1)])
                    two = Bplustree(self.order, self.values[middle:],
                                    self.childrens[(middle + 1):])
                    return two.values[0], one, two  # ???
                return None, None, None

    def insert(self, value):
        index = self.__binary_search(self.values, value)

        if len(self.childrens) == 0:
            if len(self.values) < self.order - 1:
                self.values.insert(index, value)
            else:
                self.values.insert(index, value)
                middle = self.order // 2
                one = Bplustree(self.order, self.values[:middle],
                                self.childrens[:(middle + 1)])
                two = Bplustree(self.order, self.values[middle:],
                                self.childrens[(middle + 1):])
                self.childrens = [one, two]
                self.values = [two.values[0]]
            return

        (val, left, right) = self.aux_insert(value)
        if len(self.values) + 1 < self.order:
            if val != None:
                self.values.insert(index, val)
                self.childrens[index] = left
                self.childrens.insert(index + 1, right)
        else:
            if val != None:
                self.values.insert(index, val)
                self.childrens[index] = left
                self.childrens.insert(index + 1, right)
                middle = self.order // 2
                one = Bplustree(self.order, self.values[:middle],
                                self.childrens[:(middle + 1)])
                two = Bplustree(self.order, self.values[middle:],
                                self.childrens[(middle + 1):])
                self.childrens = [one, two]
                self.values = [two.values[0]]


bpt = Bplustree(3, [10], [])
bpt.insert(14)
bpt.insert(12)
bpt.insert(20)
bpt.insert(35)

bpt.export("graph.dot", "a")
