#include <iostream>
#include <iomanip>

using namespace std;

const long long BASE = 1000000000;

struct Nodo{
    long long data;
    Nodo *next, *prev;
    Nodo(long long v) : data(v), next(nullptr), prev(nullptr) {}
};

class BigNum{
public:
    Nodo *begin, *end;
    int size;

    BigNum() : begin(nullptr), end(nullptr), size(0) {}

    // Constructor de copia para facilitar operaciones
    BigNum(const BigNum& o) : begin(nullptr), end(nullptr), size(0){
        Nodo* aux = o.begin;
        while(aux) { push_back(aux->data); aux = aux->next; }
    }

    void push_back(long long v){
        Nodo* nuevo = new Nodo(v);
        if (!begin) begin = end = nuevo;
        else { end->next = nuevo; nuevo->prev = end; end = nuevo; }
        size++;
    }

    void push_front(long long v){
        Nodo* nuevo = new Nodo(v);
        if (!begin) begin = end = nuevo;
        else { begin->prev = nuevo; nuevo->next = begin; begin = nuevo; }
        size++;
    }

    void sumar(const BigNum& otro){
        Nodo *p1 = end, *p2 = otro.end;
        long long carry = 0;
        while (p1 || p2 || carry){
            long long v1 = p1 ? p1->data : 0;
            long long v2 = p2 ? p2->data : 0;
            long long s = v1 + v2 + carry;
            if (p1) { p1->data = s % BASE; carry = s / BASE; p1 = p1->prev; }
            else { push_front(s % BASE); carry = s / BASE; }
            if (p2) p2 = p2->prev;
        }
    }

    void restar(const BigNum& otro){
        Nodo *p1 = end, *p2 = otro.end;
        long long borrow = 0;
        while (p1) {
            long long v2 = p2 ? p2->data : 0;
            p1->data -= v2 + borrow;
            if(p1->data < 0){ 
                p1->data += BASE; borrow = 1; 
            }
            else borrow = 0;
            p1 = p1->prev;
            if (p2) p2 = p2->prev;
        }
    }

    void multiplicar(long long esc){
        long long carry = 0;
        Nodo* curr = end;
        while(curr || carry){
            if(curr){
                long long p = curr->data * esc + carry;
                curr->data = p % BASE; carry = p / BASE;
                curr = curr->prev;
            }else{ 
                push_front(carry % BASE); carry /= BASE; 
            }
        }
    }

    void dividir(long long div){
        long long rem = 0;
        Nodo* curr = begin;
        while(curr){
            long long v = curr->data + rem * BASE;
            curr->data = v / div; rem = v % div;
            curr = curr->next;
        }
    }

    void imprimir(int d){
        if (!begin) return;
        cout << begin->data << ".";
        Nodo* curr = begin->next;
        for (int i = 0; i < d/9 && curr; ++i){
            cout << setfill('0') << setw(9) << curr->data;
            curr = curr->next;
        }
        cout << endl;
    }
};

void arctan(BigNum& res, int x, int precision){
    BigNum term;
    term.push_back(1);
    for(int i = 0; i < precision/9 + 3; i++) term.push_back(0);
    term.dividir(x);

    BigNum power = term;
    int k = 1;

    while(true){
        BigNum temp = power;
        temp.dividir(2 * k - 1);

        if(k % 2) res.sumar(temp);
        else      res.restar(temp);

        power.dividir(x * x);

        Nodo* p = power.begin;
        bool cero = true;
        for(int i = 0; i < precision/9; i++){
            if(p && p->data != 0){
                cero = false;
                break;
            }
            if(p) p = p->next;
        }
        if(cero) break;

        k++;
    }
}


int main(){
    const int DIGITOS = 20000;
    const int PRECISION = 22000;   // margen extra

    cout << "Calculando PI" << endl;

    BigNum a5, a239;

    arctan(a5, 5, PRECISION);
    a5.multiplicar(4);

    arctan(a239, 239, PRECISION);

    a5.restar(a239);
    a5.multiplicar(4);

    a5.imprimir(DIGITOS);
    return 0;
}
