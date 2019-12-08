from .automata import Automata
from utils import serializer

class AutomataTerminal:

    temp_automata = {}
    terminal_text = ''

    def create_automata(self, name):
        if name in self.temp_automata:
            print('Erro: já existe um autômato com esse nome')
        else:
            new_automata = Automata(name)
            self.temp_automata[name] = new_automata
            print('Autômato criado')

    def delete_automata(self, name):
        if name in self.temp_automata:
            del self.temp_automata[name]
        else:
            print('Erro: não há autômato com esse nome')

    def activate_automata(self, name):
        if name in self.temp_automata:
            self.loaded_automata = self.temp_automata[name]
            self.terminal_text = 'automata/' + name + ' -> '
        else:
            print('Erro: não há autômato com esse nome')
    
    def save_automata(self, name):
        if self.loaded_automata:
            serializer.save(self.activate_automata)
        else:
            print('Erro: não há automato ativo')

    def print_help(self):
        print('Acesse o README.PDF para ter detalhes sobre os comandos')

    FUNCTIONS = {
        'act'       : activate_automata,
        'create'    : create_automata,
        'delete'    : delete_automata,
        'save'      : save_automata,
    }


    # Transformar em classe, usar __call__
    def __init__(self):
        self.temp_automata = serializer.load_all('automata')
        self.terminal_text = 'automata -> '
        self.activate_automata = None
        operation = ''

        while operation != 'q':
            operation = input(self.terminal_text)
            splited_operation = operation.split()
            command = splited_operation[0]
            
            # COMANDOS SEM ARGUMENTO
            if len(splited_operation) == 1:
                flag = False
                if self.loaded_automata:
                    if command == 'rm_start':
                        self.activate_automata.remove_start()
                    
                    elif command == 'to_dfa':
                        self.loaded_automata = self.loaded_automata.to_DFA()
                        self.loaded_automata.name = self.loaded_automata.name + '_dfa'
                        self.terminal_text = 'automata/' + self.loaded_automata.name + ' -> '
                    
                    elif command == 'to_minimal':
                        self.loaded_automata = self.loaded_automata.to_minimal()
                        self.loaded_automata.name = self.loaded_automata.name + '_mini'
                        self.terminal_text = 'automata/' + self.loaded_automata.name + ' -> '
                    
                    elif command == 'to_RG':
                        grammar = self.loaded_automata.to_RG()
                        serializer.save(grammar)
                        print('Expressão regular criada e salva!')

                    flag = True
                
                if command == 'help':
                    self.print_help()
                    
            
                elif command == 'q':
                    print('...')

                elif command == 'des':
                    self.loaded_automata = None
                    self.terminal_text = 'automata -> '
                
                elif command == 'print':
                    self.loaded_automata.print_automata()
                
                        

                else:
                    if not flag:
                        print("1 - Comando inválido. Digite \'help\' para obter ajuda")
            
            # COMANDOS COM ARGUMENTO
            else:
                argument = splited_operation[1]
                args = splited_operation
                if command in self.FUNCTIONS:
                    self.FUNCTIONS[command](self, argument)
                
                # TODO args2 virar uma lista de tuplas ou lista de listas
                elif self.activate_automata:
                    if command == 'add_state':
                        args[2].replace(' ','')
                        args2 = args[2].split(',')
                        self.activate_automata.add_state(args[1], args2)
                    
                    elif command == 'rm_state':
                        self.activate_automata.remove_state(args[1])
                    
                    # TODO args2 virar uma lista de tuplas ou lista de listas
                    # args[1] = state_name (string) args[2] = a,[A,B,C];b,B;
                    elif command == 'add_transition':
                        args[2].replace(' ','')
                        args2 = args[2].split(',')
                        self.activate_automata.add_transition(args[1], args2)
                    
                    elif command == 'rm_transition':
                        args[2].replace(' ','')
                        args2 = args[2].split(',')
                        self.activate_automata.remove_transition(args[1], args2)
                    
                    elif command == 'set_start':
                        self.activate_automata.set_start(args[1])
                    
                    elif command == 'set_end':
                        self.activate_automata.set_end_state(args[1])
                    
                    elif command == 'rm_end':
                        self.activate_automata.remove_end_state(args[1])
                    
                    elif command == 'compute':
                        self.activate_automata.compute(args[1])
                    
                else:
                    print("2 - Comando inválido. Digite \'help\' para obter ajuda")
