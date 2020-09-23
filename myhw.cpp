#include<bits/stdc++.h>
#include <time.h>

using namespace std;

int n, Upper_bound;
vector<vector<int>> grid;

struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

bool allINF(const vector<vector<int>>& matrix) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++)
            if (matrix[i][j] != -1)
                return false;
    }
    return true;
}

int update_lowerbound(vector<vector<int>>& matrix) {
    int min_temp;
    int lower_bound = 0;
    // find row lower_bound
    for (int i=0; i<n; i++) {
        min_temp = INT_MAX;
        for (int j=0; j<n; j++) {
            if (matrix[i][j] == -1) continue;
            if (min_temp > matrix[i][j]) min_temp = matrix[i][j];
        }
        if (min_temp > 0 && min_temp != INT_MAX) {
            lower_bound += min_temp;
            for (int j=0; j<n; j++) {
                if (matrix[i][j] == -1) continue;
                matrix[i][j] -= min_temp;
            }
        }
    }

    // find col lower_bound
    for (int i=0; i<n; i++) {
        min_temp = INT_MAX;
        for (int j=0; j<n; j++) {
            if (matrix[j][i] == -1) continue;
            if (min_temp > matrix[j][i]) min_temp = matrix[j][i];
        }
        if (min_temp > 0 && min_temp != INT_MAX) {
            lower_bound += min_temp;
            for (int j=0; j<n; j++) {
                if (matrix[j][i] == -1) continue;
                matrix[j][i] -= min_temp;
            }
        }
    }
    return lower_bound;
}

void TSP_BF(vector<int> nums, vector<vector<int>>& posible, int idx) {
    if(nums.size()-1 == idx) {
        posible.push_back(nums);
        return;
    }

    for (int i=idx; i<nums.size(); i++) {
        swap(nums[i], nums[idx]);
        TSP_BF(nums, posible, idx+1);
    }
}

// find min row & min col
void min_rc(const vector<vector<int>>& matrix, int i, int j, int& min_row, int& min_col) {
    for (int k=0; k<n; k++) {
        if (matrix[i][k] != -1 && k != j) {
            min_row = min(min_row, matrix[i][k]);
        }
        if (matrix[k][j] != -1 && k != i) {
            min_col = min(min_col, matrix[k][j]);
        }
    }
}

bool check_cycle(vector<pair<int, int>> temp_result) {
    vector<int> temp;
    for (int i=0; i<n; i++) 
        temp.push_back(-1);

    int size = temp_result.size();
    for (int i=0; i<size; i++) {
        temp[temp_result[i].first] = temp_result[i].second;
    }
    
    unordered_set<int> s;
    int idx;
    for (int i=0; i<size; i++) {
        idx = i;
        while (temp[idx] != -1) {
            if (s.find(temp[idx]) != s.end()) return true;
            s.insert(temp[idx]);
            idx = temp[idx];
        }
        s.clear();
    }
    return false;
}

void TSP_BB(vector<vector<int>> matrix, int lower_bound, vector<pair<int, int>>& ans, vector<pair<int, int>> temp_result) {
    if (lower_bound >= Upper_bound) {
        matrix.clear();
        vector<vector<int>>().swap(matrix);
        return;
    }
    if (allINF(matrix)) {
        matrix.clear();
        vector<vector<int>>().swap(matrix);
        return;
    }
    if (temp_result.size() == n-1) {
        bool flag = false;
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                if (matrix[i][j] == 0){
                    flag = true;
                    temp_result.push_back(make_pair(i, j));
                    break;
                }
            }
            if (flag) break;
        }
        ans = temp_result;
        Upper_bound = lower_bound;
        /*cout << "-------------" << endl;
        for (int i=0; i<n; i++)
            cout << "(" << ans[i].first << ", " << ans[i].second << ") ";
        cout << "upper_bound: " << Upper_bound << endl;
        cout << "-------------" << endl;*/
        return;
    }
    // find target
    int target_i, target_j, min_row, min_col, max_cost = INT_MIN;
    if (temp_result.size() == 0) {
        target_i = 0;
        for (int i=0; i<n; i++) {
            if (matrix[0][i] == 0) target_j = i;
        }
        max_cost = 0;
    } else {
        min_row = INT_MAX;
        min_col = INT_MAX;
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                if (matrix[i][j] == -1) continue;

                else if (matrix[i][j] == 0) {
                    min_rc(matrix, i, j, min_row, min_col);
                    if (min_row == INT_MAX) min_row = 0;
                    if (min_col == INT_MAX) min_col = 0;
                    //cout << "min_rc: " << min_row << " " << min_col;
                    if (max_cost < min_row + min_col) {
                        max_cost = min_row + min_col;
                        //cout << "max_cost: " << max_cost << endl;
                        target_i = i;
                        target_j = j;
                    }

                    min_row = INT_MAX;
                    min_col = INT_MAX;
                }
            }
        }
        if (max_cost < 0) max_cost = 0;
    }

    // include with target node
    vector<vector<int>> temp = matrix;
    int temp_lowerbound;
    temp_result.push_back(make_pair(target_i, target_j));
    // todo: check cycle
    if (check_cycle(temp_result)) {
        temp_result.pop_back();
        temp[target_i][target_j] = -1;
        //return;
        //cout << "check_cycle: true ---> " << target_i << " " << target_j << endl << endl << endl;
    } else {
        for (int k=0; k<n; k++) {
            temp[target_i][k] = -1;
            temp[k][target_j] = -1;
        }
        temp[target_j][target_i] = -1;
    }
    // update map
    temp_lowerbound = update_lowerbound(temp);
    
    /*for (int i=0; i<temp_result.size(); i++)
        cout << "(" << temp_result[i].first << ", " << temp_result[i].second << ")  ";
    cout << endl;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << setw(5) << temp[i][j] << " ";
        }
        cout << endl;
    }
    cout << "temp_lowerbound: " << temp_lowerbound << endl;
    cout << "lower_bound: " << lower_bound + temp_lowerbound;
    cout << endl << endl;*/
    TSP_BB(temp, lower_bound + temp_lowerbound, ans, temp_result);

    // exclude with target node
    temp_result.pop_back();
    /*for (int i=0; i<temp_result.size(); i++)
        cout << "(" << temp_result[i].first << ", " << temp_result[i].second << ")  ";*/
    temp = matrix;
    temp[target_i][target_j] = -1;

    temp_lowerbound = update_lowerbound(temp);

    /*cout << "without" << endl;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << setw(5) << temp[i][j] << " ";
        }
        cout << endl;
    }
    cout << "max_cost: " << max_cost << endl;
    cout << "temp_lowerbound: " << temp_lowerbound << endl;
    cout << "lower_bound: " << lower_bound + temp_lowerbound;
    cout << endl << endl;*/
    TSP_BB(temp, lower_bound + max_cost + temp_lowerbound, ans, temp_result);
    temp.clear();
}

int main() {
    struct timespec start, end;
    double time_used;

    // input data
    ifstream input("input.txt");
    ofstream output("BF.txt");
    ofstream output1("BB.txt");
    input >> n;

    vector<int> nums;
    for (int i=0; i<n; i++) {
        vector<int> temp;
        nums.push_back(i);
        for (int j=0; j<n; j++) {
            int dot;
            input >> dot;
            temp.push_back(dot);
        }
        grid.push_back(temp);
    }

    // BF
    // 計算開始時間
    clock_gettime(CLOCK_MONOTONIC, &start);
    vector<vector<int>> posible;

    TSP_BF(nums, posible, 0);

    long size = posible.size();
    int min_path = INT_MAX, temp_sum=0, idx;

    for (long i=0; i<size; i++) {
        temp_sum = 0;
        if (posible[i][0] != 0) continue;
        for (int j=0; j<n; j++) {
            if(j == n-1) temp_sum += grid[posible[i][j]][posible[i][0]];
            else temp_sum += grid[posible[i][j]][posible[i][j+1]];
        }
        min_path = min(temp_sum, min_path);
        if (min_path == temp_sum) idx = i;
    }
    // 計算結束時間
    clock_gettime(CLOCK_MONOTONIC, &end);
    // 計算實際花費時間
    struct timespec temp = diff(start, end);
    time_used = temp.tv_sec + (double) temp.tv_nsec / 1000000000.0;
    /*cout << "min_path: ";
    for (int i=0; i<n; i++) {
        cout << posible[idx][i] << " ";
    }
    cout << "0";
    cout << endl << "min_cost: " << min_path << endl;*/
    output << "Solution: ";
    for (int i=0; i<n; i++) {
        output << posible[idx][i] << " - ";
    }
    output << "0" << endl;
    output << "Cost    : " << min_path << endl;
    output << "Time    : " << time_used << " s";

    // BB
    // 計算開始時間
    clock_gettime(CLOCK_MONOTONIC, &start);

    int min_temp;
    int lower_bound = 0;
    vector<vector<int>> test = grid;
    // find row lower_bound
    for (int i=0; i<n; i++) {
        min_temp = INT_MAX;
        for (int j=0; j<n; j++) {
            if (i==j) continue;
            if (min_temp > grid[i][j]) min_temp = grid[i][j];
        }
        if (min_temp != 0) {
            lower_bound += min_temp;
            for (int j=0; j<n; j++) {
                if (i==j) continue;
                grid[i][j] -= min_temp;
            }
        }
    }

    // find col lower_bound
    for (int i=0; i<n; i++) {
        min_temp = INT_MAX;
        for (int j=0; j<n; j++) {
            if (i==j) continue;
            if (min_temp > grid[j][i]) min_temp = grid[j][i];
        }
        if (min_temp != 0) {
            lower_bound += min_temp;
            for (int j=0; j<n; j++) {
                if (i==j) continue;
                grid[j][i] -= min_temp;
            }
        }
    }

    /*for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << setw(5) << grid[i][j] << " ";
        }
        cout << endl;
    }*/
    //cout << lower_bound << endl;
    vector<pair<int, int>> ans;
    Upper_bound = INT_MAX;
    vector<pair<int, int>> temp_result;
    TSP_BB(grid, lower_bound, ans, temp_result);

    // 計算結束時間
    clock_gettime(CLOCK_MONOTONIC, &end);
    // 計算實際花費時間
    temp = diff(start, end);
    time_used = temp.tv_sec + (double) temp.tv_nsec / 1000000000.0;


    int result[n];
    for (int i=0; i<n; i++) {
        /*cout << "(" << ans[i].first << ", " << ans[i].second << ")" << endl;*/
        result[ans[i].first] = ans[i].second;
    }
    //cout << "result: 0 ";
    output1 << "Solution: 0";
    idx = 0;
    min_path = 0;
    for (int i=0; i<n; i++) {
        min_path += test[idx][result[idx]];
        //cout << "(" << idx << ", " << result[idx] << "): " << test[idx][result[idx]] << endl;
        //cout << result[idx] << " ";
        output1 << " - " << result[idx];
        idx = result[idx];
    }
    output1 << endl;
    //cout << "min_path: " << min_path;

    output1 << "Cost    : " << min_path << endl;
    output1 << "Time    : " << time_used << " s";
}