#ifndef MINHA_ARVORE_AVL_HPP
#define MINHA_ARVORE_AVL_HPP

#include "ArvoreBinariaDeBusca.h"
#include "MinhaListaEncadeada.h"
#include <optional>

// Herança: MinhaArvoreAVL é uma ArvoreBinariaDeBusca
template <typename T>
class MinhaArvoreAVL final : public ArvoreBinariaDeBusca<T>
{
// As funções estão sob domínio privado pois são funções auxiliares para as operações públicas da classe. Proteção de dados e da estrutura
private:

/*Quantidade de nodos - Contando os nodos recursivamente: Precisa ser recursivo pois conta nodos a esquerda e direita, precisando percorrer toda a AVL
Caso base: Se nodo for nulo, não tem o que contar, retorna 0
Caso recursivo: 1 (para o nodo atual) + quant. nodos a esquerda + quant. nodos a direita
Precisa ser const pois não modifica a estrutura de dados
*/
    int quantidadeNodos(Nodo<T>* nodo) const {
        if (nodo == nullptr) {
            return 0;
        }
        return 1 + quantidadeNodos(nodo->filhoEsquerda) + quantidadeNodos(nodo->filhoDireita);
    }

/*2-Buscar nodo: Percorre a AVL comparando o dado fornecido com o nodo->dado. Precisa ser recursiva
Verificação: Se o nodo não existir (nulo), return nullptr.
É preciso verificar 3 casos: Se meu dado está no nodo atual, se o dado está a esquerda ou a direita. Isso evita percorrer toda a AVL
Se meu dado for maior que o nodo->dado, então o dado deve estar a direita, então chamo a função recursivamente para o filhoDireita
Se o dado for menor que o nodo->dado, recursão no filhoEsquerda
*/
    Nodo<T>* buscarNodo(Nodo<T>* nodo, T chave) const {
        if (nodo == nullptr) return nullptr;

        if (chave == nodo->chave) {
            return nodo;
        }
        if (chave < nodo->chave) {
            return buscarNodo(nodo->filhoEsquerda, chave);
        } else {
            return buscarNodo(nodo->filhoDireita, chave);
        }
    }

/*3-Altura do nodo: Função auxiliar para retornar a altura do nodo.
Se o nodo for nulo, nodo vazio, altura é -1
Altura folha é 0
*/
    int alturaNodo(Nodo<T>* nodo) const {
        if (nodo == nullptr) return -1;
        return nodo->altura;
    }

/*4-Máximo entre dois inteiros: Função auxiliar para definir o máximo entre dois inteiros
Ela é necessária pra atualizar a altura do nodo após operações de inserção e remoção. AtualizarAltura precisa de Maximo
0BS2: Usei if ternário para fortalecer a compreensão dessa ferramenta, deixar a função mais concisa e legível
*/
    int maximo(int a, int b) const {
        return (a > b) ? a : b;
    }

/*5 - Atualizar altura do nodo: Função auxiliar para atualizar a altura do nodo após operações na AVL
Se o nodo for diferente de nullptr, atualiza a altura do nodo com base na altura dos seus filhos.
Esse processo é essencial para manter o balanceamento da AVL, pois uso a altura para calcular o fator de balanceamento
*/
    void atualizarAltura(Nodo<T>* nodo) {
        if (nodo != nullptr) {
            nodo->altura = 1 + maximo(alturaNodo(nodo->filhoEsquerda), alturaNodo(nodo->filhoDireita));
        }
    }

/*6 - Fator de balanceamento: Função auxiliar para definir o fator de balanceamento de um nodo
Primeiro verifica se o nodo é nulo, fator de balanceamento é 0
Caso contrário, calcula a diferença entre a altura do filhoEsquerda AlturaNodo(nodo->filhoEsquerda) e a altura de filhoDireita
Necessário para as rotações
*/
    int fatorBalanceamento(Nodo<T>* nodo) const {
        if (nodo == nullptr) {
            return 0;
        } else {
            return alturaNodo(nodo->filhoEsquerda) - alturaNodo(nodo->filhoDireita);
        }
    }

/*7- Rotação direita: Se a AVL estiver mais pesada para a esquerda, precisa fazer uma rotação para a direita para balancear
Preciso de dois ponteiros auxiliares: nova Raiz(x) aponta para o filhoEsquerda do nodo atual, vai se tornar a nova raiz da subárvore após rotação
t2 guarda o filhoDireita de novaRaiz, pois após a rotação ele vai se tornar o filhoEsquerda no nodo atual
O filhoDireita de novaRaiz é atualizado para apontar para o nodo atual y e o filhoEsquerda de y é atualizado para apontar para t2. Rotação definida
Atualiza a altura de y e x após a rotação pois a estrutura da AVL foi alterada
*/
    Nodo<T>* rotacaoDireita(Nodo<T>* y) {
        Nodo<T>* x = y->filhoEsquerda;
        Nodo<T>* t2 = x->filhoDireita;

        x->filhoDireita = y;
        y->filhoEsquerda = t2;

        atualizarAltura(y);
        atualizarAltura(x);

        return x;
    }

/*8- Rotação esquerda: (Mesmo esquema de Rotação direita) Se a AVL estiver mais pesada para a direita, precisa fazer uma rotação para a esquerda para balancear
Preciso de dois ponteiros auxiliares:  para nova Raiz(y) aponta para o filhoDireita do nodo atual, vai se tornar a nova raiz da subárvore após rotação
t2 guarda o filhoEsquerda de y, pois após a rotação ele vai se tornar o filhoDireita no nodo atual
O filhoEsquerda de novaRaiz é atualizada para apontar para o nodo atual x e o filhoDireita de x é atualizado para apontar para t2. Rotação definida
Atualiza a altura de x e novaRaiz após a rotação pois a estrutura da AVL foi alterada
*/
    Nodo<T>* rotacaoEsquerda(Nodo<T>* x) {
        Nodo<T>* y = x->filhoDireita;
        Nodo<T>* t2 = y->filhoEsquerda;

        y->filhoEsquerda = x;
        x->filhoDireita = t2;

        atualizarAltura(x);
        atualizarAltura(y);

        return y;
    }

/*9- Balancear nodo: Função auxiliar necessária para manter o balanceamento da AVL
*/
    Nodo<T>* balancear(Nodo<T>* nodo) {
        atualizarAltura(nodo);

        int fb = fatorBalanceamento(nodo);

        if (fb > 1) {
            if (fatorBalanceamento(nodo->filhoEsquerda) < 0) {
                nodo->filhoEsquerda = rotacaoEsquerda(nodo->filhoEsquerda);
            }
            return  rotacaoDireita(nodo);
        }
        if (fb < -1) {
            if (fatorBalanceamento(nodo->filhoDireita) > 0) {
                nodo->filhoDireita =  rotacaoDireita(nodo->filhoDireita);
            }
            return  rotacaoEsquerda(nodo);
        }
        return nodo;
    }

/*10- Inserir Nodo: É necessário inserir onde estão as folhas da AVL
Então até quando meu ponteiro nodo encontrar um nullptr, a inserção é feita
Alocação dinâmica, aloca o novonodo e seu dado
A inserção deve ser feita recursivamente para que o nodo seja inserido corretamente na AVL 
Então se o dado passado for menor que nodo->dado, percorro recursivamente a subárvore a esquerda e faço a inserção
Analogamente se o dado for maior, inserção a direita
Após a inserção, é necessário balancear a AVL. Chamo Balancear()
*/
    Nodo<T>* inserirNodo(Nodo<T>* nodo, T chave) {
        if (nodo == nullptr) {
            Nodo<T>* novo = new Nodo<T>();
            novo->chave = chave;
            novo->altura = 0;
            return novo;
        }

        if (chave < nodo->chave) {
            nodo->filhoEsquerda = inserirNodo(nodo->filhoEsquerda, chave);
        } else if (chave > nodo->chave) {
            nodo->filhoDireita = inserirNodo(nodo->filhoDireita, chave);
        } else {
            return nodo;
        }
        return balancear(nodo);
    }

/*11- Menor nodo é necessário para retornar o menor valor de uma subárvore
Foi a forma que achei para lidar com o problem de um nodo ter dois filhos em RemoverNodo
Atual percorre filhos a esquerda com o loop até chegar no menor valor
*/
    Nodo<T>* menorNodo(Nodo<T>* nodo) const {
        Nodo<T>* atual = nodo;

        while (atual->filhoEsquerda != nullptr) {
            atual = atual->filhoEsquerda;
        }
        return  atual;
    }

/*11 - Remover nodo: A remoção é mais complexa pois pode gerar desbalanceamento e até a perda de dados se não for feita corretamente
É necessário recursão pois preciso escolher o melhor caminho.
Se o dado for menor que nodo->dado, precisa pegar o filhoEsquerda do nodo para descer a esquerda e remover
Se for maior, tenho que ir para a direita para remover 
Se o nodo foi uma folha (nodo->filhoEsquerda==nullptr && nodo->filhoDireita==nullptr) delete no nodo e retorno nullptr (posição vazia)
Se meu nodo tem apenas um filho a direita (nodo->filhoEsquerda==nullptr), preciso guardar a referência do filhoDireita do nodo, delete no nodo e retornar o filhoDireita
Analogamente para apenas um filho a esquerda (nodo->filhoDireita==nullptr)
Se meu nodo tem dois filhos (ou seja, a raiz da subárvore), então preciso declarar um sucessor para a raiz a ser apagada
Esse sucessor vai guardar o menor valor da árvore a direita e seu dado, depois de copiar o sucessor na raiz, apago o sucessor
Garantindo que todos da (esquerda < sucessor) e (todos da direita > sucessor)
Balancear a árvore pois remoções alteraram as alturas 
*/
    Nodo<T>* removerNodo(Nodo<T>* nodo, T chave) {
        if (nodo == nullptr) return nullptr;

        if (chave < nodo->chave) {
            nodo->filhoEsquerda = removerNodo(nodo->filhoEsquerda, chave);
        } else if (chave > nodo->chave) {
            nodo->filhoDireita = removerNodo(nodo->filhoDireita, chave);
        } else {
            if (nodo->filhoEsquerda == nullptr && nodo->filhoDireita == nullptr) {
                delete nodo;
                return nullptr;
            }
            if (nodo->filhoEsquerda == nullptr) {
                Nodo<T>* temp = nodo->filhoDireita;
                delete nodo;
                return temp;
            }
            if (nodo->filhoDireita == nullptr) {
                Nodo<T>* temp = nodo->filhoEsquerda;
                delete nodo;
                return temp;
            }

            Nodo<T>* sucessor = menorNodo(nodo->filhoDireita);
            nodo->chave = sucessor->chave;
            nodo->filhoDireita = removerNodo(nodo->filhoDireita, sucessor->chave);
        }
        return balancear(nodo);
    }

/*12 - Destruir AVL: Precisa ser recursiva pois preciso destruir a AVL seguindo o percurso pós-ordem (E-D-R), ou seja, primeiro os filhos e depois a raiz
Então o caso base da recursão é não ter mais filhos para destruir
Primeiro destruir filhoEsquerda, depois filhoDireita e depois raiz
*/
    void destruir(Nodo<T>* nodo) {
        if (nodo == nullptr) {
            return;
        }
        destruir(nodo->filhoEsquerda);
        destruir(nodo->filhoDireita);
        delete nodo;
    }

// Funções emOrdem, preOrdem, posOrdem - Imprime percursos em uma lista encadeada simples
// Precisam ser recursivas pois preciso percorrer a AVL em percursos não lineares
// Elas se diferenciam no momento em que minha recursão processa a raiz (nodo)

/*Em ordem: E-Nodo-D - Em ordem crescente
Caso Base da recursão é o nodo não tener filhos (nodo atual = nullptr)
Primeiro salvo o filhoEsquerda, depois o nodo atual (raiz da subárvore), depois o filhoDireita
Inserir no fim pois os dados a direita (maiores) vão shiftando os menores para a esquerda, mantendo a ordem
*/
    void emOrdemAux(Nodo<T>* nodo, MinhaListaEncadeada<T>* lista) const {
        if (nodo == nullptr) return;

        emOrdemAux(nodo->filhoEsquerda, lista);
        lista->inserirNoFim(nodo->chave);
        emOrdemAux(nodo->filhoDireita, lista);
    }

/*Pré Ordem: Nodo-E-D - Primeiro processa as raízes e depois filhoEsquerda e filhos Direita
Descendo a AVL até o caso base (nodo=nullptr)
Primeiro imprimindo as raízes, depois os filhoEsquerda e por fim filhosDireita recursivamente
*/
    void preOrdemAux(Nodo<T>* nodo, MinhaListaEncadeada<T>* lista) const {
        if (nodo == nullptr) return;

        lista->inserirNoFim(nodo->chave);
        preOrdemAux(nodo->filhoEsquerda, lista);
        preOrdemAux(nodo->filhoDireita, lista);
    }

/*Pós Ordem: E-D-Nodo - Folhas primeiro e depois nodo atual (raiz subárvore). Raiz da AVL é a última a ser processada (imprimida na lista)
Percurso explora primeiro as subárvores a esquerda, depois a direita e por fim o nodo atual recursivamente
*/
    void posOrdemAux(Nodo<T>* nodo, MinhaListaEncadeada<T>* lista) const {
        if (nodo == nullptr) return;

        posOrdemAux(nodo->filhoEsquerda, lista);
        posOrdemAux(nodo->filhoDireita, lista);
        lista->inserirNoFim(nodo->chave);
    }

/*Decidi por usar funções auxiliares para desenvolver encapsulamento e proteção da estrutura de dados.
Então os métodos públicos acessam indiretamente os privados
*/
public:

// Destrutor: Chama a auxiliar DestruirAVL, começa pela raiz e destrói recursivamente 
    virtual ~MinhaArvoreAVL() {
        destruir(this->raiz);
    };

// Vazia: Verifica se a raiz é nula, ou seja, AVL não tem nodos
    virtual bool vazia() const {
        return this->raiz == nullptr;
    };

// Quantidade de nodos: Deve chamar a função auxiliar quantidade
    virtual int quantidade() const {
        return quantidadeNodos(this->raiz);
    };

// Contem: Uso a função auxiliar buscarNodo para verificar se o dado existe na AVL
// Se buscarNodo retornar um ponteiro diferente de nullptr, dado existe e contem retorna true, senão false
    virtual bool contem(T chave) const {
        return buscarNodo(this->raiz, chave) != nullptr;
    };

/*Altura: Primeiro preciso declarar um ponteiro para o nodo que quer saber a altura, usando buscarNodo para encontrar o nodo do dado fornecido
A função é optional pois pode existir o dado ou não, e se existir, a função deve retornar um int (Atual->altura)
Se o nodo for nulo, return std::nullopt 
*/
    virtual std::optional<int> altura(T chave) const {
        Nodo<T>* nodo = buscarNodo(this->raiz, chave);

        if (nodo == nullptr) {
            return std::nullopt;
        }
        return nodo->altura;
    };

// Inserir: Aqui vou usar a função auxiliar InserirNodo. Começando o percurso pela raiz
    virtual void inserir(T chave) {
        this->raiz = inserirNodo(this->raiz, chave);
    };

// Remover: Mesmo padrão de inserir
    virtual void remover(T chave) {
        this->raiz = removerNodo(this->raiz, chave);
    };

/*Retornando dado do filhoEsquerda 
Primeiro preciso chamar BuscarNodo para percorrer a AVL a esquerda
Se meu nodo atual for nullptr ou se não existir filhoEsquerda, devo retornar nullopt
Se o filhoEsquerda (nodo->filhoEsquerda) existir, retorno o dado desse nodo
*/
    virtual std::optional<T> filhoEsquerdaDe(T chave) const {
        Nodo<T>* nodo = buscarNodo(this->raiz, chave);

        if (nodo == nullptr || nodo->filhoEsquerda == nullptr) {
            return std::nullopt;
        }
        return nodo->filhoEsquerda->chave;
    };

/*Retornando o dado do filhoDireita
Mesma lógica aqui, porém retorno nodo->filhoDireita->dado
*/
    virtual std::optional<T> filhoDireitaDe(T chave) const {
        Nodo<T>* nodo = buscarNodo(this->raiz, chave);

        if (nodo == nullptr || nodo->filhoDireita == nullptr) {
            return std::nullopt;
        }
        return nodo->filhoDireita->chave;
    };

/*Imprimir percurso em Ordem
Primeiro preciso declarar um objeto ListaEncadeada listaEmOrdem e depois passando a raiz da AVL e listaEmOrdem como parâmetros de emOrdem()
*/
    virtual ListaEncadeadaAbstrata<T>* emOrdem() const {
        MinhaListaEncadeada<T>* lista = new MinhaListaEncadeada<T>();
        emOrdemAux(this->raiz, lista);
        return lista;
    };

// O mesmo é feito para os percursos Pré e Pós ordem

    virtual ListaEncadeadaAbstrata<T>* preOrdem() const {
        MinhaListaEncadeada<T>* lista = new MinhaListaEncadeada<T>();
        preOrdemAux(this->raiz, lista);
        return lista;
    };

    virtual ListaEncadeadaAbstrata<T>* posOrdem() const {
        MinhaListaEncadeada<T>* lista = new MinhaListaEncadeada<T>();
        posOrdemAux(this->raiz, lista);
        return lista;
    };
};

#endif