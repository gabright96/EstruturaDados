#ifndef DEC0006_MINHA_LISTA_ENCADEADA_H
#define DEC0006_MINHA_LISTA_ENCADEADA_H

#include <cstddef>
// std::size_t

#include "Elemento.h"
// Elemento
#include "ListaEncadeadaAbstrata.h"
#include "excecoes.h"
// ExcecaoDadoInexistente
// ExcecaoListaEncadeadaVazia
// ExcecaoNaoImplementado
// ExcecaoPosicaoInvalida


//Cada função tem um comentário explicando a lógica e o raciocinio da implementação para facilitar localização de erros e entendimento

template<typename T>
class MinhaListaEncadeada: public ListaEncadeadaAbstrata<T>
{
    public:
//1- Destrutor da MinhaListaEncadeada - Libera a memória e evita vazamento de memória
 /*Declaro um ponteiro Elemento Atual para percorrer a lista do inicio,para não perder a referencia dos próximos nós da lista
 -Usei o ponteiro this para acessar diretamente os membros da classe MinhaListaEncadeada e não ter que ficar daclarando ponteiros
 -Uso o while para percorrer a lista até o final(onde Atual=nullptr)
 -Dentro do while,declaro o ponteiro temporario=atual para não perder referencia ,depois Atual=Atual->proximo para avançar
 -Depois de avançar,delete o elemento apontado por temporario
 */
     
    virtual ~MinhaListaEncadeada(){

    Elemento<T>* Atual= this->_primeiro;

    while(Atual !=nullptr){
    Elemento<T>* Temporario =Atual;
    Atual =Atual->proximo;
    delete Temporario;
       }
    }

  //2- Tamanho da lista - Retorna o numero de elementos na lista
//Ponteiro this acessa parametro tamanha da classe

    virtual std::size_t tamanho() const
    {
    return this->_tamanho;

    };
    
 //3 -Verificação de lista vazia -Retorna true se a lista estiver vazia
 //Retorna true se a lista estiver vazia,ou seja,tamanho=0

    virtual bool vazia() const 
    {
    return this->_tamanho == 0;

    };

  //3- Posiçao do dado - Retorna a posicao do dado solicitado
/*Inicialmente faço uma verificação se a lista está vazia chamando vazia() e lançando ExcecaoListaEncadeadaVazia() caso seja true
 -Declarei um ponteiro atual para Elemento para percorrer a lista recebendo o primeiro da lista(this->_primeiro)
 -while para percorrer a lista até o final,comparando o dado do apontado por atual com o solicitado,se for igual retorno a posição dele(pos)
 -Se não for igual(optei pelo if-else mais simples,sem {}) atual avança para o próximo e pos é incrementada
 -Else -Se após o loop o dado não for encontrado,lanço a exceçaõ ExcecaoDadoInexistente();
*/
    virtual std::size_t posicao(T dado) const 
    {
    if(vazia())
    throw ExcecaoListaEncadeadaVazia();

    Elemento<T>* atual= this->_primeiro;
    std::size_t pos=0;

while(atual!=nullptr){
 if(atual->dado==dado)
 return pos;
 
 atual=atual->proximo;
 pos++;
}
 throw ExcecaoDadoInexistente();
    };
    
 //4 -Verificação de existencia de dado
 //Mesma logica de posicao,mas retorna true se o dado for encontrado e false se chegar no final da lista e não encontrar
    virtual bool contem(T dado) const
    {
    Elemento<T>* Atual=this->_primeiro;
    while(Atual!=nullptr){
    if(Atual->dado==dado) return true;

    Atual=Atual->proximo;
    }

    return false;
    };

//5- Inserção no inicio da lista
/*this->_primeiro recebe um novo Elemento e o ponteiro desse novo Elemento aponta para o antigo primeiro
-Após alocar dinamicamente o novo Elemento,tamanho é incrementado para atualizar o tamanho da lista
*/
    virtual void inserirNoInicio(T dado) 
    {
    this->_primeiro=new Elemento<T>(dado,this->_primeiro);
    this->_tamanho++;
    };

//6 - Inserção em posição especifica
/*Inicialmente faço uma verificação se a posição solicitada é maior do que o tamanho da lista(usuario sempre quer testar os limites)
lanço a exceção ExcecaoPosicaoInvalida()caso seja true
-Decidi tratar o caso onde a posição solicitada é a 0(inicio da lista) e chamei a função inserirNoInicio()para evitar duplicação de código.
return para sair da função(antes sem return estava dando erro de compilação)
-Mesma lógica de percorrer a lista do inicio usando um ponteiro Elemento atual,mas agora o loop vai até a posição anterior da solicitada
 isso é necessário para evitar a perda de referencia do próximo nó da lista,pois o novo será inserido entre o atual e o próximo
 -DEpois de chegar na posição anterior,o novo Elemento é alocado e tamanho incrementa
*/
    virtual void inserir(std::size_t posicao, T dado)
    {
    if(posicao>this->_tamanho)
    throw ExcecaoPosicaoInvalida();

    if(posicao==0){
    inserirNoInicio(dado);
        return;
      }

      Elemento<T>* atual=this->_primeiro;
    for(std::size_t i=0;i<posicao-1;i++){
        atual=atual->proximo;
    }
    atual->proximo=new Elemento<T>(dado,atual->proximo);
    this->_tamanho++;
    };

    //7 - Inserção no fim da lista
    /*
    -Inicialmente verifico se a lista está vazia chamando vazia(),se true ,chamo InserirNoInicio() e dou o return pra sair 
    -Declaro novamente o ponteiro auxiliar atual apontando para onde o primeiro da lista aponta(this->_primeiro)
    -Sabendo que o ultimo da lista sempre aponta pra NULL,faço o loop para avançar o atual até o final(atual->proximo=nullptr)
    -Chegando no final,aloco dinamicamente um novo Elemento para o próximo do atual(atual->proximo) e aumento o tamanho da lista
    */
    
    virtual void inserirNoFim(T dado)
    {
      if(vazia()){
        inserirNoInicio(dado);
        return;
      }
    
    Elemento<T>* atual=this->_primeiro;
    while(atual->proximo!=nullptr){
        atual=atual->proximo;
    }
    atual->proximo=new Elemento<T>(dado);
    this->_tamanho++;
    };

  //8 -Remoção do inicio da lista
  /*Inicialmente verifico se a lista está vazia ,se true,lanço a exceção ExcecaoListaEncadeadaVazia()
  essa verificação evita falha de segmentação ao tentar acessar elementos de uma lista vazia
  -Declarei um ponteiro temporario para Elemento para guardar a referencia do primeiro elemento da lista
  -Agora this->primeiro é atualizado para apontar para o próximo elemento ,deixando o antigo primeiro "fora da lista"
  Toda a lógica envolvendo o ponteiro temporario é necessaria para não perder a referencia do proximo elemento da lista
  -Após atualizar o primeiro,fazendo-o apontar para o próximo, delete no elemento apontado por temporario e decremento o tamanho da lista
  -Como guardei o dado do elemento removido ,posso retornar ele no final da função
  */
    virtual T removerDoInicio()
    {
      if(vazia()){
        throw ExcecaoListaEncadeadaVazia();
      }
    Elemento<T>*temporario=this->_primeiro;
    T dado=temporario->dado;

    this->_primeiro=temporario->proximo;
    delete temporario;
    this->_tamanho--;

    return dado; 
    };

   //9- Remoção de posição especifica
   /*Inicialmente faça duas verificações: Se o usuario solicitou uma posição valida(posicao>=tamanho) ou lista vazia
    -Se passou ,atual percorre a lista até a posição anterior da solicitada.
    Isso é nnecessário para não perder a referencia do próximo nó e não quebrar a lista
    -DEclaro o ponteiro removido e faça ele apontar para o elemento que será removido(atual->proximo)
    Guardo o dado para retorna-lo no final da função
    -Atualizo o proximo do atual para apontar para o próximo do removido ,isolando o elemneto que será removido
    -Depois de isolado,delete removido,decremento o tamanho da lista e retorno o dado do removido
   */
    virtual T removerDe(std::size_t posicao)
    {
    if(posicao>=this->_tamanho){
      throw ExcecaoPosicaoInvalida();
    }
    if(posicao==0){
      return removerDoInicio();
    }

      Elemento<T>* atual =this->_primeiro;
    for(std::size_t i=0;i<posicao-1;i++){
      atual=atual->proximo;}

      Elemento<T>* removido=atual->proximo;
      T dado =removido->dado;
      atual->proximo=removido->proximo;
      delete removido;
      this->_tamanho--;
      return dado;
  
    };
//10 - Remoção do fim da lista
/* Primeiro verifico se a lista está vazia ,se true,lanço a exceção ExcecaoListaEncadeadaVazia() para evitar segmentation fault
-Para evitar duplicação de código,decidi tratar o caso onde a lista tem apenas um elemento e chamei removerDoInicio()
-Uso a lógica semelhante ,ponteiro temporario para guardar a referencia do elemento a ser removido
*/
    virtual T removerDoFim()
    {
      if (vazia()){
      throw ExcecaoListaEncadeadaVazia();}
    
      if(this->_tamanho==1){
        return removerDoInicio();
      }

      Elemento<T>* atual=this->_primeiro;

      while(atual->proximo->proximo!=nullptr){
        atual=atual->proximo;
      }

      Elemento<T>* Temporario=atual->proximo;
      T dado=Temporario->dado;

      atual->proximo=nullptr;
      delete Temporario;
      this->_tamanho--;
      return dado;

    };
//11 - Remover por valor
/*Inicialmente faça as verificações se a lista está vazia ou se o dado solicitado para a remoção existe na lista
-Para Evitar duplicação de código,decidi tratar o caso onde o dado a ser removido é o primeiro da lista,chamando RemoverDoInicio()
-Declaro um ponteiro atual para percorre a lista do inicio,comparando o próximo elemento(atual->proximo->dado) com o dado solicitado
-Se for igual,declaro um ponteiro removido para guardar a referencia do elemento a ser removido(atual->proximo)
-Atualizo o próximo do atual para apontar para o próximo do removido,seguindo a mesma lógica de isolar o elemento a ser removido
 assim não quebro minha lista...DEpois de isola-lo,delete no removido e decremento a lista
-Else - Se chegar no final da lista e não encontrar o dado,lanço a exceção de dado inexistente
*/
   
    virtual void remover(T dado)
    {
      if(vazia()){
        throw ExcecaoListaEncadeadaVazia();
       }

      if(!contem(dado)){
        throw ExcecaoDadoInexistente();
       }

      if(this->_primeiro->dado==dado){
        removerDoInicio();
        return;
       }
       
Elemento<T>* atual=this->_primeiro;

while(atual->proximo!=nullptr){
  if(atual->proximo->dado==dado){

    Elemento<T>* Removido=atual->proximo;
    atual->proximo=Removido->proximo;
    delete Removido;
    this->_tamanho--;
    return;

  }
atual=atual->proximo;
}
throw ExcecaoDadoInexistente();
    };
};

#endif