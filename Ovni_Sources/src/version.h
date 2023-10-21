#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.10";

	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";

	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 4;
	static const long BUILD  = 29;
	static const long REVISION  = 10793;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1616;
	#define RC_FILEVERSION 5,4,29,10793
	#define RC_FILEVERSION_STRING "5, 4, 29, 10793\0"
	static const char FULLVERSION_STRING [] = "5.4.29.10793";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 39;

}
#endif //VERSION_H
