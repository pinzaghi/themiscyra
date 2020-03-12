import sys

sys.path.insert(0, '..')

from pycparser import c_parser, c_ast, parse_file, c_generator
from visitors import *
from smt import *


#############

src = "int main(void){if(round==0 && ballot==1){}}"
parser = c_parser.CParser()
ast = parser.parse(src)

ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == True


src = "int main(void){if(0==round && 1==ballot){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == True

src = "int main(void){if(0==round){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == True

src = "int main(void){if(1==round){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == False


src = "int main(void){if(round==0 && (other==20 && other2==10)){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == True

src = "int main(void){if((round==0 && other==10) && (other==20 && ballot==1)){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == True

src = "int main(void){if(((round==0 && test==3) && other==10) && (other==20 && (ballot==1 && test2==10))){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == True

src = "int main(void){if(((round==0 && test==3) && other==10) && (other==20 && (ballot==0 && test2==10))){}}"
ast = parser.parse(src)
ast_pred = ast.ext[0].body.block_items[0].cond

assert is_current_round(ast_pred, context={'ballot': 1, 'round': 0}) == False