from regular.regex import Regex
class SintaxTree:

    def __init__(self):
        self.nodes = []


    def regex_tree(self, regex: Regex):
        expr = regex.expression
        while (len(expr) > 0):
            operand = expr[0]
            if operand == "(":
                for i in range(len(expr)):
                    operand += expr[i]
                    if expr[i] == ")":
                        if len(expr) > i+1:
                            if expr[i+1] == "*":
                                operand += expr[i+1]
                            break

def convert_expr_group_to_tree(expr): #resolve qualquer expr
    internal_groups = separate_expr_by_concat(expr) 
    
    if len(internal_groups) == 0:
        return "erro"
    elif len(internal_groups) > 1:
        resolved = []
        for group in internal_groups:
            resolved.append(convert_expr_group_to_tree(expr))
        my_tree = tree_concat(resolved)
        return my_tree
    
    my_tree = solve(group)
    return my_tree

def separate_expr_by_concat(expr): #separa as concats
    return []

def tree_concat(trees): #concatena trees
    return []

def solve(group): #resolve "operandos", "ou" e "fecho"
    tree = ""
    return tree

class Node:

    def __init__(self, content: str, root = False):
        self.root = root
        self.leaf = True
        self.left = None
        self.right = None
        self.content = content
        self.number = None
        self.firstpos = set()
        self.followpos = set()

    def add_son(self, node: Node, is_left: bool):
        if is_left:
            self.left = node
            self.leaf = False
        else:
            self.right = node
            self.leaf = False

    def remove_son(self, is_left: bool):
        if is_left:
            if self.left:
                self.left = None
            else:
                print("This node doesn't have a son in this position")
        else:
            if self.right:
                self.right = None
            else:
                print("This node doesn't have a son in this position")            