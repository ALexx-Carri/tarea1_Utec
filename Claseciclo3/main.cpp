#include <iostream>
#include <vector>
using namespace std;


class Tensor {
    int** ptr;
    int fil, col;
public:
    Tensor(int filas, int cols): fil(filas), col(cols) {
        ptr = new int*[fil];
        for ( int i = 0; i < fil; i++) {
            ptr[i] = new int[col];
            for ( int j = 0; j < col; j++) {
                ptr[i][j] = rand()%10;
            }
        }

    }
    Tensor operator+(const Tensor& matriz) {
        Tensor out(fil:matriz.fil, col:matriz.col);

    }
    ~Tensor() {
        for (int i = 0; i < fil; i++) {
            delete[] ptr[i];
        }
        delete[]ptr;
    }
    ostream& operator<<(ostream& os, const Tensor& matriz) {
        for ( int i = 0; i < matriz.fil; i++) {
            for (int j= 0; j < matriz.col;j++) {
                os << matriz.ptr[i][j];
            }
        }
    }
};
int main() {

    return 0;


}
