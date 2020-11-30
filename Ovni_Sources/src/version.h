#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "30";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.11";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 5;
	static const long REVISION  = 6216;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1137;
	#define RC_FILEVERSION 5,3,5,6216
	#define RC_FILEVERSION_STRING "5, 3, 5, 6216\0"
	static const char FULLVERSION_STRING [] = "5.3.5.6216";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 25;
	

}
#endif //VERSION_H
