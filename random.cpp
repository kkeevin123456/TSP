#include<bits/stdc++.h>

using namespace std;

int main() {
    int min = 1;
    int max = 99;
    int n = 11;
    ofstream output("input.txt");
    output << n << endl;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n-1; j++) {
            if (i==j) output << -1 << " ";
            int x = rand() % (max - min + 1) + min;
            output << x << " ";
        }
        if (i == n-1) output << -1;
        output << endl;
    }

    //int x = rand() % (max - min + 1) + min;
}