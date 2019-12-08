import copy
#from regular import automata

class RegGrammar:
# Considerando gramáticas regulares à esquerda    

    def __init__(self, name):
        self.name = name
        self.productions = dict()
        self.start_symbol = ""

    def set_start_symbol(self, symbol):

        symbol = symbol.upper()
        if len(symbol) > 1 or self.start_symbol:
            print("Não se pode ter mais de um símbolo inicial")
        else:
            self.start_symbol = symbol

    def remove_start_symbol(self):
        if len(self.start_symbol) > 0:
            self.start_symbol = ""
        else:
            print("Essa gramática não tem símbolo inicial no momento")   

    def print_grammar(self):

        print(self.name + ":")
        for key in self.productions:
            print(key + ' -> ', end='')
            first = True
            for production in self.productions[key]:
                if first:
                    print(production, end='')
                    first = False
                else:
                    print(' | ' + production, end='')
            print('') 

    #Exemplo: grammar.add_productions('S', ['&', 'aA', 'b', 'bB'], True)
    def add_productions(self, non_terminal, productions, is_starter=False):

        non_terminal = non_terminal.upper()

        if non_terminal not in self.productions:
            self.productions[non_terminal] = []

        if is_starter:
            self.set_start_symbol(non_terminal)

        for production in productions:
            if not production:
                continue
            if non_terminal not in self.start_symbol and production == '&':
                print(f"A produção vazia não é permitida em um símbolo que não é o inicial em uma gramática regular")
                continue
            if production in self.productions[non_terminal]:
                print(f"A produção {production} já existe na gramática {self.name}")
                continue
            if production[0].isupper():
                print(f"A produção {production} não pode ser adicionada pois não começa com um terminal.")
                continue
            if len(production) == 1:
                self.productions[non_terminal].append(production)
                continue

            if production[1].islower() or len(production) > 2:
                print(f"A produção {production} não pode ser adicionada pois gera mais de um não terminal e/ou tem comprimento maior que 2 caracteres")
                continue
            self.productions[non_terminal].append(production)

    def remove_production(self, non_terminal, productions):
        non_terminal = non_terminal.upper()
        for production in productions:
            if production in productions[non_terminal]:
                self.productions[non_terminal].remove(production)

    # Retorna o autômato finito criado a partir dessa gramática
    def to_FA(self):

        fa = automata.Automata(self.name + "_to_FA")
        symbol_list = list(self.productions.keys())
        if type(symbol_list) == type(None):
            symbol_list = []

        # Adiciona estado final arbitrariamente chamado de QF
        fa.add_state('QF')
        fa.set_end_state('QF')

        for symbol in symbol_list:

            fa.add_state(symbol)
            if symbol in self.start_symbol:
                fa.set_start(symbol)  

            for production in self.productions[symbol]:
                if len(production) == 1:
                    fa.add_transition(symbol, [(production[0], ['QF'])])
                else:
                    fa.add_transition(symbol, [(production[0], [production[1]])])

        return fa            


class CFG:
# Gramaticas livre de Contexto

    def __init__(self, name):
        self.name = name
        self.productions = {}
        self.start_symbol = ""
    
    def set_start_symbol(self, symbol):
        symbol = symbol.upper()
        self.start_symbol = symbol
    
    def remove_start_symbol(self):
        if len(start_symbol) > 0:
            self.start_symbol = ""
        else:
            print("Essa gramática não tem símbolo inicial no momento")
    
    def print_grammar(self):
        for key in self.productions:
            print(key + ' -> ', end='')
            first = True
            for production in self.productions[key]:
                if first:
                    print(production, end='')
                    first = False
                else:
                    print(' | ' + production, end='')
            print('')


    def add_productions(self, non_terminal, productions, is_starter=False):

        non_terminal = non_terminal.upper()

        if non_terminal in self.productions:
            print(f"Essa produção já foi incluida")
            return
        else:
            self.productions[non_terminal] = []

        if is_starter:
            set_start_symbol(non_terminal)

        for production in productions:
            if not production:
                continue
            if non_terminal not in self.start_symbol and production == '&':
                print(f"A produção vazia não é permitida em um símbolo que não é o inicial em GLC")
                continue
            if production in self.productions[non_terminal]:
                print(f"A produção {production} já existe na gramática {self.name}")
                continue
            if len(production) == 1:
                self.productions[non_terminal].append(production)
                continue

            self.productions[non_terminal].append(production)

    def remove_production(self, non_terminal, productions):
        non_terminal = non_terminal.upper()
        for production in productions:
            if production in productions[non_terminal]:
                self.productions[non_terminal].remove(production)

    # Questão I
    # Primeiro é necessário remover inalcansáveis, improdutivos,
    # epsilons e produções unitárias

    # Algoritmo para remoção de inalcansáveis:
    # cada vez que um não terminal é produzido, duas
    # produções são marcadas. Produções não marcadas são eliminadas.
    def remove_unreachable(self, grammar):
        reached = []
        to_check = []
        checked = []

        if grammar.start_symbol == "":
            print("Essa gramática não possui símbolo inicial!")
            return None

        to_check.append(grammar.start_symbol)
        checked.append(grammar.start_symbol)

        while len(to_check) > 0:
            checking = to_check.pop(0)
            if checking in grammar.productions:
                for symbols in grammar.productions[checking]:
                    for symbol in symbols:
                        if symbol.isupper() and symbol not in checked:
                            checked.append(symbol)
                            to_check.append(symbol)
                reached.append(checking)
        
        new_productions = {k: grammar.productions[k] for k in reached}
         
        grammar.productions = new_productions
        
        return grammar
    
    # Algoritmo para remoção de improdutivos:
    # Esse algoritmo funciona de "maneira inversa" ao de remoção de
    # não alcansáveis, pois parte das produções que são estados de
    # aceitação e vai marcando recursivamente os estados, até terminar
    # o processo, quando estados não marcados são eliminados.
    def remove_unproductive(self, grammar):
        verified = []
        repeat = True
        while repeat:
            flag = False
            for key in grammar.productions:
                if key not in verified:
                    for symbols in grammar.productions[key]:
                        verify = True
                        for symbol in symbols:
                            if symbol.isupper() and symbol not in verified:
                                verify = False
                        if verify and key not in verified:
                            verified.append(key)
                            flag = True
            if not flag:
                repeat = False
        
        new_productions = {k: grammar.productions[k] for k in verified}
        grammar.productions = new_productions

        
        return grammar

    # Algoritmo de remoção de epsilons:
    # A ideia desse algoritmo é substituir o símbolo epsilon (&)
    # por produções equivalentes trazendo as produções do estado
    # que escapava por & para as produções que levavame a esse estado.
    def remove_epsilon(self, grammar):
        removed = []
        repeat = True

        while repeat:
            for key in grammar.productions:
                if '&' in grammar.productions[key] and key not in removed:
                    # primeiro remove & da producao
                    temp = []
                    for symbols in grammar.productions[key]:
                        if symbols != '&':
                            temp.append(symbols)
                    
                    removed.append(key)
                    
                    # recursivamente tirar a key das outras produções
                    # e criar as novas produções
                    for verifying_key in grammar.productions:
                        new_productions = []
                        for symbols in grammar.productions[verifying_key]:
                            if key in symbols:
                                print(symbols)
                                return
            repeat = False

        return grammar

    # Algoritmo de remoção de produção unitária:
    # Esse algoritmo é bastante simples, apenas passando as produções
    # de A para B no caso de B -> A
    def remove_unitary(self, grammar):
        verified = []

        for key in grammar.productions:
            for symbols in grammar.productions[key]:
                if len(symbols) == 1 and symbols.isupper():
                    add = []
                    for pr in grammar.productions[key]:
                        if pr != symbols:
                            add.append(pr)
                    grammar.productions[key] = add + grammar.productions[symbols]
        
        return grammar

                    
    
    # Para o algoritmo da forma normal de chomsky, primeiro a
    # gramática é submetida aos quatro algoritmos descritos
    # anteriormente, para então ao final possuir apenas produções do
    # tipo A -> a ou A -> BC
        
    def to_chomsky_normal_form(self):
        grammar = copy.copy(self)
        grammar = self.remove_unproductive(grammar)
        grammar = self.remove_unreachable(grammar)
        grammar = self.remove_unitary(grammar)
        grammar = self.remove_epsilon(grammar)
        
        temp_grammar = {}

        for production in grammar.productions:
            counter = 1
            new_productions = []
            for symbol in grammar.productions[production]:
                
                if (len(symbol) == 1 and symbol.islower) or (len(symbol) == 2 and symbol.isupper()):
                    new_productions.append(symbol)
                else:
                    if len(symbol) == 2:
                        if symbol[0].islower() and symbol[1].isupper:
                            new_productions.append(production + str(counter) + symbol[1])
                            temp_grammar[production + str(counter)] = [symbol[0]]
                            counter = counter + 1
                        elif symbol[0].isupper() and symbol[1].islower:
                            new_productions.append(symbol[0] + production + str(counter))
                            temp_grammar[production + str(counter)] = [symbol[1]]
                            counter = counter + 1
                        elif symbol.islower():
                            new_productions.append(production + str(counter) + production + str(counter + 1))
                            temp_grammar[production + str(counter)] = [symbol[0]]
                            temp_grammar[production + str(counter + 1)] = [symbol[1]]
                            counter = counter + 2
                            
                    elif len(symbol) == 3:
                        if symbol[0].isupper() and symbol[1].isupper():
                            if symbol[2].islower():
                                new_productions.append(production + str(counter) + production + str(counter + 1))
                                temp_grammar[production + str(counter)] = [production + str(counter + 2) + symbol[1]]
                                temp_grammar[production + str(counter + 1)] = [symbol[2]]
                                temp_grammar[production + str(counter + 2)] = [symbol[0]]
                                counter = counter + 3
                            if symbol[2].isupper():
                                new_productions.append(production + str(counter) + symbol[2])
                                temp_grammar[production + str(counter)] = [production + str(counter + 1) + symbol[1]]
                                temp_grammar[production + str(counter + 1)] = [symbol[0]]
                                counter = counter + 2
                        elif symbol[0].islower() and symbol[1].isupper() and symbol[2].islower():
                            new_productions.append(production + str(counter) + production + str(counter + 1))
                            temp_grammar[production + str(counter)] = [production + str(counter + 2) + symbol[1]]
                            temp_grammar[production + str(counter + 1)] = [symbol[2]]
                            temp_grammar[production + str(counter + 2)] = [symbol[0]]
                            counter = counter + 3

            temp_grammar[production] = new_productions
        
        grammar.productions = temp_grammar
        
        return grammar
    
    # Questão J

    # Essa função executa o algoritmo apresentado na disciplina
    # para a remoção de não determinismo direto e indireto.
    # Portanto, a cada iteração as produções anteriores são avaliadas
    # para remover o não determinismo indireto, e no fim é removido
    # seus não determismos diretos.
    def remove_left_recursion(self):

        # Primeiro passo: numerar as producoes.
        # Faremos isso criando uma lista das chaves (Nao terminais)
        keys = []
        for key in self.productions:
            keys.append(key)

        # Para cada i, pucha as produções de j, caso tenha Ai -> Aj (producao a esquerda)
        # Ao fim de cada i, remove recursão direta

        temp_productions = {}
        temp_productions[keys[0]] = self.productions[keys[0]]

        for i in range(0, len(keys)):
            new_productions = []
            for j in range(0, i):
                if i != j:
                    # verificar se tem Ai -> Aj (i.e. A -> B sendo B o inspecionado por j)
                    for symbols in self.productions[keys[i]]:
                        if symbols[0] == keys[j]:
                            # adiciona producoes de j em i
                            for production in self.productions[keys[j]]:
                                new_productions.append(production + symbols[1:])
                        else:
                            new_productions.append(symbols)
            
            # só modifica se a lista não for vazia (evita bug da primeira produção)
            if new_productions:
                temp_productions[keys[i]] = new_productions
            
            # remover recursão direta
            b_new_productions = []
            a_new_productions = []
            old_productions = []
            betas = []
            alfas = []
            others = []
            list_of_productions = temp_productions[keys[i]]
            for a in range(0, len(list_of_productions)):
                production = list_of_productions[a]
                if production.islower():
                    betas.append(production)
                elif production[0].isupper() and production[1:].islower() and production[0] == keys[i]:
                    alfas.append(production[1:])
                else:
                    others.append(production)

            for b in betas:
                b_new_productions.append(b + keys[i] + str(1))
            
            if alfas:
                for a in alfas:
                    a_new_productions.append(a + keys[i] + str(1))
                a_new_productions.append('&')

            for o in others:
                old_productions.append(o)
            
            final_productions = b_new_productions + old_productions
            if final_productions:
                temp_productions[keys[i]] = final_productions
            if a_new_productions:
                temp_productions[keys[i] + str(1)] = a_new_productions

        self.productions = temp_productions
    
    # Questão K
    
    # Para remover os não determinismos foi criado um novo dicionário
    # onde são adicionados ou as produções que já eram válidas
    # ou novas produções a partir dos prefixos iguais. No final,
    # essas novas produções são atribuidas ao dicionário atual
    # da linguagem.
    def remove_non_determinism(self):
        
        temp_productions = {}
        
        for key in self.productions:
            productions_list = self.productions[key]
            string = productions_list[0]
            non_determinisms = []
            non_determinism_counter = 0
            for i in range(0, len(productions_list)):
                flag = False
                control = True
                counter = 0
                while control:
                    try:
                        if productions_list[i][counter] == string[counter] and string[counter].islower():
                            counter = counter + 1
                            flag = True
                        else:
                            control = False
                    except:
                        control = False
                if flag:
                    non_determinisms.append(productions_list[i])
                if counter < non_determinism_counter and counter > 0 or non_determinism_counter == 0:
                    non_determinism_counter = counter

            # Em non_determinsms temos as palavras que possuem não determinismo,
            # e em non_determinisms_counter o número de caracteres que causam o não determinismo
            if non_determinisms and len(non_determinisms) > 1:
                new_productions = []
                for production in self.productions[key]:
                    if production not in non_determinisms:
                        new_productions.append(production)

            
                new_productions.append(non_determinisms[0][0:non_determinism_counter] + key + str(1))
                
                determined = []
                for symbols in non_determinisms:
                    new = symbols[non_determinism_counter:]
                    if len(new) == 0:
                        new = '&'
                    if new not in determined:
                        determined.append(new)
                
                temp_productions[key + str(1)] = determined
                temp_productions[key] = new_productions
            else:
                temp_productions[key] = self.productions[key]

        self.productions = temp_productions


    # Questão L

    # Código não utlizado até a linha 600

    # def add_dot(self, string):
    #     return '*' + string


    # Primeiro é preciso criar os itens LR(0)
    # def create_lr0_items(self):
    #     first = self.start_symbol
    #     to_compute = []
    #     computed = []
    #     initial = True
    #     items = []
    #     items.append(first)
    #     counter = 0
    #     item = []
    #     visited = []
    #     visited.append(first)
    #     starting = True
    #     while items != []:
    #         to_compute.append(items.pop())
    #         item.append([])
    #         item[counter] = []
    #         add = {}
    #         while to_compute != []:
    #             current = to_compute.pop()

    #             productions = self.productions[current]
    #             if initial:
    #                 new_productions = []
    #                 for production in productions:
    #                     if production != '':
    #                         new_productions.append(self.add_dot(production))
    #                 self.productions[current] = new_productions
    #                 initial = False
    #             productions = self.productions[current]
                
    #             for production in productions:
    #                 for i in range(0, len(production)):
    #                     if production[i] == '*' and i < len(production) - 1:
    #                         if production[i + 1] not in computed and production[i + 1].isupper():
    #                             to_compute.append(production[i + 1])
    #                             initial = True
                
    #             if current not in computed:
    #                 computed.append(current)

    #             if (current, productions) not in item[counter]:
    #                 item[counter].append((current, productions))
            
    #         for tuples in item[counter]:
    #             for p in tuples[1]:
    #                 for i in range(len(p) - 1):
    #                     if p[i] == '*' and p[i+1].isupper() and p[i+1] not in visited:
    #                         items.append(p[i+1])
    #                         visited.append(p[i+1])

    #         counter = counter + 1
        
    #     for i in range(len(item)):
    #         print('I' + str(i) + ':')
    #         print(item[i])

    # def closure(self, I):
    #     r = re.compile('.*[*][A-Z]{1}.*')
    #     J = I
    #     added_to_J = True

    #     productions = []

    #     # mover mais tarde
    #     productions.append(('S1', ['*' + self.start_symbol]))
    
    #     for key in self.productions:
    #         for p in self.productions[key]:
    #             productions.append((key, p))

    #     while added_to_J:
    #         new_added = False
    #         add_to_J = []
    #         for j in J:
    #             left = j[0]
    #             print('legt: ' + left)
    #             print(productions + J)
    #             for p in productions + J:
    #                 print('p: ', p[0])
    #                 print('left: ' + left)
    #                 print('---------')

    #                 if p[0] == left and r.match(p[1][0]):
    #                     print('OLA')
    #                     first = True
    #                     B = ''
    #                     for char in p[1]:
    #                         if char.isupper() and first:
    #                             B = char
    #                             first = False
                        
    #                     for p in productions:
    #                         if p[0] == B:

    #                             if (p[0], ['*' + p[1]]) not in J:
    #                                 add_to_J.append((p[0], ['*' + p[1]]))
    #                                 new_added = True
                                    
    #                                 # to_append = []
    #                                 # to_delete = []
    #                                 # for i in range(len(productions)):
                                        
    #                                 #     if productions[i] == (p[0], p[1]):
    #                                 #         to_delete.append(productions[i])
    #                                 #         if (p[0], '*' + p[1]) not in to_append:
    #                                 #             to_append.append((p[0], '*' + p[1]))
                                    
    #                                 # productions = [x for x in productions if x not in to_delete]
    #                                 # productions = productions + to_append
    #         J = J + add_to_J
    #         print(J)
    #         if new_added:
    #             added_to_J = True
    #         else:
    #             added_to_J = False
        
    #     print(J)

    # def create_slr_table():
    #     items = create_lr0_items()

     # calculates all symbols firsts of the grammar

     # Cálculo dos FIRST utilizando como base
     # o algoritmo dos slides (numerado)


    # Algoritmo de criação de FIRST. O FIRST é representado
    # por um dicionário, cujo as chaves são terminais e não terminais
    # e os valores seus respectivos conjuntos FIRST
    # As marcações de regras são as mesmas dos slides disponibilizados
    def first(self):
        terminals = []
        nonterminals = []
        for key in self.productions:
            if key not in terminals:
                terminals.append(key)
            for production in self.productions[key]:
                for char in production:
                    if char.islower() and char not in nonterminals:
                        nonterminals.append(char)
        first = {}
        nonterminals.append('&')

        # 1
        for n in nonterminals:
            first[n] = [n]
        
        for t in terminals:
            for production in self.productions[t]:
                # 2
                if production[0].islower() or production[0] == '&':
                    if t in first:
                        first[t] = first[t] + [production[0]]
                    else:
                        first[t] = [production[0]]
                else:
                    # 3
                    for i in range(len(production)):
                        if production[i] in first:
                            if t in first:
                                first[t] = first[t] + first[production[i]]
                            else:
                                first[t] = first[production[i]]
                            if '&' in first[t]:
                                pass
                            else:
                                break
                        else:
                            break 
        
        first[self.start_symbol] = []
        for pr in self.productions[self.start_symbol]:
            if pr[0].islower():
                first[self.start_symbol] = first[self.start_symbol] + [pr[0]]
        return first
    
    # Algoritmo de criação de follow. O follow é representado
    # por um dicionário, cujo as chaves são terminais e não terminais
    # e os valores seus respectivos conjuntos follow
    # As marcações de regras são as mesmas dos slides disponibilizados
    def follow(self):
        follow = {}
        # 1

        first = self.first()

        terminals = []
        nonterminals = []
        for key in self.productions:
            if key not in nonterminals:
                nonterminals.append(key)
            for production in self.productions[key]:
                for char in production:
                    if char.islower() and char not in terminals:
                        terminals.append(char)

        terminals.append('&')
        
        for n in nonterminals:
            follow[n] = []
        

        follow[self.start_symbol] = ['$']
        
        for key in self.productions:
            for production in self.productions[key]:

                # 2
                for i in range(len(production)):
                    if production[i].isupper():
                        string = production[i+1:]
                        apply = production[i]
                        for i in range(len(string)):
                            # if(string[i].isupper()):
                                for add in first[string[i]]:
                                    if add not in follow[apply] and add != '&':
                                        follow[apply] = follow[apply] + [add]
                        
                
                # 3
                if production[-1:].isupper():
                    add = production[-1:]
                    for a in follow[key]:
                        print(a)
                        if a not in follow[add]:
                            follow[add] = follow[add] + [a]
        print(follow)
        return follow
        
    
    # Algoritmo de criação de tabela. A função retorna uma lista
    # de listas, com tamanho não terminais + 1 por terminais + 1.
    # A primeira linha e a primeira coluna contém os terminais e os
    # não terminais, respectivamente, e o restante das cédulas os valores.
    # Os erros são denotados por /, e a célula 0,0 contém o valor '@'.
    def create_table(self):

        first = self.first()
        follow = self.follow()
         
        terminals = []
        nonterminals = []
        for key in self.productions:
            if key not in nonterminals:
                nonterminals.append(key)
            for production in self.productions[key]:
                for char in production:
                    if char.islower() and char not in terminals:
                        terminals.append(char)
        table = [['/' for i in range(len(terminals) + 1)] for i in range(len(nonterminals) + 1)]
        table[0][0] = '@'
        for i in range(1, len(nonterminals) + 1):
            table[i][0] = nonterminals[i - 1]
            
        for i in range(1, len(terminals) + 1):
            table[0][i] = terminals[i - 1]
        
        productions = []
        for key in self.productions:
            for production in self.productions[key]:
                productions.append((key, production))
        
        print('Lista de produções:')
        for i in range(0, len(productions)):
            print(str(i) + ': ' + productions[i][0] + ' -> ' + productions[i][1])

        for i in range(0, len(productions)):
            indice_A = 0
            for j in range(0, len(nonterminals)):
                if nonterminals[j] == productions[i][0]:
                    indice_A = j + 1
                    break
            
            # Regra 1 da criação da tabela
            first_a = []
            for production in productions[i]:
                for char in production:
                    first_a = first_a + first[char]
            for a in first_a:
                if a.islower():
                    indice_a = 0
                    for j in range(0, len(terminals)):
                        if terminals[j] == a:
                            indice_a = j + 1
                    table[indice_A][indice_a] = str(i)
            
            # Regra 2 da criação da tabela
            if productions[i][1] == '&':
                follow_a = []
                for production in productions[i]:
                    for char in production:
                        if char in follow:
                            follow_a = follow_a + follow[char]
                for a in follow_a:
                    if a.islower():
                        indice_a = 0
                        for j in range(0, len(terminals)):
                            if terminals[j] == a:
                                indice_a = j + 1
                        table[indice_A][indice_a] = str(i)

                    
        
        self.print_table(table, len(nonterminals) + 1, len(terminals) + 1)
        
    def print_table(self, table, n, m):
        print('Tabela:')
        for i in range(n):
            for j in range(m):
                print(table[i][j] + '   ', end='')
            print('')

        
    
