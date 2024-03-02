import sys
import os
import pandas
import datetime
import math as m

if len(sys.argv) < 3:
    print('please provide a path to a phybox toolbox suite gps log folder')
    print('and a start UTC unix timestamp. Find this timestamp in the \'time.csv\' file in the \'meta\' folder.')
    print(f'{sys.argv[0]} <path> <timestamp>')
    exit(1)



path = sys.argv[1]
start_timestamp = float(sys.argv[2])
#print(path)

start_timestamp_dt = datetime.datetime.fromtimestamp(start_timestamp, tz=datetime.timezone.utc)
year_str = f'{start_timestamp_dt.year}'
output_name = f'{year_str[-2:]}{start_timestamp_dt.month:02}{start_timestamp_dt.day:02}{start_timestamp_dt.hour:02}.CSV'

dataframe = pandas.read_csv(os.path.join(path, 'Raw Data.csv'))
output_path = os.path.join(path, output_name)

#print(dataframe.size)
#print(dataframe.shape[0])

with open(output_path, 'w') as file:
    file.write('time, lat, lon, SOG [m/s], Alt\n')
    for i in range(dataframe.shape[0]):
        if i % 4 == 0:
            # time
            time = start_timestamp + dataframe.iloc[i,0]
            time_dt = datetime.datetime.fromtimestamp(time)
            #print(time)
            #print(time_dt.microsecond / 1000000.0)
            file.write(f'{time_dt.hour:02}:{time_dt.minute:02}:{time_dt.second + round(time_dt.microsecond/1000000.0):02}')

            # lat
            lat = float(dataframe.iloc[i,1])
            lat_deg = m.floor(lat)
            if lat < 0.0:
                lat_arcmin = -(lat - (lat_deg+1))*60.0
                file.write(f', {-lat_deg-1:2}*{lat_arcmin:08.5f}\'S')
            else:
                lat_arcmin = (lat - lat_deg)*60.0
                file.write(f', {lat_deg:2}*{lat_arcmin:08.5f}\'N')

            # lon
            lon = float(dataframe.iloc[i,2])
            lon_deg = m.floor(lon)
            if lon < 0.0:
                lon_arcmin = -(lon - (lon_deg+1))*60.0
                file.write(f', {-lon_deg-1:3}*{lon_arcmin:08.5f}\'W')
            else:
                lon_arcmin = (lon - lon_deg)*60.0
                file.write(f', {lon_deg:3}*{lon_arcmin:08.5f}\'E')

            # Sog
            file.write(f', {dataframe.iloc[i,5]:6.5f}')

            # alt
            file.write(f', {dataframe.iloc[i,3]:6.5f}\n')
            #if i == 20:
            #    exit()

