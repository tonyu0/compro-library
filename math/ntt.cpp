#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;
constexpr ll MOD = 1e9 + 7;

ll extmod(ll a, ll mod) { return (a % mod + mod) % mod; }

void extGCD(ll a, ll b, ll &p, ll &q)
{
	if (b == 0)
	{
		p = 1, q = 0;
		return;
	}
	extGCD(b, a % b, q, p);
	q -= a / b * p;
}

ll modinv(ll a, ll mod)
{
	ll x, y;
	extGCD(a, mod, x, y);
	return extmod(x, mod);
}

ll modpow(ll a, ll b, ll mod)
{
	ll r = 1;
	while (b)
	{
		if (b & 1)
			(r *= a) %= mod;
		(a *= a) %= mod;
		b >>= 1;
	}
	return r;
}

vector<ll> modfft(vector<ll> A, ll mod, int root, bool inverse = false)
{
	int n = A.size(), mask = n - 1;
	vector<ll> tmp(n);
	for (int i = n >> 1; i >= 1; i >>= 1)
	{
		ll zeta = modpow(root, (mod - 1) / n * i, mod);
		if (inverse)
			zeta = modinv(zeta, mod);
		ll powzeta = 1;
		for (int j = 0; j < n; j += i)
		{
			for (int k = 0; k < i; ++k)
				tmp[j | k] = (A[((j << 1) | k) & mask] + powzeta * A[((j << 1) | i | k) & mask]) % mod;
			powzeta = powzeta * zeta % mod;
		}
		swap(A, tmp);
	}

	if (inverse)
	{
		ll invn = modinv(n, mod);
		for (int i = 0; i < n; ++i)
			(A[i] *= invn) %= mod;
	}
	return A;
}

vector<ll> conv(vector<ll> A, vector<ll> B, ll mod, int root)
{
	int siz = A.size() + B.size() - 1, n = 1;
	while (n < siz)
		n <<= 1;
	A.resize(n), B.resize(n);
	A = modfft(A, mod, root), B = modfft(B, mod, root);
	for (int i = 0; i < n; ++i)
		(A[i] *= B[i]) %= mod;
	A = modfft(A, mod, root, true);
	return A;
}

vector<ll> sqconv(vector<ll> A, ll mod, int root)
{
	int siz = 2 * A.size() - 1, n = 1;
	while (n < siz)
		n <<= 1;
	A.resize(n);
	A = modfft(A, mod, root);
	for (int i = 0; i < n; ++i)
		(A[i] *= A[i]) %= mod;
	A = modfft(A, mod, root, true);
	return A;
}

ll n, m;
int main()
{
	cin >> n >> m;
	vector<ll> a(n + 1);
	for (int i = 1; i <= n; ++i)
		cin >> a[i];
	sort(a.begin() + 1, a.end(), greater<ll>());

	// mod = 2^24 * 73 + 1
	vector<ll> c = a;
	int mod = 0x49000001, root = 3;
	vector<ll> b = conv(a, c, mod, root);
	ll ans = 0;
	for (int i = 1; i <= m; ++i)
	{
		cout << a[i] << " " << b[i] << endl;
		ans += b[i];
	}
	cout << ans;
	return 0;
}