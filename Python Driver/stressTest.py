import StaticDB

filename = "testdb.sdb"
password = "root"
            
def main():
    db = StaticDB.Database()
    t = db.open(filename, password)
