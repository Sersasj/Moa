# -*- coding: utf-8 -*-
from sys import stdin

import sys
import math
class Vertice:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        

def read_input():
  input_str = []
  for line in stdin:
    input_str.append(line.replace("\n",""))    
    if(line.replace("\n","") == "EOF"):
      break
  return input_str[6:len(input_str)-1]

"""
def read_input():
  with open(sys.argv[1], 'r') as f:
    for i, line in enumerate(f):
      if i == 5:          
        content = f.read()
        break;
        
  return content[:len(content)-4]
"""

# Cria lista com os Vertices
def populate(text):
    list_vertice = []
    for element in text:
        element = element.split(" ")
        element = element[1:]  
        list_vertice.append(Vertice(int(element[0]), int(element[1])))
    return list_vertice

# Inicializa vertices como não visitado
# Nao sei se pode iniciar com um visitado/ nao visitado na classe
# coloquei aqui como : [[Vertice,visitado],...]            
def init_false(list_vertice):
    list_aux = []
    for vertice in list_vertice:
        list_aux.append([vertice,False])
    return list_aux    

def dist(x1,y1,x2,y2):
  resp = math.dist([x1,y1],[x2,y2])
  return resp
  
#ACHEI MUITO ESQUISITO ISSO AQUIM DEVE TER COMO FAZER MELHOR
# percorre os não visitados, acha o menor
def find_nearest(visited_list, vertice_index):
    # acha o primeiro não visitado e assume como o menor
    count = 0
    while visited_list[count][1] == True:
        count+=1
    min_index = count
    min_value = math.dist([visited_list[vertice_index][0].x, visited_list[vertice_index][0].y], [visited_list[min_index][0].x, visited_list[min_index][0].y])

  
    count2 = 0

    # percorre outros vertices procurando um menor
    for vertice in visited_list:
        if (vertice[1] == False and math.dist([visited_list[vertice_index][0].x, visited_list[vertice_index][0].y], [vertice[0].x,vertice[0].y]) < min_value):
            min_value = math.dist([visited_list[vertice_index][0].x, visited_list[vertice_index][0].y], [vertice[0].x, vertice[0].y])
            min_index = count2 
        count2+=1 
  
    return min_index, min_value

# Metodo construtivo 
# Neareast Neighbor
def nearest(list_vertice):
    path = [] 
    path_w = []
    
    visited_list = init_false(list_vertice)
    # Inicia no primeiro vertice
    visited_list[0][1] = True
    path = [0]
    for i in range(len(list_vertice)-1):
        near, w = find_nearest(visited_list, path[i])
        path.append(near)
        path_w.append(w)
        visited_list[near][1] = True
    #Volta pro inicio
    path_w.append(dist(list_vertice[path[0]].x, list_vertice[path[0]].y, list_vertice[path[len(path)-1]].x, list_vertice[path[len(path)-1]].y))
    path.append(path[0])

    print(sum(path_w))
    

def main():
    content = read_input()
    list_vertice = populate(content)
    nearest(list_vertice)
if __name__ == '__main__':
   main()