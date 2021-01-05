#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2021";
	static const char UBUNTU_VERSION_STYLE[] =  "21.01";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 13;
	static const long REVISION  = 8899;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1553;
	#define RC_FILEVERSION 5,3,13,8899
	#define RC_FILEVERSION_STRING "5, 3, 13, 8899\0"
	static const char FULLVERSION_STRING [] = "5.3.13.8899";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 22;
	

}
#endif //VERSION_H
