#include <sparsehash/dense_hash_map>
#include <sparsehash/sparse_hash_map>
using GOOGLE_NAMESPACE::dense_hash_map;
using GOOGLE_NAMESPACE::sparse_hash_map;

template <typename Value, typename Key = std::string>
class TStringDenseHashMap : public dense_hash_map<Key, Value>
{
public:
	TStringDenseHashMap()
	{
		set_empty_key( std::string("") );
		set_deleted_key( std::string(" ") );
	}
};
template <typename Value, typename Key = std::string>
class TStringSparseHashMap : public sparse_hash_map<Key, Value>
{
public:
	TStringSparseHashMap()
	{
		set_deleted_key( (" ") );
	}
};

namespace detail
{
	struct eqstr
	{
		bool operator()(const char* s1, const char* s2) const
		{
			return (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
		}
	};
}
template <typename Value, typename Key = const char*>
class TConstCharDenseHashMap : public dense_hash_map<Key, Value, stdext::hash_compare<Key>, detail::eqstr>
{
public:
	TConstCharDenseHashMap()
	{
		set_empty_key( ("") );
		set_deleted_key( (" ") );
	}
};
template <typename Value, typename Key = const char*>
class TConstCharSparseHashMap : public sparse_hash_map<Key, Value, stdext::hash_compare<Key>, detail::eqstr>
{
public:
	TConstCharSparseHashMap()
	{
		set_deleted_key( (" ") );
	}
};