import sys
sys.path.append("..")

from regular.automata import Automata
from utils import file_loader

automata = file_loader.load('d', 'automata')
automata.print_automata()
string = 'aabca'
print("String computada: " + string)
accepted = automata.compute(string)
print(accepted)