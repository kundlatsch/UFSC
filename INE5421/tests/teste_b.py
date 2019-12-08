import sys
sys.path.append("..")

from regular.automata import Automata
from utils import file_loader

automata = file_loader.load('b', 'automata')
automata.print_automata()
print("Determinizado:")
_automata = automata.to_DFA()
_automata.print_automata()
print('Minimizado:')
_automata = _automata.to_minimal()