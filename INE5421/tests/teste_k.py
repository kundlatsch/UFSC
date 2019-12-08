import sys
sys.path.append("..")

from regular.grammar import CFG
from utils import file_loader

grammar = file_loader.load('k', 'cfg')
grammar.print_grammar()
print('Sem não determinismo:')
grammar.remove_non_determinism()
grammar.print_grammar()