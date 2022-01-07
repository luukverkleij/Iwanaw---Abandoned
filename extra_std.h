#ifndef _EXTRA_STD_H_
#define _EXTRA_STD_H_

#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

namespace SDL {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	#define RMASK = 0xff000000;
	#define GMASK = 0x00ff0000;
	#define BMASK = 0x0000ff00;
	#define AMASK = 0x000000ff;
#else
    #define RMASK = 0x000000ff;
    #define GMASK = 0x0000ff00;
    #define BMASK = 0x00ff0000;
    #define AMASK = 0xff000000;
#endif

	#define DEPTH = 32;
}

namespace estd {
	using namespace std;

	std::string loadFile(std::string filename);

	template <typename T>
	std::string toString(T t) {
		stringstream ss;
		string s;
		ss << t;
		ss >> s;
		return s;
	}

	template<typename A, typename B>
	std::pair<B,A> flip_pair(const std::pair<A,B> &p)
	{
		return std::pair<B,A>(p.second, p.first);
	}

	template<typename A, typename B>
	std::multimap<B,A> flip_map(const std::map<A,B> &src)
	{
		std::multimap<B,A> dst;
		std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
					   flip_pair<A,B>);
		return dst;
	}

	template<typename A, typename B>
	std::multimap<B,A> flip_map(const std::unordered_map<A,B> &src)
	{
		std::multimap<B,A> dst;
		std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
					   flip_pair<A,B>);
		return dst;
	}

	template <typename T>
	T max2d(std::vector<std::vector<T>> &vec) {
		std::vector<T> maxes;
		for (unsigned int index = 0; index < vec.size(); ++index) {
			maxes.push_back((*std::max_element(vec[index].begin(), vec[index].end())));
		}

		return (*std::max_element(maxes.begin(), maxes.end()));
	}
};

#endif