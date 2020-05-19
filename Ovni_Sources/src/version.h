#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "19";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.05";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 2;
	static const long BUILD  = 7;
	static const long REVISION  = 1686;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 408;
	#define RC_FILEVERSION 5,2,7,1686
	#define RC_FILEVERSION_STRING "5, 2, 7, 1686\0"
	static const char FULLVERSION_STRING [] = "5.2.7.1686";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 30;
	

}
#endif //VERSION_H
