#include "../src/PhysicalLayer/fileObj.h"

using namespace StaticDB;

int main()
{
	FileLayer file;

	file.open("test.txt","rb+");
	int num = 5;
	int p[num]  ; //= {1,2,3,4,5} ;

	file.readRaw(&p,sizeof(p));
//	p=45;
//	File.seek(100);
//	File.read(&p,sizeof(p));

	//file.writeRaw(&p,sizeof(p));

	int i;
	for(i = 0; i < num; i++) {
      cout << p[i] << " , " ;
   }
   file.close();
}

