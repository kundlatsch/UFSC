from .grammar import *
from .automata import *
from utils import serializer
from utils import file_loader

class GrammarTerminal:

    temp_grammar = {}
    terminal_text = ''
    active_grammar = None

    def create_grammar(self, name):
        if name in self.temp_grammar:
            print('Erro: já existe uma gramática com esse nome')
        else:
            new_grammar = RegGrammar(name)
            self.temp_grammar[name] = new_grammar
            print('Gramática criada')

    def delete_grammar(self, name):
        if name in self.temp_grammar:
            del temp_grammar[name]
        else:
            print('Erro: não há gramática com esse nome')

    def activate_grammar(self, name):
        if any(name in key for key in self.temp_grammar):
            self.active_grammar = self.temp_grammar[name]
            self.terminal_text = 'grammar/' + name + ' -> '
        else:
            print('Erro: não há gramática com esse nome')
    
    def save_grammar(self, name):
        if self.active_grammar:
            serializer.save(self.active_grammar)
        else:
            print('Erro: não há gramática ativa')

    def print_help(self):
        print('Acesse o README.PDF para ter detalhes sobre os comandos')

    FUNCTIONS = {
        'act'       : activate_grammar,
        'create'    : create_grammar,
        'delete'    : delete_grammar,
        'save'      : save_grammar,
    }


    # Transformar em classe, usar __call__
    def __init__(self):
        self.temp_grammar = serializer.load_all('reggrammar')
        self.terminal_text = 'grammar -> '
        self.active_grammar = None
        operation = ''
        while operation != 'q':
            operation = input(self.terminal_text)
            splited_operation = operation.split()
            command = splited_operation[0]
            if len(splited_operation) == 1:
                flag = False
                if self.active_grammar:

                    if command == 'remove_start_symbol':
                        self.active_grammar.remove_start_symbol()
                    
                    elif command == 'to_fa':
                        automata = self.active_grammar.to_fa()
                        serializer.save(automata)
                        print('Autômato criado e salvo!')
                    
                    flag = True
                
                if command == 'help':
                    print_help()
            
                elif command == 'q':
                    print('...')

                elif command == 'des':
                    self.active_grammar = None
                    self.terminal_text = 'grammar -> '
                
                elif command == 'save':
                    self.save_grammar(self.active_grammar.name)

                
                elif command == 'print':
                    self.active_grammar.print_grammar()
                
                
                        

                else:
                    if not flag:
                        print("1 - Comando inválido. Digite \'help\' para obter ajuda")

            else:
                argument = splited_operation[1]
                args = splited_operation
                if command in self.FUNCTIONS:
                    self.FUNCTIONS[command](self, argument)
                
                elif command == 'act-file':
                    _grammar = file_loader.load(argument, 'grammar')
                    if _grammar:
                        self.activate_grammar = _grammar
                        self.terminal_text = 'grammar/' + argument + ' -> '
                
                elif self.active_grammar:
                    
                    if command == 'set_start_symbol':
                        self.active_grammar.set_start_symbol(argument)
                    
                    elif command == 'add_production':
                        args[2].replace(' ','')
                        args2 = args[2].split(',')
                        self.active_grammar.add_productions(argument, args2)
                    
                    elif command == 'remove_productions':
                        args[2].replace(' ','')
                        args2 = args[2].split(',')
                        self.active_grammar.remove_production(argument, args2)
                    
                else:
                    print("2 - Comando inválido. Digite \'help\' para obter ajuda")
