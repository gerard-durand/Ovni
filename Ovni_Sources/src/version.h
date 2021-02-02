#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2021";
	static const char UBUNTU_VERSION_STYLE[] =  "21.01";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 17;
	static const long REVISION  = 9202;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1555;
	#define RC_FILEVERSION 5,3,17,9202
	#define RC_FILEVERSION_STRING "5, 3, 17, 9202\0"
	static const char FULLVERSION_STRING [] = "5.3.17.9202";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 24;
	

}
#endif //VERSION_H
