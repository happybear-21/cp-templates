import math
from collections import defaultdict

def mo_process(arr, queries):
    n = len(arr)
    q = len(queries)
    blk = max(1, int(n ** 0.5))
    order = list(range(q))
    order.sort(key=lambda i: ((queries[i][0] // blk, queries[i][1] if (queries[i][0] // blk) % 2 == 0 else -queries[i][1])))
    cnt = defaultdict(int)
    cur = 0
    res = [0] * q
    L, R = 0, -1
    for idx in order:
        l, r = queries[idx]
        while L > l:
            L -= 1
            x = arr[L]
            cur -= cnt[x] * cnt[x] * x
            cnt[x] += 1
            cur += cnt[x] * cnt[x] * x
        while R < r:
            R += 1
            x = arr[R]
            cur -= cnt[x] * cnt[x] * x
            cnt[x] += 1
            cur += cnt[x] * cnt[x] * x
        while L < l:
            x = arr[L]
            cur -= cnt[x] * cnt[x] * x
            cnt[x] -= 1
            cur += cnt[x] * cnt[x] * x
            L += 1
        while R > r:
            x = arr[R]
            cur -= cnt[x] * cnt[x] * x
            cnt[x] -= 1
            cur += cnt[x] * cnt[x] * x
            R -= 1
        res[idx] = cur
    return res

def tree_mo(g, values, queries):
    n = len(g)
    euler = []
    st = [0] * n
    en = [0] * n
    stack = [(0, -1, 0)]
    while stack:
        u, p, state = stack.pop()
        if state == 0:
            st[u] = len(euler)
            euler.append(u)
            stack.append((u, p, 1))
            for v in g[u]:
                if v != p:
                    stack.append((v, u, 0))
        else:
            en[u] = len(euler) - 1
    ord_queries = []
    for idx, (u, v) in enumerate(queries):
        l, r = st[u], st[v]
        if l > r:
            l, r = r, l
        ord_queries.append((l, r, idx))
    blk = max(1, int(len(euler) ** 0.5))
    ord_queries.sort(key=lambda q: ((q[0] // blk, q[1] if (q[0] // blk) % 2 == 0 else -q[1])))
    freq = defaultdict(int)
    vis = [False] * n
    cur = 0
    ans = [0] * len(queries)
    L, R = 0, -1
    def toggle(node):
        nonlocal cur
        x = values[node]
        if vis[node]:
            cur -= freq[x] * freq[x] * x
            freq[x] -= 1
            cur += freq[x] * freq[x] * x
        else:
            cur -= freq[x] * freq[x] * x
            freq[x] += 1
            cur += freq[x] * freq[x] * x
        vis[node] = not vis[node]
    for l, r, idx in ord_queries:
        while L > l:
            L -= 1
            toggle(euler[L])
        while R < r:
            R += 1
            toggle(euler[R])
        while L < l:
            toggle(euler[L])
            L += 1
        while R > r:
            toggle(euler[R])
            R -= 1
        ans[idx] = cur
    return ans
