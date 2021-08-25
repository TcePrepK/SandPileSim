#ifndef PAIRHASH_H
#define PAIRHASH_H

struct pair_hash {
	template <class T1, class T2>
	size_t operator() (const pair<T1, T2>& pair) const {
		return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
	}
};

#endif // PAIRHASH_H