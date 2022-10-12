#!/usr/bin/python3
import serial
import time
import datetime
import os,sys
import argparse
import json

#ID=0x7fa3f33da6a0

baud=115200
from serial.tools import list_ports
from time import sleep

default_filetag="_2"
default_directory="/home/msgc/status"


int_def=10
parser = argparse.ArgumentParser(description='PT-100 reader (via MAX31865)')
parser.add_argument("-o",help="one shot flag",action='store_true')
parser.add_argument("-i",help="interval (sec)",type=int,default=int_def)
parser.add_argument('-d', '--directory', default=default_directory, help='output directory')
parser.add_argument('-t', '--file_tag', default=default_filetag, help='file tag')

args=parser.parse_args();
interval=args.i
oneshot=args.o
dirname=args.directory
filetag=args.file_tag

if dirname[-1] != "/":
    dirname=dirname+"/"

#config_output = 'both'

#stadir='~/isaislowdata01'
ocsv, odb = True, True
#if config_output == 'db':
#    ocsv, odb = False, True
#if config_output == 'both':
#    ocsv, odb = True, True

sql_dbname='PT100_1'
sql_tablename='PT100_table1'
               
if odb:
    import pymysql.cursors
#    conn = pymysql.connect(host='isaislow01',port=3306,user='isai',passwd='',autocommit='true')
    conn = pymysql.connect(host='isaiSM',port=3306,user='isai',autocommit='true')
    cursor = conn.cursor()
    cursor.execute("CREATE DATABASE IF NOT EXISTS " + sql_dbname)
    cursor.execute("USE " + sql_dbname)
    cursor.execute("show tables")
    cursor.execute("CREATE TABLE IF NOT EXISTS PT100_table1(time TIMESTAMP not null default CURRENT_TIMESTAMP, TM00R FLOAT, TM00T FLOAT,TM01R FLOAT,TM01T FLOAT)")


print ("PT-100 serial reader")
print (" interval = "+format(interval)+" sec.")
#print ("device ID=",DEV_ID)

ports=list_ports.comports();
print(" ports: ",end="")
print(ports)

device=[info.device for info in ports]
print(" device: ",end="")
print(device)


device=[info.description for info in ports]
print(device)


if len(device)==0:
    print("device not found")


today=datetime.datetime.today()
while 1:
    ser=serial.Serial('/dev/ttyACM0',baud,timeout=10)
    line=ser.readline().decode()
    data=line.split()
    dt=datetime.datetime.today()
    if today != dt:
        today = dt
    fout = dirname
    fout += '{0:%Y%m%d}'.format(today)
    fout += filetag
    f = open(fout,'a')

    if len(data) == 4:
        print(dt.strftime('%Y/%m/%d/%H:%M:%S'),"\t",data[0],"\t",data[1],"\t",data[2],"\t",data[3])
                   
        if ocsv:
            print(dt.strftime('%Y/%m/%d/%H:%M:%S'),"\t",data[0],"\t",data[1],"\t",data[2],"\t",data[3],file=f)

        if odb:
            date_now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            cursor.execute("insert into PT100_table1(TM00R,TM00T,TM01R,TM01T) values(%s, %s, %s, %s)",(str(data[0]),str(data[1]),str(data[2]),str(data[3])))

    if oneshot:
        break

    ser.close()    
    sleep(interval)



