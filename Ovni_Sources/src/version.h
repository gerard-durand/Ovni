#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "16";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2021";
	static const char UBUNTU_VERSION_STYLE[] =  "21.12";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 4;
	static const long BUILD  = 10;
	static const long REVISION  = 9778;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1592;
	#define RC_FILEVERSION 5,4,10,9778
	#define RC_FILEVERSION_STRING "5, 4, 10, 9778\0"
	static const char FULLVERSION_STRING [] = "5.4.10.9778";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 30;
	

}
#endif //VERSION_H
