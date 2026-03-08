#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void readInput(string filename, int &n, int &m, vector<int> &E, vector<vector<int>> &C, vector<vector<int>> &R)
{
    ifstream file(filename);

    file >> n >> m;

    E.resize(m);
    for (int i = 0; i < m; i++)
        file >> E[i];

    C.resize(n, vector<int>(m));
    R.resize(n, vector<int>(m));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            file >> C[i][j];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            file >> R[i][j];
}

vector<int> computeAvailable(int n, int m,
                             vector<int> &E,
                             vector<vector<int>> &C)
{
    vector<int> A(m, 0);

    for (int j = 0; j < m; j++)
    {
        int sum = 0;

        for (int i = 0; i < n; i++)
            sum += C[i][j];

        A[j] = E[j] - sum;
    }

    return A;
}

bool canProcessRun(int process,
                   int m,
                   vector<vector<int>> &R,
                   vector<int> &A)
{
    for (int j = 0; j < m; j++)
    {
        if (R[process][j] > A[j])
            return false;
    }

    return true;
}

vector<int> detectDeadlock(int n, int m,
                           vector<vector<int>> &C,
                           vector<vector<int>> &R,
                           vector<int> &A)
{
    vector<bool> finished(n, false);
    bool progress = true;

    while (progress)
    {
        progress = false;

        for (int i = 0; i < n; i++)
        {
            if (finished[i])
                continue;

            if (canProcessRun(i, m, R, A))
            {
                for (int j = 0; j < m; j++)
                    A[j] += C[i][j];

                finished[i] = true;
                progress = true;
            }
        }
    }

    vector<int> deadlocked;

    for (int i = 0; i < n; i++)
        if (!finished[i])
            deadlocked.push_back(i);

    return deadlocked;
}

void printResult(vector<int> deadlocked)
{
    if (deadlocked.empty())
    {
        cout << "No deadlock detected." << endl;
    }
    else
    {
        cout << "Deadlock detected among processes: ";

        for (int p : deadlocked)
            cout << "P" << p << " ";

        cout << endl;
    }
}

int main()
{
    int n, m;

    vector<int> E;
    vector<vector<int>> C;
    vector<vector<int>> R;

    readInput("deadlock.txt", n, m, E, C, R);

    vector<int> A = computeAvailable(n, m, E, C);

    vector<int> deadlocked = detectDeadlock(n, m, C, R, A);

    printResult(deadlocked);

    return 0;
}