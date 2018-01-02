from struct import *

sig = "staticDB v1.0"
bi_sig = len("bi")
bd_sig = len("bd")
filename = "testdb"
disk_util_size = 12 + bd_sig


def go_get_data(filepos):
    value = {}
    with open(filename, "rb") as f:
        f.seek(filepos)
        byte = f.read(bd_sig)
        # value["sig"] = byte
        byte = f.read(12)
        tmp = unpack("III", byte)
        v = {}
        v["size"] = tmp[0]
        v["ref"] = tmp[1]
        v["mem"] = tmp[2]
        value['a'] = v
        if (tmp[0] < 10):
            byte = f.read(tmp[0])
            value["data"] = byte
    return value


with open(filename, "rb") as f:
    if f.read(len(sig)) != sig:
        print "DB BEGIN Signature Error"
    f.seek(-len(sig), 2)
    if f.read(len(sig)) != sig:
        print "DB END Signature Error"
    f.seek(len(sig))

    byte = f.read(16)
    baseindex, pairindex = unpack("QQ", byte)
    print "Base Index Start From ", baseindex, " pair Index : ", pairindex
    f.seek(baseindex)
    byte = f.read(bi_sig)
    cnt = unpack(str(bi_sig) + "s", byte)
    print "baseIndex Signature : ", cnt[0]
    byte = f.read(16)
    base_count, base_index_count = unpack("QQ", byte)
    print "No of Data elements ", base_count, " Next Index To Be Given : ", base_index_count
    base_index_list = {}
    for i in xrange(base_count):
        byte = f.read(16)
        tmp = unpack("QQ", byte)
        base_index_list[tmp[0]] = tmp[1]
        # print tmp
    byte = f.read(8)
    base_del_count = unpack("Q", byte)[0]
    print "No of deleted Region : ", base_del_count
    base_del_list = []
    for i in xrange(base_del_count):
        byte = f.read(16)
        tmp = unpack("QQ", byte)
        base_del_list.append(list(tmp))
    print base_del_list


    ##    for i,val in base_index_list.iteritems():
    ##        print i, val , go_get_data(val)
    # def compute_open_block():
    def tupcmp(x, y):
        return cmp(x[0], y[0])


    for i, val in base_index_list.iteritems():
        base_del_list.append([val, go_get_data(val)["a"]["mem"] + disk_util_size])
    base_del_list.sort(tupcmp)


    def side_by_side(val1, val2):
        if (val1[0] + val1[1] == val2[0]):
            return True
        if (val2[0] - val1[1] == val1[0]):
            return True
        return False


    for i, val in enumerate(base_del_list):
        p = 0
        while p < len(base_del_list):
            val2 = base_del_list[p]
            if (side_by_side(val, val2)):
                if (val[0] > val2[0]):
                    val[0] = val2[0]
                val[1] += val2[1]
                del base_del_list[p]
                p = 0
            p += 1
    # print base_index_list.values()#.count(49)
    counted = base_del_list[0][0] + base_del_list[0][1]
    print "CHECK SUM : ", base_del_list, counted
    if counted == baseindex:
        print "\nTHE DB FILE IS 100% VALID"
    else:
        print "DB FILE CONTAINS ERRORS"

        # for name, val in base_index_list.iteritems():
        #     print val
        #     if val == 49 :
        #         print name
