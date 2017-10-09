#include <iostream>
#include <vector>
#include <cassert>
//---------------------------------------------------------------------------
// Тип массива целых чисел
typedef std::vector<int> IntArray;

//---------------------------------------------------------------------------
// Обмен значениями двух переменных
template <class Type> inline void swap(Type &a, Type &b) {
	register Type oldA = a;
	a = b;
	b = oldA;
}

//---------------------------------------------------------------------------
// Инвертирование значения переменной
// с множеством значений {0, 1}
inline void invert(int &a) {
	assert (a == 1 || a == 0);
	a = 1 - a;
}

//---------------------------------------------------------------------------
// Вывод на экран содержимого массива
void print(const IntArray &x, const unsigned begin, const unsigned end) {
	if (begin < end) {
		assert (end < x.size());
		for (unsigned i = begin; i <= end; ++i) {
			std::cout << x[i] << ' ';
		}
	} else {
		assert (begin < x.size());
		for (unsigned i = begin; i >= end; --i) {
			std::cout << x[i] << ' ';
		}
	}
	std::cout << std::endl;
}

//---------------------------------------------------------------------------
// Вывод на экран множества, представленного массивом из нулей и единиц
void printSet(const IntArray &x, const unsigned begin, const unsigned end) {
	bool empty = true;
	if (begin < end) {
		assert (end < x.size());
		for (unsigned i = end; i >= begin; --i) {
			if (x[i] > 0) {
				empty = false;
				std::cout << (i - begin + 1) << ' ';
			}
		}
	} else {
		assert (begin < x.size());
		for (unsigned i = end; i <= begin; ++i) {
			if (x[i] > 0) {
				empty = false;
				std::cout << (i - end + 1) << ' ';
			}
		}
	}
	if (empty) {
		// Множество пустое
		std::cout << 0;
	}
	std::cout << std::endl;
}

//---------------------------------------------------------------------------
// Порождение всех помножеств множества из n элементов
// в лексикографическом порядке
void tuples(const int n) {
	assert (n >= 1);

	int i;
	IntArray b(n + 1);

	for (i = 0; i <= n; ++i) {
		b[i] = 0;
	}
	while (b[n] != 1) {
		printSet(b, n - 1, 0);
		for (i = 0; b[i] == 1; ++i) {
			b[i] = 0;
		}
		b[i] = 1;
	}
}

//---------------------------------------------------------------------------
// Порождение всех подмножеств множества из n элементов
// в порядке минимального изменения
// (двоично-отражённый код Грея)
void tuplesMinChange(const int n) {
	assert (n >= 1);

	int i, j, t;
	IntArray q(n + 1), s(n + 1);

	t = 0;
	for (j = n; j >= 1; --j) {
		q[j] = 0;
		++t;
		s[t] = j;
	}
	while (t >= 0) {
		printSet(q, n, 1);
		i = s[t];
		--t;
		invert(q[i]);
		for (j = i - 1; j >= 1; --j) {
			++t;
			s[t] = j;
		}
	}
}

//---------------------------------------------------------------------------
// Порождение сочетаний из n элементов по k
// в лексикографическом порядке
void combinations(const int n, const int k) {
	assert (k >= 1);
	assert (n >= k);

	IntArray c(k + 1);
	int i, j;

	c[0] = -1;
	for (i = 1; i <= k; ++i) {
		c[i] = i;
	}
	j = 1;
	while (j != 0) {
		print(c, 1, k);
		j = k;
		while (c[j] == (n - k + j)) {
			--j;
		}
		++c[j];
		for (i = j + 1; i <= k; ++i) {
			c[i] = c[i - 1] + 1;
		}
	}
}

//---------------------------------------------------------------------------
// Порождение сочетаний из n элементов по k
// в порядке минимального изменения
void combinationsMinChange(const int n, const int k) {
	assert (k >= 1);
	assert (n >= k);

	int i, j, t;
	IntArray g(n + 2), f(n + 2);

	for (j = 1; j <= k; ++j) {
		g[j] = 1;
		f[j] = j + 1;
	}
	for (j = k + 1; j <= n + 1; ++j) {
		g[j] = 0;
		f[j] = j + 1;
	}
	t = k;
	f[1] = k + 1;
	i = 0;
	while (i != n + 1) {
		printSet(g, n, 1);
		i = f[1];
		f[1] = f[i];
		f[i] = i + 1;
		if (g[i] == 1) {
			if (t != 0) {
				invert(g[t]);
			} else {
				invert(g[i - 1]);
			}
			++t;
		} else {
			if (t != 1) {
				invert(g[t - 1]);
			} else {
				invert(g[i - 1]);
			}
			--t;
		}
		invert(g[i]);
		if ((t == i - 1) || (t == 0)) {
			++t;
		} else {
			t -= g[i - 1];
			f[i - 1] = f[1];
			if (t == 0) {
				f[1] = i - 1;
			} else {
				f[1] = t + 1;
			}
		}
	}
}

//---------------------------------------------------------------------------
// Порождение перестановок из n элементов
// в лексикографическом порядке
void permutations(const int n) {
	assert (n >= 1);

	int i, j, r, s;
	IntArray p(n + 1);

	for (j = 0; j <= n; ++j) {
		p[j] = j;
	}
	i = 1;
	while (i != 0) {
		print(p, 1, n);
		// Найти самое правое место, где p[i] < p[i + 1]
		i = n - 1;
		while (p[i] > p[i + 1]) {
			--i;
		}
		// Найти p[j], наименьший элемент справа от p[i] и больший его
		j = n;
		while (p[i] > p[j]) {
			--j;
		}
		// Поменять местами p[i] и p[j],
		// а затем перевернуть p[i + 1], ..., p[n]
		swap(p[i], p[j]);
		for (r = n, s = i + 1; r > s; --r, ++s) {
			swap(p[r], p[s]);
		}
	}
}

//---------------------------------------------------------------------------
// Порождение перестановок из n элементов
// в порядке минимального изменения
void permutationsMinChange(const int n) {
	assert (n >= 1);

	int i, m;
	IntArray d(n + 1), q(n + 1), p(n + 2);

	for (i = 1; i <= n; ++i) {
		q[i] = p[i] = i;
		d[i] = -1;
	}
	d[1] = 0;
	p[0] = p[n + 1] = m = n + 1;
	while (m != 1) {
		print(p, 1, n);
		for (m = n; p[q[m] + d[m]] > m; --m) {
			d[m] = -d[m];
		}
		swap(p[q[m]], p[q[m] + d[m]]);
		// в этот момент p[q[m] + d[m]] == m
		swap(q[p[q[m]]], q[m]);
	}
}

//---------------------------------------------------------------------------
// Порождение размещений из n элементов по k с повторениями
// в лексикографическом порядке
void arrangementsWithRepeats(const int n, const int k) {
	assert (k >= 1);
	assert (n >= 1);

	IntArray b(k + 1);
	int i;

	for (i = 0; i <= k; ++i) {
		b[i] = 1;
	}
	while (b[k] != 2) {
		print(b, k - 1, 0);
		for (i = 0; b[i] == n; ++i) {
			b[i] = 1;
		}
		++b[i];
	}
}

//---------------------------------------------------------------------------
// Порождение размещений из n элементов по k без повторений
void arrangements(const int n, const int r) {
	assert (r >= 1);
	assert (n >= r);

	int i, j, k, l, a, f;
	IntArray m(n + 1);

	a = 1;
	for (i = n - r + 1; i <= n; ++i) {
		a *= i;
	}
	for (i = 1; i <= a; ++i) {
		k = i - 1;
		for (j = 1; j <= n; ++j) {
			m[j] = j;
		}
		for (j = 1; j <= r; ++j) {
			l = k % (n - j + 1) + 1;
			k /= n - j + 1;
			std::cout << m[l] << ' ';
			for (f = l; f <= n - j; ++f) {
				m[f] = m[f + 1];
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//---------------------------------------------------------------------------
int main() {
	//arrangements(5, 3);
	unsigned n, r;
	std::cin >> n >> r;
	arrangements(n, r);
	return 0;
}
