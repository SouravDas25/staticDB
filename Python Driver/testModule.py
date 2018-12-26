import StaticDB


def main():
    db = StaticDB.Database()
    # print(StaticDB.start_Msg())
    while True:
        filename = raw_input("Enter File Name : ")
        if filename == "quit" or filename == "QUIT":
            break
        if filename == "default" or len(filename) < 1:
            filename = "testdb.sdb"
            password = "root"
        else:
            password = raw_input("Enter Password : ")
        print("Connecting To Database : '%s' \nwith password : '%s' " %
              (filename, password))
        t = db.open(filename, password)
        while t:
            query = raw_input("\n>>> ")
            if query.lower() == "quit":
                break
            try:
                print(db.execute(query))
            except RuntimeError as err:
                print(err)
        db.close()
        print("\n")


if __name__ == "__main__":
    main()
