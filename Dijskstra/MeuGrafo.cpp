#include "MeuGrafo.h"
// MeuGrafo

#include <cstddef>
// std::size_t
#include <iterator>
// std::empty
// std::size
#include <queue>
// std::priority_queue
#include <vector>
// std::vector

#include "excecoes.h"
// ExcecaoMatrizAdjacenciaVazia
// ExcecaoMatrizAdjacenciaNaoQuadrada
// ExcecaoCustoNegativo
// ExcecaoVerticeComLaco
// ExcecaoVerticeInvalido


/*Contrutor inicializado com a matriz de adjacencia do grafo
-Preciso fazer as verificações de exceções:matriz vazia,matriz não quadrada e custo e laço negativo
-Matriz vazia : size()==0 ,se true,lançar exceção da matriz vazia
-Matriz quadrada:percorrer cada linha da matriz e verificar se o tamanho é igual ao da matriz,se true ,lançar exceção
-Laço e custo negativo : Se i==j e custo !=infinito ,exceçaõ do laço ,se custo<0,exceção do custo negativo
*/
MeuGrafo::MeuGrafo(const MatrizAdjacencia& matrizAdjacencia):GrafoAbstrato(matrizAdjacencia){
if(matrizAdjacencia.size()==0){
        throw ExcecaoMatrizAdjacenciaVazia();
    }

std::size_t n=matrizAdjacencia.size();
for(std::size_t i=0;i<n;i++){
    if(matrizAdjacencia[i].size()!=n){
        throw ExcecaoMatrizAdjacenciaNaoQuadrada();
    }
}

for(std::size_t i=0;i<n;i++){
    for(std::size_t j=0;j<n;j++){
        double custo_verificar=matrizAdjacencia[i][j];

    if(i==j && custo_verificar!=GrafoAbstrato::inf){
        throw ExcecaoVerticeComLaco();
    }
    if(custo_verificar<0){
        throw ExcecaoCustoNegativo();
    }
    }
}

}

//Quantidade de vértices do grafo: Só preciso retornar o tamanho da matriz adjacência
std::size_t MeuGrafo::quantidadeVertices()const{
    return _matrizAdjacencia.size();
}

/*Quantidade de arestas : Retorna a quantidade de arestas a partir de um vertice
-Inicialmente verifico se o vertice é valido,ou seja,se ele pertence ao grafo,se não,lanço excecao
-Preciso declarar dois size_t ,um contador para contar as arestas e outro pra receber quantidadeVertices
-Percorro a linha da matriz adjacencia do vertice passado e devo verficar se o custo é diferente de infinito,se true,incremento quantidade
-Depois só retornar quantiade
*/
std::size_t MeuGrafo::quantidadeArestas(std::size_t vertice)const{
    if(vertice>=quantidadeVertices()){
        throw ExcecaoVerticeInvalido();
    }

std::size_t quantidade_arestas=0;
std::size_t n=quantidadeVertices();
for(std::size_t i=0;i<n;i++){
if(_matrizAdjacencia[vertice][i]!=GrafoAbstrato::inf){
    quantidade_arestas++;
}
}
return quantidade_arestas;
}

/*Quantidade total de arestas: TRetorna a quantidade total de arestas do grafo
-Só preciso declarar dois size_t :um para o total e outro para receber quantidadeVertices()
-Posso chamar quantidadeArestas e criar um método por sobrecarga
*/
std::size_t MeuGrafo::quantidadeArestas()const{
    std::size_t total=0;
    std::size_t n=quantidadeVertices();
    for(std::size_t i=0;i<n;i++){
        total+=quantidadeArestas(i);
    }
    return total;
}

/*Custo da aresta: Deve retornar o custo da aresta partindo do vertice origem até o vertice destino
-Primeiro preciso verificar se os vertices são válidos,se não,lanço exceção
-Depois só retorno o custo da aresta,que é o valor da matriz adjacencia na posição[origem][destino]
*/
double MeuGrafo::custo(std::size_t origem,std::size_t destino)const{
    if(origem>=quantidadeVertices()||destino>=quantidadeVertices()){
        throw ExcecaoVerticeInvalido();
    }
    return _matrizAdjacencia[origem][destino];
}

/*Menores distancias: Retorna um vetor com as menores distancias do vertice de origem até os outros vertices do grafo
-Primeiro preciso verificar se o vertice origem é valido,se não,lanço a exceção
-Como visto em aula,se não conheço o caminho,então a distancia é infinita,então declarar vector double distancia com tamanho quantidadeVerticess()
e inicializar com infinito (GrafoAbstrato::inf)
-Preciso declacar um vetor bool visitado inicializado com false para saber se o vertice ja foi visitado
-Deve garantir que o vertice de origem tenha distancia 0 até ela mesma,entao distancia[origem]=0; como visto em aula
-Usando uma fila de prioridade(std::priority_queue) para amazenar os vertices visitados e suas distancias
-Uma priority_queue tem a seguinte sintaxe: priority_queue<tipo,container,comparador> nome;
-Tipo: Melhor escolha é um pair<double,size_t>,<distancia,vertice>
-Container: Melhor escolha é um vector de pair porque tem acesso rápido aos elementos e funções eficientes na biblioteca<vector>
-Comparador :std::greater para que a fila seja ordenada do menor para o maior
-Operadores de pair e vector a lembrar: first e second(distancia e vertice),top()para acessar o elemento do topo da fila,empty()verifica se a fila está vazia
pop()remove o elemento do topo da fila,push()adiciona um elemento na fila
-Se o vertice atual ja foi vistado então devo continuar para o próximo vertice,então se ja_visitei[vertice_atual],continue
-Preciso usar o for para percorrer todos os vertices e fazer as comparações iterativamente
-Declarar um double peso_aresta para receber o valor de matrizAdjacencia[vertice atual][i]
-Se o peso_aresta for infinito,então preciso continuar para o próximo
-Se a distancia do vertice atual + peso da aresta for menor que a distancia do vertice i,atualizo a distancia de i e adiciono(push) na fila de prioridade
-Depois de percorrer todos os vertices,atualizando as distancias,só retorno o vetor distancia
*/
std::vector<double> MeuGrafo::menoresDistancias(std::size_t origem)const{
    if(origem>=quantidadeVertices()){
        throw ExcecaoVerticeInvalido();
    }

std::size_t n=quantidadeVertices();
std::vector<double> distancia(n,GrafoAbstrato::inf);
std::vector<bool> ja_visitei(n,false);
distancia[origem]=0.0;

std::priority_queue<std::pair<double,std::size_t>,std::vector<std::pair<double,std::size_t>>,std::greater<std::pair<double,std::size_t>>>fila;

fila.push(std::pair<double,std::size_t>(0.0,origem));

while(!fila.empty()){
    double dist_Atual=fila.top().first;
    std::size_t vertice_Atual=fila.top().second;
    fila.pop();

if(ja_visitei[vertice_Atual]){
    continue;
}
ja_visitei[vertice_Atual]=true;

for (std::size_t i=0;i<n;i++){
    
    double peso_aresta=_matrizAdjacencia[vertice_Atual][i];

    if(peso_aresta==GrafoAbstrato::inf){
        continue;
    }
    if(dist_Atual+peso_aresta<distancia[i]){
        distancia[i]=dist_Atual+peso_aresta;
        fila.push(std::pair<double,std::size_t>(distancia[i],i));
    }
}
}
return distancia;
}