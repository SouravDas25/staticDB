#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "02";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.10";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 6;
	static const long BUILD  = 486;
	static const long REVISION  = 486;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2049;
	#define RC_FILEVERSION 0,6,486,486
	#define RC_FILEVERSION_STRING "0, 6, 486, 486\0"
	static const char FULLVERSION_STRING [] = "0.6.486.486";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 488;
	

}
#endif //VERSION_H
