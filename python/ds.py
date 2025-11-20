import math

class Fenwick:
    def __init__(self, n):
        self.n = n
        self.bit = [0] * (n + 1)
    def add(self, idx, val):
        i = idx + 1
        while i <= self.n:
            self.bit[i] += val
            i += i & -i
    def sum_prefix(self, idx):
        res, i = 0, idx + 1
        while i > 0:
            res += self.bit[i]
            i -= i & -i
        return res
    def sum_range(self, l, r):
        return self.sum_prefix(r) - (self.sum_prefix(l - 1) if l else 0)

class FenwickRange:
    def __init__(self, n):
        self.b1 = Fenwick(n)
        self.b2 = Fenwick(n)
    def _add(self, b, idx, val):
        i = idx + 1
        while i <= b.n:
            b.bit[i] += val
            i += i & -i
    def range_add(self, l, r, val):
        self._add(self.b1, l, val)
        self._add(self.b1, r + 1, -val)
        self._add(self.b2, l, val * l)
        self._add(self.b2, r + 1, -val * (r + 1))
    def _sum(self, b, idx):
        res, i = 0, idx + 1
        while i > 0:
            res += b.bit[i]
            i -= i & -i
        return res
    def prefix_sum(self, idx):
        return self._sum(self.b1, idx) * (idx + 1) - self._sum(self.b2, idx)
    def range_sum(self, l, r):
        return self.prefix_sum(r) - (self.prefix_sum(l - 1) if l else 0)

class DSU:
    def __init__(self, n):
        self.p = list(range(n))
        self.sz = [1] * n
    def find(self, x):
        while x != self.p[x]:
            self.p[x] = self.p[self.p[x]]
            x = self.p[x]
        return x
    def unite(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b:
            return False
        if self.sz[a] < self.sz[b]:
            a, b = b, a
        self.p[b] = a
        self.sz[a] += self.sz[b]
        return True

class RollbackDSU:
    def __init__(self, n):
        self.p = list(range(n))
        self.sz = [1] * n
        self.st = []
        self.comps = n
    def find(self, x):
        while x != self.p[x]:
            x = self.p[x]
        return x
    def snapshot(self):
        return len(self.st)
    def rollback(self, t):
        while len(self.st) > t:
            b, a = self.st.pop()
            if b == -1:
                continue
            self.p[b] = b
            self.sz[a] -= self.sz[b]
            self.comps += 1
    def unite(self, a, b):
        a = self.find(a)
        b = self.find(b)
        if a == b:
            self.st.append((-1, -1))
            return False
        if self.sz[a] < self.sz[b]:
            a, b = b, a
        self.st.append((b, a))
        self.p[b] = a
        self.sz[a] += self.sz[b]
        self.comps -= 1
        return True

class SegmentTree:
    def __init__(self, arr, merge, neutral):
        self.n = len(arr)
        self.merge = merge
        self.neutral = neutral
        self.st = [neutral] * (4 * self.n)
        self._build(arr, 1, 0, self.n - 1)
    def _build(self, arr, p, l, r):
        if l == r:
            self.st[p] = arr[l]
            return
        m = (l + r) // 2
        self._build(arr, p * 2, l, m)
        self._build(arr, p * 2 + 1, m + 1, r)
        self.st[p] = self.merge(self.st[p * 2], self.st[p * 2 + 1])
    def query(self, L, R):
        return self._query(1, 0, self.n - 1, L, R)
    def _query(self, p, l, r, ql, qr):
        if qr < l or r < ql:
            return self.neutral
        if ql <= l and r <= qr:
            return self.st[p]
        m = (l + r) // 2
        return self.merge(self._query(p * 2, l, m, ql, qr), self._query(p * 2 + 1, m + 1, r, ql, qr))
    def update(self, idx, val):
        self._update(1, 0, self.n - 1, idx, val)
    def _update(self, p, l, r, idx, val):
        if l == r:
            self.st[p] = val
            return
        m = (l + r) // 2
        if idx <= m:
            self._update(p * 2, l, m, idx, val)
        else:
            self._update(p * 2 + 1, m + 1, r, idx, val)
        self.st[p] = self.merge(self.st[p * 2], self.st[p * 2 + 1])

class SparseTable:
    def __init__(self, arr, op):
        self.n = len(arr)
        self.op = op
        self.K = (self.n).bit_length()
        self.st = [arr[:]]
        for k in range(1, self.K):
            row = []
            for i in range(self.n - (1 << k) + 1):
                row.append(op(self.st[k - 1][i], self.st[k - 1][i + (1 << (k - 1))]))
            self.st.append(row)
    def query(self, l, r):
        k = (r - l + 1).bit_length() - 1
        return self.op(self.st[k][l], self.st[k][r - (1 << k) + 1])
