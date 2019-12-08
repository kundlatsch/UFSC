import sys
sys.path.append("..")

from regular.automata import Automata
from regular.grammar import RegGrammar
from utils import file_loader

automata: Automata = file_loader.load('c', 'automata')
automata.print_automata()
print('AF -> GR')
_grammar = automata.to_RG()
_grammar.print_grammar()

grammar = file_loader.load('c', 'grammar')
grammar.print_grammar()
print('GR -> AF')
_automata = grammar.to_FA()
_automata.print_automata()