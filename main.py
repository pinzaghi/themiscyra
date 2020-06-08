import sys
import argparse
import os.path 
from z3 import *
from pycparser import c_parser, c_ast, parse_file, c_generator

import syntaxlib
import semanticlib
import athos

def prepare_for_pycparser(filename):

    with open(filename) as f:
        original_file_str = f.read()

    result_str = syntaxlib.remove_c99_comments(original_file_str)
    
    return result_str

def check_positive(value):
    ivalue = int(value)
    if ivalue < 1:
      raise argparse.ArgumentTypeError("%s is an invalid positive int value" % value)
    return ivalue

if __name__ == "__main__":
    # Parse the program arguments
    argparser = argparse.ArgumentParser('Event driven async to sync')
    argparser.add_argument('filename', help='name of file to parse')
    argparser.add_argument('unfolds', help='number of unfolds to perform', type=check_positive)
    args = argparser.parse_args()

    # Pycparser doesn't support directives, we replace them for its content
    input_str_pycparser = prepare_for_pycparser(args.filename)

    # Now we can use pycparser to obtain the AST
    parser = c_parser.CParser()

    ast = parser.parse(input_str_pycparser)
    syntaxlib.unfold(ast, args.unfolds)

    # Dead code elimination
    semanticlib.dead_code_elimination(ast)

    compho = athos.async_to_sync(ast, 'vround', ['STARTVIEWCHANGE', 'DOVIEWCHANGE', 'STARTVIEW'])

    # Generate the C99 code
    generator = c_generator.CGenerator()
    final_code = generator.visit(ast)
    
    #print(final_code)
    
    # frama-c-gui -val -main func examples/roundAtoB.unfold1.c


