"""Main script file"""
import argparse
import linecache
import json
import sys
from dataclasses import asdict

from loguru import logger

from compiler.lexer.CC20202_lexer import CC20202Lexer
from compiler.exceptions import (InvalidTokenError,
                                 SyntaticError,
                                 BreakWithoutLoopError,
                                 VariableAlreadyDeclaredInScopeError,
                                 InvalidTypeOperationError,
                                 VariableNotDeclared)
from compiler.symbol_table import generate_symbol_table
from compiler.parser.CC20202_parser import parser
from compiler.semantic.CC20202_semantic import parse
from compiler.gci.CC20202_gci import gen_code


def main(filepath: str):
    """Main function"""
    with open(filepath) as f:
        source_code = f.read()

    tokens = []
    lexer = CC20202Lexer()
    lexer.build()
    lexer.input(source_code)
    while True:
        try:
            token = lexer.token()
        except InvalidTokenError as exp:
            logger.error(exp)
            exit(1)

        if not token:
            break
        else:
            tokens.append(token)

    logger.info('Total tokens: %s' % len(tokens))

    logger.info('Running parser for list of tokens...')
    success, fail_token = parser.parse(tokens=tokens)

    if not success:
        line = linecache.getline(filepath, fail_token.lineno)
        logger.info('Invalid sintax at line %s:\n\t%s' %
                    (fail_token.lineno, line.strip()))
        logger.info('Token: %s' % fail_token)
        logger.error('Syntatic error detected!')

        sys.exit(1)

    else:
        logger.info('Syntatic analysis completed with success!')

    logger.info('Running semantic analyser...')

    try:
        semantic_analyser_result = parse(source_code)

        symbol_tables = semantic_analyser_result['scopes']
        num_expressions = semantic_analyser_result['num_expressions']

        logger.info("All break statments are inside loops")

    except BreakWithoutLoopError as exp:
        line = linecache.getline(filepath, int(str(exp)))
        logger.info('Invalid break usage at line %s:\n\t%s' %
                    (exp, line.strip()))
        logger.error('Semantic error detected!')

        sys.exit(1)

    except VariableAlreadyDeclaredInScopeError as exp:
        lineno = int(str(exp))
        line = linecache.getline(filepath, lineno)
        logger.info(
            'Variable already declared! First declaration at line %s' % int(str(exp)))
        logger.info(line)
        logger.error('Semantic error detected!')

        sys.exit(1)

    except InvalidTypeOperationError as exp:
        left, right, lineno = str(exp).split(',')
        line = linecache.getline(filepath, int(lineno))
        logger.info(
            'Invalid operation between %s and %s at line %s' % (left, right, lineno))
        logger.info(line)
        logger.error('Semantic error detected!')

        sys.exit(1)

    except VariableNotDeclared as exp:
        ident, lineno = str(exp).split(',')
        line = linecache.getline(filepath, int(lineno))
        logger.info('Variable %s used before declare at line %s' %
                    (ident, lineno))
        logger.info(line)
        logger.error('Semantic error detected!')

        sys.exit(1)

    logger.info('Semantic analyser run successfuly')
    symbol_table_file = 'symbol_tables.json'
    logger.info('Exporting symbol tables to %s' % symbol_table_file)

    with open(symbol_table_file, 'w') as f:
        json.dump(symbol_tables, f, indent=2, sort_keys=False)

    expressions_file = 'expressions.json'
    logger.info('Exporting symbol tables to %s' % expressions_file)

    with open(expressions_file, 'w') as f:
        json.dump(num_expressions, f, indent=2, sort_keys=False)

    logger.info('Running intermediary code generation...')
    code = gen_code(source_code)
    intermediary_code_file = 'intermediary_code.gic'
    logger.info('Exportind intermediary code to %s' % intermediary_code_file)

    with open(intermediary_code_file, 'w') as f:
        f.write(code)


if __name__ == '__main__':
    arg_parser = argparse.ArgumentParser(
        description='Auxiliar script to execute compiler')
    arg_parser.add_argument('filepath',
                            help='Target source code file')

    args = arg_parser.parse_args()
    main(args.filepath)
