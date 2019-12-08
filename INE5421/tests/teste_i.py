import sys
sys.path.append("..")

from regular.grammar import CFG
from utils import file_loader

grammar = file_loader.load('i', 'cfg')
grammar.print_grammar()
print('Forma normal de Chomsky:')
_grammar = grammar.to_chomsky_normal_form()
_grammar.print_grammar()