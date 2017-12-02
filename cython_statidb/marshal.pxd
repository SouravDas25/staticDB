
from libcpp.string cimport string
from db_tuple_module cimport *

cdef extern from "marshal.h" namespace "StaticDB":
	cdef cppclass Database:
		Database() except +
		nosqlRT connect(string& file,string& passwrd)
		nosqlRT execute(string& str)
		nosqlRT disconnect()



