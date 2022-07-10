#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2022";
	static const char UBUNTU_VERSION_STYLE[] =  "22.06";

	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";

	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 4;
	static const long BUILD  = 14;
	static const long REVISION  = 10120;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1592;
	#define RC_FILEVERSION 5,4,14,10120
	#define RC_FILEVERSION_STRING "5, 4, 14, 10120\0"
	static const char FULLVERSION_STRING [] = "5.4.14.10120";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 33;

}
#endif //VERSION_H
