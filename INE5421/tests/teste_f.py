import sys
sys.path.append("..")

from regular.automata import *
from utils import file_loader

automata1 = file_loader.load('f1', 'automata')
automata2 = file_loader.load('f2', 'automata')
print('Automato 1')
automata1.print_automata()
print('Automato 2')
automata2.print_automata()
print("União:")
union = afd_union_intersection(automata1, automata2, True)
union.print_automata()
print("Intersecção:")
intersection = afd_union_intersection(automata1, automata2, False)
intersection.print_automata()