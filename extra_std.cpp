#include "extra_std.h"

namespace estd {
	using namespace std;
	string loadFile(string filename) {
		ifstream ifs(filename);
		return string( (istreambuf_iterator<char>(ifs) ),
							(istreambuf_iterator<char>()    ) );
	}
}