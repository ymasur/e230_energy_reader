#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Module: setup.py
Contains the initialization steps at startup

@author: Yves Masur (YM)

xx.11.2020/YM
"""
NAME = "e230_query"
VERSION = "Version 0.10 - Yves Masur"

site = "localhost"
outfile = "/tmp/e230-data.txt"


def help():
    """ Aide en ligne sur le programme
    :rtype : object void
    """
    print(u"\nUtilisation:\n"
          u"%s <url> <outfile> \n"
          u"\nAvec:\n"
          u"url : nom du site à lire (ou ad. IP)\n"
          u"outfile : chemin/fichier d'enregistrement\n\n"
          # u"Le fichier log de l'application est " + NAME + "\n"
          % NAME
          )
    exit(0)


def setargs(argv):
    """ lit les arguments en ligne de commande
        1: L'url d'entrée
        2: Le fichier de sortie, à créer
    """
    global site, outfile

    if len(argv) > 1 and ("/?" in argv[1]):
        help()

    if len(argv) > 1 :
        site = argv[1]

    if len(argv) > 2 :
        outfile = argv[2]
