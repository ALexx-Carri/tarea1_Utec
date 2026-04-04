#include <iostream>
#include <vector>
#include <stdexcept> // Para invalid_argument
using namespace std;

class Tensor {
private:
    double* datos;
    vector<size_t> shape;
    size_t total_size;

public:
    Tensor(const vector<size_t>& tamaño, const vector<double>& values) : shape(tamaño) {
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
    Tensor view(const vector<size_t>& new_shape) const {

        if (new_shape.size() == 0 || new_shape.size() > 3) {
            throw invalid_argument("La nueva forma debe tener entre 1 y 3 dimensiones");
        }
        size_t new_total = 1;
        for (size_t dim : new_shape) {
            new_total *= dim;
        }
        if (new_total != total_size) {
            throw invalid_argument("El numero total de elementos debe mantenerse");
        }
        Tensor nuevo = *this;
        nuevo.shape = new_shape;

        return nuevo;
    }
    Tensor unsqueeze(size_t dim) const {
        if (shape.size() >= 3) {
            throw invalid_argument("No se pueden tener mas de 3 dimensiones");
        }
        if (dim > shape.size()) {
            throw invalid_argument("Posicion de dimension invalida");
        }
        vector<size_t> new_shape = shape;
        new_shape.insert(new_shape.begin() + dim, 1);
        Tensor nuevo = *this;
        nuevo.shape = new_shape;
        return nuevo;
    }

    friend ostream& operator<<(ostream& os, const Tensor& t) {
        if (t.total_size == 0) {return os << "[]";}
        size_t dims = t.shape.size();
        if (dims == 1) {
            os << "[";
            for (size_t i = 0; i < t.total_size; ++i) {
                os << t.datos[i] << (i == t.total_size - 1 ? "" : ", ");
            }
            os << "]";
        }
        else if (dims == 2) {

            size_t filas = t.shape[0];
            size_t columnas = t.shape[1];
            os << "[";
            for (size_t i = 0; i < filas; i++) {
                os << "[";
                for (size_t j = 0; j < columnas; j++) {
                    os << t.datos[i * columnas + j] << (j == columnas - 1 ? "" : ", ");
                }
                os << "]" << (i == filas - 1 ? "" : ",\n");
            }
            os << "]";
        }
        else if (dims == 3) {
            size_t profundidad = t.shape[0];
            size_t filas = t.shape[1];
            size_t columnas = t.shape[2];
            os << "[\n";
            for (size_t d = 0; d < profundidad; d++) {
                os << " [\n";
                for (size_t j = 0; j < filas; j++) {
                    os << " [\n";
                    for (size_t k = 0; k < columnas; k++) {
                        os << t.datos[d * filas * columnas + d * columnas + j] << (j == columnas - 1 ? "" : ", ");
                    }
                    os << "]" << (j == filas - 1 ? "" : ",\n");
                }
                os << "\n ]" << (d == profundidad - 1 ? "" : ",\n");
            }
            os << "\n]";
        }
        return os;
    }
    static Tensor zeros(const vector<size_t>& shape) {
        size_t size=1;
        for (size_t dimension : shape) size *= dimension;
        vector<double> valores(size,0.0);
        return Tensor(shape, valores);
    }
    static Tensor ones(const vector<size_t>& shape) {
        size_t size=1;
        for (size_t dimension : shape) size *= dimension;
        vector<double> valores(size,1.0);
        return Tensor(shape, valores);
    }
    static Tensor random(const vector<size_t>& shape,double minimo,double maximo) {
        size_t size=1;
        for (size_t dimension : shape) {
            if (dimension == 0)throw invalid_argument("Dimension no puede ser 0");
            size *= dimension;
        }
        vector<double> valores(size);
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(minimo,maximo);
        for (size_t i=0;i<size;i++) {
            valores[i]=dis(gen);
        }
        return Tensor(shape, valores);
    }
    static Tensor arange(double start, double stop) {
        if (stop <= start) {
            throw std::invalid_argument("El valor de 'stop' debe ser mayor que 'start'");
        }
        size_t tamano = static_cast<size_t>(stop - start);
        std::vector<double> valores;
        valores.reserve(tamano);
        for (double i = start; i < stop; ++i) {
            valores.push_back(i);
        }
        std::vector<size_t> shape_1D = { valores.size() };
        return Tensor(shape_1D, valores);
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
    friend Tensor operator *(const Tensor& a, const Tensor& b) {
        if (a.shape != b.shape) {
            throw invalid_argument("Dimensiones no compatibles para la multiplicacion de elemento a elemento");
        }
        vector<double> resultado(a.total_size);
        for (size_t i = 0; i < a.total_size; i++) {
            resultado[i] = a.datos[i] * b.datos[i];
        }
        return Tensor(a.shape, resultado);
    }
    friend Tensor operator *(const Tensor& a, const double& b) {
        vector<double> resultado(a.total_size);
        for (size_t i = 0; i < a.total_size; i++) {
            resultado[i] = a.datos[i] * b;
        }
        return Tensor(a.shape, resultado);
    }
    friend Tensor matmul(const Tensor& tensor1, const Tensor& tensor2) {
        if (tensor1.shape.size() != 2 || tensor2.shape.size() != 2 || tensor1.shape[1] != tensor2.shape[0]) {
            throw invalid_argument("Dimensiones no compatibles para multiplicar matrices");
        }
        size_t M = tensor1.shape[0];
        size_t N = tensor1.shape[1];
        size_t P = tensor2.shape[1];
        vector<size_t> nuevo_shape = {M, P};
        vector<double> res_values(M * P, 0.0);
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < P; j++) {
                double suma = 0;
                for (size_t k = 0; k < N; k++) {
                    suma += tensor1.datos[i * N + k] * tensor2.datos[k * P + j];
                }
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
        Tensor t3 = t2 * t1;
        cout << "Primera matriz:\n" << t1 << endl;
        cout << "Segunda matriz:\n" << t2 << endl;
        cout << "Resultado suma:\n" << t3 << endl;
        Tensor A({4}, {0,1,2,3});
        Tensor B = A.view({2,2});
        cout << "\nVista (punto 7):\n" << B << endl;
        Tensor C = A.unsqueeze(0);
        cout << "\nPunto 7::\n" << C << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
