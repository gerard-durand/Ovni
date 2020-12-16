#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "16";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.12";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 6;
	static const long REVISION  = 8356;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1543;
	#define RC_FILEVERSION 5,3,6,8356
	#define RC_FILEVERSION_STRING "5, 3, 6, 8356\0"
	static const char FULLVERSION_STRING [] = "5.3.6.8356";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 18;
	

}
#endif //VERSION_H
