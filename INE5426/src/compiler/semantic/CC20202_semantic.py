"""Semantic analyser"""
from typing import List, Optional, Dict, Tuple
from dataclasses import dataclass

from ply import yacc

from compiler.lexer.CC20202_lexer import CC20202Lexer
from utils.data_structures import Scope, ScopeStack, TableEntry, Node
from compiler.exceptions import (BreakWithoutLoopError,
                                 InvalidTypeOperationError,
                                 VariableNotDeclared)

# Necessary for yacc instatiation
lexer = CC20202Lexer()
lexer.build()
tokens = lexer.tokens

# Used for controlling scopes
scope_stack = ScopeStack()

num_expressions: List[Tuple[Node, int]] = []


def get_var_type(ident, lineno):
    scope = scope_stack.seek()
    while True:
        for entry in scope.table:
            if entry.identifier_label == ident:
                return entry.datatype

        scope = scope.upper_scope

        if scope is None:
            break

    raise VariableNotDeclared(f'{ident},{lineno}')


def num_expressions_as_json() -> List[Dict]:
    output = []

    for exp, line in num_expressions:
        if exp.left == None and exp.right == None:
            continue

        output.append({
            'ID': str(exp),
            'lineno': line,
            'tree': exp.as_json()
        })

    return output


def new_scope(is_loop: bool):
    """Create a new scope on the scope stack"""
    top_scope = scope_stack.seek()
    new_scope = Scope(upper_scope=top_scope, is_loop=is_loop)
    if top_scope:
        top_scope.add_inner(new_scope)
    scope_stack.push(new_scope)


def check_type(left: Node, right: Node, operation: str, lineno: int) -> str:
    valids = {
        ('string', '+', 'string'): 'string',
        ('int', '+', 'int'): 'int',
        ('int', '-', 'int'): 'int',
        ('int', '*', 'int'): 'int',
        ('int', '%', 'int'): 'int',
        ('int', '/', 'int'): 'float',
        ('float', '+', 'float'): 'float',
        ('float', '-', 'float'): 'float',
        ('float', '*', 'float'): 'float',
        ('float', '/', 'float'): 'float',
        ('float', '+', 'int'): 'float',
        ('float', '-', 'int'): 'float',
        ('float', '*', 'int'): 'float',
        ('float', '/', 'int'): 'float',
        ('int', '+', 'float'): 'float',
        ('int', '-', 'float'): 'float',
        ('int', '*', 'float'): 'float',
        ('int', '/', 'float'): 'float',
    }

    result = valids.get(
        (left.result_type, operation, right.result_type), None)

    if result is None:
        raise InvalidTypeOperationError(
            f'{left.result_type},{right.result_type},{lineno}')

    return result


def p_error(p):
    print('ERROR!')


def p_empty(p: yacc.YaccProduction):
    """empty :"""
    pass


def p_new_scope(p: yacc.YaccProduction):
    """new_scope :"""
    new_scope(is_loop=False)


def p_new_loop_scope(p: yacc.YaccProduction):
    """new_loop_scope :"""
    new_scope(is_loop=True)


def p_prog_statment(p: yacc.YaccProduction):
    """PROGRAM : new_scope STATEMENT
               | new_scope FUNCLIST
               | empty
    """
    global_scope = scope_stack.pop()
    p[0] = {
        'scopes': global_scope.as_json(),
        'num_expressions': num_expressions_as_json()
    }

    # Grants that all all tables where used and popper correctly
    assert len(scope_stack) == 0  # nosec


def p_funclist_funcdef(p: yacc.YaccProduction):
    """FUNCLIST : FUNCDEF FUNCLISTAUX"""
    pass


def p_funclistaux_funclist(p: yacc.YaccProduction):
    """FUNCLISTAUX : FUNCLIST
                   | empty
    """
    pass


def p_funcdef(p: yacc.YaccProduction):
    """FUNCDEF : DEF IDENT new_scope LPAREN PARAMLIST RPAREN LBRACKETS STATELIST RBRACKETS"""
    # Go back to upper scope
    scope_stack.pop()

    # Add function declaration to current scope
    scope = scope_stack.seek()
    entry = TableEntry(p[2], 'function', [], p.lineno(2))
    scope.add_entry(entry)


def p_paralist_param(p: yacc.YaccProduction):
    """PARAMLIST : DATATYPE IDENT PARAMLISTAUX
                 | empty
    """
    if len(p) > 2:
        scope = scope_stack.seek()
        entry = TableEntry(p[2], p[1], [], p.lineno(2))
        scope.add_entry(entry)


def p_paramlistaux_paramlist(p: yacc.YaccProduction):
    """PARAMLISTAUX : COMMA PARAMLIST
                    | empty
    """
    pass


def p_datatype(p: yacc.YaccProduction):
    """DATATYPE : INT_KEYWORD
                | FLOAT_KEYWORD
                | STRING_KEYWORD
    """
    p[0] = p[1]


def p_statement_vardecl(p: yacc.YaccProduction):
    """STATEMENT : VARDECL SEMICOLON"""
    pass


def p_statement_atrib(p: yacc.YaccProduction):
    """STATEMENT : ATRIBSTAT SEMICOLON"""
    pass


def p_statement_print(p: yacc.YaccProduction):
    """STATEMENT : PRINTSTAT SEMICOLON"""
    pass


def p_statement_read(p: yacc.YaccProduction):
    """STATEMENT : READSTAT SEMICOLON"""
    pass


def p_statement_return(p: yacc.YaccProduction):
    """STATEMENT : RETURNSTAT SEMICOLON"""
    pass


def p_statement_if(p: yacc.YaccProduction):
    """STATEMENT : IFSTAT"""
    pass


def p_statement_for(p: yacc.YaccProduction):
    """STATEMENT : FORSTAT"""
    pass


def p_statement_statelist(p: yacc.YaccProduction):
    """STATEMENT : new_scope LBRACKETS STATELIST RBRACKETS """
    # Return to previous scope
    scope_stack.pop()


def p_statement_break(p: yacc.YaccProduction):
    """STATEMENT : BREAK SEMICOLON"""
    # If is not inside loop scope, consider semantic failure
    current_scope = scope_stack.seek()

    # Go into upper scopes trying to find a for loop
    while True:
        if current_scope.is_loop:
            break

        current_scope = current_scope.upper_scope

        if current_scope is None:
            raise BreakWithoutLoopError(p.lineno(2))


def p_statement_end(p: yacc.YaccProduction):
    """STATEMENT : SEMICOLON"""
    pass


def p_vardecl(p: yacc.YaccProduction):
    """VARDECL : DATATYPE IDENT OPT_VECTOR"""
    entry = TableEntry(p[2], p[1], p[3], p.lineno(2))
    scope = scope_stack.seek()
    scope.add_entry(entry)


def p_opt_vector(p: yacc.YaccProduction):
    """OPT_VECTOR : LSQBRACKETS INT_CONSTANT RSQBRACKETS OPT_VECTOR
                  | empty
    """
    if len(p) > 2:
        p[0] = [p[2], *p[4]]
    else:
        p[0] = []


def p_atribstat(p: yacc.YaccProduction):
    """ATRIBSTAT : LVALUE ATTRIBUTION ATRIB_RIGHT"""
    pass


def p_atribright_func_or_exp(p: yacc.YaccProduction):
    """ATRIB_RIGHT : FUNCCALL_OR_EXPRESSION"""
    pass


def p_atribright_alloc(p: yacc.YaccProduction):
    """ATRIB_RIGHT : ALLOCEXPRESSION"""
    pass


def p_funccall_or_exp_plus(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : PLUS FACTOR REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR
                              | MINUS FACTOR REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    right_node = p[2]['node']
    if p[1] == '-':
        right_node.value *= -1

    if p[3]:
        result_type = check_type(p[3]['node'],
                                 right_node,
                                 p[3]['operation'],
                                 p.lineno(1))
        right_node = Node(p[3]['node'],
                          right_node,
                          p[3]['operation'],
                          result_type)

    if p[4]:
        result_type = check_type(p[4]['node'],
                                 right_node,
                                 p[4]['operation'],
                                 p.lineno(1))
        right_node = Node(p[4]['node'],
                          right_node,
                          p[4]['operation'],
                          result_type)

    num_expressions.append(right_node)


def p_funccal_or_exp_int_const(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : INT_CONSTANT REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    node = Node(None, None, p[1], 'int')

    if p[2]:
        result_type = check_type(node,
                                 p[2]['node'],
                                 p[2]['operation'],
                                 p.lineno(2))
        node = Node(node, p[2]['node'], p[2]['operation'], result_type)

    if p[3]:
        result_type = check_type(node,
                                 p[3]['node'],
                                 p[3]['operation'],
                                 p.lineno(2))
        node = Node(node, p[3]['node'], p[3]['operation'], result_type)

    p[0] = {
        'node': node
    }

    num_expressions.append((node, p.lineno(2)))


def p_funccal_or_exp_float_const(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : FLOAT_CONSTANT REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    node = Node(None, None, p[1], 'float')

    if p[2]:
        result_type = check_type(node,
                                 p[2]['node'],
                                 p[2]['operation'],
                                 p.lineno(2))
        node = Node(node, p[2]['node'], p[2]['operation'], result_type)

    if p[3]:
        result_type = check_type(node,
                                 p[3]['node'],
                                 p[3]['operation'],
                                 p.lineno(2))
        node = Node(node, p[3]['node'], p[3]['operation'], result_type)

    p[0] = {
        'node': node
    }

    num_expressions.append((node, p.lineno(2)))


def p_funccal_or_exp_string_const(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : STRING_CONSTANT REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    node = Node(None, None, p[1], 'string')

    if p[2]:
        result_type = check_type(node,
                                 p[2]['node'],
                                 p[2]['operation'],
                                 p.lineno(1))
        node = Node(node, p[2]['node'], p[2]['operation'], result_type)

    if p[3]:
        result_type = check_type(node,
                                 p[3]['node'],
                                 p[3]['operation'],
                                 p.lineno(1))
        node = Node(node, p[3]['node'], p[3]['operation'], result_type)

    p[0] = {
        'node': node
    }

    num_expressions.append((node, p.lineno(1)))


def p_funccall_or_exp_null(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : NULL REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    pass


def p_funccall_or_exp_parentesis(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : LPAREN NUMEXPRESSION RPAREN REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    node = p[2]['node']

    if p[4]:
        result_type = check_type(node,
                                 p[4]['node'],
                                 p[4]['operation'],
                                 p.lineno(1))
        node = Node(node, p[4]['node'], p[4]['operation'], result_type)

    if p[5]:
        result_type = check_type(node,
                                 p[5]['node'],
                                 p[5]['operation'],
                                 p.lineno(1))
        node = Node(node, p[5]['node'], p[5]['operation'], result_type)

    p[0] = {
        'node': node
    }

    num_expressions.append((node, p.lineno(1)))


def p_funccall_or_exp_ident(p: yacc.YaccProduction):
    """FUNCCALL_OR_EXPRESSION : IDENT FOLLOW_IDENT"""
    node = Node(None, None, p[1], get_var_type(p[1], p.lineno(1)))

    if p[2] is None or p[2]['node'] == None:
        return

    if p[2]:
        node.value += p[2]['vec_access']
        result_type = check_type(node,
                                 p[2]['node'],
                                 p[2]['operation'],
                                 p.lineno(1))
        node = Node(node, p[2]['node'], p[2]['operation'], result_type)

        num_expressions.append((node, p.lineno(1)))


def p_follow_ident_alloc(p: yacc.YaccProduction):
    """FOLLOW_IDENT : OPT_ALLOC_NUMEXP REC_UNARYEXPR REC_PLUS_MINUS_TERM OPT_REL_OP_NUM_EXPR"""
    node = None
    operation = ''

    if p[2]:
        node = p[2]['node']
        operation = p[2]['operation']

    if p[3]:
        if node is None:
            node = p[3]['node']
            operation = p[3]['operation']

        else:
            result_type = check_type(node,
                                     p[3]['node'],
                                     p[3]['operation'],
                                     p.lineno(0))
            node = Node(node, p[3]['node'], p[3]['operation'], result_type)

    p[0] = {
        'vec_access': p[1],
        'node': node,
        'operation': operation
    }


def p_follow_ident_parentesis(p: yacc.YaccProduction):
    """FOLLOW_IDENT : LPAREN PARAMLISTCALL RPAREN """
    pass


def p_paramlistcall_ident(p: yacc.YaccProduction):
    """PARAMLISTCALL : IDENT PARAMLISTCALLAUX
                     | empty
    """
    pass


def p_paramlistcallaux(p: yacc.YaccProduction):
    """PARAMLISTCALLAUX : COMMA PARAMLISTCALL
                        | empty
    """
    pass


def p_printstat(p: yacc.YaccProduction):
    """PRINTSTAT : PRINT EXPRESSION"""
    pass


def p_readstat(p: yacc.YaccProduction):
    """READSTAT : READ LVALUE"""
    pass


def p_returnstat(p: yacc.YaccProduction):
    """RETURNSTAT : RETURN"""
    pass


def p_ifstat(p: yacc.YaccProduction):
    """IFSTAT : IF LPAREN EXPRESSION RPAREN new_scope LBRACKETS STATELIST RBRACKETS OPT_ELSE"""
    # Go back to previous scope
    scope_stack.pop()


def p_opt_else(p: yacc.YaccProduction):
    """OPT_ELSE : ELSE new_scope LBRACKETS STATELIST RBRACKETS
                | empty
    """
    if len(p) > 2:
        # Go back to previous scope
        scope_stack.pop()


def p_forstat(p: yacc.YaccProduction):
    """FORSTAT : FOR LPAREN ATRIBSTAT SEMICOLON EXPRESSION SEMICOLON ATRIBSTAT RPAREN new_loop_scope LBRACKETS STATELIST RBRACKETS"""
    scope_stack.pop()


def p_statelist(p: yacc.YaccProduction):
    """STATELIST : STATEMENT OPT_STATELIST"""
    pass


def p_opt_statelist(p: yacc.YaccProduction):
    """OPT_STATELIST : STATELIST
                     | empty
    """
    pass


def p_allocexp(p: yacc.YaccProduction):
    """ALLOCEXPRESSION : NEW DATATYPE LSQBRACKETS NUMEXPRESSION RSQBRACKETS OPT_ALLOC_NUMEXP"""
    num_expressions.append((p[4]['node'], p.lineno(1)))


def p_opt_allocexp(p: yacc.YaccProduction):
    """OPT_ALLOC_NUMEXP : LSQBRACKETS NUMEXPRESSION RSQBRACKETS OPT_ALLOC_NUMEXP
                        | empty
    """
    if len(p) < 3:
        p[0] = ''
    else:
        p[0] = '[' + str(p[2]) + ']' + p[4]

        num_expressions.append((p[2]['node'], p.lineno(1)))


def p_expression(p: yacc.YaccProduction):
    """EXPRESSION : NUMEXPRESSION OPT_REL_OP_NUM_EXPR"""
    num_expressions.append((p[1]['node'], p.lineno(1)))


def p_opt_rel_op_num_expr(p: yacc.YaccProduction):
    """OPT_REL_OP_NUM_EXPR : REL_OP NUMEXPRESSION
                           | empty
    """
    if len(p) < 3:
        pass

    else:
        num_expressions.append((p[2]['node'], p.lineno(1)))


def p_relop_lt(p: yacc.YaccProduction):
    """REL_OP : LOWER_THAN"""
    pass


def p_relop_gt(p: yacc.YaccProduction):
    """REL_OP : GREATER_THAN"""
    pass


def p_relop_lte(p: yacc.YaccProduction):
    """REL_OP : LOWER_OR_EQUALS_THAN"""
    pass


def p_relop_gte(p: yacc.YaccProduction):
    """REL_OP : GREATER_OR_EQUALS_THAN"""


def p_relop_eq(p: yacc.YaccProduction):
    """REL_OP : EQ_COMPARISON"""
    pass


def p_relop_neq(p: yacc.YaccProduction):
    """REL_OP : NEQ_COMPARISON"""
    pass


def p_numexp(p: yacc.YaccProduction):
    """NUMEXPRESSION : TERM REC_PLUS_MINUS_TERM"""
    if p[2] is None:
        p[0] = p[1]

    else:
        result_type = check_type(p[1]['node'],
                                 p[2]['node'],
                                 p[2]['operation'],
                                 p.lineno(1))
        p[0] = {
            'node': Node(p[1]['node'],
                         p[2]['node'],
                         p[2]['operation'],
                         result_type)
        }


def p_rec_plus_minus(p: yacc.YaccProduction):
    """REC_PLUS_MINUS_TERM : PLUS_OR_MINUS TERM REC_PLUS_MINUS_TERM
                           | empty
    """
    if len(p) < 3:
        # Case empty
        p[0] = None

    elif p[3]:
        # Case there's another recursive operation being made
        result_type = check_type(p[2]['node'],
                                 p[3]['node'],
                                 p[3]['operation'],
                                 p.lineno(1))
        p[0] = {
            'node': Node(p[2]['node'], p[3]['node'],
                         p[3]['operation'], result_type),
            'operation': p[1]['operation']
        }
    else:
        # Case there's no more operattions to the right
        p[0] = {
            'node': p[2]['node'],
            'operation': p[1]['operation']
        }


def p_plus(p: yacc.YaccProduction):
    """PLUS_OR_MINUS : PLUS
                     | MINUS"""
    p[0] = {'operation': p[1]}


def p_term_unary_exp(p: yacc.YaccProduction):
    """TERM : UNARYEXPR REC_UNARYEXPR"""
    if p[2]:
        # If there's another operation being made
        result_type = check_type(p[1]['node'],
                                 p[2]['node'],
                                 p[2]['operation'],
                                 p.lineno(1))
        p[0] = {
            'node': Node(p[1]['node'], p[2]['node'], p[2]['operation'], result_type),
            'operation': p[2]['operation']
        }

    else:
        # Pass the UNARYEXPR node upwards
        p[0] = {
            'node': p[1]['node']
        }


def p_rec_unaryexp_op(p: yacc.YaccProduction):
    """REC_UNARYEXPR : UNARYEXPR_OP TERM
                     | empty
    """
    if len(p) < 3:
        # Case empty
        p[0] = None

    else:
        p[0] = {
            'node': p[2]['node'],
            'operation': p[1]['operation']
        }


def p_rec_unaryexp_times(p: yacc.YaccProduction):
    """UNARYEXPR_OP : TIMES
                    | MODULE
                    | DIVIDE """
    p[0] = {'operation': p[1]}


def p_rec_unaryexp_plusminus(p: yacc.YaccProduction):
    """UNARYEXPR : PLUS_OR_MINUS FACTOR"""
    if p[1]['operation'] == '-':
        p[2]['node'].value *= -1

    p[0] = p[2]


def p_rec_unaryexp_factor(p: yacc.YaccProduction):
    """UNARYEXPR : FACTOR"""
    p[0] = p[1]


def p_factor_int_cte(p: yacc.YaccProduction):
    """FACTOR : INT_CONSTANT"""
    p[0] = {'node': Node(None, None, p[1], 'int')}


def p_factor_float_cte(p: yacc.YaccProduction):
    """FACTOR : FLOAT_CONSTANT"""
    p[0] = {'node': Node(None, None, p[1], 'float')}


def p_factor_string_cte(p: yacc.YaccProduction):
    """FACTOR : STRING_CONSTANT"""
    p[0] = {'node': Node(None, None, p[1], 'string')}


def p_factor_null(p: yacc.YaccProduction):
    """FACTOR : NULL"""
    p[0] = {'node': Node(None, None, None, 'null')}


def p_factor_lvalue(p: yacc.YaccProduction):
    """FACTOR : LVALUE"""
    p[0] = p[1]


def p_factor_expr(p: yacc.YaccProduction):
    """FACTOR : LPAREN NUMEXPRESSION RPAREN"""
    p[0] = p[2]

    num_expressions.append((p[2]['node'], p.lineno(1)))


def p_lvalue_ident(p: yacc.YaccProduction):
    """LVALUE : IDENT OPT_ALLOC_NUMEXP"""
    p[0] = {
        'node': Node(None, None, p[1] + p[2],
                     result_type=get_var_type(p[1], p.lineno(1)))
    }


_parser = yacc.yacc(start='PROGRAM', check_recursion=False)


def parse(text: str):
    return _parser.parse(text, lexer=lexer)
