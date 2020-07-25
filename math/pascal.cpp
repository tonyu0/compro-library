constexpr int MOD = 1e9 + 7;
long table[1000][1000];

void make_pascal()
{
    for (int i = 0; i < 1000; ++i)
    {
        table[i][0] = 1;
        for (int j = 1; j < i; j++)
        {
            table[i][j] = (table[i - 1][j - 1] + table[i - 1][j]) % MOD;
        }
        table[i][i] = 1;
    }
}
