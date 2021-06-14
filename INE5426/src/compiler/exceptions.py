"""Shared exceptions"""


class InvalidTokenError(Exception):
    """Lexical analyser fails to parse source code"""


class SyntaticError(Exception):
    """Invalid production to be applied on parsing"""


class BreakWithoutLoopError(Exception):
    """Semantic error when a break is written without a loop scope"""


class InvalidTypeOperationError(Exception):
    """Semantic error when two variables are invalid operated"""


class VariableAlreadyDeclaredInScopeError(Exception):
    """Semantic error when a variable is declared twice inside the same scope"""


class VariableNotDeclared(Exception):
    """Semantic error when a variable was used before declared"""
