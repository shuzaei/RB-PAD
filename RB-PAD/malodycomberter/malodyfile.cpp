#include <bits/stdc++.h>
using namespace std;

// Define
using ll = long long;
using ull = unsigned long long;
using ld = long double;
const ll dx[4] = {1, 0, -1, 0};
const ll dy[4] = {0, 1, 0, -1};
const ll MOD = 1e9 + 7;
const ll mod = 998244353;
const ll inf = 1 << 30;
const ll LINF = LONG_MAX;
const ll INF = 1LL << 60;
const ull MAX = ULONG_MAX;
#define mp make_pair
#define pb push_back
#define elif else if
#define endl '\n'
#define space ' '
#define def inline auto
#define func inline constexpr ll
#define run(a) __attribute__((constructor)) def _##a()
#define all(v) begin(v), end(v)
#define rall(v) rbegin(v), rend(v)
#define input(a) scanf("%lld", &(a))
#define print(a) printf("%lld\n", (a))
#define fi first
#define se second
#define ok(a, b) (0 <= (a) && (a) < (b))
template <class T> using vvector = vector<vector<T>>;
template <class T> using pvector = vector<pair<T, T>>;
template <class T>
using rpriority_queue = priority_queue<T, vector<T>, greater<T>>;

template <class T> bool chmax(T &a, const T &b) {
    if (a < b) {
        a = b;
        return 1;
    }
    return 0;
}
template <class T> bool chmin(T &a, const T &b) {
    if (a > b) {
        a = b;
        return 1;
    }
    return 0;
}

// Debug
#define debug(...)                                                             \
    {                                                                          \
        cerr << __LINE__ << ": " << #__VA_ARGS__ << " = ";                     \
        for (auto &&X : {__VA_ARGS__}) cerr << "[" << X << "] ";               \
        cerr << endl;                                                          \
    }

#define dump(a, h, w)                                                          \
    {                                                                          \
        cerr << __LINE__ << ": " << #a << " = [" << endl;                      \
        rep(__i, h) {                                                          \
            rep(__j, w) { cerr << a[__i][__j] << space; }                      \
            cerr << endl;                                                      \
        }                                                                      \
        cerr << "]" << endl;                                                   \
    }

#define vdump(a, n)                                                            \
    {                                                                          \
        cerr << __LINE__ << ": " << #a << " = [";                              \
        rep(__i, n) if (__i) cerr << space << a[__i];                          \
        else cerr << a[__i];                                                   \
        cerr << "]" << endl;                                                   \
    }

// Loop
#define inc(i, a, n) for (ll i = (a), _##i = (n); i <= _##i; ++i)
#define dec(i, a, n) for (ll i = (a), _##i = (n); i >= _##i; --i)
#define rep(i, n) for (ll i = 0, _##i = (n); i < _##i; ++i)
#define each(i, a) for (auto &&i : a)
#define loop() for (;;)

// Stream
#define fout(n) cout << fixed << setprecision(n)
#define fasten cin.tie(0), ios::sync_with_stdio(0)
run(0) { fasten, fout(10); }

// Speed
#pragma GCC optimize("O3")
#pragma GCC target("avx")

// Math
func gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
func lcm(ll a, ll b) { return a * b / gcd(a, b); }
func sign(ll a) { return a ? abs(a) / a : 0; }

inline constexpr ll modulo(const ll n, const ll m = MOD) {
    ll k = n % m;
    return k + m * (k < 0);
}

inline constexpr ll chmod(ll &n, const ll m = MOD) {
    n %= m;
    return n += m * (n < 0);
}

inline constexpr ll mpow(ll a, ll n, const ll m = MOD) {
    ll r = 1;
    while (n) {
        if (n & 1) r *= a;
        chmod(r, m);
        a *= a;
        chmod(a, m);
        n >>= 1;
    }
    return r;
}

inline ll inv(const ll n, const ll m = MOD) {
    ll a = n, b = m, x = 1, y = 0;
    while (b) {
        ll t = a / b;
        a -= t * b;
        swap(a, b);
        x -= t * y;
        swap(x, y);
    }
    return modulo(x, m);
}

struct Factor {
    inline vector<ll> factors(ll N) {
        vector<ll> A;
        ll i = 2;
        while (i * i <= N) {
            if (N % i == 0) {
                A.push_back(i);
                N /= i;
            } else {
                i++;
            }
        }
        if (N != 1) A.push_back(N);
        sort(all(A));
        return A;
    }
    inline vector<ll> divisor(ll N) {
        vector<ll> A;
        inc(i, 1, sqrt(N)) {
            if (N % i == 0) {
                A.push_back(i);
                if (i * i != N) A.push_back(N / i);
            }
        }
        sort(all(A));
        return A;
    }
};

struct csum {
    ll N;
    vector<ll> A, S;
    csum(vector<ll> v) : A(v), S(1) { each(k, v) S.push_back(k + S.back()); }
    ll sum(ll l, ll r) { return S[r] - S[l]; }
    ll lsum(ll r) { return S[r]; }
    ll rsum(ll l) { return S.back() - S[l]; }
};

template <ull mod = MOD> struct mi {
    inline constexpr ll modulo(const ll n, const ll m) const noexcept {
        ll k = n % m;
        return k + m * (k < 0);
    }

    ll num;
    inline constexpr mi() noexcept : num() { num = 0; }
    inline constexpr mi(const int n) noexcept : num() { num = modulo(n, mod); }
    inline constexpr mi(const ll n) noexcept : num() { num = modulo(n, mod); }

    inline constexpr mi<mod> inv() const noexcept {
        ll a = num, b = mod, x = 1, y = 0;
        while (b) {
            ll t = a / b;
            a -= t * b;
            swap(a, b);
            x -= t * y;
            swap(x, y);
        }
        return mi<mod>(x);
    }
    inline constexpr mi<mod> inv(ll n) const noexcept {
        ll a = n, b = mod, x = 1, y = 0;
        while (b) {
            ll t = a / b;
            a -= t * b;
            swap(a, b);
            x -= t * y;
            swap(x, y);
        }
        return mi<mod>(x);
    }
    inline constexpr mi<mod> inv(const mi<mod> m) const noexcept {
        return inv(m.num);
    }

    inline constexpr mi<mod> operator+() const noexcept { return mi(num); }
    inline constexpr mi<mod> operator+(const int n) const noexcept {
        return mi<mod>(num + n);
    }
    inline constexpr mi<mod> operator+(const ll n) const noexcept {
        return mi<mod>(num + n);
    }
    inline constexpr mi<mod> operator+(const mi<mod> m) const noexcept {
        return mi<mod>(num + m.num);
    }
    inline constexpr mi<mod> operator-() const noexcept { return -num; }
    inline constexpr mi<mod> operator-(const int n) const noexcept {
        return mi<mod>(num - n);
    }
    inline constexpr mi<mod> operator-(const ll n) const noexcept {
        return mi<mod>(num - n);
    }
    inline constexpr mi<mod> operator-(const mi<mod> m) const noexcept {
        return mi<mod>(num - m.num);
    }
    inline constexpr mi<mod> operator*(const int n) const noexcept {
        return mi<mod>(num * n);
    }
    inline constexpr mi<mod> operator*(const ll n) const noexcept {
        return mi<mod>(num * n);
    }
    inline constexpr mi<mod> operator*(const mi<mod> m) const noexcept {
        return mi<mod>(num * m);
    }
    inline constexpr mi<mod> operator/(const int n) const noexcept {
        return mi<mod>(num * (ll) inv(n));
    }
    inline constexpr mi<mod> operator/(const ll n) const noexcept {
        return mi<mod>(num * (ll) inv(n));
    }
    inline constexpr mi<mod> operator/(const mi<mod> m) const noexcept {
        return mi<mod>(num * (ll) inv(m));
    }
    inline constexpr mi<mod> &operator=(const int n) noexcept {
        num = modulo(n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator=(const ll n) noexcept {
        num = modulo(n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator=(const mi<mod> m) noexcept {
        num = m.num;
        return *this;
    }
    inline constexpr mi<mod> &operator+=(const int n) noexcept {
        num = modulo(num + n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator+=(const ll n) noexcept {
        num = modulo(num + n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator+=(const mi<mod> m) noexcept {
        num = modulo(num + m.num, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator++() noexcept {
        num = modulo(num + 1, mod);
        return *this;
    }
    inline constexpr mi<mod> operator++(int) noexcept {
        mi &pre = *this;
        num = modulo(num + 1, mod);
        return pre;
    }
    inline constexpr mi<mod> &operator-=(const int n) noexcept {
        num = modulo(num - n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator-=(const ll n) noexcept {
        num = modulo(num - n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator-=(const mi<mod> m) noexcept {
        num = modulo(num - m.num, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator--() noexcept {
        num = modulo(num - 1, mod);
        return *this;
    }
    inline constexpr mi<mod> operator--(int) noexcept {
        mi &pre = *this;
        num = modulo(num - 1, mod);
        return pre;
    }
    inline constexpr mi<mod> &operator*=(const int n) noexcept {
        num = modulo(num * n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator*=(const ll n) noexcept {
        num = modulo(num * n, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator*=(const mi<mod> m) noexcept {
        num = modulo(num * m.num, mod);
        return *this;
    }
    inline constexpr mi<mod> &operator/=(const int n) noexcept {
        num = modulo(num * (ll) inv(n), mod);
        return *this;
    }
    inline constexpr mi<mod> &operator/=(const ll n) noexcept {
        num = modulo(num * (ll) inv(n), mod);
        return *this;
    }
    inline constexpr mi<mod> &operator/=(const mi<mod> m) noexcept {
        num = modulo(num * (ll) inv(m), mod);
        return *this;
    }
    inline constexpr mi<mod> operator==(const int n) const noexcept {
        return num == modulo(n, mod);
    }
    inline constexpr mi<mod> operator==(const ll n) const noexcept {
        return num == modulo(n, mod);
    }
    inline constexpr mi<mod> operator==(const mi<mod> m) const noexcept {
        return num == m.num;
    }
    inline constexpr mi<mod> operator!=(const int n) const noexcept {
        return num != modulo(n, mod);
    }
    inline constexpr mi<mod> operator!=(const ll n) const noexcept {
        return num != modulo(n, mod);
    }
    inline constexpr mi<mod> operator!=(const mi<mod> m) const noexcept {
        return num != m.num;
    }
    constexpr operator int() const noexcept { return num; }
    constexpr operator ll() const noexcept { return num; }
    friend std::istream &operator>>(std::istream &, const mi<> &);
    friend std::ostream &operator<<(std::ostream &, const mi<> &);
};

template <ull mod = MOD>
inline constexpr mi<mod> operator+(const int n, const mi<mod> m) noexcept {
    return mi<mod>(n + m.num);
}
template <ull mod = MOD>
inline constexpr mi<mod> operator+(const ll n, const mi<mod> m) noexcept {
    return mi<mod>(n + m.num);
}
template <ull mod = MOD>
inline constexpr mi<mod> operator-(const int n, const mi<mod> m) noexcept {
    return mi<mod>(n - m.num);
}
template <ull mod = MOD>
inline constexpr mi<mod> operator-(const ll n, const mi<mod> m) noexcept {
    return mi<mod>(n - m.num);
}
template <ull mod = MOD>
inline constexpr mi<mod> operator*(const int n, const mi<mod> m) noexcept {
    return mi<mod>(n * m.num);
}
template <ull mod = MOD>
inline constexpr mi<mod> operator*(const ll n, const mi<mod> m) noexcept {
    return mi<mod>(n * m.num);
}
template <ull mod = MOD>
inline constexpr mi<mod> operator/(const int n, const mi<mod> m) noexcept {
    return mi<mod>(n * (ll) m.inv());
}
template <ull mod = MOD>
inline constexpr mi<mod> operator/(const ll n, const mi<mod> m) noexcept {
    return mi<mod>(n * (ll) m.inv());
}
inline constexpr mi<MOD> operator""_m(ull n) { return mi<MOD>((ll) n); }

template <ull mod = MOD>
inline constexpr mi<mod> pow(mi<mod> m, ll n) noexcept {
    mi<mod> r = mi<mod>(1);
    while (n) {
        if (n & 1) r *= m;
        m *= m;
        n >>= 1;
    }
    return r;
}
template <ull mod> istream &operator>>(istream &is, mi<mod> &m) {
    is >> m.num;
    return is;
}
template <ull mod> ostream &operator<<(ostream &is, mi<mod> &m) {
    is << (ll) m;
    return is;
}

template <ull mod = MOD> struct modmath {
    ll max;
    vector<mi<mod>> fac, inv;
    modmath() : max(1 << 20), fac(max + 1), inv(max + 1) {
        fac[0] = mi<mod>(1);
        rep(i, max) fac[i + 1] = fac[i] * (i + 1);
        inv[max] = fac[max].inv();
        dec(i, max - 1, 0) inv[i] = inv[i + 1] * (i + 1);
    }
    modmath(ll n) : max(n), fac(n + 1), inv(n + 1) {
        fac[0] = 1;
        rep(i, n) fac[i + 1] = fac[i] * (i + 1);
        inv[n] = 1 / fac[n];
        dec(i, n - 1, 0) inv[i] = inv[i + 1] * (i + 1);
    }

    inline mi<mod> fact(ll n) {
        if (n < 0) return mi<mod>(0);
        return fac[n];
    }
    inline mi<mod> perm(ll n, ll r) {
        if (r < 0 || n < r) return mi<mod>(0);
        return fac[n] * inv[n - r];
    }
    inline mi<mod> comb(ll n, ll r) {
        if (r < 0 || n < r) return mi<mod>(0);
        return fac[n] * inv[r] * inv[n - r];
    }
    inline mi<mod> nHr(ll n, ll r) { return comb(n + r - 1, n - 1); }
};

struct UFR {
    vector<ll> data;
    UFR(ll N) : data(N) { rep(i, N) data[i] = -1; }
    def root(ll x) {
        if (data[x] < 0)
            return x;
        else
            return data[x] = root(data[x]);
    }
    def unite(ll x, ll y) {
        ll root_x = root(x), root_y = root(y);
        if (root_x != root_y) {
            if (data[root_x] > data[root_y]) swap(root_x, root_y);
            data[root_x] -= data[root_x] == data[root_y];
            data[root_y] = root_x;
            return true;
        }
        return false;
    }
    def same(ll x, ll y) { return root(x) == root(y); }
};

struct position {
    ll x, y;
    position() {}
    position(ll a, ll b) : x(a), y(b) {}
    position next(ll i) { return {x + dx[i], y + dy[i]}; }
    ll mdist() { return abs(x) + abs(y); }
    double dist() { return sqrt(x * x + y * y); }
    double norm(ll d) {
        if (d == inf) return max(x, y);
        if (d == 1) return mdist();
        if (d == 2) return dist();
        return 0;
    }
    ll num(ll width) { return abs(x) * width + abs(y); }

    bool operator==(position a) { return x == a.x && y == a.y; }
    bool operator!=(position a) { return x != a.x || y != a.y; }
    bool operator<(position a) { return x < a.x && y < a.y; }
    bool operator>(position a) { return x > a.x && y > a.y; }
    bool operator<=(position a) { return x <= a.x && y <= a.y; }
    bool operator>=(position a) { return x >= a.x && y >= a.y; }
    position operator+(position a) { return position(x + a.x, y + a.y); }
    position operator-(position a) { return position(x - a.x, y - a.y); }
    position operator*(position a) { return position(x * a.x, y * a.y); }
    position operator/(position a) { return position(x / a.x, y / a.y); }
    position operator%(position a) { return position(x % a.x, y % a.y); }
    position complex(position a) {
        return position(x * a.x - y * a.y, x * a.y + y * a.x);
    }
    /*
        // for sort:
        bool operator<(position a) { return x ^ a.x ? x < a.x : y < a.y; }
        bool operator>(position a) { return x ^ a.x ? x > a.x : y > a.y; }
        bool operator<=(position a) { return x ^ a.x ? x < a.x : y <= a.y; }
        bool operator>=(position a) { return x ^ a.x ? x > a.x : y >= a.y; }
    */
};
position Origin = position(0, 0);
using pos = position;
using vec = position;

template <class T> struct Array {
    struct node {
        ll childl, childr;
        T data;
        node(ll l, ll r, T t) : childl(l), childr(r), data(t) {}
    };
    ll n, depth;
    vector<ll> versions;
    vector<ll> prev_versions;
    vector<node> nodes;
    Array(ll n = 1 << 20, T val = T()) : n(n), depth(0) {
        while (n /= 2) depth++;
        init(val);
    }
    void init(T val) {
        versions.push_back(0);
        prev_versions.push_back(0);
        rep(i, 2 * n - 1) {
            if (i < n - 1) {
                nodes.push_back(node(2 * i + 1, 2 * i + 2, T()));
            } else {
                nodes.push_back(node(0, 0, val));
            }
        }
    }
    void set(ll index, ll val, ll version = -1) {
        ll id, par = nodes.size(), left = 0, right = n;
        if (version == -1) {
            id = versions.back();
            version = versions.size() - 1;
        } else {
            id = versions[version];
        }
        versions.push_back(par);
        prev_versions.push_back(version);
        if (right == -1) right = n;
        rep(i, depth) {
            ll mid = (left + right) / 2;
            if (index < mid) {
                nodes.push_back(node(par + i + 1, nodes[id].childr, T()));
                id = nodes[id].childl;
                right = mid;
            } else {
                nodes.push_back(node(nodes[id].childl, par + i + 1, T()));
                id = nodes[id].childr;
                left = mid;
            }
        }
        nodes.push_back(node(0, 0, val));
    }
    T get(ll index, ll version = -1) {
        ll id, left = 0, right = n;
        if (version == -1) {
            id = versions.back();
        } else {
            id = versions[version];
        }
        rep(i, depth) {
            ll mid = (left + right) / 2;
            if (index < mid) {
                id = nodes[id].childl;
                right = mid;
            } else {
                id = nodes[id].childr;
                left = mid;
            }
        }
        return nodes[id].data;
    }
    ll latest() { return versions.size() - 1; }
};

signed main() {
    ifstream fin("/Users/shuzaei/Desktop/f.mc");
    ofstream out("/Users/shuzaei/Desktop/f.txt");
    deque<char> S;
    char c;
    rep(i, 19) {
        fin >> c;
        S.push_back(c);
    }
    ll bpm = 174, l;
    double s, t;
    vector<tuple<ll, double, double>> chart;
    while (fin >> c) {
        S.push_back(c);
        vdump(S, 20);
        if (S[0] == '\"' && S[1] == 'c' && S[2] == 'o') {
            l = S[9] - '0';
            chart.push_back({l, s, t});
            debug(l);
        }
        if (S[0] == '\"' && S[1] == 'b' && S[2] == 'e') {
            ll itr = 0, f = 0, T = 0, K = 0, G = 0;
            while (itr++ < 20) {
                if (S[itr] == ',' || S[itr] == '[') f++;
                if (f == 0 && '0' <= S[itr] && S[itr] <= '9') {
                    T *= 10;
                    T += S[itr] - '0';
                }
                if (f == 1 && '0' <= S[itr] && S[itr] <= '9') {
                    K *= 10;
                    K += S[itr] - '0';
                }
                if (f == 2 && '0' <= S[itr] && S[itr] <= '9') {
                    G += S[itr] - '0';
                }
            }
            s = t = (4 * T + K + G / 4.0) * 60 / double(bpm);
            debug(s, t);
        }
        if (S[0] == '\"' && S[1] == 'e' && S[2] == 'n') {
            ll itr = 0, f = 0, T = 0, K = 0, G = 0;
            while (itr++ < 20) {
                if (S[itr] == ',' || S[itr] == '[') f++;
                if (f == 0 && '0' <= S[itr] && S[itr] <= '9') {
                    T *= 10;
                    T += S[itr] - '0';
                }
                if (f == 1 && '0' <= S[itr] && S[itr] <= '9') {
                    K *= 10;
                    K += S[itr] - '0';
                }
                if (f == 2 && '0' <= S[itr] && S[itr] <= '9') {
                    G += S[itr] - '0';
                }
            }
            t = (4 * T + K + G / 4.0) * 60 / double(bpm);
            debug(t);
        }
        S.pop_front();
    }
    sort(all(chart),
         [](tuple<ll, double, double> A, tuple<ll, double, double> B) {
             return get<1>(A) <= get<1>(B);
         });
    each(i, chart) {
        out << get<0>(i) + 1 << space << get<1>(i) << space << get<2>(i)
            << endl;
    }
}