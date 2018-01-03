cimport CDatabase as cdb

cdef class Database:
    cdef cdb.Database db ;

    def __cinit__(self):
        pass

    def __dealloc__(self):
        self.db.close()

    def open(self,filename,password):
        return self.db.open(filename,password)

    def execute(self,query):
        return self.db.execute(query)

    def close(self):
        return self.db.close()