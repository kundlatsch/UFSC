import sys
sys.path.append("..")

from regular.grammar import CFG
from utils import file_loader

grammar = file_loader.load('l', 'cfg')
grammar.print_grammar()
print('Tabela SLR:')
grammar.create_table()
