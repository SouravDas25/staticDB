#ifndef TUPLE_H
#define TUPLE_H

#include <iostream>
#include <exception>

#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdarg>
#include <stdint.h>
#include <string.h>

using namespace std;

namespace SDB {

    class tuple {

    public:
        /** Default constructor */
        tuple() {};

        tuple(const tuple &t) : arr(t.arr) {
            int i;
            for (i = 0; i < (int) t.data.size(); ++i) {
                void *v = calloc(1, arr[i]);
                memcpy(v, t.data[i], arr[i]);
                data.push_back(v);
            }
        }

        /** Default destructor */
        virtual ~tuple() {
            int i;
            for (i = 0; i < (int) data.size(); ++i) {
                free(data[i]);
            }
        };

        template<typename T>
        void get(int i, T &v) {
            if (arr[i] != sizeof(T)) {
                throw "TYPE ERROR";
            }
            v = *(T *) data[i];
        }

        void get(int i, char *v) {
            const char *c = (const char *) data[i];
            strcpy(v, c);
        }

        void get(int i, void *val, int size) {
            if (size == arr[i])
                memcpy(val, data[i], size);
        }

        int size(int i) {
            return arr[i];
        }

        template<typename T>
        void replace(int i, const T &val) {
            arr[i] = sizeof(T);
            free(data[i]);
            T *v = (T *) malloc(sizeof(T));
            *v = val;
            data[i] = v;
        }

        void replace(int i, char *val) {
            replace(i, (const char *) val);
        }

        void replace(int i, const char *val) {
            arr[i] = strlen(val) + 1;
            free(data[i]);
            char *v = (char *) calloc(arr[i], sizeof(char));
            strcpy(v, val);
            data[i] = v;
        }

        void replace(int i, const void *val, int size) {
            arr[i] = size;
            free(data[i]);
            void *v = malloc(size);
            memcpy(v, val, size);
            data[i] = v;
        }

        template<typename T>
        void push_back(const T &val) {
            arr.push_back(sizeof(T));
            T *v = (T *) malloc(sizeof(T));
            *v = val;
            data.push_back(v);
        }

        void push_back(const void *val, int size) {
            arr.push_back(size);
            void *v = malloc(size);
            memcpy(v, val, size);
            data.push_back(v);
        }

        void push_back(char *val) {
            const char *c = val;
            push_back(c);
        }

        void push_back(const char *val) {
            int len = strlen(val) + 1;
            arr.push_back(len);
            char *v = (char *) calloc(len, sizeof(char));
            strcpy(v, val);
            data.push_back(v);
        }

    protected:
        vector<int> arr;
        vector<void *> data;

    private:
    };

}
/*

int main ()
{
	SDB::tuple t;

	char name[50];
	int b = 89 ;

	t.push_back("SOURAV Das");
	t.push_back(b);
	//t.replace(0,"Gourav Das");

	SDB::tuple t2 = t;

	t2.get(0,name);
	t2.get(1,b);
	printf(" name = %s , age = %d",name,b);

}*/


#endif // TUPLE_H
