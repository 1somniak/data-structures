
def binary_search(array, value):
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


class Node:
    def __init__(self, order, values=[], childrens=[]):
        self.order = order
        self.values = values
        self.childrens = childrens

    def __str__(self):
        return str(self.values)

    def to_dot(self, extension=""):
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
        return s

    def __add_value(self, value):
        index = binary_search(self.values, value)
        self.values.insert(index, value)
        return None, None, None, None

    def __add_leaf_full(self, value):
        index = binary_search(self.values, value)
        self.values.insert(index, value)
        middle = self.order // 2
        left = Node(self.order, self.values[:middle])
        right = Node(self.order, self.values[middle:])
        return right.values[0], None, left, right

    def insert(self, value):
        if len(self.childrens) == 0:  # no children (leaf)
            if len(self.values) + 1 < self.order:  # leaf not full
                return self.__add_value(value)
            else:  # leaf full
                return self.__add_leaf_full(value)
        else:  # there is children (node)
            index = binary_search(self.values, value)
            (val, root, left, right) = self.childrens[index].insert(value)
            if val is not None:
                self.values.insert(index, val)
                self.childrens[index] = left
                self.childrens.insert(index + 1, right)

            if len(self.values) == self.order:  # node null
                middle = self.order // 2
                one = Node(self.order, self.values[:middle], self.childrens[:(middle + 1)])
                two = Node(self.order, self.values[middle + 1:], self.childrens[(middle + 1):])
                return self.values[middle], None, one, two
            return None, None, None, None

    def remove(self, value):
        if len(self.childrens) == 0:
            index = binary_search(self.values, value)
            if len(self.values) > self.order / 2 - 1:
                pass
            else:
                pass

    def search(self, value):
        index = binary_search(self.values, value)
        if len(self.childrens) == 0:
            if self.values == value:
                return self.values[index]
            else:
                return None
        else:
            return self.childrens[index].search(value)

class Bplustree:
    def __init__(self, order):
        self.order = order
        self.root = Node(order)

    def export(self, filename, extension=""):
        file = open(filename, "w")
        file.write(self.root.to_dot(extension))
        file.close()

    def insert(self, value):
        index = binary_search(self.root.values, value)

        if len(self.root.childrens) == 0:
            if len(self.root.values) < self.order - 1:
                self.root.values.insert(index, value)
            else:
                self.root.values.insert(index, value)
                middle = self.order // 2
                one = Node(self.order, self.root.values[:middle], self.root.childrens[:(middle + 1)])
                two = Node(self.order, self.root.values[middle:], self.root.childrens[(middle + 1):])
                self.root.childrens = [one, two]
                self.root.values = [two.values[0]]
            return
        (val, root, left, right) = self.root.insert(value)
        if len(self.root.values) + 1 < self.order:
            if val != None:
                self.root.values.insert(index, val)
                self.root.childrens[index] = left
                self.root.childrens.insert(index + 1, right)

        else:
            if val != None:
                self.root.values = [val]
                self.root.childrens = [left, right]
                return
                self.root.childrens[index] = left
                self.root.childrens.insert(index + 1, right)
                """middle = self.order // 2
                one = Node(self.order, self.root.values[:middle], self.root.childrens[:(middle + 1)])
                two = Node(self.order, self.root.values[middle:], self.root.childrens[(middle + 1):])
                self.root.childrens = [one, two]
                self.root.values = [two.values[0]]"""

    def search(self, value):
        return self.root.search(value)

    def remove(self, value):
        if self.root.childrens == 0:
            index = binary_search(self.root.values, value)
            self.root.values.pop(index)


if __name__ == '__main__':
    import random
    print("hey!")
    random.seed(1000)
    bpt = Bplustree(3)
    """if bpt.order == 30:
        print(f"average insertion time : {1.9}s")
    elif bpt.order == 10:
        print(f"average insertion time : {8.8}s")

    L = list(range(100000))"""
    for v in [3, 10, 16, 19, 17, 17, 17, 17, 17]:
        bpt.insert(v)

    """while len(L) != 0:
        bpt.insert(L.pop(random.randint(0, len(L) - 1)))

    for i in range(1000000):
        a = bpt.search(i % 100000)

    bpt.export("bplustree.txt")"""

    bpt.export("graph.dot", "")
