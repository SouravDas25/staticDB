
#ifndef SDB_PARSER_C
#define SDB_PARSER_C

#include "../include/parser.h"

namespace StaticDB {

/*
    static char * remove_space_completely(char * str)
    {
        int i,len = strlen(str),cnt =0;bool qou1 = false,qou2 = false;
        for(i=0;i<len;i++)
        {
            if(isprint(str[i]) == false )continue;
            if( str[i]  == '\'' && qou2 == false ) qou1 = !qou1;
            if( str[i]  == '"' && qou1 == false ) qou2 = !qou2;
            if( str[i] != ' ' ) str[cnt++] = str[i];
            else if( str[i] == ' ' && (qou1 == true || qou2 == true) ) str[cnt++] = str[i];
            //else if( str[i] == ' ' &&  str[i+1] != ' ' && str[i-1] != ' '  ) str[cnt++] = str[i];
        }
        str[cnt] = '\0';
        len = cnt;
        return str;
    }
*/
    static bool str_cmp_icase(int start, size_t len, const string &a, const string &b) {
        len += start;
        if (a.size() < len || b.size() < len) return false;
        for (unsigned int i = start; i < len; ++i)
            if (tolower(a[i]) != tolower(b[i]))
                return false;
        return true;
    }


    static int db_tuple_comma_count(const string &cmd, int size) {
        int i, clevel = 0, cnt = 0;
        bool qou1 = false, qou2 = false;
        for (i = 0; i <= size; i++) {
            if (startbrac(cmd[i]) == true && (!qou2 && !qou1)) clevel++;
            else if (endbrac(cmd[i]) == true && (!qou2 && !qou1)) clevel--;
            if (cmd[i] == '\'' && qou2 == false) qou1 = !qou1;
            if (cmd[i] == '"' && qou1 == false) qou2 = !qou2;
            if (clevel == 0 && qou1 == false && qou2 == false) {
                if (cmd[i] == ',') cnt++;
            }
        }
        return cnt;
    }

    const int db_QUERY = 254;

    static int recognize_type(const string &str) {
        /**
        * string = 'string'
        * int = 1235
        * double = 2.15
        * tuple = { ... }
        * array = [ ... ]
        * null = null
        * bool = true | false
        * query = ( find [] in identifier where {} )
        */
        size_t i;
        bool int_bit = true;
        for (i = 0; i < str.size(); i++) {
            if (str[i] == '\'' || str[i] == '"') return db_STRING;
            if (str[i] == '(') return db_QUERY;
            if (str[i] == '{') return db_TUPLE;
            if (str[i] == '[') return db_ARRAY;
            if (str[i] == '.') return db_DOUBLE;
            if (str_cmp_icase(i, 4, str, "null") == true) return db_NULL;
            if (str_cmp_icase(i, 4, str, "true") == true || str_cmp_icase(i, 5, str, "false") == true) return db_BOOL;
            if (int_bit && isdigit(str[i]) == false) int_bit = false;
        }
        if (int_bit && str.size()) return db_INT;
        throw sdb_error("Domain Cannot be Identified : " + str);
    }

    string remove_space(const string &str) {
        string rs;
        int clevel = 0;
        unsigned int i;
        bool qou1 = false, qou2 = false;
        for (i = 0; i < str.size(); i++) {
            if (str[i] == '(' && (!qou2 && !qou1)) clevel++;
            else if (str[i] == ')' && (!qou2 && !qou1)) clevel--;
            if (str[i] == '\'' && qou2 == false) qou1 = !qou1;
            if (str[i] == '"' && qou1 == false) qou2 = !qou2;
            if (str[i] != ' ' && isprint(str[i])) rs.push_back(str[i]);
            else if (str[i] == ' ' && (qou1 || qou2 || clevel)) rs.push_back(str[i]);
        }
        return rs;
    }

    static string GivenKey(const string &str, int len, int *p) {
        int i, clevel = 0;
        bool qou1 = false, qou2 = false;
        string key;
        for (i = 0; i < len; i++) {
            if (startbrac(str[i]) == true && (!qou2 && !qou1)) clevel++;
            else if (endbrac(str[i]) == true && (!qou2 && !qou1)) clevel--;
            if (str[i] == '\'' && qou2 == false) qou1 = !qou1;
            if (str[i] == '"' && qou1 == false) qou2 = !qou2;
            if (clevel == 0 && qou1 == false && qou2 == false) {
                if (str[i] == ':') {
                    key = str.substr(0, i);
                    size_t b = key.find("\"");
                    if (b != string::npos)key.erase(b, 1);
                    b = key.rfind("\"");
                    if (b != string::npos)key.erase(b, 1);
                    b = key.find("'");
                    if (b != string::npos)key.erase(b, 1);
                    b = key.rfind("'");
                    if (b != string::npos)key.erase(b, 1);
                    *p = *p + i + 1;
                    return key;
                }
            }
        }
        return string();
    }

    static tuple generate_db_tuple(staticDB *db, const string &str, bool is_array);

    static tuple db_tupleParser(staticDB *db, const string &str, size_t len) {
        string st = remove_space(str);
        size_t i;
        for (i = 0; i < st.size(); i++) {
            if (startbrac(st[i]) == true && endbrac(st[i + 1]) != true) {
                bool ia = false;
                if (st[i] == '[')ia = true;
                len = st.rfind(opbrac(st[i]));
                if (len == string::npos) throw sdb_error("Brackets Did not Match.");
                string tup_str = st.substr(i + 1, len - (i + 1));
                tuple t = generate_db_tuple(db, tup_str, ia);
                return t;
            }
        }
        return null_tuple;
    }

    static bool is_complete_Brac(const string &str, int len) {
        int i;
        stack<unsigned char> st;
        bool qou1 = false, qou2 = false;
        for (i = 0; i < len; i++) {
            if (startbrac(str[i]) == true && (!qou2 && !qou1)) {
                st.push(str[i]);
            } else if (endbrac(str[i]) == true && (!qou2 && !qou1)) {
                if (st.top() != opbrac(str[i])) {
                    throw sdb_error(string_sprintf("Brackets MisMatch : No End Bracket For '%c' ", st.top()));
                }
                st.pop();
            }
            if (str[i] == '\'' && qou2 == false) qou1 = !qou1;
            if (str[i] == '"' && qou1 == false) qou2 = !qou2;
        }
        if (st.empty()) return true;
        return false;
    }

    static tuple generate_db_tuple(staticDB *db, const string &str, bool is_array) {
        int len = str.size();
        int size = db_tuple_comma_count(str, len) + 1;
        bool qou1 = false, qou2 = false;
        tuple t(size, is_array);
        int i, clevel = 0, p = 0;
        for (i = 0; i < len + 1; i++) {
            if (startbrac(str[i]) == true && (!qou2 && !qou1)) clevel++;
            else if (endbrac(str[i]) == true && (!qou2 && !qou1)) clevel--;
            if (str[i] == '\'' && qou2 == false) qou1 = !qou1;
            if (str[i] == '"' && qou1 == false) qou2 = !qou2;
            if (clevel == 0 && qou1 == false && qou2 == false) {
                if (str[i] == ',' || str[i] == 0) {
                    string key = GivenKey(&str[p], i - p, &p);
                    if (!is_array) {
                        if (!key.size())
                            throw sdb_error(
                                    "Tuple Should Always Contain Keys For Each Data Value. Try Array[...] For Indexed Data.");
                        if (identifierValidity(key, true) == false)
                            throw sdb_error("Invalid Tuple Key : '" + key + "'");
                    }
                    if (is_array) {
                        if (key.size())
                            throw sdb_error(
                                    "Array Should Not Contain Any Keys Only Data Accessed by Index Are Allowed.");
                    }
                    string str_val = str.substr(p, i - p);
                    switch (recognize_type(str_val)) {
                        case db_INT : {
                            stringstream ss(str_val);
                            int64_t value;//= stoi(str_val);
                            ss >> value;
                            t.append_int(value, key);
                            break;
                        }
                        case db_DOUBLE : {
                            double value;
                            sscanf(&str_val[0], "%lf,", &value);
                            t.append_double(value, key);
                            break;
                        }
                        case db_STRING : {
                            string value = str_val.substr(1, i - p - 2);
                            t.append_string(value, key);
                            break;
                        }
                        case db_NULL : {
                            t.append_null(key);
                            break;
                        }
                        case db_ARRAY : {
                            tuple value = db_tupleParser(db, &str_val[0], i - p);
                            //value.print();
                            t.append_array(value, key);
                            break;
                        }
                        case db_TUPLE : {
                            tuple value = db_tupleParser(db, &str_val[0], i - p);
                            //value.print();
                            t.append_tuple(value, key);
                            break;
                        }
                        case db_BOOL : {
                            bool value = (toupper(str_val[0]) == 'T') ? true : false;
                            t.append_bool(value, key);
                            break;
                        }
                        case db_QUERY : {
                            if (db) {
                                str_val.erase(str_val.find('('), 1);
                                str_val.erase(str_val.rfind(')'), 1);
                                vector <string> v = Split(str_val, ":");
                                nosqlRT r = sdb_parse(db, v[0]);
                                if (r == RETURN_TUPLE) {
                                    tuple value = get_NoSqlReturnTuple();
                                    if (value.empty()) {
                                        throw sdb_error("Inner Query : " + v[0] + " Did Not Return Any Value.");
                                    }
                                    db_tuple_var dtv = value.to_var(key);
                                    if (v.size() == 2) dtv = value.traverse(v[1]);
                                    t.append(dtv);
                                } else {
                                    throw sdb_error(get_NoSqlMsg());
                                }
                                break;
                            }
                        }
                    }
                    p = i + 1;
                }
            }
            //cout << " printing tup ";
            //t.print();
        }
        t.shrink();
        return t;
    }

    tuple tupleLiteral(staticDB *db, const string &n) {
        int len = n.size();
        if (!is_complete_Brac(n, len)) {
            throw sdb_error("Brackets and Braces Did Not Match.");
        }
        return db_tupleParser(db, n, len);
    }

/*
static char * GivenKey(char * str )
{
	return remove_space_completely(str);
}*/

    nosqlcmd GetCommand(const string &strt) {
        const char *s = strt.c_str();
        while (*s != '\0') {
            if (isalnum(*s) != false) break;
            s++;
        }
        string str = s;
        if (stricmp(str, "create") == 0) {
            return nosql_CREATE;
        }
        if (stricmp(str, "delete") == 0) {
            return nosql_DELETE;
        }
        if (stricmp(str, "set") == 0) {
            return nosql_SET;
        }
        if (stricmp(str, "get") == 0) {
            return nosql_GET;
        }
        if (stricmp(str, "find") == 0) {
            return nosql_FIND;
        }
        if (stricmp(str, "update") == 0) {
            return nosql_UPDATE;
        }
        if (stricmp(str, "insert") == 0) {
            return nosql_INSERT;
        }
        if (stricmp(str, "commit") == 0) {
            return nosql_COMMIT;
        }
        if (stricmp(str, "quit") == 0) {
            return nosql_QUIT;
        }
        if (stricmp(str, "help") == 0) {
            return nosql_HELP;
        }
        if (stricmp(str, "import") == 0) {
            return nosql_IMPORT;
        }
        if (stricmp(str, "export") == 0) {
            return nosql_EXPORT;
        }
        return nosql_NOCMD;
    }

    nosqlRT sdb_parse(staticDB *db, const string &str) {
        try {
            sdb_resetNoSqlRT();
            vector <string> v = Split(str, " ");
            if (v.size() < 1) {
                return noSql_Msg(ERROR_MSG, "NoSQL Syntax Error USE(HELP Command For details).");
            }
            return sdb_execute(db, v);
        }
        catch (const sdb_error &e) {
            return noSql_Msg(ERROR_MSG, e.what());
        }
    }

}
/*
int main()
{
	parser("create key name : { 1 , 2 , 3 , 6 } ");
	parser("delete key name");
	parser("get key name");
	return true;
}*/


#endif // SDB_PARSER_C
