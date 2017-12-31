from staticDB import Database


def main():
    db = Database.Database()
    print Database.start_Msg()
    while True:
        filename = raw_input("Enter File Name : ")
        if filename == "quit" or filename == "QUIT":
            break
        if filename == "default" or len(filename) < 1:
            filename = "testdb.sdb"
            password = "root"
        else:
            password = raw_input("Enter Password : ")
        print "Connecting To Database : '%s' \nwith password : '%s'" % (filename, password)
        t = db.connect(filename, password)
        if t == Database.SUCCESS_MSG:
            while True:
                st = raw_input("\n>>> ")
                t = db.execute(st)
                if t == None:
                    break
                elif t == Database.SUCCESS_MSG or t == Database.ERROR_MSG:
                    print db.get_Msg()
                else:
                    print Database.pretty_print(t)
        else:
            print db.get_Msg()
        db.disconnect()
        print db.get_Msg(), "\n"


##db = Database.Database()
##db.connect("testdb.sdb", "root")
##l= db.execute("GET tbl")

main()
