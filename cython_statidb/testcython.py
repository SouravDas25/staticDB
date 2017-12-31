import staticDB


def main():
    db = staticDB.Database()
    print(staticDB.start_Msg())
    while True:
        filename = raw_input("Enter File Name : ")
        if filename == "quit" or filename == "QUIT":
            break
        if filename == "default" or len(filename) < 1:
            filename = "testdb.sdb"
            password = "root"
        else:
            password = raw_input("Enter Password : ")
        print(" Connecting To Database : '%s' \nwith password : '%s' " % (filename, password))
        t = db.connect(filename, password)
        if t == staticDB.SUCCESS_MSG:
            while True:
                st = raw_input("\n>>> ")
                t = db.execute(st)
                if t == None:
                    break
                elif t == staticDB.SUCCESS_MSG or t == staticDB.ERROR_MSG:
                    print(db.get_Msg())
                else:
                    print(staticDB.pretty_print(t))
        else:
            print(db.get_Msg())
        db.disconnect()
        print (db.get_Msg(), "\n")


##db = staticDB.Database()
##db.connect("testdb.sdb", "root")
##l= db.execute("GET tbl")

main()
