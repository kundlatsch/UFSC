import os.path
from os import path
from regular import *

def load(name, type_input):
    if path.isfile("./input/" + name + "." + type_input + ".txt"):
        if type_input == 'grammar':
            return parse_grammar(name, type_input)
        if type_input == 'cfg':
            return parse_cfg(name, type_input)
        if type_input == 'regex':
            pass
        if type_input == 'automata':
            return parse_automata(name, type_input)
        
    else:
        return None

def parse_cfg(name, type_input):
    f = open("./input/" + name + "." + type_input + ".txt", "r")
    _grammar = grammar.CFG(name)
    first = True
    for line in f:
        line = line.replace(" ", "")
        parts = line.split('->')
        left = parts[0]
        right = parts[1]
        productions = right.split('|')
        _grammar.add_productions(left, productions)
        if first:
            _grammar.set_start_symbol(left)
            first = False
    new_productions = {}
    for key in _grammar.productions:
        prod_list = []
        for production in _grammar.productions[key]:
            prod_list.append(production.replace('\n', ""))
        new_productions[key] = prod_list
    _grammar.productions = new_productions
    return _grammar

# Q: a - A, B, C; b - C;
def parse_automata(name, type_input):
    f = open("./input/" + name + "." + type_input + ".txt", "r")
    _automata = automata.Automata(name)
    first = True
    initial = ""
    end_states = []

    for line in f:
        line = line.replace(" ", "")
        parts = line.split(':')
        state = parts[0]
        if state[0] == '*':
            end_states.append(state[1:])
            state = state[1:]
        
        transitions = parts[1].split(';')
        transition_list = []
        for transition in transitions:
            parts = transition.split('-')
            if len(parts) > 1:
                state_list = parts[1].split(',')
                transition_list.append((parts[0], state_list))
        
        if first:
            initial = state
            first = False
        _automata.add_state(state, transition_list)
    
    for state in end_states:
        _automata.set_end_state(state)
    
    _automata.set_start(initial)
    return _automata

def parse_grammar(name, type_input):
    f = open("./input/" + name + "." + type_input + ".txt", "r")
    _grammar = grammar.RegGrammar(name)
    first = True
    for line in f:
        line = "".join(line.split("\n"))
        line = line.replace(" ", "")
        parts = line.split('->')
        left = parts[0]
        right = parts[1]
        productions = right.split('|')
        _grammar.add_productions(left, productions)
        if first:
            _grammar.set_start_symbol(left)
            first = False
    new_productions = {}
    for key in _grammar.productions:
        prod_list = []
        for production in _grammar.productions[key]:
            prod_list.append(production.replace('\n', ""))
        new_productions[key] = prod_list
    _grammar.productions = new_productions
    return _grammar