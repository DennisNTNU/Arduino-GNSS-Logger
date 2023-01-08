import sys
import os
import pandas
import datetime
import math as m

path = sys.argv[1]
#print(path)

dataframe = pandas.read_csv(os.path.join(path, 'Raw Data.csv'))

#print(dataframe.size)
#print(dataframe.shape[0])

with open('output.CSV', 'w') as file:
    file.write('time, lat, lon, SOG [m/s], Alt\n')
    for i in range(dataframe.shape[0]):
    	   if i % 4 == 0:
            # time
            time = 1664612788.229 + dataframe.iloc[i,0]
            time_dt = datetime.datetime.fromtimestamp(time)
            #print(time)
            #print(time_dt.microsecond / 1000000.0)
            file.write(f'{time_dt.hour:02}:{time_dt.minute:02}:{time_dt.second + round(time_dt.microsecond/1000000.0):02}')

            # lat
            lat = dataframe.iloc[i,1]
            lat_deg = m.floor(lat)
            lat_arcmin = (lat - lat_deg)*60.0
            file.write(f', {lat_deg:2}*{lat_arcmin:8.7f}\'N')

            # lon
            lon = dataframe.iloc[i,2]
            lon_deg = m.floor(lon)
            lon_arcmin = (lon - lon_deg)*60.0
            file.write(f', {lon_deg:3}*{lon_arcmin:8.7f}\'E')

            # Sog
            file.write(f', {dataframe.iloc[i,5]:6.5f}')

            # alt
            file.write(f', {dataframe.iloc[i,3]:6.5f}\n')
            #if i == 20:
            #    exit()

