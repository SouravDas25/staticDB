#ifndef SDB_FILESOBJ_H
#define SDB_FILESOBJ_H

#include "utility.h"

extern int errno;

namespace StaticDB {

    typedef uint64_t filepos;
    typedef uint8_t byte;
    typedef vector<byte> byte_array;

    class db_file {

        enum file_mode { CLOSED = 0 , READ , WRITE , READ_WRITE };

        string filename;
        file_mode mode;
        FILE * fp;

        public :

            db_file() : filename(), mode(CLOSED) , fp(nullptr)  {};

            db_file(const db_file& df) : filename(), mode(CLOSED) , fp(nullptr)
            {
                *this = df;
            };

            const db_file& operator=(const db_file& df)
            {
                filename = df.filename;
                mode = df.mode;
                fp = df.fp;
                return *this;
            }

            ~db_file(){};

            bool open(const string& fn,const string& mde)
            {
                this->filename = fn;
                fp = fopen(fn.c_str(),mde.c_str());
                if(fp == nullptr )
                {
                    cerr << "\nError opening '" << fn << "' in '" << mde << "' mode : " << strerror( errno ) << endl ;
                    return false;
                }
                if( mde.find('r') != string::npos ) this->mode = READ;
                if( mde.find('w') != string::npos ) this->mode = WRITE;
                if( mde.find('+') != string::npos ) this->mode = READ_WRITE;
                return true;
            }

            uint64_t size() const
            {
                filepos tmp = tell();
                seek(0L, db_file::end );
                uint64_t s = ftell(fp);
                seek( tmp );
                return s;
            }

            bool close()
            {
                if(fp)fclose(fp);
                fp = nullptr;
                mode = CLOSED;
                return true;
            }

            int read(void) const
            {
                return fgetc(fp);
            }

            int read(void * buffer , int length) const
            {
                if( mode != READ && mode != READ_WRITE ) return 0;
                return fread(buffer,length,sizeof(byte),fp);
            }

            byte_array read(int length) const
            {
                if( mode != READ && mode != READ_WRITE ) return byte_array();
                byte * bt = new byte[length+1];
                memset(bt,0,length+1);
                int red = read(bt,length);
                bt[length] = 0;
                byte_array ba(bt,bt+red/sizeof(byte));
                delete bt;
                return ba;
            }

            byte_array read(filepos pos,int length) const
            {
                if( !seek(pos) ) return byte_array();
                return read(length);
            }

            int write(int byt) const
            {
                return fputc(byt,fp);
            }

            int write(const void * buffer, int length) const
            {
                if( mode != WRITE && mode != READ_WRITE ) return 0;
                return fwrite(buffer,sizeof(byte),length,fp);
            }

            int write(const byte_array& ba) const
            {
                return write(ba.data(),ba.size());
            }

            int write(const string& s) const
            {
                return write(s.data(),s.size());
            }

            filepos tell() const
            {
                #if UINTPTR_MAX == 0xffffffffffffffff
                return ftello64(fp);
                #else
                return ftell(fp);
                #endif
            }

            bool seek(int64_t pos,int whence = SEEK_SET) const
            {
                #if UINTPTR_MAX == 0xffffffffffffffff
                if ( fseeko64(fp,pos,whence) != 0 )
				{
					cerr << endl << strerror( errno ) << endl ;
					return false;
				}
                #else
                if ( fseek(fp,pos,whence) != 0 )
                {
                	cerr << endl << strerror( errno ) << endl ;
					return false;
				}
                #endif
                return true;
            }

            static const int end = SEEK_END;
            static const int current = SEEK_CUR;
            static const int start = SEEK_SET;

    };

    ostream& operator<<(ostream& out,byte_array ba);


}

#endif
