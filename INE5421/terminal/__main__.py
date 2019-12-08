import sys
sys.path.append("..")

from terminal import run

# Test imports
from regular import grammar
from utils import serializer
from regular import automata


def main():
    print('------------------------\nTrabalho de Formais v0.1\n------------------------')
    print('Para obter ajuda digite \'help\'')
    run()
    

if __name__ == "__main__":
    # Create grammar save file for tests
    # _grammar = grammar.grammar('teste')
    # _grammar.add_state('A', [['a', 'A'], ['b', 'B']], 1)
    # _grammar.add_state('B', [['b', 'B']])
    # _grammar.set_start('A')
    # _grammar.compute("aabc")
    # serializer.save(_grammar)

    _grammar = grammar.CFG('teste')
    _grammar.add_productions('S', ['abA', 'a', 'A', 'Sa'])
    _grammar.add_productions('A', ['SaA', 'AA', 'a'])
    _grammar.set_start_symbol('S')
    #_grammar.print_grammar()
    #print('-------------')
    #_grammar.to_chomsky_normal_form()
    # _grammar.remove_left_recursion()
    _grammar.remove_non_determinism()
    serializer.save(_grammar)
    main()