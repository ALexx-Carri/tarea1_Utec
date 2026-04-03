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
    friend Tensor operator-(const Tensor& tensor1, const Tensor& tensor2) {
        if (tensor1.shape != tensor2.shape) {
            throw invalid_argument("Dimensiones no compatibles para la restar");
        }
        vector<double> resultado(tensor1.total_size);
        for (size_t i = 0; i < tensor1.total_size; i++) {
            resultado[i] = tensor1.datos[i] - tensor2.datos[i];
        }
        return Tensor(tensor1.shape, resultado);
    }
    friend Tensor operator*(const Tensor& tensor1, const Tensor& tensor2) {
        // 1. Validar dimensiones (M x N) * (N x P) -> (M x P)
        if (tensor1.shape.size() != 2 || tensor2.shape.size() != 2 || tensor1.shape[1] != tensor2.shape[0]) {
            throw invalid_argument("Dimensiones no compatibles para multiplicar matrices");
        }

        size_t M = tensor1.shape[0]; // Filas de T1
        size_t N = tensor1.shape[1]; // Columnas de T1 (y filas de T2)
        size_t P = tensor2.shape[1]; // Columnas de T2

        vector<size_t> nuevo_shape = {M, P};
        vector<double> res_values(M * P, 0.0);

        for (size_t i = 0; i < M; i++) {         // Recorre filas de T1
            for (size_t j = 0; j < P; j++) {     // Recorre columnas de T2
                double suma = 0.0;

                // 2. Bucle interno: hace el producto punto de la fila i y la columna j
                for (size_t k = 0; k < N; k++) {
                    // T1[i][k] * T2[k][j]
                    double valA = tensor1.datos[i * N + k];
                    double valB = tensor2.datos[k * P + j];
                    suma += valA * valB;
                }

                // 3. Guardar el resultado en la posición [i][j] de la matriz resultante
                res_values[i * P + j] = suma;
            }
        }
        return Tensor(nuevo_shape, res_values);
    }
};
int main() {
    try {
        Tensor t1({2, 2}, {1.0, 2.0, 3.0, 4.0});
        Tensor t2({2, 2}, {5.0, 6.0, 7.0, 8.0});
        Tensor t3 = t2*t1;
        cout<<"Primera matriz:\n"<<t1<<endl;
        cout<<"Segunda matriz:\n"<<t2<<endl;
        cout << "Resultado suma: " << t3 << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}