from regular import (automata_terminal, regex_terminal, CFG_terminal, grammar_terminal)

MODES = {
    'automata'  : automata_terminal.AutomataTerminal,
    'cfg'   : CFG_terminal.CFGTerminal,
    'regex'     : regex_terminal.run,
    'grammar' : grammar_terminal.GrammarTerminal
}

def run():
    command = ''
    while command != "q":
        command = input('-> ')
        compute(command)

def print_help():
    print('O programa funciona em modos, referentes a cada tipo de formalização.')
    print('Digite o nome do módulo para escolhê-lo.')
    print('Opções disponíveis: automata, cfg e regex.')
    print('Digite \'q\' para sair.')

def compute(command : str):
    if command == 'help':
        print_help()
    
    elif command == 'q':
        print('...')
    
    elif command in MODES:
        terminal = MODES[command]()

    else:
        print('Comando inválido. Digite \'help\' para obter ajuda')
