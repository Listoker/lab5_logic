#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

void printByDegree(const char* title, const std::vector<int>& deg, int target) {
    std::cout << title << ": ";
    bool found = false;
    for (size_t i = 0; i < deg.size(); ++i) {
        if (deg[i] == target) {
            std::cout << "v" << i + 1 << " ";
            found = true;
        }
    }
    if (!found) std::cout << "нет";
    std::cout << "\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(nullptr)));

    int n, p;
    std::cout << "Количество вершин n: ";
    if (!(std::cin >> n) || n <= 0) return 0;

    std::cout << "Вероятность появления ребра/дуги (0-100%): ";
    if (!(std::cin >> p) || p < 0 || p > 100) return 0;

    // ЧАСТЬ 1
    std::cout << "\n================ НЕОРИЕНТИРОВАННЫЙ ГРАФ ================\n";
    std::vector<std::vector<int>> A(n, std::vector<int>(n, 0));
    std::vector<std::pair<int, int>> edges;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            A[i][j] = A[j][i] = (rand() % 100 < p) ? 1 : 0;
            if (A[i][j]) edges.push_back({ i, j });
        }
    }

    std::cout << "\n--- Матрица смежности ---\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) std::cout << A[i][j] << " ";
        std::cout << "\n";
    }

    int m_adj = 0;
    std::vector<int> degA(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) degA[i] += A[i][j];
        for (int j = i + 1; j < n; ++j) m_adj += A[i][j];
    }

    std::cout << "Размер графа |E|: " << m_adj << "\n";
    printByDegree("Изолированные (deg=0)", degA, 0);
    printByDegree("Концевые (deg=1)", degA, 1);
    printByDegree("Доминирующие (deg=n-1)", degA, n > 1 ? n - 1 : -1);

    int m = (int)edges.size();
    std::vector<std::vector<int>> B(n, std::vector<int>(m, 0));
    for (int j = 0; j < m; ++j) {
        B[edges[j].first][j] = 1;
        B[edges[j].second][j] = 1;
    }

    std::cout << "\n--- Матрица инцидентности (" << n << "x" << m << ") ---\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) std::cout << B[i][j] << " ";
        std::cout << "\n";
    }

    std::vector<int> degB(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) degB[i] += B[i][j];
    }

    std::cout << "Размер графа |E| (число столбцов): " << m << "\n";
    printByDegree("Изолированные (deg=0)", degB, 0);
    printByDegree("Концевые (deg=1)", degB, 1);
    printByDegree("Доминирующие (deg=n-1)", degB, n > 1 ? n - 1 : -1);


    // ЧАСТЬ 2
    std::cout << "\n================ ОРИЕНТИРОВАННЫЙ ГРАФ ================\n";
    std::vector<std::vector<int>> dirA(n, std::vector<int>(n, 0));
    std::vector<std::pair<int, int>> arcs;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                dirA[i][j] = (rand() % 100 < p) ? 1 : 0;
                if (dirA[i][j]) arcs.push_back({ i, j });
            }
        }
    }

    std::cout << "\n--- Матрица смежности (орграф) ---\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) std::cout << dirA[i][j] << " ";
        std::cout << "\n";
    }

    int m_dir_adj = 0;
    std::vector<int> degOutA(n, 0), degInA(n, 0), degTotalA(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            degOutA[i] += dirA[i][j]; 
            degInA[i] += dirA[j][i]; 
            m_dir_adj += dirA[i][j];
        }
        degTotalA[i] = degOutA[i] + degInA[i];
    }

    std::cout << "Размер орграфа |E|: " << m_dir_adj << "\n";
    printByDegree("Изолированные (deg=0)", degTotalA, 0);
    printByDegree("Концевые / висячие (deg=1)", degTotalA, 1);
    printByDegree("Источники (deg_in=0)", degInA, 0);
    printByDegree("Стоки (deg_out=0)", degOutA, 0);
    printByDegree("Доминирующие", degOutA, n > 1 ? n - 1 : -1);

    int m_dir = (int)arcs.size();
    std::vector<std::vector<int>> dirB(n, std::vector<int>(m_dir, 0));
    for (int j = 0; j < m_dir; ++j) {
        dirB[arcs[j].first][j] = -1; 
        dirB[arcs[j].second][j] = 1; 
    }

    std::cout << "\n--- Матрица инцидентности орграфа (" << n << "x" << m_dir << ") ---\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m_dir; ++j) std::cout << std::setw(2) << dirB[i][j] << " ";
        std::cout << "\n";
    }

    std::vector<int> degOutB(n, 0), degInB(n, 0), degTotalB(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m_dir; ++j) {
            if (dirB[i][j] == -1) degOutB[i]++;
            if (dirB[i][j] == 1)  degInB[i]++;
        }
        degTotalB[i] = degOutB[i] + degInB[i];
    }

    std::cout << "Размер орграфа |E| (число столбцов): " << m_dir << "\n";
    printByDegree("Изолированные (deg=0)", degTotalB, 0);
    printByDegree("Концевые / висячие (deg=1)", degTotalB, 1);
    printByDegree("Источники (deg_in=0)", degInB, 0);
    printByDegree("Стоки (deg_out=0)", degOutB, 0);
    printByDegree("Доминирующие", degOutA, n > 1 ? n - 1 : -1);

    return 0;
}