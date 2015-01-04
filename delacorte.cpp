#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

int cgcd(int a, int b)
{
    int c;
    while (a != 0) {
        c = a; a = b % a; b = c;
    }
    return b;
}

class Mat {
private:
    int n;
    int m;

    vector<vector<int>> gt; // gcd(a,b)
    vector<pair<int, int>> pt; // pos(a)

    vector<vector<int>> mat;
    vector<pair<int, int>> neigh1;
    vector<pair<int, int>> neigh2;

    void randomInit() {
        vector<int> rem(m);
        for (int i = 1; i <= m; ++i) {
            rem[i-1] = i;
        }

        random_shuffle(rem.begin(), rem.end());

        int pos = 0;
        int val;
        while (!rem.empty()) {
            val = rem.back();
            setPos(val, make_pair(pos % n, pos / n));
            rem.pop_back();
            pos++;
        }
    }

    void tableInit() {
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= m; ++j) {
                gt[i][j] = cgcd(i, j);
            }
        }

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i != 0 || j != 0) {
                    neigh1.push_back(make_pair(i, j));
                }
            }
        }

        for (int i = -2; i <= 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                if (i != 0 || j != 0) {
                    neigh2.push_back(make_pair(i, j));
                }
            }
        }
    }

    void setPos(int num, pair<int, int> p) {
        pt[num] = p;
        mat[p.first][p.second] = num;
    }

    int gcd(int a, int b) {
        return gt[a][b];
    }

    pair<int, int> pos(int num) {
        return pt[num];
    }

    void swap(int i, int j) {
        pair<int, int> a = pos(i);
        pair<int, int> b = pos(j);
        setPos(i, b);
        setPos(j, a);
    }

    void randomSwap(int j) {
        for (int i = 0; i < j; ++i) {
            swap(1 + rand() % m, 1 + rand() % m);
        }
    }

    int onecost(int i) {
        int c = 0;
        for (int j = 1; j <= m; ++j) {
            c += gcd(i, j) * dist(i, j);
        }
        return c;
    }

    int gcdcost(int i) {
        int c = 0;
        for (int j = 1; j <= m; ++j) {
            c += gcd(i, j);
        }
        return c;
    }

    int twocost(int i, int j) {
        return onecost(i) + onecost(j);
    }

    int dist(int i, int j) {
        pair<int, int> a = pos(i);
        pair<int, int> b = pos(j);
        return (a.first - b.first) * (a.first - b.first)
            + (a.second - b.second) * (a.second - b.second);
    }

    int swapcost(int i, int j) {
        int before = twocost(i, j);
        swap(i, j);
        int after  = twocost(i, j);
        swap(i, j);

        return after - before;
    }

public:
    Mat(int n) :
        n(n),
        m(n*n),
        pt(m+1),
        mat(n, vector<int>(n)),
        gt(m+1, vector<int>(m+1)) {

        tableInit();
        randomInit();
    }

    Mat(int n, string filename) :
        n(n),
        m(n*n),
        pt(m+1),
        mat(n, vector<int>(n)),
        gt(m+1, vector<int>(m+1)) {

        tableInit();

        ifstream ifile(filename);
        int goal;
        ifile >> goal;

        char c;
        int a;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                ifile >> c >> a;
                setPos(a, make_pair(i, j));
            }
            ifile >> c;
            ifile >> c;
        }

        ifile.close();
    }

    int dsum() {
        int s = 0;

        for (int i = 1; i <= m; ++i) {
            for (int j = i+1; j <= m; ++j) {
                pair<int, int> a = pos(i);
                pair<int, int> b = pos(j);
                int dist = (a.first - b.first) * (a.first - b.first)
                         + (a.second - b.second) * (a.second - b.second);
                s += dist * gcd(i, j);
            }
        }

        return s;
    }

    pair<int, int> cand(pair<int, int> a) {
        int x, y;
        do {
            vector<pair<int, int>>::const_iterator it(neigh1.begin());
            advance(it, rand() % neigh1.size());
            x = a.first + it->first;
            y = a.second + it->second;
        } while (x < 0 || x >= n || y < 0 || y >= n);

        return make_pair(x, y);
    }

    void candAll(set<pair<int, int>> &candpos) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                candpos.insert(make_pair(i, j));
            }
        }
    }

    void optimize(bool min, vector<int>& sc) {
        bool swapped = true;
        while (swapped) {
            swapped = false;

            set<pair<int, int>> candpos;

            candAll(candpos);

            for (auto num = sc.begin(); num != sc.end(); ++num) {
                int bestcost;
                if (min) {
                    bestcost = 1000000000;
                }
                else {
                    bestcost = -1000000000;
                }
                pair<int, int> bestpos;

                for (auto cand = candpos.begin(); cand != candpos.end(); ++cand) {
                    int cost = swapcost(*num, mat[cand->first][cand->second]);
                    if (min && cost < bestcost) {
                        bestcost = cost;
                        bestpos  = *cand;
                    }
                    else if (!min && cost > bestcost) {
                        bestcost = cost;
                        bestpos  = *cand;
                    }
                }
                if (min && bestcost < 0) {
                    swap(*num, mat[bestpos.first][bestpos.second]);
                    swapped = true;
                }
                else if (!min && bestcost > 0) {
                    swap(*num, mat[bestpos.first][bestpos.second]);
                    swapped = true;
                }
            }
        }
    }

    void find(bool min, int goal, double div, int niter) {

        vector<int> score(niter+1);
        vector<int> sc(m);
        for (int i = 1; i <= m; ++i) {
            sc[i-1] = i;
        }
        random_shuffle(sc.begin(), sc.end());

        //int niter = 100000;
        bool swapped = true;
        int save = 0;
    
        double T = div;

        double prev = 0;
        if (!min)
            prev = 1000000000;
        int tcount = 0;
        int initround = 3;
        while (T > 5 /*&& swapped*/) {

            initround--;
            tcount++;
            for (long long iter = 0; iter < niter /* && swapped */; ++iter) {
                swapped = false;

                int swaps = 0;

                if (iter % 10000 == 0) {
                    cout << iter << " of " << niter << " T=";
                    cout << fixed << setprecision(1) << T << " sum = " << dsum() << endl;
                }

                if (iter % 3000 == 0 && T < 50) {
                    optimize(min, sc);
                }

                for (auto num = sc.begin(); num != sc.end(); ++num) {

                    int bestcost;
                    if (min) {
                        bestcost = 1000000000;
                    }
                    else {
                        bestcost = -1000000000;
                    }
                    pair<int, int> bestpos;

                    bestpos  = cand(pos(*num));
                    bestcost = swapcost(*num, mat[bestpos.first][bestpos.second]);

                    double r = double(rand()) / double(RAND_MAX);
                    if (min && (bestcost < 0 || r < exp(-double(bestcost) / double(T)))) {
                        swap(*num, mat[bestpos.first][bestpos.second]);
                        swapped = true;
                        swaps++;
                    }
                    else if (!min && (bestcost > 0 || r < exp(+double(bestcost) / double(T)))) {
                        swap(*num, mat[bestpos.first][bestpos.second]);
                        swapped = true;
                        swaps++;
                    }

                }

                score[iter] = dsum();
            }

            if (initround > 0) continue;

            int over = 0;

            double neww = 0.0;
            for (int i = 0; i < 10; ++i) {
                int sum_of_elems = accumulate(score.begin()
                        + (niter / 10) * i,
                        score.begin()
                        + (niter / 10) * (i + 1), 0);

                if (min && sum_of_elems / (double(niter) / 10.0) >= prev-20)
                    over++;
                if (!min && sum_of_elems / (double(niter) / 10.0) <= prev+20)
                    over++;
                neww += sum_of_elems / (double(niter) / 10.0);
            }

            if (over > 1 || tcount > 40) {
                T /= 1.01;
                tcount = 0;
                prev = neww / 10.0;
            }
        }
        
        optimize(min, sc);
    }

    string str() {
        stringstream ss;
        for (int i = 0; i < n; ++i) {
            ss << "(";
            for (int j = 0; j < n; ++j) {
                ss << mat[i][j];
                if (j < n-1)
                    ss << ", ";
            }
            if (i < n-1)
                ss << ")," << endl;
            else
                ss << ")" << endl;
        }
        return ss.str();
    }
};

int main(int argc, char *argv[]) {
    srand(time(0));
    cout << "argv" << argv[1] << endl;
    bool min = false;

    if (argc >= 2 && strcmp(argv[1], "min") == 0) {
        min = true;
    }

    int bot = atoi(argv[2]);
    int top = atoi(argv[3]);
    int div = atoi(argv[4]);
    int niter = atoi(argv[5]);

    while (true) {
        for (int i = bot; i <= top; ++i) {
            stringstream ss;
            if (min)
                ss << "squares/min/" << i;
            else
                ss << "squares/max/" << i;

            ifstream ifile(ss.str());
            int goal;
            ifile >> goal;
            ifile.close();

            Mat m(i, ss.str());

            int fail = 0;
            while (fail < 2) {
                cout << "Goal for " << i << " is " << goal << endl;
                m.find(min, goal, div, niter);

                cout << m.dsum() << endl;

                if ((min && m.dsum() < goal) || (!min && m.dsum() > goal)) {
                    cout << "WOOOP WE MADE IT" << endl;
                    cout << m.dsum() << " better than " << goal << endl;
                    ofstream ofile(ss.str(), ios::out | ios::trunc);
                    ofile << m.dsum() << endl;
                    ofile << m.str() << endl;
                    ofile.close();

                    ofstream logf("log", ios::out | ios::app);
                    logf << i << " " << niter << " " << div;
                    logf << " " << m.dsum() - goal << endl;
                    logf.close();
                    goal = m.dsum();
                }
                else {
                    cout << "Nah, break out. goal = " << goal;
                    cout << " sum = " << m.dsum() << endl;
                    fail++;
                }
            }
        }
    }
}

