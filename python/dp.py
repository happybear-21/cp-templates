def divide_conquer_dp(l, r, optl, optr, cur, prev, cost):
    if l > r:
        return
    mid = (l + r) // 2
    best = (10 ** 30, -1)
    start, end = optl, min(mid, optr)
    for k in range(start, end + 1):
        cand = prev[k] + cost(k, mid)
        if cand < best[0]:
            best = (cand, k)
    cur[mid] = best[0]
    opt = best[1]
    divide_conquer_dp(l, mid - 1, optl, opt, cur, prev, cost)
    divide_conquer_dp(mid + 1, r, opt, optr, cur, prev, cost)
