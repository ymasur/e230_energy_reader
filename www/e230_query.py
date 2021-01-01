#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Module: e230_query
Contains: main part of get and store e230 datas

@author: Yves Masur (YM)
30.11.2020 - YM: firt try
"""

# modules standard
import sys
import subprocess
import time
import setup as Set


# set the site name (or IP adress) of the Arduino
# site = "192.168.1.108"
reqData = "/arduino/start"
getData = "/arduino/print"
answ_ok = "=OK="


def main(argv):
    """ main process
    """

    Set.setargs(argv)
    print("site : " + Set.site)
    print("outfile: " + Set.outfile)

    t1 = time.time()

    # first, send the begin trnasaction command
    res = subprocess.Popen("curl " + Set.site + reqData, shell=True, stdout=subprocess.PIPE).stdout.read()

    if res.find(answ_ok) == -1 :                   # Q: is command correctly done?
        print("Echec lecture compteur 'start':")   # A: no, exit with error
        print(res)
        exit(1)

    print(reqData + " OK\n")

    # get the datas
    res = subprocess.Popen("curl " + Set.site + getData, shell=True, stdout=subprocess.PIPE).stdout.read()

    if res.find(answ_ok) == -1 :
        print("Echec lecture compteur 'print':")
        print(res)
        exit(1)

    print(getData + " OK\n")

    # note: b mode does not add a newline '\n'
    p_outfile = open(Set.outfile, "wb")
    p_outfile.write(res)
    p_outfile.close()

    t2 = time.time()

    print(Set.outfile + " file written\n")
    print("Time of transaction: %2.3f sec." % (t2-t1))


# Starts here
if __name__ == '__main__':
    main(sys.argv)
# end main
