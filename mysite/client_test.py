from urllib2 import Request, urlopen, URLError, HTTPError
import urllib
import os


def getURL(url):
    req = Request(url)
    try:
        response = urlopen(req)
    except HTTPError as e:
        print 'The server couldn\'t fulfill the request.'
        print 'Error code: ', e.code
    except URLError as e:
        print 'We failed to reach a server.'
        print 'Reason: ', e.reason
    else:
        return response.read()
   


srt_msg = """Static Database Client Script v0.25.

Database Server 	: http://localhost:8000/client_execute.
Database User 		: AllUser
Database Name 		: testdb_client.sdb
Database Password 	: ****
"""

def main():
    Address = "http://localhost:8000/client_execute?execute_msg="
    print srt_msg
    while 1:
        ino = raw_input(">>> ")
        if ino == "clear" :
            os.system('cls')
            print srt_msg
            continue
        QUERY = urllib.quote_plus(ino)
        print "sending query :" , QUERY
        print "response :" ,getURL(Address+QUERY)
        
main()

