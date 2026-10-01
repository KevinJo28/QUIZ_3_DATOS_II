#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;
using namespace std::chrono;
int binarySearch(const vector<int>& nums, int inf, int sup, int dato) {
    //Busqueda Binaria
    while (inf <= sup) {
        int mitad = (inf+sup)/2;
        if (nums[mitad] == dato) {
            return mitad;
        }
        if (nums[mitad] > dato) {
            sup = mitad - 1;
        }
        else if (nums[mitad] < dato) {
            inf = mitad + 1;

        }
    }
    return -1;
}


void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int i = 0; i < n2; i++) {
        R[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i]; i++;
        }
        else {
            arr[k] = R[j]; j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}



int main() {
    ofstream file("benchmark_resultados.csv");
    file << "N,MergeSort_ms,BinarySearch_ns\n";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10000000); // Valores aleatorios

    // Tamaños de arreglos a probar
    vector<int> sizes = {10000, 50000, 100000, 500000, 1000000, 2000000, 5000000};

    cout << "Iniciando Benchmark...\n";

    for (int n : sizes) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = dis(gen);

        // 1. Benchmark de MergeSort
        vector<int> arrCopy = arr;
        auto startMS = high_resolution_clock::now();
        mergeSort(arrCopy, 0, n - 1);
        auto stopMS = high_resolution_clock::now();
        auto durationMS = duration_cast<milliseconds>(stopMS - startMS).count();

        // 2. Benchmark de Búsqueda Binaria
        int num_searches = 10000;
        vector<int> targets(num_searches);
        for (int i = 0; i < num_searches; i++) targets[i] = dis(gen);

        auto startBS = high_resolution_clock::now();
        for(int i = 0; i < num_searches; i++){
            // arrCopy ya está ordenado por el paso anterior
            binarySearch(arrCopy, 0, n - 1, targets[i]);
        }
        auto stopBS = high_resolution_clock::now();
        // Calculamos los nanosegundos promedio por búsqueda
        auto durationBS = duration_cast<nanoseconds>(stopBS - startBS).count();
        double avg_bs_ns = (double)durationBS / num_searches;

        cout << "N: " << n << " | MergeSort: " << durationMS << " ms | Busqueda Binaria: " << avg_bs_ns << " ns\n";
        file << n << "," << durationMS << "," << avg_bs_ns << "\n";
    }

    file.close();
    cout << "\nDatos exportados a benchmark_resultados.csv exitosamente.\n";
    return 0;

}