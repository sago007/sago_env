#include <iostream>
#include "sago/platform_folders.h"

using namespace std;
using namespace sago;

int main()
{
	cout << "Config: " << GetConfigHome() << endl;
	cout << "Data: " << GetDataHome() << endl;
	cout << "Cache: " << GetCacheDir() << endl;
	return 0;
}
