#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

template<typename T>
class SegmentTree {
private:
    int n;
    vector<T> tree;
    T default_value;
    function<T(T, T)> merge;

    void build(const vector<T>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = (start + end) / 2;
        build(arr, 2 * node + 1, start, mid);
        build(arr, 2 * node + 2, mid + 1, end);
        tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
    }

    T query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) {
            return default_value;
        }
        if (l <= start && end <= r) {
            return tree[node];
        }
        int mid = (start + end) / 2;
        T left_query = query(2 * node + 1, start, mid, l, r);
        T right_query = query(2 * node + 2, mid + 1, end, l, r);
        return merge(left_query, right_query);
    }

    void update(int node, int start, int end, int idx, T val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (start <= idx && idx <= mid) {
            update(2 * node + 1, start, mid, idx, val);
        } else {
            update(2 * node + 2, mid + 1, end, idx, val);
        }
        tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
    }

public:
    SegmentTree(const vector<T>& arr, function<T(T, T)> merge_func, T default_val)
        : n(arr.size()), merge(merge_func), default_value(default_val) {
        tree.resize(4 * n);
        build(arr, 0, 0, n - 1);
    }

    T query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }

    void update(int idx, T val) {
        update(0, 0, n - 1, idx, val);
    }
};


// ===========RANGE SUM QUERY===========

// int main() {
//     vector<int> arr = {1, 3, 5, 7, 9, 11};
//     function<int(int, int)> merge = [](int a, int b) {
//         return a + b;
//     };
//     SegmentTree<int> segTree(arr, merge, 0);
//     int sum = segTree.query(1, 3);
//     cout << "Sum from index 1 to 3: " << sum << endl;
//     segTree.update(1, 10);
//     sum = segTree.query(1, 3);
//     cout << "After update, " << sum << endl;
//     return 0;
// }

// ===========RANGE MINIMUM QUERY===========

// int main() {
//     vector<int> arr = {1, 3, 5, 7, 9, 11};
//     function<int(int, int)> merge = [](int a, int b) {
//         return min(a,b);
//     };
//     SegmentTree<int> segTree(arr, merge, INT_MAX);
//     int mn = segTree.query(1, 3);
//     cout << "Mimumum from index 1 to 3: " << mn << endl;
//     segTree.update(1, 10);
//     mn = segTree.query(1, 3);
//     cout << "After update, " << mn << endl;
//     return 0;
// }

// ===========RANGE MAXIMUM QUERY===========

// int main() {
//     vector<int> arr = {1, 3, 5, 7, 9, 11};
//     function<int(int, int)> merge = [](int a, int b) {
//         return max(a,b);
//     };
//     SegmentTree<int> segTree(arr, merge, INT_MIN);
//     int mx = segTree.query(1, 3);
//     cout << "Maximum from index 1 to 3: " << mx << endl;
//     segTree.update(1, 10);
//     mx = segTree.query(1, 3);
//     cout << "After update, " << mx << endl;
//     return 0;
// }