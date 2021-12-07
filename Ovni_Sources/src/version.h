#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2021";
	static const char UBUNTU_VERSION_STYLE[] =  "21.11";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 4;
	static const long BUILD  = 8;
	static const long REVISION  = 9527;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1588;
	#define RC_FILEVERSION 5,4,8,9527
	#define RC_FILEVERSION_STRING "5, 4, 8, 9527\0"
	static const char FULLVERSION_STRING [] = "5.4.8.9527";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 28;
	

}
#endif //VERSION_H
