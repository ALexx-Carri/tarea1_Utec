#include <iostream>
#include <vector>
#include <stdexcept> // Para invalid_argument
using namespace std;
class Tensor {
    double* datos;
    vector<size_t> shape;
    size_t total_size;
public:
    Tensor(const vector<size_t>& tamaño, const vector<double>& values) : shape(tamaño) {
    for (size_t i = 0; i < shape.size(); i++) {
        if (shape[i] ==0) {throw invalid_argument("No puedes crear una matriz de 0 dimensiones");}
    }
        total_size = 1;
        for (size_t dim : shape) total_size *= dim;
        if (values.size() != total_size) {
            throw invalid_argument("El tamaño de values no coincide con la forma (shape)");
        }
        datos = new double[total_size];
        for (size_t i = 0; i < total_size; i++) {
            datos[i] = values[i];
        }
    }
    ~Tensor() {
        delete[] datos;
    }
    friend ostream& operator<<(ostream& os, const Tensor& t) {
        for (size_t i = 0; i < t.total_size; i++) {
            os << t.datos[i] << " ";
        }
        return os;
    }

    friend Tensor operator+(const Tensor& tensor1, const Tensor& tensor2) {
        if (tensor1.shape != tensor2.shape) {
            throw invalid_argument("Dimensiones no compatibles para la suma");
        }

        vector<double> resultado(tensor1.total_size);
        for (size_t i = 0; i < tensor1.total_size; i++) {
            resultado[i] = tensor1.datos[i] + tensor2.datos[i];
        }
        return Tensor(tensor1.shape, resultado);
    }
};
int main() {
    try {
        Tensor t1({2, 2}, {1.0, 2.0, 3.0, 4.0});
        Tensor t2({2, 2}, {5.0, 6.0, 7.0, 8.0});
        Tensor t3 = t1 + t2;
        cout<<"Primera matriz:\n"<<t1<<endl;
        cout<<"Segunda matriz:\n"<<t2<<endl;
        cout << "Resultado suma: " << t3 << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}