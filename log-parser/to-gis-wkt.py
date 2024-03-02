import os
import sys
import pandas as pd

def main():

    if len(sys.argv) < 2:
        print('  This script requires a path to an input file')
        exit()


    decimation = 1
    if len(sys.argv) > 2:
        decimation = int(sys.argv[2])

    data_path = sys.argv[1]
    data_path_base = data_path.split('.CSV')[0]
    data = pd.read_csv(data_path)
    #print(data_path.split('.csv'))
    #print(data)

    last_index = data.shape[0]-1
    wkt_str = 'WKT,name,datetime\n"LINESTRING('
    for i in range(last_index):

        if (i % decimation) != 0:
            continue
        lat = data.values[i,1]
        lon = data.values[i,2]
        lat_float = int(lat[1:3]) + float(lat[4:12])/60.0
        lon_float = int(lon[1:4]) + float(lon[5:13])/60.0
        if lon[14] == 'W':
            lon_float *= -1

        #print(i, end=' ')
        #print('|' + data.values[i,1] + '|', end=' ')
        #print('|' + data.values[i,2] + '| ', end=' ')
        #print(lat_float, lon_float)
        wkt_str += f'{lon_float} {lat_float}, '

    lat = data.values[last_index - last_index % decimation,1]
    lon = data.values[last_index - last_index % decimation,2]
    lat_float = int(lat[1:3]) + float(lat[4:12])/60.0
    lon_float = int(lon[1:4]) + float(lon[5:13])/60.0
    if lon[14] == 'W':
        lon_float *= -1

    wkt_str += f'{lon_float} {lat_float})",path,' + os.path.basename(data_path_base)[:6] + f'T{data.values[0,0]}\n'
    wkt_str += f'"POINT({lon_float} {lat_float})",final_position,' + os.path.basename(data_path_base)[:6] + f'T{data.values[last_index,0]}\n'

    output_path_wkt = data_path_base + '-wkt.csv'
    with open(output_path_wkt, 'w') as f:
        f.write(wkt_str)


    output_path_prj = data_path_base + '-wkt.prj'
    prj_file_contents = 'GEOGCS["WGS 84",DATUM["WGS_1984",SPHEROID["WGS 84",6378137,298.257223563,AUTHORITY["EPSG","7030"]],AUTHORITY["EPSG","6326"]],PRIMEM["Greenwich",0,AUTHORITY["EPSG","8901"]],UNIT["degree",0.0174532925199433,AUTHORITY["EPSG","9122"]],AXIS["Latitude",NORTH],AXIS["Longitude",EAST],AUTHORITY["EPSG","4326"]]'
    with open(output_path_prj, 'w') as f:
        f.write(prj_file_contents)
        f.write('\n')

if __name__ == '__main__':
    main()
