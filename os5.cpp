#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

vector<vector<int>> getData(string name) {
    ifstream file(name);
    vector<vector<int>> v;

    int a, b;
    int id = 1;

    while (file >> a >> b) {
        v.push_back({id, a, b});
        id++;
    }

    return v;
}

double firstCome(vector<vector<int>> v) {
    sort(v.begin(), v.end(), [](vector<int> x, vector<int> y) {
        return x[1] < y[1];
    });

    int t = 0;
    double w = 0;

    for (int i = 0; i < v.size(); i++) {
        if (t < v[i][1])
            t = v[i][1];

        w += (t - v[i][1]);
        t += v[i][2];
    }

    return w / v.size();
}

double shortJob(vector<vector<int>> v) {
    int n = v.size();
    vector<bool> used(n, false);

    int t = 0, done = 0;
    double w = 0;

    while (done < n) {
        int idx = -1;
        int best = 1000000000;

        for (int i = 0; i < n; i++) {
            if (!used[i] && v[i][1] <= t) {
                if (v[i][2] < best) {
                    best = v[i][2];
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            t++;
            continue;
        }

        w += (t - v[idx][1]);
        t += v[idx][2];

        used[idx] = true;
        done++;
    }

    return w / n;
}

double takeTurns(vector<vector<int>> v) {
    int n = v.size();

    sort(v.begin(), v.end(), [](vector<int> x, vector<int> y) {
        return x[1] < y[1];
    });

    vector<int> left(n);
    vector<int> finish(n, 0);
    vector<bool> added(n, false);

    for (int i = 0; i < n; i++)
        left[i] = v[i][2];

    queue<int> q;

    int t = v[0][1];
    int step = 2;

    q.push(0);
    added[0] = true;

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        int run = min(step, left[i]);
        left[i] -= run;
        t += run;

        for (int j = 0; j < n; j++) {
            if (!added[j] && v[j][1] <= t) {
                q.push(j);
                added[j] = true;
            }
        }

        if (left[i] > 0) {
            q.push(i);
        } else {
            finish[i] = t;
        }
    }

    double w = 0;

    for (int i = 0; i < n; i++) {
        int total = finish[i] - v[i][1];
        w += (total - v[i][2]);
    }

    return w / n;
}

void save(string name, double a, double b, double c) {
    ofstream file(name);

    file << "Algorithm,Average Waiting Time\n";
    file << "FCFS," << a << "\n";
    file << "SJF," << b << "\n";
    file << "Round Robin," << c << "\n";
}

int main() {
    vector<vector<int>> data = getData("input.txt");

    if (data.size() == 0) {
        cout << "input.txt empty\n";
        return 0;
    }

    double a = firstCome(data);
    double b = shortJob(data);
    double c = takeTurns(data);

    save("output.csv", a, b, c);

    cout << "done\n";
}