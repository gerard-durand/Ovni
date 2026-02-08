#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2026";
	static const char UBUNTU_VERSION_STYLE[] =  "26.02";

	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";

	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 5;
	static const long BUILD  = 1;
	static const long REVISION  = 10793;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1628;
	#define RC_FILEVERSION 5,5,1,10793
	#define RC_FILEVERSION_STRING "5, 5, 1, 10793\0"
	static const char FULLVERSION_STRING [] = "5.5.1.10793";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 39;

}
#endif //VERSION_H
