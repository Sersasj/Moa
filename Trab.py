# -*- coding: utf-8 -*-
# Sergio Alvarez
# Guilherme Ferrari
from sys import stdin

import sys
import math
import time
end = 0
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



# Cria lista com os Vertices
def populate(text):
    vertex_list = []
    for element in text:
        element = element.split()
        element = element[1:]  
        vertex_list.append(Vertice(int(element[0]), int(element[1])))
    return vertex_list

# Inicializa vertices como não visitado
# Nao sei se pode iniciar com um visitado/ nao visitado na classe
# coloquei aqui como : [[Vertice,visitado],...]            
def init_false(vertex_list):
    list_aux = []
    for vertice in vertex_list:
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
def nearest(vertex_list):
    path = [] 
    path_w = []
    
    visited_list = init_false(vertex_list)
    # Inicia no primeiro vertice
    visited_list[0][1] = True
    path = [0]
    for i in range(len(vertex_list)-1):
        near, w = find_nearest(visited_list, path[i])
        path.append(near)
        path_w.append(w)
        visited_list[near][1] = True
    #Volta pro inicio
    path_w.append(dist(vertex_list[path[0]].x, vertex_list[path[0]].y, vertex_list[path[len(path)-1]].x, vertex_list[path[len(path)-1]].y))
    path.append(path[0])

    return path

def cost(path, vertex_list):
  path_w = 0
  for i in range(len(path)-1):    
    path_w += dist(vertex_list[path[i]].x,vertex_list[path[i]].y,
                   vertex_list[path[i+1]].x,vertex_list[path[i+1]].y)
  return path_w

def two_opt(path,vertex_list):
  start = time.time()
  best = path.copy()
  improved = True
  while improved:
    improved = False
    for i in range(1, len(path)-2):
      for j in range(i+1, len(path)):
        if j-i == 1:
          continue
        new_path = path[:]
  
        new_path[i:j] = path[j-1:i-1:-1]
        if(cost(new_path, vertex_list) < cost(path, vertex_list)):
          end = time.time()
          #print(end - start)
          #print("antigo ", cost(path, vertex_list), "novo ",cost(new_path, vertex_list))
          best = new_path
          if (end - start > 5):
            return best
          improved = True
          path = best.copy()
  return best  


def main():
    start = time.time()

    content = read_input()
    vertex_list = populate(content)
    
    path = nearest(vertex_list)
    path2 = two_opt(path, vertex_list)
    end = time.time()
    #print(end - start)
    print("aa", cost(path, vertex_list))
  
    print("bb", cost(path2, vertex_list))
if __name__ == '__main__':
   main()