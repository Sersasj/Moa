#include <string>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <limits>
#include <stdio.h>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Sergio Alvarez
// Guilherme Ferrari
class Vertex {
    public:
    float x, y; 
    bool visited;

    // construtor vazio
    Vertex() {}

    // construtor com parametros
    Vertex( float x_value, float y_value, bool visited_value){
        x = x_value;
        y = y_value;
        visited = visited_value;
                
    }
};


void read_input(vector<Vertex> &vertex_list){
    string input;
    vector <string> input_list;
    int pos, x, y;
    // joga 6 linhas fora
    for(int i = 0; i < 6; i ++){
        getline(cin, input);
    }    
    // adiciona na classe "Vertex"
    while (scanf("%d %d %d", &pos, &x, &y)){
        vertex_list.push_back(Vertex(x, y, false));
    }  

}

float distance(int x1, int y1, int x2, int y2){    
    float pow1 = pow(x2 - x1, 2);
    float pow2 = pow(y2 - y1, 2);
    return sqrt(pow1 + pow2);
}

int find_nearest(vector<Vertex> vertex_list, int path_index){
    int min_index = 0;
    float min_value = numeric_limits<float>::infinity();

    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        if(vertex_list[i].visited == false && distance(vertex_list[path_index].x, vertex_list[path_index].y, vertex_list[i].x, vertex_list[i].y) < min_value){
            min_value = distance(vertex_list[path_index].x, vertex_list[path_index].y, vertex_list[i].x, vertex_list[i].y);
            min_index = i;            
        }
    }
    return min_index;
}
float find_nearest_w(vector<Vertex> vertex_list, int path_index){
    float min_value = numeric_limits<float>::infinity();

    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        if(vertex_list[i].visited == false && distance(vertex_list[path_index].x, vertex_list[path_index].y, vertex_list[i].x, vertex_list[i].y) < min_value){
              min_value = distance(vertex_list[path_index].x, vertex_list[path_index].y, vertex_list[i].x, vertex_list[i].y);
        }
    }
    return min_value;
}

void nearest_neighbor(vector<Vertex> &vertex_list, vector<int>& path){
    path.push_back(0);
    vertex_list[0].visited = true;
    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        int nearest_index = find_nearest(vertex_list, path[i]);
        path.push_back(nearest_index);
        vertex_list[nearest_index].visited = true;
    }
}




// calcula o custo do caminho
float cost(vector<int> path, vector<Vertex> vertex_list){
    float path_w = 0;
    for(long unsigned int i = 0; i < path.size()-1; i++){
        path_w += distance(vertex_list[path[i]].x, vertex_list[path[i]].y,
            vertex_list[path[i+1]].x, vertex_list[path[i+1]].y);
    }
    return path_w;
}

vector<int> slice(vector<int> &arr, int x, int y){
  auto start = arr.begin() + x;
  auto end = arr.begin() + y + 1;
  vector<int> result(y-x + 1);
  copy(start, end, result.begin());
  return result;
}

float cost2(vector<int> path, float **matrix){
  float path_w = 0;
  for (long unsigned int i = 0; i < path.size()-1; i++){
    path_w += matrix[path[i]][path[i+1]];
  }
  return path_w;
}

float cost3(vector<int> path, vector<Vertex> vertex_list, unsigned long int i, unsigned long int j)
{
  if (i-1 < 0)
    i = 0;
  if (j+1 > vertex_list.size())
    j = vertex_list.size() - 1;
  float path_i_antigo = distance(vertex_list[path[i - 1]].x, vertex_list[path[i - 1]].y, vertex_list[path[i]].x, vertex_list[path[i]].y);
  float path_j_antigo = distance(vertex_list[path[j]].x, vertex_list[path[j]].y, vertex_list[path[j + 1]].x, vertex_list[path[j + 1]].y);
  float path_i_novo = distance(vertex_list[path[i - 1]].x, vertex_list[path[i - 1]].y, vertex_list[path[j]].x, vertex_list[path[j]].y);
  float path_j_novo = distance(vertex_list[path[i]].x, vertex_list[path[i]].y, vertex_list[path[j + 1]].x, vertex_list[path[j + 1]].y);
  

  return (path_i_antigo + path_j_antigo) - (path_i_novo + path_j_novo);
}

vector<int> two_opt(vector<int> path, vector<Vertex> vertex_list)
{
  ofstream MyFile("Saida_d18512_2opt_Insertion.txt");

  bool improved = true;
  while (improved)
  {
    improved = false;
    for (long unsigned int i = 1; i < path.size() - 2; i++)
    {
      for (long unsigned int j = i + 1; j < path.size(); j++)
      {
        if (cost3(path, vertex_list, i, j) > 0)
        {
 
          reverse(path.begin() + i, path.begin() + j + 1);
          MyFile << cost(path, vertex_list) << endl;

         improved = true;
          //j = i;
            
        }

      }
    }
  }
  MyFile.close();
  return path;
}

void nearest_insertion(vector<Vertex> &vertex_list, vector<int>& path){
    // iniciar sub grafo
    path.push_back(0);
    vertex_list[0].visited = true;

    // achar vertice que é minimo e formar um sub-tour
    int nearest_index = find_nearest(vertex_list, path[0]);
    vertex_list[nearest_index].visited = true;
    path.push_back(nearest_index);
 
    // selection
    while(path.size() < vertex_list.size()){
        int nearest_index = find_nearest(vertex_list, path[0]);
        // insertion
        float best = numeric_limits<float>::infinity();
        int bestJ = 0;
        for (long unsigned int i = 0; i < path.size() - 1; i++){
           int j = i+1;
           float Cir = distance(vertex_list[path[i]].x, vertex_list[path[i]].y, vertex_list[nearest_index].x, vertex_list[nearest_index].y); //matrix[path[i]][nearest_index];
           float Crj = distance(vertex_list[path[j]].x, vertex_list[path[j]].y, vertex_list[nearest_index].x, vertex_list[nearest_index].y); //matrix[nearest_index][path[j]];            
           float Cij = distance(vertex_list[path[i]].x, vertex_list[path[i]].y, vertex_list[path[j]].x, vertex_list[path[j]].y); //matrix[path[i]][path[j]];
           if ((Cir + Crj - Cij) < best){
             best = Cir + Crj - Cij;
             bestJ = j;
           }
        }
       path.insert(path.begin() + bestJ, nearest_index);
       vertex_list[nearest_index].visited = true;


    }
    path.push_back(0);
}



int reverse_better(vector<int>& new_path, int i, int j, int k, vector<Vertex> vertex_list){
    int A = new_path[i-1], B = new_path[i], C = new_path[j-1], D = new_path[j], E = new_path[k-1], F = new_path[k];

    float A_B = distance(vertex_list[A].x, vertex_list[A].y, vertex_list[B].x, vertex_list[B].y);
    float A_C =  distance(vertex_list[A].x, vertex_list[A].y, vertex_list[C].x, vertex_list[C].y);
    float A_D = distance(vertex_list[A].x, vertex_list[A].y, vertex_list[D].x, vertex_list[D].y);

    float B_D =   distance(vertex_list[B].x, vertex_list[B].y, vertex_list[D].x, vertex_list[D].y);
    
    float C_D =  distance(vertex_list[C].x, vertex_list[C].y, vertex_list[D].x, vertex_list[D].y);
    float C_E =  distance(vertex_list[C].x, vertex_list[C].y, vertex_list[E].x, vertex_list[E].y);
    float C_F =  distance(vertex_list[C].x, vertex_list[C].y, vertex_list[F].x, vertex_list[F].y);
    
    float D_F =  distance(vertex_list[D].x, vertex_list[D].y, vertex_list[F].x, vertex_list[F].y);
    
    float E_A =  distance(vertex_list[E].x, vertex_list[E].y, vertex_list[A].x, vertex_list[A].y);  
    float E_F =  distance(vertex_list[E].x, vertex_list[E].y, vertex_list[F].x, vertex_list[F].y);
    float E_B =  distance(vertex_list[E].x, vertex_list[E].y, vertex_list[B].x, vertex_list[B].y);    

    float F_B =  distance(vertex_list[F].x, vertex_list[F].y, vertex_list[B].x, vertex_list[B].y);
    
    float d0 = A_B + C_D + E_F; //matrix[A][B] +  matrix[C][D] +  matrix[E][F];
    float d1 = A_C + B_D + E_F; // matrix[A][C] +  matrix[B][D] +  matrix[E][F];
    float d2 = A_B + C_E + D_F; //matrix[A][B] +  matrix[C][E] +  matrix[D][F];
    float d3 = A_D + E_B + C_F; //matrix[A][D] +  matrix[E][B] +  matrix[C][F];
    float d4 = F_B + C_D + E_A; //matrix[F][B] +  matrix[C][D] +  matrix[E][A];
    
    if (d0 > d1){
      reverse(new_path.begin() + i, new_path.begin() + j); 

      return -d0+d1;
    }
    else if (d0 > d2){
     reverse(new_path.begin() + j, new_path.begin() + k);

      return -d0 + d2;
    }else if(d0 > d4){
      reverse(new_path.begin() + i, new_path.begin() + k);

      return -d0 + d4;
    }else if(d0 > d3){
        vector<int> tmp;
        tmp.insert(tmp.end(), new_path.begin() + j, new_path.begin() + k);
        tmp.insert(tmp.end(), new_path.begin() + i, new_path.begin() + j);
        for(int x = i; x < k; x++) {
            new_path[x] = tmp[x-i];
        }
      return -d0 + d3;
    }
    
    
    return 0;
    
}

vector<int> three_opt(vector<int> path, vector<Vertex> vertex_list){
    ofstream MyFile("Saida_pla7397_3opt.txt");

    while(true){
        int delta = 0;
        for(long unsigned int i = 1; i < path.size() ; i++){
            for(long unsigned int j = i + 1; j < path.size(); j++){
                for(long unsigned int k = j + 1; k < path.size(); k++){
                    int aux = reverse_better(path, i, j, k, vertex_list);
                    delta += aux;
                    if (aux != 0){
                        MyFile << cost(path,vertex_list) << endl;

                    }
                }
            }
        } 

      if (delta >= 0){
        break;
      }
    }
    MyFile.close();
  return path;
}

int main()
{
    // le entrada e preenche vetores
    vector<Vertex> vertex_list;    
    read_input(vertex_list);
  
 
//-------------------------- Neareast ----------------------------//
   /*
    vector<int> path;    
    nearest_neighbor(vertex_list, path);
    cout << "Nearest " << cost(path, vertex_list) << endl; 


    vector<int> path_3opt;
    path_3opt = three_opt(path,vertex_list);  
    cout << "Custo 3opt N "  << cost(path_3opt, vertex_list) << endl;

  
    */
    
//------------------------ Min Insertion ------------------- //
    
    vector<int> path_min;    

    nearest_insertion(vertex_list, path_min);
    //cout <<"Insertion " << cost(path_min, vertex_list)<< endl ;
    
    vector <int> path2_opt;

    path2_opt = two_opt(path_min, vertex_list);

    cout<<cost(path2_opt, vertex_list) << endl;


    
    
// --------------------------------------------------------------//  
    



    
  return 0;
}