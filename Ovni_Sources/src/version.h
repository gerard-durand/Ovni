#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "14";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.11";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 3;
	static const long BUILD  = 3;
	static const long REVISION  = 5076;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 949;
	#define RC_FILEVERSION 5,3,3,5076
	#define RC_FILEVERSION_STRING "5, 3, 3, 5076\0"
	static const char FULLVERSION_STRING [] = "5.3.3.5076";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 13;
	

}
#endif //VERSION_H
