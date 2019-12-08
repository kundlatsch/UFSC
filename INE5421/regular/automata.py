import copy
from .grammar import RegGrammar
import math

class Automata:
    
    def __init__(self, name: str):
        self.name = name
        self.transitions = {}
        self.start_state = ""
        self.end_states = []

    # Imprime o autômato
    def print_automata(self):
        print(self.name + ":\n")
        for state in self.transitions:
            aux1 = ""
            if state in self.end_states:
                aux1 += "*"    
            if state == self.start_state:
                aux1 += "->"
            for symbol in self.transitions[state]:
                aux2 = aux1 + "transição({0}; {1}) = ".format(state, symbol)
                state_list = ", ".join(self.transitions[state][symbol])
                if len(self.transitions[state][symbol]) > 1:
                    finalprint = aux2 + "{" + state_list + "}"
                else:    
                    finalprint = aux2 + state_list

                print(finalprint)

    # Recebe uma lista de tuplas transição na forma (símbolo, [estados alcançáveis por esse símbolo])
    # Exemplo: [('a', ['B']), ('b', ['C','D','E'])]
    # O dict transitions fica na forma transitions[estado] = {símbolo1: [estados],
    #                                                         símbolo2: [estados],
    #                                                         ...}
    # Exemplo: self.transitions['A'] = {'a': ['A','B'],
    #                                   'b': ['C'] 
    #                                   }
    def add_state(self, state_name: str, transition_list = [], end_state=False):

        if not state_name.isalnum() and "," not in state_name:
            print("The state's name must be alphanumeric.")
            return
        
        state_name = state_name.upper()

        if state_name in self.transitions:
            self.add_transition(state_name, transition_list)
        else:
            self.transitions[state_name] = {}

            for transition in transition_list:
                symbol: str = transition[0]
                reachable_states: list = transition[1]
                reachable_states.sort()
                for state in reachable_states:
                    state: str = state.upper()

                if symbol in self.transitions[state_name]:
                    for state in reachable_states:

                        if state not in self.transitions[state_name][symbol]:
                            self.transitions[state_name][symbol].append(state)
                            self.transitions[state_name][symbol].sort()

                        if state not in self.transitions:
                            self.add_state(state)    

                else:
                    self.transitions[state_name][symbol] = reachable_states
                    if state not in self.transitions:
                        self.add_state(state)

            if (end_state):
                self.end_states.append(state_name)

    # Remove o estado e todas as transições de outros estados que chegam até ele
    def remove_state(self, state_name: str):
        state_name = state_name.upper()
        if state_name in self.transitions:
            if state_name in self.end_states:
                self.remove_end_state(state_name)   
            del self.transitions[state_name]

            for state in self.transitions:
                symbol_to_remove = []
                for symbol in self.transitions[state]:
                    if state_name in self.transitions[state][symbol]:
                        self.remove_transition(state, [(symbol, [state_name])])
                        self.transitions[state][symbol].sort()
                        if len(self.transitions[state][symbol]) == 0:
                            symbol_to_remove.append(symbol)
                for symbol in symbol_to_remove:
                    del self.transitions[state][symbol]        

        else:
            print("State not removed: it does not exist")

    # Adiciona uma ou mais transições em um estado que já existe
    def add_transition(self, state_name: str, transition_list: list):

        state_name = state_name.upper()

        if state_name in self.transitions:

            for transition in transition_list:
                symbol: str = transition[0]
                reachable_states: list = transition[1]
                reachable_states.sort()
                for r_state in reachable_states:
                    r_state: str = r_state.upper()

                if symbol in self.transitions[state_name]:

                    for state in reachable_states:

                        if state not in self.transitions:
                            self.add_state(state)
                        if state not in self.transitions[state_name][symbol]:
                            self.transitions[state_name][symbol].append(state)
                            self.transitions[state_name][symbol].sort()

                else:
                    self.transitions[state_name][symbol] = reachable_states
        else:
            print("Transition not added: the given state does not exist")

    # [(a, [D])]
    def remove_transition(self, state_name: str, transition_list: list):

        state_name = state_name.upper()
        if state_name in self.transitions:

            for transition in transition_list:
                symbol: str = transition[0]
                reachable_states: list = transition[1]
                reachable_states.sort()
                for state in reachable_states:
                    state: str = state.upper()    

                if symbol in self.transitions[state_name]:

                    for state in reachable_states:

                        if state in self.transitions[state_name][symbol]:
                            self.transitions[state_name][symbol].remove(state)
                            self.transitions[state_name][symbol].sort()
                        else:
                            print(f"There's no transition from {state_name} through {symbol} to {state}")

                else:
                    print(f"There's no transition from {state_name} through {symbol}")

        else:
            print("Transition not removed: No such state exists")

    # Coloca o estado dado (que já existe no autômato) como estado inicial
    # Se o autômato já tiver outro estado inicial, ele é substituído
    def set_start(self, name: str):

        name = name.upper()
        if name in self.transitions:
            self.start_state = name
        else:
            print("State not set to start state: No such state exists")

    def remove_start(self):
        if self.start_state:
            self.start_state = ""                      
        else:
            print(f"O autômato {self.name} não possui estado inicial no momento")

    def set_end_state(self, state_name: str):

        if state_name in self.transitions:
            if state_name not in self.end_states:
                self.end_states.append(state_name)
                self.end_states.sort()
        else:
            print("State not set to end state: No such state exists")

    def remove_end_state(self, state_name: str):

        if state_name in self.transitions:
            if state_name in self.end_states:
                self.end_states.remove(state_name)
                self.end_states.sort()
            else:
                print("No such end state exists")    
        else:
            print("End state not removed: No such state exists")

    # Checa se a string inserida é computável pelo autômato atual
    def compute(self, string: str):
        if not(self.start_state):
            print("You first need to define the start state!")
        current_state = ""
        backtracks = [self.start_state]
        i = 0
        while i < len(backtracks):
            current_state: str = backtracks[i]
            for symbol in string:
                if symbol in self.transitions[current_state]:
                    next_states = self.transitions[current_state][symbol]
                else:
                    next_states = ""

                    # Se o autômato for não determinístico, guarda os estados "extras" para rever
                    # os outros caminhos possíveis caso a computação pelo caminho atual dê errado (backtracking)
                if next_states:    
                    if len(next_states) > 1:
                        current_state = next_states[0]
                        for state in next_states[1:]:
                            backtracks.append(state)
                    else:    
                        current_state = self.transitions[current_state][symbol][0]
                else:
                    current_state = ""
                    break
      
            if current_state in self.end_states:            
                return True
            i += 1

        return False    

    # Checa se o autômato é não-determinístico
    def is_NFA(self):

        for state in self.transitions:
            if '&' in self.transitions[state]:
                return True
            for symbol in self.transitions[state]:
                if(len(self.transitions[state][symbol]) > 1):
                    return True
        return False

    # Calcula epsilon-fecho do estado e o retorna como uma lista de estados
    def epsilon_closure(self, state_name: str, e_closure = []):

        state_name = state_name.upper()
        if state_name not in e_closure:
            e_closure.append(state_name)
        if state_name in self.transitions:
            
            if '&' in self.transitions[state_name]:

                    for state in self.transitions[state_name]['&']:

                        state_list = state.split(",")
                        for _ in state_list:

                            if _ not in e_closure:
                                state_e_closure = self.epsilon_closure(_, e_closure)

                                for new_state in state_e_closure:
                                    if new_state not in e_closure:
                                        e_closure.append(new_state)      

        e_closure.sort()
        return e_closure      

    # "Bota chaves" nos estados do autômato (faz com que múltiplos estados possíveis virem um estado só)
    # Exemplo: ['A', 'B', 'C'] --> ['A,B,C']
    def put_brackets(self):
        states_to_add = []
        for state in self.transitions:
            for symbol in self.transitions[state]:
                next_states = ",".join(self.transitions[state][symbol])
                if "," in next_states:
                    states_to_add.append(next_states)
                    self.transitions[state][symbol] = [next_states]
        for state in states_to_add:
            self.add_state(state)
            for add_state in state.split(","):
                if add_state in self.end_states:
                    self.set_end_state(state)
                    break                      

    # Retorna o versão determinística do autômato
    # Não há garantia que o autômato seja mínimo
    # se union = True, a determinização faz a união
    # dos estados de aceitação, caso contrário,
    # faz a interseccção
    def to_DFA(self, union = True, end_states_1 = [], end_states_2 = []):
        # Se o autômato já for determinístico, retorna ele mesmo
        if not self.is_NFA():
            return self

        aux = copy.deepcopy(self) 
        aux.put_brackets()
        
        dfa = Automata(self.name + "_DFA")

        for state in aux.transitions:

            # Se o estado tiver uma transição por épsilon, ele não existe no autômato determinístico,
            # sendo substituído pelo seu épsilon-fecho
            if '&' in aux.transitions[state]:

                state_e_closure = aux.epsilon_closure(state)
                new_state = ",".join(state_e_closure)
                dfa.add_state(new_state)

                if state == aux.start_state:
                    dfa.set_start(new_state)    
                if new_state in dfa.end_states:
                    continue
                if (union):
                    for e_state in new_state.split(","):
                        if e_state in aux.end_states:
                            dfa.set_end_state(new_state)
                            break
                else:
                    is_end_1 = False
                    is_end_2 = False
                    for e_state in new_state.split(","):
                        if e_state in end_states_1:
                            is_end_1 = True
                            break
                    for e_state in new_state.split(","):
                        if e_state in end_states_2:
                            is_end_2 = True
                            break
                    if is_end_1 and is_end_2:
                            dfa.set_end_state(new_state)
                continue
            
            
            # Se o estado não possuir transição por épsilon, ele é adicionado normalmente, e adiciona
            # à AFD os estados não determinísticos existentes
            # Não há garantia de que o estado adicionado seja alcançável por um novo estado de aceitação, caso exista
            for symbol in aux.transitions[state]:

                reachable_states = ",".join(aux.transitions[state][symbol])

                dfa.add_state(state, [(symbol, [reachable_states])])
                if state == aux.start_state:
                    dfa.set_start(state)
                if union:
                    if state in aux.end_states:
                        dfa.set_end_state(state)  
                else:
                    if state in end_states_1 and state in end_states_2:
                        dfa.set_end_state(state)
                if "," in reachable_states:
                    if (union):
                        for r_state in reachable_states.split(","):
                            if r_state in aux.end_states:
                                dfa.set_end_state(reachable_states)
                                break
                    else:
                        is_end_1 = False
                        is_end_2 = False
                        for r_state in reachable_states.split(","):
                            if e_state in end_states_1:
                                is_end_1 = True
                                break
                        for r_state in reachable_states.split(","):
                            if r_state in end_states_2:
                                is_end_2 = True
                                break
                        if is_end_1 and is_end_2:
                                dfa.set_end_state(reachable_states)
        # Adiciona as transições faltantes dos estados não-determinísticos
        # Caso a transição de um desses estados originar um novo estado não-determinístico,
        # adicioná-lo também até que todos os estados possíveis tenham sido completos

        i = 0
        states = list(dfa.transitions.keys())
        if type(states) == type(None):
            states = []

        alphabet = []
        for state in aux.transitions:
            for symbol in aux.transitions[state]:
                if symbol not in alphabet and symbol != '&':
                    alphabet.append(symbol)    

        while i < len(states):

            if "," in states[i]:
                for symbol in alphabet:
                    next_state_list = []
                    for state in states[i].split(","):
                        if symbol in aux.transitions[state]:
                            for n_state in aux.transitions[state][symbol]:
                                if n_state not in next_state_list:
                                    next_state_list.append(n_state)
                    next_state_list.sort()
                    next_state = ",".join(next_state_list)
                    if not (next_state):
                        continue
                    if next_state not in dfa.transitions:
                        if next_state in aux.transitions and '&' in aux.transitions[next_state]:
                            next_state = ",".join(aux.epsilon_closure(next_state))
                            if next_state in dfa.transitions:
                                dfa.add_transition(states[i], [(symbol, [next_state])])           
                                continue

                        dfa.add_state(next_state)
                        states.append(next_state)

                        if (union):
                            for n_state in next_state.split(","):
                                if n_state in aux.end_states:
                                    dfa.set_end_state(next_state)
                                    break
                        else:

                            all_end = True
                            for n_state in next_state.split(","):
                                if n_state not in aux.end_states:
                                    all_end = False
                                    break
                            if all_end:
                                dfa.set_end_state(next_state)
                    dfa.add_transition(states[i], [(symbol, [next_state])])           

            i += 1
        dfa.put_brackets()    
        return dfa

    # Minimiza o autômato --->determinístico<----
    def to_minimal(self):
        if self.is_NFA():
            return ("The automaton must be deterministic")   
        self.name = self.name + "_minimal"
        self.remove_unreachables()
        self.remove_dead_states()
        self.remove_equivalents()

    # Remove os estados inalcançáveis
    def remove_unreachables(self):
        marked_states = [self.start_state]
        i = 0
        while i < len(marked_states):
            for symbol in self.transitions[marked_states[i]]:

                next_states = self.transitions[marked_states[i]][symbol]

                for next_state in next_states:
                    if next_state not in marked_states:
                        marked_states.append(next_state)

            i += 1

        state_to_remove = []
        for state in self.transitions:
            if state not in marked_states:
                state_to_remove.append(state)    
        for state in state_to_remove:
            self.remove_state(state)        

    # Remove os estados mortos
    def remove_dead_states(self):
        marked_states = copy.deepcopy(self.end_states)

        iterator = 0
        while iterator < len(marked_states):

            for state in self.transitions:
                for symbol in self.transitions[state]:
                    for m_state in marked_states:
                        if m_state in self.transitions[state][symbol]:
                            if state not in marked_states:
                                marked_states.append(state)
                                break
            iterator += 1

        state_to_remove = []
        for state in self.transitions:
            if state not in marked_states:
                state_to_remove.append(state)  
        for state in state_to_remove:
            self.remove_state(state)         

    # Remove os estados equivalentes de acordo com o algoritmo de Hopcroft
    def remove_equivalents(self):
        # Hopcroft's algorithm
        # P := {F, Q \ F};
        # W := {F, Q \ F};
        # while (W is not empty) do
        #     choose and remove a set A from W
        #     for each c in Σ do
        #         let X be the set of states for which a transition on c leads to a state in A
        #         for each set Y in P for which X ∩ Y is nonempty and Y \ X is nonempty do
        #             replace Y in P by the two sets X ∩ Y and Y \ X
        #             if Y is in W
        #                 replace Y in W by the same two sets
        #             else
        #                 if |X ∩ Y| <= |Y \ X|
        #                     add X ∩ Y to W
        #                 else
        #                     add Y \ X to W

        alphabet = []
        for state in self.transitions:
            for symbol in self.transitions[state]:
                if symbol not in alphabet:
                    alphabet.append(symbol)

        partitions = [set(self.end_states), set(set(self.transitions.keys()).difference(set(self.end_states)))]
        word = copy.deepcopy(partitions)

        while len(word) > 0:
            set_A = word.pop()
            for symbol in alphabet:
                set_X = set()
                for state in self.transitions:
                    if symbol in self.transitions[state]:
                        if self.transitions[state][symbol][0] in set_A:
                            set_X.add(state)       
                for set_Y in partitions:
                    intersect = set_X.intersection(set_Y)
                    diff = set_Y.difference(set_X)
                    if intersect and diff:
                        partitions.remove(set_Y)
                        partitions.append(intersect)
                        partitions.append(diff)
                        if set_Y in word:
                            word.remove(set_Y)
                            word.append(intersect)
                            word.append(diff)
                        else:
                            if intersect <= diff:
                                word.append(intersect)
                            else:
                                word.append(diff)                
            for set_p in partitions:
                set_p = list(set_p)
                if len(set_p) > 1:
                    start = self.start_state
                    if start in set_p:
                        swap_state = set_p.pop(set_p.index(start))
                    else:    
                        swap_state = set_p.pop(0)    
                    for state in set_p:

                        self.substitute(state, swap_state)

    # Substitui um estado por outro
    def substitute(self, removed_state, swap_state):
        self.add_state(swap_state)
        self.transitions[swap_state] = self.transitions[removed_state]
        if removed_state == self.start_state:
            self.set_start(swap_state)
        if removed_state in self.end_states:
            self.remove_end_state(removed_state)
            self.set_end_state(swap_state)    
        for state in self.transitions:
            for symbol in self.transitions[state]:
                if removed_state in self.transitions[state][symbol]:
                    self.transitions[state][symbol].remove(removed_state)
                    if swap_state not in self.transitions[state][symbol]:
                        self.transitions[state][symbol].append(swap_state)
                    self.transitions[state][symbol].sort()         
        self.remove_state(removed_state)                

    # Retorna a gramática regular criada a partir do autômato -->determinístico<--
    def to_RG(self):

        if self.is_NFA:
            self.to_DFA()

        grammar = RegGrammar(self.name + "_to_RG")
        aux = copy.deepcopy(self)

        alphabet = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
                    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
        i = 1

        for state in aux.transitions:

            if "," in state or len(state) > 1:

                new_name = alphabet[-i]
                i += 1
                aux.transitions[new_name] = self.transitions[state]

                # Eu não sei se isso vai funcionar
                for state in aux.transitions:
                    for symbol in aux.transitions[state]:
                        state_list = aux.transitions[state][symbol].split(",")
                        if state in state_list:
                            state_list[state_list.index(state)] = new_name
                            state_list_det = ",".join(state_list)
                            aux.transitions[state][symbol] = state_list_det
                del aux.transitions[state]         

            if state in aux.start_state:
                if not(grammar.start_symbol):
                    grammar.set_start_symbol(state)

            productions = []

            for symbol in aux.transitions[state]:

                next_state = "".join(aux.transitions[state][symbol])

                productions.append(symbol + state)
                if next_state in aux.end_states:
                    productions.append(symbol)

            grammar.add_productions(state, productions) 

        return grammar

# Faz a união ou intersecção não determinística de dois autômatos 
# e depois a determiniza (por padrão, faz a união dos estados de aceitação)
# Se union = True, faz a união, caso contrário faz a interseccção
def afd_union_intersection(afd1: Automata, afd2: Automata, union: bool):

    if union:
        afd = Automata(afd1.name + "_" + afd2.name + "_union")
    else:
        afd = Automata(afd1.name + "_" + afd2.name + "_intersection")

    # Se os autômatos tiverem estados com o mesmo nome,
    # altera o nome de um deles
    substitutions = []
    for state1 in afd1.transitions:
        for state2 in afd2.transitions:
            if state1 == state2:
                substitutions.append(state2)
    for state in substitutions:
        afd2.substitute(state, state + "1")

    afd.add_state("QS0")
    afd.add_transition("QS0", [('&', [afd1.start_state, afd2.start_state])])
    afd.set_start("QS0")
    afd.transitions.update(afd1.transitions)
    afd.transitions.update(afd2.transitions)
    if union:
        for end_state in afd1.end_states + afd2.end_states:
            afd.set_end_state(end_state)
    dfa = afd.to_DFA(union, afd1.end_states, afd2.end_states)
    return dfa
