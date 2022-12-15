#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "time.h"
#include <fstream>
#include <string>
//#include <mpi.h>
using namespace std;

void mull(double*& A, double*& B, double*& C, int &Size)
{
    int s = Size;
    int p, ind;

    MPI_Status st;
    int ProcPartSize = s / ProcNum;
    int ProcPartElem = ProcPartSize * s;
    double* bufA = new double[s * ProcPartSize];
    double* bufB = new double[s * ProcPartSize];
    double* bufC = new double[s * ProcPartSize];
    int ProcPart = s / ProcNum, part = ProcPart * s;

    MPI_Scatter(A, part, MPI_DOUBLE, bufA, part, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, part, MPI_DOUBLE, bufB, part, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double temp = 0.0;
    for (int i = 0; i < ProcPartSize; i++) {
        for (int j = 0; j < ProcPartSize; j++) {
            for (int k = 0; k < s; k++)
                temp += bufA[i * s + k] * bufB[k * s + j];
            bufC[i * s + j + ProcPartSize * ProcRank] = temp;
        }
        temp = 0.0;
    }
}

double go(double* a, double* b, double* c, int N)
{
    a = new double [N*N];
    cout << "Ввод элементов первой матрицы..." << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            a[i*N + j] = rand() % 10;
        }
    }
    ofstream fout;
    fout.open("A.txt");


    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            fout << a[i*N+j] << ' ';
        fout << '\n';
    }
    fout.close();
    cout << "comlete write file A.txt!\n";





    ifstream fin;
    fin.open("A.txt");
    double* A = new double [N*N];
    cout << "new A: \n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fin >> A[i*N+j];
        }
    }
    fin.close();



    b = new double [N*N];
    cout << "Ввод элементов второй матрицы..." << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            b[i*N+j] = rand() % 10;
        }
    }

    fout.open("B.txt");

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            fout << b[i*N+j] << ' ';
        fout << '\n';

    }
    fout.close();
    cout << "comlete write file B.txt!\n";


    fin.open("B.txt");
    double* B = new double [N*N];
    cout << "new B: \n";
    for (int i = 0; i < N; i++)
    {
        
        for (int j = 0; j < N; j++)
        {
            fin >> B[i*N+j];
        }
    }
    fin.close();
    cout << "complete read files!\n";

    c = new double [N*N];
    size_t count = 0;
    clock_t start = clock();
    mull(A, B, c, N);
    
    
    double result_time = double(clock() - start) / CLOCKS_PER_SEC;
    cout << endl << "Calculation time:" << result_time << " seconds" << endl;
    cout << "Матрица произведения посчитана!" << endl;
    fout.open("C.txt");

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            fout << c[i*N+j] << ' ';
        fout << '\n';
    }
    fout.close();

    return result_time;
}

int main()
{
    int N = 500;
    double** a = NULL, ** b = NULL, ** c = NULL;
    system("cls");

    ofstream fio;
    fio.open("info.txt");
    for (int i = 0; i < 9; i++)
    {
        fio << N;
        fio << '\n';
        fio << "Calculation time for " << N << '*' << N << " matrix = " << go(a, b, c, N) << " seconds\n";
        N += 250;
        cout << i + 1 << endl;
    }


    fio.close();

    return 0;
}