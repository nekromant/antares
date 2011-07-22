#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
# Hello, this program is written in Python - http://python.org
programname = 'html2csv - version 2002-09-20 - http://sebsauvage.net'

import sys, getopt, os.path, glob, HTMLParser, re

try:    import psyco ; psyco.jit()  # If present, use psyco to accelerate the program
except: pass

def usage(progname):
    ''' Display program usage. '''
    progname = os.path.split(progname)[1]
    if os.path.splitext(progname)[1] in ['.py','.pyc']: progname = 'python '+progname
    return '''%s
A coarse HTML tables to CSV (Comma-Separated Values) converter.

Syntax    : %s source.html

Arguments : source.html is the HTML file you want to convert to CSV.
            By default, the file will be converted to csv with the same
            name and the csv extension (source.html -> source.csv)
            You can use * and ?.

Examples   : %s mypage.html
           : %s *.html

This program is public domain.
Author : Sebastien SAUVAGE <sebsauvage at sebsauvage dot net>
         http://sebsauvage.net
''' % (programname, progname, progname, progname)

class html2csv(HTMLParser.HTMLParser):
    ''' A basic parser which converts HTML tables into CSV.
        Feed HTML with feed(). Get CSV with getCSV(). (See example below.)
        All tables in HTML will be converted to CSV (in the order they occur
        in the HTML file).
        You can process very large HTML files by feeding this class with chunks
        of html while getting chunks of CSV by calling getCSV().
        Should handle badly formated html (missing <tr>, </tr>, </td>,
        extraneous </td>, </tr>...).
        This parser uses HTMLParser from the HTMLParser module,
        not HTMLParser from the htmllib module.
        Example: parser = html2csv()
                 parser.feed( open('mypage.html','rb').read() )
                 open('mytables.csv','w+b').write( parser.getCSV() )
        This class is public domain.
        Author: Sébastien SAUVAGE <sebsauvage at sebsauvage dot net>
                http://sebsauvage.net
        Versions:
           2002-09-19 : - First version
           2002-09-20 : - now uses HTMLParser.HTMLParser instead of htmllib.HTMLParser.
                        - now parses command-line.
        To do:
            - handle <PRE> tags
            - convert html entities (&name; and &#ref;) to Ascii.
            '''
    def __init__(self):
        HTMLParser.HTMLParser.__init__(self)
        self.CSV = ''      # The CSV data
        self.CSVrow = ''   # The current CSV row beeing constructed from HTML
        self.inTD = 0      # Used to track if we are inside or outside a <TD>...</TD> tag.
        self.inTR = 0      # Used to track if we are inside or outside a <TR>...</TR> tag.
        self.re_multiplespaces = re.compile('\s+')  # regular expression used to remove spaces in excess
        self.rowCount = 0  # CSV output line counter.
    def handle_starttag(self, tag, attrs):
        if   tag == 'tr': self.start_tr()
        elif tag == 'td': self.start_td()
    def handle_endtag(self, tag):
        if   tag == 'tr': self.end_tr()
        elif tag == 'td': self.end_td()         
    def start_tr(self):
        if self.inTR: self.end_tr()  # <TR> implies </TR>
        self.inTR = 1
    def end_tr(self):
        if self.inTD: self.end_td()  # </TR> implies </TD>
        self.inTR = 0            
        if len(self.CSVrow) > 0:
            self.CSV += self.CSVrow[:-1]
            self.CSVrow = ''
        self.CSV += '\n'
        self.rowCount += 1
    def start_td(self):
        if not self.inTR: self.start_tr() # <TD> implies <TR>
        self.CSVrow += '"'
        self.inTD = 1
    def end_td(self):
        if self.inTD:
            self.CSVrow += '",'  
            self.inTD = 0
    def handle_data(self, data):
        if self.inTD:
            self.CSVrow += self.re_multiplespaces.sub(' ',data.replace('\t',' ').replace('\n','').replace('\r','').replace('"','""'))
    def getCSV(self,purge=False):
        ''' Get output CSV.
            If purge is true, getCSV() will return all remaining data,
            even if <td> or <tr> are not properly closed.
            (You would typically call getCSV with purge=True when you do not have
            any more HTML to feed and you suspect dirty HTML (unclosed tags). '''
        if purge and self.inTR: self.end_tr()  # This will also end_td and append last CSV row to output CSV.
        dataout = self.CSV[:]
        self.CSV = ''
        return dataout


if __name__ == "__main__":
    try: # Put getopt in place for future usage.
        opts, args = getopt.getopt(sys.argv[1:],None)
    except getopt.GetoptError:
        print usage(sys.argv[0])  # print help information and exit:
        sys.exit(2)
    if len(args) == 0:
        print usage(sys.argv[0])  # print help information and exit:
        sys.exit(2)       
    print programname
    html_files = glob.glob(args[0])
    for htmlfilename in html_files:
        outputfilename = os.path.splitext(htmlfilename)[0]+'.csv'
        parser = html2csv()
        print 'Reading %s, writing %s...' % (htmlfilename, outputfilename)
        try:
            htmlfile = open(htmlfilename, 'rb')
            csvfile = open( outputfilename, 'w+b')
            data = htmlfile.read(8192)
            while data:
                parser.feed( data )
                csvfile.write( parser.getCSV() )
                sys.stdout.write('%d CSV rows written.\r' % parser.rowCount)
                data = htmlfile.read(8192)
            csvfile.write( parser.getCSV(True) )
            csvfile.close()
            htmlfile.close()
        except:
            print 'Error converting %s        ' % htmlfilename
            try:    htmlfile.close()
            except: pass
            try:    csvfile.close()
            except: pass
    print 'All done.                                      '
