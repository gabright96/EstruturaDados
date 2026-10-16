#ifndef MINHA_ARVORE_AVL_HPP
#define MINHA_ARVORE_AVL_HPP

#include "ArvoreBinariaDeBusca.h"
#include <optional>

/**
 * @brief Representa uma árvore AVL.
 *
 * @tparam T O tipo de dado guardado na árvore.
 */
template <typename T>
class MinhaArvoreAVL final : public ArvoreBinariaDeBusca<T>
{
private:
    int _quantidade{0};

    int alturaNodo(Nodo<T>* nodo) const
    {
        if(nodo == nullptr)
            return -1;

        return nodo->altura;
    }

    int maximo(int a, int b) const
    {
        if(a > b)
            return a;

        return b;
    }

    void atualizarAltura(Nodo<T>* nodo)
    {
        if(nodo != nullptr)
        {
            nodo->altura =
                1 + maximo(
                    alturaNodo(nodo->filhoEsquerda),
                    alturaNodo(nodo->filhoDireita)
                );
        }
    }

    int fatorBalanceamento(Nodo<T>* nodo) const
    {
        if(nodo == nullptr)
            return 0;

        return alturaNodo(nodo->filhoEsquerda)
             - alturaNodo(nodo->filhoDireita);
    }

    Nodo<T>* rotacaoDireita(Nodo<T>* y)
    {
        Nodo<T>* x = y->filhoEsquerda;
        Nodo<T>* t2 = x->filhoDireita;

        x->filhoDireita = y;
        y->filhoEsquerda = t2;

        atualizarAltura(y);
        atualizarAltura(x);

        return x;
    }

    Nodo<T>* rotacaoEsquerda(Nodo<T>* x)
    {
        Nodo<T>* y = x->filhoDireita;
        Nodo<T>* t2 = y->filhoEsquerda;

        y->filhoEsquerda = x;
        x->filhoDireita = t2;

        atualizarAltura(x);
        atualizarAltura(y);

        return y;
    }

    Nodo<T>* balancear(Nodo<T>* nodo)
    {
        atualizarAltura(nodo);

        int fb = fatorBalanceamento(nodo);

        if(fb > 1)
        {
            if(fatorBalanceamento(nodo->filhoEsquerda) < 0)
            {
                nodo->filhoEsquerda =
                    rotacaoEsquerda(nodo->filhoEsquerda);
            }

            return rotacaoDireita(nodo);
        }

        if(fb < -1)
        {
            if(fatorBalanceamento(nodo->filhoDireita) > 0)
            {
                nodo->filhoDireita =
                    rotacaoDireita(nodo->filhoDireita);
            }

            return rotacaoEsquerda(nodo);
        }

        return nodo;
    }

    Nodo<T>* inserirRec(Nodo<T>* nodo, T chave)
    {
        if(nodo == nullptr)
        {
            Nodo<T>* novo = new Nodo<T>;
            novo->chave = chave;
            novo->altura = 0;

            _quantidade++;

            return novo;
        }

        if(chave < nodo->chave)
        {
            nodo->filhoEsquerda =
                inserirRec(nodo->filhoEsquerda, chave);
        }
        else if(chave > nodo->chave)
        {
            nodo->filhoDireita =
                inserirRec(nodo->filhoDireita, chave);
        }

        return balancear(nodo);
    }

    Nodo<T>* menorNodo(Nodo<T>* nodo) const
    {
        Nodo<T>* atual = nodo;

        while(atual->filhoEsquerda != nullptr)
        {
            atual = atual->filhoEsquerda;
        }

        return atual;
    }

    Nodo<T>* removerRec(Nodo<T>* nodo, T chave)
    {
        if(nodo == nullptr)
            return nullptr;

        if(chave < nodo->chave)
        {
            nodo->filhoEsquerda =
                removerRec(nodo->filhoEsquerda, chave);
        }
        else if(chave > nodo->chave)
        {
            nodo->filhoDireita =
                removerRec(nodo->filhoDireita, chave);
        }
        else
        {
            if(
                nodo->filhoEsquerda == nullptr &&
                nodo->filhoDireita == nullptr
            )
            {
                delete nodo;
                _quantidade--;

                return nullptr;
            }

            if(nodo->filhoEsquerda == nullptr)
            {
                Nodo<T>* temp = nodo->filhoDireita;

                delete nodo;
                _quantidade--;

                return temp;
            }

            if(nodo->filhoDireita == nullptr)
            {
                Nodo<T>* temp = nodo->filhoEsquerda;

                delete nodo;
                _quantidade--;

                return temp;
            }

            Nodo<T>* sucessor =
                menorNodo(nodo->filhoDireita);

            nodo->chave = sucessor->chave;

            nodo->filhoDireita =
                removerRec(
                    nodo->filhoDireita,
                    sucessor->chave
                );
        }

        return balancear(nodo);
    }

    Nodo<T>* buscarNodo(
        Nodo<T>* nodo,
        T chave
    ) const
    {
        if(nodo == nullptr)
            return nullptr;

        if(chave == nodo->chave)
            return nodo;

        if(chave < nodo->chave)
        {
            return buscarNodo(
                nodo->filhoEsquerda,
                chave
            );
        }

        return buscarNodo(
            nodo->filhoDireita,
            chave
        );
    }

    void destruir(Nodo<T>* nodo)
    {
        if(nodo == nullptr)
            return;

        destruir(nodo->filhoEsquerda);
        destruir(nodo->filhoDireita);

        delete nodo;
    }

    void emOrdemAux(
        Nodo<T>* nodo,
        MinhaListaEncadeada<T>* lista
    ) const
    {
        if(nodo == nullptr)
            return;

        emOrdemAux(
            nodo->filhoEsquerda,
            lista
        );

        lista->inserirNoFim(nodo->chave);

        emOrdemAux(
            nodo->filhoDireita,
            lista
        );
    }

    void preOrdemAux(
        Nodo<T>* nodo,
        MinhaListaEncadeada<T>* lista
    ) const
    {
        if(nodo == nullptr)
            return;

        lista->inserirNoFim(nodo->chave);

        preOrdemAux(
            nodo->filhoEsquerda,
            lista
        );

        preOrdemAux(
            nodo->filhoDireita,
            lista
        );
    }

    void posOrdemAux(
        Nodo<T>* nodo,
        MinhaListaEncadeada<T>* lista
    ) const
    {
        if(nodo == nullptr)
            return;

        posOrdemAux(
            nodo->filhoEsquerda,
            lista
        );

        posOrdemAux(
            nodo->filhoDireita,
            lista
        );

        lista->inserirNoFim(nodo->chave);
    }

public:
    virtual ~MinhaArvoreAVL(){
        destruir(this->raiz);
    };

    /**
     * @brief Verifica se a arvore esta vazia
     * @return Verdade se a arvore esta vazia.
     */
    virtual bool vazia() const
    {
        return _quantidade == 0;
    };

    /**
     * @brief Retornar quantidade de chaves na arvore
     * @return Numero natural que representa a quantidade de chaves na arvore
     */
    virtual int quantidade() const
    {
        return _quantidade;
    };

    /**
     * @brief Verifica se a arvore contem uma chave
     * @param chave chave a ser procurada na arvore
     * @return Verdade se a arvore contem a chave
     */
    virtual bool contem(T chave) const
    {
        return buscarNodo(
            this->raiz,
            chave
        ) != nullptr;
    };

    /**
     * @brief Retorna a altura da (sub)arvore
     * @param chave chave que é raiz da (sub)arvore cuja altura queremos.
     * @return Numero inteiro representando a altura da (subarvore). Se chave nao esta na arvore, retorna std::nullopt
     */
    virtual std::optional<int> altura(T chave) const
    {
        Nodo<T>* nodo =
            buscarNodo(
                this->raiz,
                chave
            );

        if(nodo == nullptr)
            return std::nullopt;

        return nodo->altura;
    };

    /**
     * @brief Insere uma chave na arvore
     * @param chave chave a ser inserida
     */
    virtual void inserir(T chave){
        this->raiz =
            inserirRec(
                this->raiz,
                chave
            );
    };

    /**
     * @brief Remove uma chave da arvore
     * @param chave chave a removida
     */
    virtual void remover(T chave){
        if(!contem(chave))
            return;

        this->raiz =
            removerRec(
                this->raiz,
                chave
            );
    };

    /**
     * @brief Busca a chave do filho a esquerda de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a esquerda
     * @return Chave do filho a esquerda. Se chave nao esta na arvore, retorna std::nullopt
     */
    virtual std::optional<T> filhoEsquerdaDe(T chave) const
    {
        Nodo<T>* nodo =
            buscarNodo(
                this->raiz,
                chave
            );

        if(
            nodo == nullptr ||
            nodo->filhoEsquerda == nullptr
        )
        {
            return std::nullopt;
        }

        return nodo->filhoEsquerda->chave;
    };

    /**
     * @brief Busca a chave do filho a direita de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a direita
     * @return Chave do filho a direita. Se chave nao esta na arvore, retorna nullptr
     */
    virtual std::optional<T> filhoDireitaDe(T chave) const
    {
        Nodo<T>* nodo =
            buscarNodo(
                this->raiz,
                chave
            );

        if(
            nodo == nullptr ||
            nodo->filhoDireita == nullptr
        )
        {
            return std::nullopt;
        }

        return nodo->filhoDireita->chave;
    };

    /**
     * @brief Lista chaves visitando a arvore em ordem
     * @return Lista encadeada contendo as chaves em ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *emOrdem() const
    {
        MinhaListaEncadeada<T>* lista =
            new MinhaListaEncadeada<T>;

        emOrdemAux(
            this->raiz,
            lista
        );

        return lista;
    };

    /**
     * @brief Lista chaves visitando a arvore em pre-ordem
     * @return Lista encadeada contendo as chaves em pre-ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *preOrdem() const
    {
        MinhaListaEncadeada<T>* lista =
            new MinhaListaEncadeada<T>;

        preOrdemAux(
            this->raiz,
            lista
        );

        return lista;
    };

    /**
     * @brief Lista chaves visitando a arvore em pos-ordem
     * @return Lista encadeada contendo as chaves em pos ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *posOrdem() const
    {
        MinhaListaEncadeada<T>* lista =
            new MinhaListaEncadeada<T>;

        posOrdemAux(
            this->raiz,
            lista
        );

        return lista;
    };
};

#endif