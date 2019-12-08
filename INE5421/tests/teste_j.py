import sys
sys.path.append("..")

from regular.grammar import CFG
from utils import file_loader

grammar = file_loader.load('j', 'cfg')
grammar.print_grammar()
print('Sem recursão à esquerda:')
grammar.remove_left_recursion()
grammar.print_grammar()