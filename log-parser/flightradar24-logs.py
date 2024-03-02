import sys
import os
import pandas
import datetime
import math as m

if len(sys.argv) < 2:
    print('please provide a path to a .csv file from flightradar24')
    print(f'{sys.argv[0]} <path>')
    exit(1)



path = sys.argv[1]


dataframe = pandas.read_csv(path)

print(dataframe)

#print(type(dataframe.iloc[0,0]))


start_timestamp_dt = datetime.datetime.fromtimestamp(dataframe.iloc[0,0], tz=datetime.timezone.utc)
year_str = f'{start_timestamp_dt.year}'
output_name = f'{year_str[-2:]}{start_timestamp_dt.month:02}{start_timestamp_dt.day:02}{start_timestamp_dt.hour:02}.CSV'
output_path = os.path.join(os.path.dirname(path), output_name)

#print(output_path)

#exit()

#print(dataframe.size)
#print(dataframe.shape[0])

with open(output_path, 'w') as file:
    file.write('time, lat, lon, SOG [m/s], Alt\n')
    for i in range(dataframe.shape[0]):
        # optional subsampling
        if i % 1 == 0:
            # time
            time_dt = datetime.datetime.fromtimestamp(dataframe.iloc[i,0], tz=datetime.timezone.utc)
            #print(time)
            #print(time_dt.microsecond / 1000000.0)
            file.write(f'{time_dt.hour:02}:{time_dt.minute:02}:{time_dt.second + round(time_dt.microsecond/1000000.0):02}')

            # lat,lon
            latlon = dataframe.iloc[i,3].split(',')

            # lat
            lat = float(latlon[0])
            lat_deg = m.floor(lat)
            if lat < 0.0:
                lat_arcmin = -(lat - (lat_deg+1))*60.0
                file.write(f', {-lat_deg-1:2}*{lat_arcmin:08.5f}\'S')
            else:
                lat_arcmin = (lat - lat_deg)*60.0
                file.write(f', {lat_deg:2}*{lat_arcmin:08.5f}\'N')

            # lon
            lon = float(latlon[1])
            lon_deg = m.floor(lon)
            if lon < 0.0:
                lon_arcmin = -(lon - (lon_deg+1))*60.0
                file.write(f', {-lon_deg-1:3}*{lon_arcmin:08.5f}\'W')
            else:
                lon_arcmin = (lon - lon_deg)*60.0
                file.write(f', {lon_deg:3}*{lon_arcmin:08.5f}\'E')

            # Sog - knots to m/s
            file.write(f', {dataframe.iloc[i,5]*0.514:6.5f}')

            # alt - feet to meters
            file.write(f', {dataframe.iloc[i,4]*0.3048:6.5f}\n')
            #if i == 20:
            #    exit()

