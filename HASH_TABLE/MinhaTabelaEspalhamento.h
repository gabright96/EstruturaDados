#ifndef MINHA_TABELA_ESPALHAMENTO_H
#define MINHA_TABELA_ESPALHAMENTO_H

#include "MinhaListaEncadeada.h"
// MinhaListaEncadeada
#include "TabelaEspalhamentoAbstrata.h"
// TabelaEspalhamentoAbstrata
#include "excecoes.h"
// ExcecaoDadoInexistente
#include<string>

template<typename T, std::size_t capac>
class MinhaTabelaEspalhamento final:
    public TabelaEspalhamentoAbstrata<T, capac>
{
    public:
    
    //MinhaTabelaEspalhamento inicia com nenhum elemento
    MinhaTabelaEspalhamento():quantidadeElementos(0){}

//Função capacidade retorna a capac da tabela hash:numero máximo de elementos que a tabela pode armazenar.O(1)
    virtual std::size_t capacidade() const{
        return capac;
    }
//Função quantidade deve apenas retornar a quantidade atual de elementos da tabela hash
virtual std::size_t quantidade()const{
    return quantidadeElementos;
}
/*Funçaõ contem :Verifica se o dado existe na tabela hash. Posso reutilizar contem da lista encadeada ,pois uso encadeamento fechado
-Primeiro devo calcular o indice relativo ao dado usando a função hash(funcaoEspalhamento)
-Depois verifico se o dado existe na tabela hash chamando o método contem da lista encadeada correspondente ao indice calculado
*/
virtual bool contem(T dado)const{
    std::size_t i_hash=funcaoEspalhamento(dado);
    return this->tabela[i_hash].contem(dado);
}

/*Função inserir: Insere um dado na tabela hash.Aqui tambem posso reutilizar a função inserir da lista encadeada
-Primeiro preciso verificar de o dado ja existe na tabela hash usando contem.Se existir,dou return; para evitar duplicatas
-Calcular indice relativo ao dado usando a função hash
-Chamo inseriNoFim da lista encadeada pois Porque os testes esperam que os elementos dentro de cada lista de colisão permaneçam na mesma ordem de inserção
OBS:Testei com inserirNoInicio e os testes falharam
-Incrementar quantidadeElementos após inserção
*/
virtual void inserir(T dado){
    if(contem(dado)){return;}
    
    std::size_t i_hash=funcaoEspalhamento(dado);
    this->tabela[i_hash].inserirNoFim(dado);
    quantidadeElementos++;
}
/*Remover: Remove um dado da tabela hash.Tambem posso reutilizar a função remover da lista encadeada
-Mesmo processo de inserir :declarar i_hash recebendo o resultado da função hash
-Chamar remover da lista encadeada correspondente ao i_hash
-Decrementar quantidadeElementos após remoção
*/
virtual void remover(T dado){

  std::size_t i_hash=funcaoEspalhamento(dado);
  this->tabela[i_hash].remover(dado);
  quantidadeElementos--;
}
    protected:
//Função hash(espalhamento):Predefinida ,returna resto da divisão entre o código de espalhamento e capacidade da tabela
virtual std::size_t funcaoEspalhamento(T dado)const{
    return codigoEspalhamento(dado)%capac;
}
    
    private:
      size_t quantidadeElementos;
        /**
         * @brief Calcula a representação numérica de um tipo integral.
         * 
         * @tparam U O tipo integral.
         * @param integral Um valor integral.
         * @return Um inteiro calculado através de static_cast<std::size_t>(integral)
         */
        template<typename U>
      
//Se o tipo for integral,então só devo retornar o valor convertido para size_t
        std::size_t codigoEspalhamento(U integral) const
        {
         return integral;
        }
        
    
        /**
         * @brief Calcula a representação numérica de um string.
         * 
         * @param s um string.
         * @return Um inteiro calculado conforme s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1].
         */
/*Código de espalhamento para string:Calcula através da fórmula acima
-Percorrer string caractere por caractere e aplicar a formula iterativamente
-Depois só retornar o código em size_t calculado
*/
        std::size_t codigoEspalhamento(std::string const& string) const
        {
           std::size_t codigo = 0;
           for(std::size_t i=0;i<string.length();i++){
            codigo=codigo*31+string[i];
           }
           return codigo;
        }
    };

#endif
