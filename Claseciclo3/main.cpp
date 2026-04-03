#include <iostream>
using namespace std;


class Mat20 {
    int** ptr;
    int fil, col;
public:
    Mat20(int filas, int cols): fil(filas), col(cols) {
        ptr = new int*[fil];
        for ( int i = 0; i < fil; i++) {
            ptr[i] = new int[col];
            for ( int j = 0; j < col; j++) {
                ptr[i][j] = rand()%10;
            }
        }

    }
    Mat20 operator+(const Mat20& matriz) {
        Mat20 out(fil:matriz.fil, col:matriz.col);

    }
    ~Mat20() {
        for (int i = 0; i < fil; i++) {
            delete[] ptr[i];
        }
        delete[]ptr;
    }
    ostream& operator<<(ostream& os, const Mat20& matriz) {
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