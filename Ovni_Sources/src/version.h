#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.04";
	
	//Software Status
	static const char STATUS[] =  "Beta";
	static const char STATUS_SHORT[] =  "b";
	
	//Standard Version Type
	static const long MAJOR  = 5;
	static const long MINOR  = 2;
	static const long BUILD  = 1;
	static const long REVISION  = 757;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 225;
	#define RC_FILEVERSION 5,2,1,757
	#define RC_FILEVERSION_STRING "5, 2, 1, 757\0"
	static const char FULLVERSION_STRING [] = "5.2.1.757";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 59;
	

}
#endif //VERSION_H
