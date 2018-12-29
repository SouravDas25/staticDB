import staticDb as sdb


class Database(object):

    def __init__(self):
        self.db = sdb.Database()

    def open(self, filename, password):
        return self.db.open(filename, password)

    def execute(self, query):
        return self.db.execute(query)

    def close(self):
        return self.db.close()
