#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2022";
	static const char UBUNTU_VERSION_STYLE[] =  "22.04";

	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";

	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 4;
	static const long BUILD  = 13;
	static const long REVISION  = 9968;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1592;
	#define RC_FILEVERSION 5,4,13,9968
	#define RC_FILEVERSION_STRING "5, 4, 13, 9968\0"
	static const char FULLVERSION_STRING [] = "5.4.13.9968";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 32;

}
#endif //VERSION_H
