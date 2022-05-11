#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include <numeric>


using namespace std;
using namespace std::chrono;


//#define tam_pop 20
//#define taxa_mutacao 0.01
int tam_pop = 10;
float taxa_mutacao = 0.02;
int conjunto_based = 10;

// Sergio Alvarez
// Guilherme Ferrari
class Vertex
{
public:
  int pos;
  float x, y;
  bool visited;

  // construtor vazio
  Vertex() {}

  // construtor com parametros
  Vertex(int pos_value, float x_value, float y_value, bool visited_value)
  {
    pos = pos_value;
    x = x_value;
    y = y_value;
    visited = visited_value;
  }
};

void read_input(vector<Vertex> &vertex_list)
{
  string input;
  vector<string> input_list;
  int pos, x, y;
  // joga 6 linhas fora
  for (int i = 0; i < 6; i++)
  {
    getline(cin, input);
  }
  // adiciona na classe "Vertex"
  while (scanf("%d %d %d", &pos, &x, &y))
  {
    vertex_list.push_back(Vertex(pos-1, x, y, false));
  }
}

int rand_num(int start, int end)
{
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int> distribution(start, end);
  int rand = distribution(generator);
  return rand;
}

// calcula distancia
float distance(int x1, int y1, int x2, int y2)
{
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
// populacao = conjunto de cromossomos
void create_population(vector<Vertex> &vertex_list, vector<vector<int>> &population){
  srand(time(0));
  for(int i = 0; i < tam_pop; i++){
    vector<int> random;  
    for(int i = 0; i < vertex_list.size(); i++){
      random.push_back(i);
    }
    random_shuffle(random.begin(), random.end());
    
    for(int j = 0; j < vertex_list.size(); j++){        
      population[i].push_back(vertex_list[random[j]].pos);
    }
  }
    
}

// calcula o custo do caminho
float cost(vector<int> &path, vector<Vertex>& vertex_list){
    float path_w = 0;
    for(long unsigned int i = 0; i < path.size()-1; i++){
        path_w += distance(vertex_list[path[i]].x, vertex_list[path[i]].y,
            vertex_list[path[i+1]].x, vertex_list[path[i+1]].y);
    }
    //retorna ao inicio
   path_w += distance(vertex_list[path[path.size()-1]].x, vertex_list[path[path.size()-1]].y,
            vertex_list[path[0]].x, vertex_list[path[0]].y);
    return path_w;
}
auto has_duplicates(const std::vector<int>& v) -> bool
{
    for (auto i1 = v.begin(); i1 != v.end(); ++i1)
        for (auto i2 = i1 + 1; i2 != v.end(); ++i2)
            if (*i1 == *i2)
                return true;
    
    return false;
}
// calcula fitness da populacao Windowing
void calculate_fit(vector<Vertex> &vertex_list, vector<vector<int>> &population, vector<float> &fitness){
  vector<float> fitnessAux;
  for(int i = 0; i < tam_pop; i++){
    fitnessAux.push_back(cost(population[i],vertex_list));
  }

 int maxElementIndex = max_element(fitnessAux.begin(),fitnessAux.end()) - fitnessAux.begin(); 
  for(int i = 0; i < tam_pop; i++){
    fitness[i] = (-1 * (fitnessAux[i] - fitnessAux[maxElementIndex]));
  }
}

// calcula fitness da populacao Windowing
float calculate_fit2(vector<Vertex> &vertex_list, vector<vector<int>>& population, vector<int>& filho){
  vector<float> fitnessAux;
  for(int i = 0; i < tam_pop; i++){
    fitnessAux.push_back(cost(population[i],vertex_list));
  }

 int maxElementIndex = max_element(fitnessAux.begin(),fitnessAux.end()) -     fitnessAux.begin(); 

  return -1 * ( cost(filho, vertex_list) -  fitnessAux[maxElementIndex]);
}


// Operação de Seleção Roleta
int select_roleta(vector<float> &fitness){
  vector<float> select;

  float sum = accumulate(fitness.begin(), fitness.end(), 0.);
  for(int i = 0; i < fitness.size(); i++){
    select.push_back(fitness[i]/sum);
  }  

  for(int i = 1; i < fitness.size(); i++){
    select[i] = select[i-1] + select[i];
  }

  //Gerar numero aleatorio entre 0 e 1
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> distr(0, 1);
  float rand = distr(eng);
  for(int i = 1; i < select.size(); i++){

    if(rand < select[i] && rand > select[i-1]){
      return i;      
    }
  }
  return 0;
}


// // Cruzamento de um ponto simples
// vector<vector <int>> cruzamento_simples(vector<int> &pai, vector<int> &mae, vector<Vertex> &vertex_list){

//   int rand_index = rand_num(0,vertex_list.size());
  
//    vector<vector <int>> filhos(2);

//   for(int i = 0; i < vertex_list.size(); i++){
//     if(i < rand_index){
//       filhos[0].push_back(mae[i]); 
//       filhos[1].push_back(pai[i]); 
//     }
//     else{
//       filhos[0].push_back(pai[i]); 
//       filhos[1].push_back(mae[i]);       
//     }
    
//   }
//   return filhos;
// }

int indexOf(vector<int>& pai, int filho){
  vector<int>::iterator index;
  index = std::find (pai.begin(), pai.end(), filho);
    if (index != pai.end())
    {
        return index - pai.begin();
    }
  cout << "Filho = " << filho << endl;
    for(auto i : pai){
    cout << i << " ";
      
    }       
  cout << "\n\n";

  return 0;

}

// Cruzamento Position Based CrossOver
vector<vector <int>> positionBased(vector<int>& pai, vector<int>& mae){
  vector<int> aux = {pai};
  vector<vector <int>> filhos = {pai, mae};

  vector<int> random;  
  for(int i = 0; i < pai.size(); i++){
    random.push_back(i);
  }
  random_shuffle(random.begin(), random.end());

  for(int i = 0; i < conjunto_based; i++){
    pai = filhos[0];
    filhos[0][random[i]] = mae[random[i]];
    filhos[0][indexOf(pai,filhos[0][random[i]])] = pai[random[i]];
    }    
  pai = aux;
  for(int i = 0; i < conjunto_based; i++){
    mae = filhos[1];  
    filhos[1][random[i]] = pai[random[i]];
    filhos[1][indexOf(mae,filhos[1][random[i]])] = mae[random[i]]; 
    }

  return filhos;
}

// Cruzamento CX
vector<vector <int>> cycle_crossover(vector<int> &pai, vector<int> &mae){
  vector<vector <int>> filhos = {pai, mae};
  // cruzamento filho 1
  filhos[0][0] = mae[0];
  int index = indexOf(pai, filhos[0][0]);
  while(index != 0){
    filhos[0][index] = mae[index];
    index = indexOf(pai, filhos[0][index]);    
  }
  // cruzamento filho 2
  filhos[1][0] = pai[0];
  index = indexOf(mae, filhos[1][0]);
  while(index != 0){
    filhos[1][index] = pai[index];
    index = indexOf(mae, filhos[1][index]);    
  }
      
  return filhos;
}
float dif_cost(vector<int>& path, vector<Vertex>& vertex_list, unsigned long int i,
               unsigned long int j)
{
  if (i - 1 < 0)
    i = 0;
  if (j + 1 > vertex_list.size())
    j = vertex_list.size() - 1;
  if (path[j + 1] > vertex_list.size())
    j--;
  float path_i_antigo =
      distance(vertex_list[path[i - 1]].x, vertex_list[path[i - 1]].y,
               vertex_list[path[i]].x, vertex_list[path[i]].y);
  float path_j_antigo =
      distance(vertex_list[path[j]].x, vertex_list[path[j]].y,
               vertex_list[path[j + 1]].x, vertex_list[path[j + 1]].y);
  float path_i_novo =
      distance(vertex_list[path[i - 1]].x, vertex_list[path[i - 1]].y,
               vertex_list[path[j]].x, vertex_list[path[j]].y);
  float path_j_novo =
      distance(vertex_list[path[i]].x, vertex_list[path[i]].y,
               vertex_list[path[j + 1]].x, vertex_list[path[j + 1]].y);

  return (path_i_antigo + path_j_antigo) - (path_i_novo + path_j_novo);
}
void two_opt(vector<int>& path, vector<Vertex>& vertex_list)
{
  bool improved = true;
  int aux = 0;
  while (aux++ < 1)
  {
    improved = false;
    for (long unsigned int i = 1; i < path.size() - 2; i++)
    {
      for (long unsigned int j = i + 1; j < path.size(); j++)
      {
        if (dif_cost(path, vertex_list, i, j) > 0)
        {
          reverse(path.begin() + i, path.begin() + j + 1);
          improved = true;
        }
      }
    }
  }
  //return path;
}

void mutacao(vector<Vertex>& vertex_list, vector<vector <int>>& filhos){
  //Gerar numero aleatorio entre 0 e 1
  random_device rd;
  default_random_engine eng(rd());
  uniform_real_distribution<float> distr(0, 1);


  if(distr(eng) < taxa_mutacao){
    
    int rand1 = rand_num(0, vertex_list.size()-1);
    int rand2 = rand_num(0, vertex_list.size()-1);
    int aux = filhos[0][rand1];
    filhos[0][rand1] =  filhos[0][rand2];
    filhos[0][rand2] = aux;
  }
  if(distr(eng) < taxa_mutacao){
    
    int rand1 = rand_num(0, vertex_list.size()-1);
    int rand2 = rand_num(0, vertex_list.size()-1);
    int aux = filhos[0][rand1];
    filhos[0][rand1] =  filhos[0][rand2];
    filhos[0][rand2] = aux;   
  }
  
}



int main()
{
  long long milliseconds = 0;
  // le entrada e preenche vetores
  vector<Vertex> vertex_list;
  read_input(vertex_list);
  vector<vector <int>> population(tam_pop);  
  vector<float> select;
  vector<float> custos(tam_pop);
  int gen = 0;
  // Cria população inicial
  create_population(vertex_list, population);

  ofstream MyFile("1002_CX.txt");	
  cout << fixed << setprecision(5);
  MyFile << fixed << setprecision(5);
  auto start = std::chrono::high_resolution_clock::now();
  while(milliseconds < 1800000){
    vector<float> fitness(tam_pop);  
    vector<vector <int>> filhos; 
    // Calcula o fit
    calculate_fit(vertex_list, population, fitness);
 
    // Calcula a probabilidade de ser escolhido
    int index_pai = select_roleta(fitness);
    int index_mae = select_roleta(fitness);
    vector<int> pai;
    vector<int> mae;   
    for(int i = 0; i < vertex_list.size(); i++){
      pai.push_back(population[index_pai][i]);    
    }
    for(int i = 0; i < vertex_list.size(); i++){
      mae.push_back(population[index_mae][i]);    
    }    
    // Calcula custo    
    for(int i = 0; i < tam_pop; i++){
      custos[i] = cost(population[i], vertex_list);
    }
    int min_index = min_element(custos.begin(),custos.end()) - custos.begin();
   MyFile << custos[min_index];
    // Cross Over
    filhos = cycle_crossover(pai, mae);
    // Mutacao
    mutacao(vertex_list, filhos);
    // Busca Local  
    two_opt(filhos[0], vertex_list);
    two_opt(filhos[1], vertex_list);

    float fit1 = calculate_fit2(vertex_list, population, filhos[0]);
    min_index = min_element(fitness.begin(),fitness.end()) - fitness.begin(); 

  
    // verifica se filho é melhor q ultimo
    if(fit1 > 0){
      population[min_index] = filhos[0];
    }  
    calculate_fit(vertex_list, population, fitness);
    float fit2 = calculate_fit2(vertex_list, population, filhos[0]);
    min_index = min_element(fitness.begin(),fitness.end()) - fitness.begin(); 
 
    // verifica se filho é melhor q ultimo
    if(fit2 > 0){
      population[min_index] = filhos[1];
    }    
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        elapsed).count();

    MyFile << " " <<  milliseconds << endl;
    }
  MyFile.close();

  return 0;
}

