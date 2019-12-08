import sys
sys.path.append("..")

from regular.automata import Automata
from utils import file_loader

automata = file_loader.load('e', 'automata')
automata.print_automata()
print("Minimizado:")
automata.to_minimal()
automata.print_automata()