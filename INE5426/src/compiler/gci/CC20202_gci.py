"""Semantic analyser"""
from typing import List, Optional, Dict
from dataclasses import dataclass

from ply import yacc

from compiler.lexer.CC20202_lexer import CC20202Lexer
from utils.data_structures import Scope, ScopeStack, TableEntry

# Necessary for yacc instatiation
lexer = CC20202Lexer()
lexer.build()
tokens = lexer.tokens

DEALLOCATED_TEMP_VARS: List[str] = []
ALLOCATED_TEMP_VARS: List[str] = []
LABEL_COUNT: int = 0


class ForLoopNextLabelControl():
    def __init__(self):
        self.label = ''

    def set_for_loop_next_label(self, label: str):
        self.label = label

    def get_for_loop_next_label(self) -> str:
        return self.label


for_loop_control = ForLoopNextLabelControl()


def get_temp_var() -> str:
    """Create or return a deallocated temporary variable"""
    if len(DEALLOCATED_TEMP_VARS) > 1:
        var = DEALLOCATED_TEMP_VARS.pop()
        ALLOCATED_TEMP_VARS.append(var)

        return var

    # Create a t<0~> variable name to be used
    else:
        i = 0
        while True:
            var = f't{i}'
            if var not in ALLOCATED_TEMP_VARS:
                ALLOCATED_TEMP_VARS.append(var)
                return var

            i += 1


def free_temp_var(var: str):
    ALLOCATED_TEMP_VARS.remove(var)
    DEALLOCATED_TEMP_VARS.append(var)


def new_label() -> str:
    global LABEL_COUNT
    lbl = f'LABEL{LABEL_COUNT}'
    LABEL_COUNT += 1

    return lbl


def p_empty(p: yacc.YaccProduction):
    """empty :"""
    pass


def p_new_for_loop_label(p: yacc.YaccProduction):
    """new_for_loop_label :"""
    for_loop_control.set_for_loop_next_label(new_label())


def p_prog_statment(p: yacc.YaccProduction):
    """PROGRAM : STATEMENT
               | FUNCLIST
               | empty
    """
    p[0] = p[1]['code']


def p_funclist_funcdef(p: yacc.YaccProduction):
    """FUNCLIST : FUNCDEF FUNCLISTAUX"""
    p[0] = {
        'code': p[1]['code'] + p[2]['code']
    }


def p_funclistaux_funclist(p: yacc.YaccProduction):
    """FUNCLISTAUX : FUNCLIST
                   | empty
    """
    if p[1] is None:
        p[0] = {'code': ''}
    else:
        p[0] = {'code': p[1]['code']}


def p_funcdef(p: yacc.YaccProduction):
    """FUNCDEF : DEF IDENT LPAREN PARAMLIST RPAREN LBRACKETS STATELIST RBRACKETS"""
    next_label = new_label()
    if len(p) < 3:
        p[0] = {'code': ''}

    else:
        p[0] = {
            'code': f'goto {next_label}\n{p[2]}:\n{p[4]["code"]}{p[7]["code"]}\n{next_label}:\n'
        }


def p_paralist_param(p: yacc.YaccProduction):
    """PARAMLIST : DATATYPE IDENT PARAMLISTAUX
                 | empty
    """
    if len(p) < 3:
        p[0] = {'code': ''}

    else:
        p[0] = {
            'code': 'from_params ' + p[2] + '\n' + p[3]['code']
        }


def p_paramlistaux_paramlist(p: yacc.YaccProduction):
    """PARAMLISTAUX : COMMA PARAMLIST
                    | empty
    """
    if len(p) < 3:
        p[0] = {'code': ''}

    else:
        p[0] = {'code': p[2]['code']}


def p_datatype(p: yacc.YaccProduction):
    """DATATYPE : INT_KEYWORD
                | FLOAT_KEYWORD
                | STRING_KEYWORD
    """
    p[0] = {'code': p[1]}


def p_statement_vardecl(p: yacc.YaccProduction):
    """STATEMENT : VARDECL SEMICOLON"""
    p[0] = {'code': p[1]['code']}


def p_statement_atrib(p: yacc.YaccProduction):
    """STATEMENT : ATRIBSTAT SEMICOLON"""
    p[0] = {'code': p[1]['code']}


def p_statement_print(p: yacc.YaccProduction):
    """STATEMENT : PRINTSTAT SEMICOLON"""
    p[0] = {'code': p[1]['code']}


def p_statement_read(p: yacc.YaccProduction):
    """STATEMENT : READSTAT SEMICOLON"""
    p[0] = {'code': p[1]['code']}


def p_statement_return(p: yacc.YaccProduction):
    """STATEMENT : RETURNSTAT SEMICOLON"""
    p[0] = {'code': p[1]['code']}


def p_statement_if(p: yacc.YaccProduction):
    """STATEMENT : IFSTAT"""
    p[0] = {
        'code': p[1]['code']
    }


def p_statement_for(p: yacc.YaccProduction):
    """STATEMENT : FORSTAT"""
    p[0] = {
        'code': p[1]['code']
    }


def p_statement_statelist(p: yacc.YaccProduction):
    """STATEMENT : LBRACKETS STATELIST RBRACKETS """
    p[0] = {
        'code': p[2]['code']
    }


def p_statement_break(p: yacc.YaccProduction):
    """STATEMENT : BREAK SEMICOLON"""
    p[0] = {'code': f'goto {for_loop_control.get_for_loop_next_label()}\n'}


def p_statement_end(p: yacc.YaccProduction):
    """STATEMENT : SEMICOLON"""
    p[0] = {'code': ''}


def p_vardecl(p: yacc.YaccProduction):
    """VARDECL : DATATYPE IDENT OPT_VECTOR"""
    p[0] = {
        'code': f'{p[1]["code"]} {p[2]}{p[3]["code"]}\n'
    }


def p_opt_vector(p: yacc.YaccProduction):
    """OPT_VECTOR : LSQBRACKETS INT_CONSTANT RSQBRACKETS OPT_VECTOR
                  | empty
    """
    if len(p) < 3:
        p[0] = {'code': ''}
    else:
        p[0] = {
            'code': '[' + str(p[2]) + ']' + p[4]['code']
        }


def p_atribstat(p: yacc.YaccProduction):
    """ATRIBSTAT : LVALUE ATTRIBUTION ATRIB_RIGHT"""
    p[0] = {
        'code': p[3]['code'] + p[1]['code'] + f'{p[1]["var_name"]} = {p[3]["temp_var"]}\n'
    }


def p_atribright_func_or_exp(p: yacc.YaccProduction):
    """ATRIB_RIGHT : FUNCCALL_OR_EXPRESSION"""
    p[0] = p[1]


def p_atribright_alloc(p: yacc.YaccProduction):
    """ATRIB_RIGHT : ALLOCEXPRESSION"""
    p[0] = p[1]


def p_funccall_or_exp_minus(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : MINUS FACTOR REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR
                              | PLUS FACTOR REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    previous_code = ''
    curr_left_value = f'{p[1]} {p[2]["temp_var"]}'
    for i in range(3, 6):
        if p[i]['code']:
            temp_var = get_temp_var()
            previous_code += p[i]['code']
            previous_code += f'{temp_var} = {curr_left_value} {p[i]["operation"]} {p[i]["temp_var"]}\n'

            curr_left_value = temp_var

    p[0] = {
        'temp_var': curr_left_value,
        'code': previous_code
    }


def p_funccall_or_exp_string(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : INT_CONSTANT REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR
                              | STRING_CONSTANT REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR
                              | FLOAT_CONSTANT REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""

    curr_left_value = get_temp_var()
    previous_code = f'{curr_left_value} = {p[1]}\n'
    for i in range(2, 5):
        if p[i]['code']:
            temp_var = get_temp_var()
            previous_code += p[i]['code']
            previous_code += f'{temp_var} = {curr_left_value} {p[i]["operation"]} {p[i]["temp_var"]}\n'

            curr_left_value = temp_var

    p[0] = {
        'temp_var': curr_left_value,
        'code': previous_code
    }


def p_funccall_or_exp_null(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : NULL REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    previous_code = ''
    curr_left_value = p[1]
    for i in range(2, 5):
        if p[i]['code']:
            temp_var = get_temp_var()
            previous_code += p[i]['code']
            previous_code += f'{temp_var} = {curr_left_value} {p[i]["operation"]} {p[i]["temp_var"]}\n'

            curr_left_value = temp_var

    p[0] = {
        'temp_var': curr_left_value,
        'code': previous_code
    }


def p_funccall_or_exp_parentesis(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : LPAREN NUMEXPRESSION RPAREN REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    previous_code = p[2]['code']
    curr_left_value = p[2]['temp_var']
    for i in range(4, 7):
        if p[i]['code']:
            temp_var = get_temp_var()
            previous_code += p[i]['code']
            previous_code += f'{temp_var} = {curr_left_value} {p[i]["operation"]} {p[i]["temp_var"]}\n'

            curr_left_value = temp_var

    p[0] = {
        'temp_var': curr_left_value,
        'code': previous_code
    }


def p_funccall_or_exp_ident(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : IDENT FOLLOW_IDENT"""
    if p[2]['funcall']:
        p[0] = {
            'code': p[2]['code'],
            'temp_var': p[2]['temp_var'],
        }

    elif p[2].get('operation', False):
        temp_var = get_temp_var()
        op = p[2]['operation']
        right_temp = p[2]['temp_var']
        code = f'{temp_var} = {p[1]} {op} {right_temp}\n'
        p[0] = {
            'code': p[2]['code'] + code,
            'temp_var': temp_var
        }
    else:
        p[0] = {
            'code': '',
            'temp_var': p[1]
        }


def p_follow_ident_alloc(p: yacc.YaccProduction):
    """FOLLOW_IDENT : OPT_ALLOC_NUMEXP REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    # Assumes that only one of theese operations will succeed
    # could be wrong, but letting the expression be mounted here loog wrong too
    index_access = ''

    if p[1]['code']:
        index_access = p[1]['code']

    for i in range(2, 5):
        if p[i]['code']:
            p[0] = {
                'code': p[i]['code'],
                'temp_var': p[i]['temp_var'],
                'operation': p[i]['operation'],
                'index_access': index_access,
                'funcall': False
            }
            break

    else:
        # If the loop completes without finding anything...
        p[0] = {
            'code': '',
            'funcall': False
        }


def p_follow_ident_parentesis(p: yacc.YaccProduction):
    """FOLLOW_IDENT : LPAREN PARAMLISTCALL RPAREN"""
    func_ident = p[-1]
    num_params = p[2]['num_params']
    temp_var = get_temp_var()
    code = f'{temp_var} = call {func_ident}, {num_params}\n'
    p[0] = {
        'code': p[2]['code'] + code,
        'temp_var': temp_var,
        'funcall': True
    }


def p_paramlistcall_ident(p: yacc.YaccProduction):
    """PARAMLISTCALL : IDENT PARAMLISTCALLAUX
                     | empty
    """
    if len(p) < 3:
        p[0] = {'code': '', 'num_params': 0}
    else:
        p[0] = {
            'code': 'param ' + p[1] + '\n' + p[2]['code'],
            'num_params': p[2]['num_params']
        }


def p_paramlistcallaux(p: yacc.YaccProduction):
    """PARAMLISTCALLAUX : COMMA PARAMLISTCALL
                        | empty
    """
    if len(p) < 3:
        p[0] = {'code': '', 'num_params': 0}
    else:
        p[0] = {
            'code': p[2]['code'],
            'num_params': p[2]['num_params'] + 1
        }


def p_printstat(p: yacc.YaccProduction):
    """PRINTSTAT : PRINT EXPRESSION"""
    p[0] = {'code': p[2]['code'] + f'print {p[2]["temp_var"]}\n'}


def p_readstat(p: yacc.YaccProduction):
    """READSTAT : READ LVALUE"""
    p[0] = {'code': 'read ' + p[2]['var_name']}


def p_returnstat(p: yacc.YaccProduction):
    """RETURNSTAT : RETURN"""
    p[0] = {'code': f'{p[1]}\n'}


def p_ifstat(p: yacc.YaccProduction):
    """IFSTAT : IF LPAREN EXPRESSION RPAREN LBRACKETS STATELIST RBRACKETS OPT_ELSE"""
    cond_temp_var = p[3]['temp_var']
    next_label = new_label()

    else_start_label = p[8].get('start_label', None)
    cond_false_next_label = else_start_label if else_start_label else next_label

    jump_over_else = f'goto {next_label}\n' if else_start_label is not None else ''

    code = p[3]['code'] + f"if False {cond_temp_var} goto {cond_false_next_label}\n" + \
        p[6]['code'] + jump_over_else + p[8]['code'] + next_label + ':\n'

    p[0] = {
        'code': code
    }


def p_opt_else(p: yacc.YaccProduction):
    """OPT_ELSE : ELSE LBRACKETS STATELIST RBRACKETS
                | empty
    """
    if len(p) < 3:
        p[0] = {
            'code': "",
        }
    else:
        start_label = new_label()
        p[0] = {
            'code': start_label + ':\n' + p[3]['code'],
            'start_label': start_label
        }


def p_forstat(p: yacc.YaccProduction):
    """FORSTAT : new_for_loop_label FOR LPAREN ATRIBSTAT SEMICOLON EXPRESSION SEMICOLON ATRIBSTAT RPAREN LBRACKETS STATELIST RBRACKETS"""
    start_label = new_label()
    next_label = for_loop_control.get_for_loop_next_label()
    cond_code_body = p[6]['code']
    cond_temp_var = p[6]['temp_var']

    first_atrib_code = p[4]['code'] + '\n'
    cond_code = f'if False {cond_temp_var} goto {next_label}\n'
    body_code = p[11]['code']
    increment_code = p[8]['code']
    go_to_start_code = f'goto {start_label}\n'

    code = first_atrib_code +\
        start_label + ':\n' +\
        cond_code_body +\
        cond_code +\
        body_code +\
        increment_code +\
        go_to_start_code +\
        next_label + ':\n'

    p[0] = {
        'code': code
    }


def p_statelist(p: yacc.YaccProduction):
    """STATELIST : STATEMENT OPT_STATELIST"""
    p[0] = {
        'code': p[1]['code'] + p[2]['code']
    }


def p_opt_statelist(p: yacc.YaccProduction):
    """OPT_STATELIST : STATELIST
                     | empty
    """
    if p[1]:
        p[0] = {'code': p[1]['code']}
    else:
        p[0] = {'code': ''}


def p_allocexp(p: yacc.YaccProduction):
    """ALLOCEXPRESSION : NEW DATATYPE LSQBRACKETS NUMEXPRESSION RSQBRACKETS OPT_ALLOC_NUMEXP"""
    temp_var = get_temp_var()

    p[0] = {
        'temp_var': temp_var,
        'code': p[6]['code'] + f'{temp_var} = new {p[2]["code"]}{p[6]["alloc_brackets"]}\n',
    }


def p_opt_allocexp(p: yacc.YaccProduction):
    """OPT_ALLOC_NUMEXP : LSQBRACKETS NUMEXPRESSION RSQBRACKETS OPT_ALLOC_NUMEXP
                        | empty
    """
    if len(p) < 3:
        p[0] = {
            'code': '',
            'alloc_brackets': ''
        }
    else:
        p[0] = {
            'code': p[2]['code'] + p[4]['code'],
            'alloc_brackets': f'[{p[2]["temp_var"]}]' + p[4]['alloc_brackets']
        }


def p_expression(p: yacc.YaccProduction):
    """EXPRESSION : NUMEXPRESSION OPT_REL_OP_NUM_EXPR"""
    opt_code = p[2]['code']
    result_temp_var = get_temp_var()

    code = f'{result_temp_var} = {p[1]["temp_var"]}'

    if opt_code:
        operator = p[2]['operation']
        code += f' {operator} {p[2]["temp_var"]}\n'
    else:
        code += '\n'

    p[0] = {
        'temp_var': result_temp_var,
        'code': p[2]['code'] + p[1]['code'] + code
    }


def p_opt_rel_op_num_expr(p: yacc.YaccProduction):
    """OPT_REL_OP_NUM_EXPR : REL_OP NUMEXPRESSION
                           | empty
    """
    if len(p) < 3:
        p[0] = {'code': ''}

    else:
        p[0] = {
            'temp_var': p[2]['temp_var'],
            'operation': p[1]['code'],
            'code': p[2]['code'],
        }


def p_relop_lt(p: yacc.YaccProduction):
    """REL_OP : LOWER_THAN"""
    p[0] = {'code': '<'}


def p_relop_gt(p: yacc.YaccProduction):
    """REL_OP : GREATER_THAN"""
    p[0] = {'code': '>'}


def p_relop_lte(p: yacc.YaccProduction):
    """REL_OP : LOWER_OR_EQUALS_THAN"""
    p[0] = {'code': '<='}


def p_relop_gte(p: yacc.YaccProduction):
    """REL_OP : GREATER_OR_EQUALS_THAN"""
    p[0] = {'code': '>='}


def p_relop_eq(p: yacc.YaccProduction):
    """REL_OP : EQ_COMPARISON"""
    p[0] = {'code': '=='}


def p_relop_neq(p: yacc.YaccProduction):
    """REL_OP : NEQ_COMPARISON"""
    p[0] = {'code': '!='}


def p_numexp(p: yacc.YaccProduction):
    """NUMEXPRESSION : TERM REC_PLUS_MINUS_TERM"""
    if not p[2]['code']:
        p[0] = p[1]

    else:
        temp_var = get_temp_var()
        left_temp = p[1]['temp_var']
        op = p[2]['operation']
        right_temp = p[2]['temp_var']
        p[0] = {
            'code': p[2]['code'] + p[1]['code'] + f'{temp_var} = {left_temp} {op} {right_temp}\n',
            'temp_var': temp_var
        }


def p_rec_plus_minus(p: yacc.YaccProduction):
    """REC_PLUS_MINUS_TERM : PLUS_OR_MINUS TERM REC_PLUS_MINUS_TERM
                           | empty
    """
    if len(p) < 3:
        p[0] = {'code': ''}

    elif p[3]['code']:
        temp_var = get_temp_var()
        left_temp = p[2]['temp_var']
        op = p[3]['operation']
        right_temp = p[3]['temp_var']

        p[0] = {
            'code': p[3]['code'] + p[2]['code'] + f'{temp_var} = {left_temp} {op} {right_temp}\n',
            'temp_var': temp_var,
            'operation': p[1]['code']
        }

    else:
        p[0] = {
            'code': p[2]['code'],
            'temp_var': p[2]['temp_var'],
            'operation': p[1]['code']
        }


def p_operators(p: yacc.YaccProduction):
    """PLUS_OR_MINUS : PLUS
                     | MINUS
       UNARYEXPR_OP : TIMES
                    | MODULE
                    | DIVIDE """
    p[0] = {'code': p[1]}


def p_term_unary_exp(p: yacc.YaccProduction):
    """TERM : UNARYEXPR REC_UNARYEXPR"""
    if p[2]['code']:
        temp_var = get_temp_var()
        left_temp = p[1]['temp_var']
        op = p[2]['operation']
        right_temp = p[2]['temp_var']
        p[0] = {
            'code': p[1]['code'] + f'{temp_var} = {left_temp} {op} {right_temp}\n',
            'temp_var': temp_var
        }

    else:
        p[0] = p[1]


def p_rec_unaryexp_op(p: yacc.YaccProduction):
    """REC_UNARYEXPR : UNARYEXPR_OP TERM
                     | empty
    """
    if len(p) < 3:
        p[0] = {'code': ''}

    else:
        p[0] = {
            'code': p[2]['code'],
            'temp_var': p[2]['temp_var'],
            'operation': p[1]['code']
        }


def p_rec_unaryexp_plusminus(p: yacc.YaccProduction):
    """UNARYEXPR : PLUS_OR_MINUS FACTOR"""
    p[0] = {
        'code': p[2]['code'] + f'{p[1]["code"]}{p[2]["code"]}\n',
        'temp_var': p[2]['temp_var']
    }


def p_rec_unaryexp_factor(p: yacc.YaccProduction):
    """UNARYEXPR : FACTOR"""
    p[0] = p[1]


def p_factor_const(p: yacc.YaccProduction):
    """FACTOR : INT_CONSTANT
              | FLOAT_CONSTANT
              | STRING_CONSTANT
              | NULL"""
    temp_var = get_temp_var()
    p[0] = {
        'code': f'{temp_var} = {p[1]}\n',
        'temp_var': temp_var
    }


def p_factor_lvalue(p: yacc.YaccProduction):
    """FACTOR : LVALUE"""
    p[0] = {
        'code': f'{p[1]["temp_var"]} = {p[1]["var_name"]}\n',
        'temp_var': p[1]['temp_var']
    }


def p_factor_expr(p: yacc.YaccProduction):
    """FACTOR : LPAREN NUMEXPRESSION RPAREN"""
    p[0] = p[2]


def p_lvalue_ident(p: yacc.YaccProduction):
    """LVALUE : IDENT OPT_ALLOC_NUMEXP"""
    temp_var = get_temp_var()
    p[0] = {
        'code': p[2]['code'],
        'temp_var': temp_var,
        'var_name': f'{p[1]}{p[2]["alloc_brackets"]}'
    }


_parser = yacc.yacc(start='PROGRAM', check_recursion=False)


def gen_code(text: str):
    return _parser.parse(text, lexer=lexer)
