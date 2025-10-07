#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
using namespace std;

template<typename T>
class SegmentTree {
private:
    int n; // Size of the input array
    vector<T> tree; // Segment tree storage
    vector<T> lazy; // Lazy propagation storage
    T default_value; // Default value for nodes (e.g., 0 for sum, INT_MAX for min)
    function<T(T, T)> merge; // User-defined merge function
    function<T(T, T, int)> apply_update; // User-defined function to apply update to a node

    // Build the segment tree from the input array
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

    // Propagate lazy updates to children
    void propagate(int node, int start, int end) {
        if (lazy[node] != default_value) {
            // Apply lazy update to current node
            tree[node] = apply_update(tree[node], lazy[node], end - start + 1);
            if (start != end) {
                // Propagate to children
                lazy[2 * node + 1] = apply_update(lazy[2 * node + 1], lazy[node], 1);
                lazy[2 * node + 2] = apply_update(lazy[2 * node + 2], lazy[node], 1);
            }
            lazy[node] = default_value; // Clear lazy value
        }
    }

    // Range query helper
    T query(int node, int start, int end, int l, int r) {
        propagate(node, start, end); // Apply pending updates
        if (r < start || end < l) {
            return default_value; // Out of range
        }
        if (l <= start && end <= r) {
            return tree[node]; // Fully within range
        }
        int mid = (start + end) / 2;
        T left_query = query(2 * node + 1, start, mid, l, r);
        T right_query = query(2 * node + 2, mid + 1, end, l, r);
        return merge(left_query, right_query);
    }

    // Range update helper
    void update_range(int node, int start, int end, int l, int r, T val) {
        propagate(node, start, end); // Apply pending updates
        if (r < start || end < l) {
            return; // Out of range
        }
        if (l <= start && end <= r) {
            // Mark lazy update for this node
            lazy[node] = apply_update(lazy[node], val, 1);
            propagate(node, start, end); // Apply the update immediately
            return;
        }
        int mid = (start + end) / 2;
        update_range(2 * node + 1, start, mid, l, r, val);
        update_range(2 * node + 2, mid + 1, end, l, r, val);
        tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
    }

    // Point update helper
    void update(int node, int start, int end, int idx, T val) {
        propagate(node, start, end); // Apply pending updates
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) {
            update(2 * node + 1, start, mid, idx, val);
        } else {
            update(2 * node + 2, mid + 1, end, idx, val);
        }
        tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
    }

public:
    // Constructor
    SegmentTree(const vector<T>& arr, function<T(T, T)> merge_func,
                function<T(T, T, int)> apply_func, T default_val)
        : n(arr.size()), merge(merge_func), apply_update(apply_func), default_value(default_val) {
        tree.resize(4 * n);
        lazy.resize(4 * n, default_val);
        build(arr, 0, 0, n - 1);
    }

    // Public range query [l, r]
    T query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }

    // Public range update [l, r]
    void update_range(int l, int r, T val) {
        update_range(0, 0, n - 1, l, r, val);
    }

    // Public point update at index idx
    void update(int idx, T val) {
        update(0, 0, n - 1, idx, val);
    }
};

// ===========RANGE SUM QUERY WITH RANGE UPDATES===========
int main() {
    vector<long long> arr = {1, 3, 5, 7, 9, 11};
    function<long long(long long, long long)> merge = [](long long a, long long b) {
        return a + b;
    };
    function<long long(long long, long long, int)> apply = [](long long current, long long update, int length) {
        return current + update * length;
    };
    SegmentTree<long long> segTree(arr, merge, apply, 0);
    long long sum = segTree.query(1, 3);
    cout << "Sum from index 1 to 3: " << sum << endl; // Output: 15 (3 + 5 + 7)
    segTree.update_range(1, 4, 10); // Add 10 to elements from index 1 to 4
    sum = segTree.query(1, 3);
    cout << "After range update, " << sum << endl; // Output: 45 (13 + 15 + 17)
    segTree.update(1, 10); // Set index 1 to 10
    sum = segTree.query(1, 3);
    cout << "After point update, " << sum << endl; // Output: 32 (10 + 15 + 17)
    return 0;
}

// ===========RANGE MINIMUM QUERY WITH RANGE UPDATES===========
/*
int main() {
    vector<long long> arr = {1, 3, 5, 7, 9, 11};
    function<long long(long long, long long)> merge = [](long long a, long long b) {
        return min(a, b);
    };
    function<long long(long long, long long, int)> apply = [](long long current, long long update, int) {
        return current + update;
    };
    SegmentTree<long long> segTree(arr, merge, apply, LLONG_MAX);
    long long mn = segTree.query(1, 3);
    cout << "Minimum from index 1 to 3: " << mn << endl; // Output: 3
    segTree.update_range(1, 4, 10); // Add 10 to elements from index 1 to 4
    mn = segTree.query(1, 3);
    cout << "After range update, " << mn << endl; // Output: 13
    segTree.update(1, 10); // Set index 1 to 10
    mn = segTree.query(1, 3);
    cout << "After point update, " << mn << endl; // Output: 10
    return 0;
}
*/

// ===========RANGE MAXIMUM QUERY WITH RANGE UPDATES===========
/*
int main() {
    vector<long long> arr = {1, 3, 5, 7, 9, 11};
    function<long long(long long, long long)> merge = [](long long a, long long b) {
        return max(a, b);
    };
    function<long long(long long, long long, int)> apply = [](long long current, long long update, int) {
        return current + update;
    };
    SegmentTree<long long> segTree(arr, merge, apply, LLONG_MIN);
    long long mx = segTree.query(1, 3);
    cout << "Maximum from index 1 to 3: " << mx << endl; // Output: 7
    segTree.update_range(1, 4, 10); // Add 10 to elements from index 1 to 4
    mx = segTree.query(1, 3);
    cout << "After range update, " << mx << endl; // Output: 17
    segTree.update(1, 10); // Set index 1 to 10
    mx = segTree.query(1, 3);
    cout << "After point update, " << mx << endl; // Output: 17
    return 0;
}
*/