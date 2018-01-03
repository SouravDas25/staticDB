
from libcpp.string cimport string
from libcpp.string cimport string as string
from libcpp cimport bool
from libc.stdint cimport *

cdef extern from "Database.h" namespace "StaticDB":
	cdef cppclass Database:
		Database() except +
		bool open(const string& filename,const string& passwrd) except +
		string execute(const string& query) except +
		bool close() except +



