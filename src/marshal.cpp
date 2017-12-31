


#include "../include/marshal.h"

namespace StaticDB {


    nosqlRT Database::connect(const string &file, const string &passwrd) {
        if (this->obj != nullptr) {
            return noSql_Msg(ERROR_MSG, "Database Not Initialized With 'new_SDB' |  Database Already Connected.");
        }
        try {
            return ads_connectDB(&this->obj, file, passwrd);
        }
        catch (const sdb_error &e) {
            return noSql_Msg(ERROR_MSG, e.what());
        }
    }

    nosqlRT Database::execute(const string &str) {
        if (this->obj == nullptr) {
            return noSql_Msg(ERROR_MSG, "Database Not Connected");
        }
        try {
            return sdb_parse(this->obj, str);
        }
        catch (const sdb_error &e) {
            return noSql_Msg(ERROR_MSG, e.what());
        }
    }

    nosqlRT Database::disconnect() {
        if (this->obj == nullptr) {
            return noSql_Msg(ERROR_MSG, "Database Not Connected");
        }
        try {
            ads_disconnectDB(this->obj);
            this->obj = nullptr;
            return noSql_Msg(SUCCESS_MSG, "Database Successfully Disconnected.");
        }
        catch (const sdb_error &e) {
            return noSql_Msg(ERROR_MSG, e.what());
        }
    }

    static nosqlRT run_Script(staticDB *db, const char *file) {
        FILE *fptr = fopen(file, "r");
        if (fptr == nullptr) {
            return noSql_Msg(ERROR_MSG, "File Cannot Be Read.");
        }
        char ch;
        ch = fgetc(fptr);
        string val;
        bool qou1 = false, qou2 = false;
        while (ch != EOF) {
            if (ch == '\'' && qou2 == false) qou1 = !qou1;
            if (ch == '"' && qou1 == false) qou2 = !qou2;
            if ((ch == ';' && qou1 == false && qou2 == false) || ch == EOF) {
                //cout <<"\n val = "<< val;
                nosqlRT rt = sdb_parse(db, val);
                if (rt != RETURN_TUPLE) cout << get_NoSqlMsg() << endl;
                val = string();
            } else val.push_back(ch);
            ch = fgetc(fptr);
        }
        fclose(fptr);
        //cout <<"\n val = "<< val;
        sdb_parse(db, val);
        return noSql_Msg(SUCCESS_MSG, "Successfully Imported All Statement.");
    }

    static void db_export(staticDB *db, const string &file, bool jsn) {
        using StaticDB::tuple;
        ofstream fout;
        fout.open(file.c_str(), ios::out);
        ads_pairKeys(db);
        tuple t = get_NoSqlReturnTuple();
        for (int i = 0; i < t.size(); i++) {
            string s = "GET " + t.get_string(i);
            sdb_parse(db, s);
            if (!jsn)fout << "CREATE " << t.get_string(i) << " = ";
            get_NoSqlReturnTuple().print(fout);
            if (!jsn)fout << " ;";
            fout << "\n\n";
        }
        fout.close();
    }

#define helpMsg \
"Commands Are : \n"\
"+ CREATE - create a collection in database.\n"\
"    - CREATE keyname = {1,'Dummy',3.235}  \n"\
"    - CREATE COLLECTION collectioname = {1,'Dummy',3.235}  \n\n"\
"+ GET - just return values of specific keys no query.\n"\
"    - GET keyname.childname[index]  \n"\
"    - GET collectioname  \n"\
"    - GET collectioname[rowid]  \n\n"\
"+ DELETE - delete data from existing collection or entire collection.\n" \
"    - DELETE keyname.attribute \n"\
"    - DELETE collectioname \n"\
"    - DELETE collectioname[row Index]  \n\n"\
"+ INSERT - insert data into exsisting collection. \n"\
"    - INSERT INTO keyname.attribute VALUES = {1,'Dummy',3.235} \n"\
"    - INSERT ROW INTO collectioname VALUES = {1,'Dummy',3.235}  \n\n"\
"+ UPDATE - change the value of any collection or its keys \n"\
"    - UPDATE keyname.attribute = value ) \n"\
"    - UPDATE collectioname[row index]  = value ) \n\n"\
"+ FIND - query collection of data.\n"\
"    - FIND {} IN keyname.attribute  \n"\
"    - FIND {'attribute',...} IN keyname.attribute \n"\
"    - FIND {'attribute',...} IN keyname.attribute WHERE { conditions ,... } \n\n"\
"+ SET - used to change value of many system variable.\n"\
"    - SET { cache_size : 50 }   \n\n"\
"- COMMIT - permanently saves the changes made to the disk \n"\
"- HELP   - gives detail view of different commands\n"\
"- QUIT   - returns a quit signal and frees up all cache.\n\n"\
"+ IMPORT - execute database statement wriiten on a file.\n"\
"    - IMPORT 'test.sdb' \n\n"\
"+ EXPORT - export all contained collection into the given file in a perticular format.\n"\
"    - EXPORT 'test.sdb' SDB \n"\
"    - EXPORT 'test.sdb' JSON \n\n"\



    nosqlRT sdb_execute(staticDB *db, vector<string> &cmd) {
        nosqlRT r = noSql_Msg(ERROR_MSG, "NoSQL Statement Not Recognized.");

        proc p;
        p.cmd = GetCommand(cmd[0]);
        //printvos(res);cout << endl;
        //printvos(cmd);
        if (p.cmd == nosql_CREATE) {
            if (cmd.size() == 4) //! >>> CREATE keyname = { key : data } ;
            {
                p.key = cmd[1];
                if (stricmp(cmd[2], "=") != 0) return noSql_Msg(ERROR_MSG, "Not A Valid CREATE Statement.");
                if (pairExists(db, p.key) != false) return noSql_Msg(ERROR_MSG, "Identifier Already Exists.");
                tuple t = tupleLiteral(db, cmd[3]);
                //t.print();
                if (t.empty() || t.size() == 0) return noSql_Msg(ERROR_MSG, "Cannot Insert Null Tuple.");
                return pairCreate(db, p.key, t);
            } else if (cmd.size() == 5) //! >>> CREATE COLLECTION keyname = { data } ;
            {
                if (stricmp(cmd[1], "COLLECTION") != 0) return noSql_Msg(ERROR_MSG, "Not A Valid CREATE Statement.");
                p.key = cmd[2];
                if (pairExists(db, p.key) != false) return noSql_Msg(ERROR_MSG, "Identifier Already Exists.");
                if (stricmp(cmd[3], "=") != 0) return noSql_Msg(ERROR_MSG, "Not A Valid CREATE Statement.");
                p.key = cmd[2];
                tuple t = tupleLiteral(db, cmd[4]);
                if (t.empty() || t.size() == 0) return noSql_Msg(ERROR_MSG, "Cannot Insert Null Tuple.");
                return createTable(db, p.key, t);
            }
        } else if (p.cmd == nosql_DELETE) //! >>> DELETE keyname.attribute
        {
            if (cmd.size() == 2) {
                p.key = cmd[1];
                tuple kt = splitKey(p.key);
                if (kt.size() && kt.get_type(0) == db_STRING) {
                    if (pairExists(db, kt.get_string(0))) return ads_pairDelete(db, kt);
                }
                return noSql_Msg(ERROR_MSG, "Identifier Not Present In Database.");
            }
            return noSql_Msg(ERROR_MSG, "Not A Valid Delete statement.");
        } else if (p.cmd == nosql_UPDATE) //! >>> UPDATE keyname.attribute = value ;
        {
            if (cmd.size() == 4) {
                if (stricmp(cmd[2], "=") != 0) return noSql_Msg(ERROR_MSG, "Not A Valid UPDATE statement.");
                p.key = cmd[1];
                tuple kt = splitKey(p.key);
                if (kt.size() && kt.get_type(0) == db_STRING) {
                    if (pairExists(db, kt.get_string(0))) {
                        cmd[3].insert(0, "[");
                        cmd[3].push_back(']');
                        tuple t = tupleLiteral(db, cmd[3]);
                        if (t.empty() || t.size() == 0) return noSql_Msg(ERROR_MSG, "Cannot Insert Empty Tuple.");
                        return pairUpdate(db, kt, t);
                    }
                }
                return noSql_Msg(ERROR_MSG, "Identifier Not Present In Database.");
            }
            return noSql_Msg(ERROR_MSG, "Not A Valid UPDATE statement.");
        } else if (p.cmd == nosql_INSERT) {
            if (cmd.size() == 6) //! >>> INSERT INTO keyname.attr VALUES = { data } ;
            {
                if (stricmp(cmd[1], "INTO") == 0 && stricmp(cmd[3], "VALUES") == 0 && stricmp(cmd[4], "=") == 0) {
                    p.key = cmd[2];
                    tuple kt = splitKey(p.key);
                    //kt.print();
                    if (kt.size() && kt.get_type(0) == db_STRING) {
                        if (pairExists(db, kt.get_string(0))) {
                            tuple t = tupleLiteral(db, cmd[5]);
                            if (t.empty() || t.size() == 0) return noSql_Msg(ERROR_MSG, "Cannot Insert Empty Tuple.");
                            return pairInsert(db, kt, t);
                        }
                        return noSql_Msg(ERROR_MSG, "Identifier Not Present in Database.");
                    }
                    return noSql_Msg(ERROR_MSG, "Identifier Should Be A Name Of A Key Or Collection.");
                }
                return noSql_Msg(ERROR_MSG, "Insert Statement Should Contain An INTO And A VALUES Keyword.");
            } else if (cmd.size() == 7) //! >>> INSERT ROW INTO keyname.attr VALUES = { data } ;
            {
                if (stricmp(cmd[1], "ROW") == 0) {
                    if (stricmp(cmd[2], "INTO") == 0 && stricmp(cmd[4], "VALUES") == 0 && stricmp(cmd[5], "=") == 0) {
                        p.key = cmd[3];
                        if (pairExists(db, p.key)) {
                            tuple t = tupleLiteral(db, cmd[6]);
                            if (t.empty() || t.size() == 0) return noSql_Msg(ERROR_MSG, "Cannot Insert Null Tuple.");
                            return tableInsert(db, p.key, t);
                        }
                        return noSql_Msg(ERROR_MSG, "Identifier Should Be A Name Of A Key Or Collection.");
                    }
                    return noSql_Msg(ERROR_MSG, "Insert Statement Should Contain An INTO And A VALUES Keyword.");
                }
            }
            return noSql_Msg(ERROR_MSG, "Not A Proper Insert Statement.");
        } else if (p.cmd == nosql_GET) //! >>> GET keyname.attr ;
        {
            if (cmd.size() == 2) {
                return pair_get(db, cmd[1]);
            }
            return noSql_Msg(ERROR_MSG, "Not A Valid GET Statement.");
        } else if (p.cmd == nosql_COMMIT) //! >>> COMMIT ;
        {
            return ads_dbCommit(db);
        } else if (p.cmd == nosql_QUIT) //! >>> QUIT ;
        {
            return noSql_Msg(QUIT_MSG, "Database Not Disconnected Should Be Disconnected Manually.");
        } else if (p.cmd == nosql_HELP) //! >>> HELP ;
        {
            return noSql_Msg(SUCCESS_MSG, helpMsg);
        } else if (p.cmd == nosql_IMPORT) //! >>> IMPORT "filename" ;
        {
            if (cmd.size() == 2) {
                if (cmd[1].find("'") == string::npos && cmd[1].find("\"") == string::npos) {
                    return noSql_Msg(ERROR_MSG, "File Name Must Be Within Quotes.");
                }
                cmd[1].erase(0, 1);
                cmd[1].erase(cmd[1].size() - 1, 1);
                run_Script(db, cmd[1].c_str());
            }
        } else if (p.cmd == nosql_EXPORT) //! >>> EXPORT "filename" SDB|JSON ;
        {
            if (cmd.size() == 3) {
                if (cmd[1].find("'") == string::npos && cmd[1].find("\"") == string::npos) {
                    return noSql_Msg(ERROR_MSG, "File Name Must Be Within Quotes.");
                }
                cmd[1].erase(0, 1);
                cmd[1].erase(cmd[1].size() - 1, 1);
                bool jsn = false;
                if (stricmp(cmd[2], "JSON") == 0)jsn = true;
                db_export(db, cmd[1], jsn);
                return noSql_Msg(SUCCESS_MSG, "Exported Successfully.");
            }
        } else if (p.cmd == nosql_FIND) //! >>> FIND { projection } IN kilo WHERE { selection } ;
        {
            if (cmd.size() == 4 || cmd.size() == 6) {
                if (stricmp(cmd[2], "IN") != 0)
                    return noSql_Msg(ERROR_MSG, "An IN Keyword Should Be Present In FIND Query Statement.");
                if (cmd.size() == 6 && stricmp(cmd[4], "WHERE") != 0)
                    return noSql_Msg(ERROR_MSG, "A WHERE Keyword Should Be Present Before The Query.");
                p.key = cmd[3];
                string res = (cmd.size() == 6) ? cmd[5] : string();
                tuple pt = tupleLiteral(db, cmd[1]);
                tuple st = tupleLiteral(db, res);
                return cmd_find(db, pt, p.key, st);
            }
            return noSql_Msg(ERROR_MSG, "Not A Valid Find Command");
        } else if (p.cmd == nosql_SET) //! >>> SET { "CACHE_SIZE" : 45 } ;
        {
            if (cmd.size() == 2) {
                tuple st = tupleLiteral(db, cmd[1]);
                if (st.has_key("CACHE_SIZE")) {
                    return set_cache_size(db, st.get_int("CACHE_SIZE"));
                }
                return noSql_Msg(ERROR_MSG, "Command Successfully Executed.");
            }
            return noSql_Msg(ERROR_MSG, "Not A Valid SET Command");
        }
        return r;
    }


}

#undef helpMsg
