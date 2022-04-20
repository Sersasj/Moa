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
    int pos;
    float x, y; 
    bool visited;

    // construtor vazio
    Vertex() {}

    // construtor com parametros
    Vertex(int pos_value, float x_value, float y_value, bool visited_value){
        pos = pos_value;
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
        vertex_list.push_back(Vertex(pos-1, x, y, false));
    }  

}

float distance(int x1, int y1, int x2, int y2){    
    return sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2) * 1.0);
}

void create_matrix(float **matrix, vector<Vertex> vertex_list){  
    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        for(long unsigned int j = 0; j < vertex_list.size(); j++){
            matrix[i][j] = distance(vertex_list[i].x, vertex_list[i].y, 
                vertex_list[j].x, vertex_list[j].y);
        }
    }
}
// metodo constutivo mais proximo
int find_nearest(float **matrix,  vector<Vertex> vertex_list, int path_index){
    int min_index = 0;
    float min_value = numeric_limits<float>::infinity();

    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        if(vertex_list[i].visited == false && matrix[path_index][i] < min_value){
            min_value =  matrix[path_index][i];
            min_index = i;            
        }
    }
    return min_index;
}


int random_construct(vector<Vertex> vertex_list, vector<int> disp_vertex){
    
    int random = rand() % disp_vertex.size();
    
    int index = disp_vertex[random];
    cout << index;
    disp_vertex.erase(disp_vertex.begin() + random);

    return index;
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

vector<int> two_opt(vector<int> path, float **matrix){
  ofstream MyFile("Saida_88k_2opt.txt");
  time_t start, end;
  time(&start);
  vector<int> best = path;
  bool improved = true;
  while (improved){
    improved = false;
    for (long unsigned int i = 1; i < path.size() - 2; i++){
      for (long unsigned int j = i+1; j < path.size(); j++){
        if (j-i == 1){
          continue;
        }
        vector<int> new_path = path;
        // corta
        vector<int> new_path_slice(new_path.begin() + i, new_path.begin() + j);
        // inverte  
        reverse(new_path_slice.begin(), new_path_slice.end());
        // junta
        for(long unsigned int x = i; x < j; x++){
            new_path[x] = new_path_slice[x-i];
        }         
        if (cost2(new_path, matrix) < cost2(path, matrix)){
            best = new_path;    
            MyFile << cost2(new_path, matrix) << endl;
            time(&end);
            double time_taken = double(end - start);
            path = best;
            //if(time_taken > 7200){
            //  return best;
            //}
            improved = true;
            j = i;
        }
      }
    }
  }
    MyFile.close();

  return best;
}



int reverse_better(vector<int>& new_path, int i, int j, int k, float **matrix){
    int A = new_path[i-1], B = new_path[i], C = new_path[j-1], D = new_path[j], E = new_path[k-1], F = new_path[k];

    float d0 = matrix[A][B] +  matrix[C][D] +  matrix[E][F];
    float d1 = matrix[A][C] +  matrix[B][D] +  matrix[E][F];
    float d2 = matrix[A][B] +  matrix[C][E] +  matrix[D][F];
    float d3 = matrix[A][D] +  matrix[E][B] +  matrix[C][F];
    float d4 = matrix[F][B] +  matrix[C][D] +  matrix[E][A];
    //cout << endl << d0 << " - " << d1 << " - " << d2 << " - " << d3 << " - " << d4 << endl;
    
    if (d0 > d1){
      reverse(new_path.begin() + i, new_path.begin() + j); // acho q isso é reverse(tour[i:j]) 

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

vector<int> three_opt(vector<int> path, float **matrix){
    ofstream MyFile("Saida_pla7397_3opt.txt");

    while(true){
        int delta = 0;
        for(long unsigned int i = 1; i < path.size() ; i++){
            for(long unsigned int j = i + 1; j < path.size(); j++){
                for(long unsigned int k = j + 1; k < path.size(); k++){
                    int aux = reverse_better(path, i, j, k, matrix);
                    delta += aux;
                    if (aux != 0){
                        MyFile << cost2(path,matrix) << endl;

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
  
    // aloca matriz
    float **matrix;
    matrix = new float*[vertex_list.size()];    
    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        matrix[i] = new float[vertex_list.size()];            
    }

    // adiciona distancias na matriz
    create_matrix(matrix, vertex_list);
    
//-------------------------- Neareast ----------------------------//
  /*
    auto start_near_construct = high_resolution_clock::now();  
    vector<int> path;    
    path.push_back(0);
    vertex_list[0].visited = true;
    for(long unsigned int i = 0; i < vertex_list.size(); i++){
        int nearest_index = find_nearest(matrix, vertex_list, path[i]);
        path.push_back(nearest_index);
        vertex_list[nearest_index].visited = true;
    }
    auto stop_near_construct = high_resolution_clock::now();  
    cout << "Nearest " << cost2(path, matrix) << endl;

    
    auto start_near_opt2 = high_resolution_clock::now();
    vector <int> path2;
    path2 = two_opt(path, matrix);
    auto stop_near_opt2 = high_resolution_clock::now();
        cout << "custo 2opt N "  << cost2(path2, matrix) << endl;


    auto start_near_opt3 = high_resolution_clock::now();   
    vector<int> path_3opt;
    path_3opt = three_opt(path,matrix);
    auto stop_near_opt3 = high_resolution_clock::now(); 
    //for (auto & element : path2){
    //  cout << element << " " ;
    //}    
    cout << "Custo 3opt N "  << cost2(path_3opt, matrix) << endl;

    auto duration_construct_near = duration_cast<microseconds>(stop_near_construct - start_near_construct);
    auto duration_near_opt2 = duration_cast<microseconds>(stop_near_opt2 - start_near_opt2);    
    auto duration_near_opt3 = duration_cast<microseconds>(stop_near_opt3 - start_near_opt3);
    
    cout << duration_construct_near.count() << " " << duration_near_opt2.count() << " " << duration_near_opt3.count() << " " << endl;    

    */
//-------------------------- Random ---------------------------// 
  
    auto start_random_construct = high_resolution_clock::now();   
    vector<int> path3;
    srand(time(0));
    random_shuffle(vertex_list.begin(), vertex_list.end());
    for(int i = 0; i < vertex_list.size(); i++){       
        path3.push_back(vertex_list[i].pos);        
    }
    path3.push_back(path3[0]);
    auto stop_random_construct = high_resolution_clock::now();
    
    auto start_random_opt2 = high_resolution_clock::now();
    vector <int> path4;
    path4 = two_opt(path3, matrix);
    //for (auto & element : path2){
    //  cout << element << " " ;
    //}
    auto stop_random_opt2 = high_resolution_clock::now();
    auto start_random_opt3 = high_resolution_clock::now();
    //vector <int> path_3opt_random;
    //path_3opt_random = three_opt(path3,matrix);
    auto stop_random_opt3 = high_resolution_clock::now();

    cout << "random " << cost2(path3, matrix) << endl;
    cout << "Custo 2opt R " << cost2(path4, matrix) << endl;
    //cout << "Custo 3opt R " << cost2(path_3opt_random, matrix) << endl;
    

    auto duration_construct_random = duration_cast<microseconds>(stop_random_construct - start_random_construct);
    auto duration_random_opt = duration_cast<microseconds>(stop_random_opt2 - start_random_opt2);
    auto duration_random_total = duration_cast<microseconds>(stop_random_opt3 - start_random_opt3);
    
    cout << duration_construct_random.count() << " " << duration_random_opt.count() << " " << duration_random_total.count() << " " << endl;
    
// -------------------------------------------------------------//    
    



    
  return 0;
}