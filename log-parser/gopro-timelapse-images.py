from PIL import Image
from PIL.TiffTags import TAGS
import os
import sys

if len(sys.argv) < 2:
	print('Need args:')
	print('   python', sys.argv[0], '<path to gopro images> <starting gopro image index>')
	exit()

images_path = sys.argv[1]
startindex = int(sys.argv[2])

running = True

file_created = False

f = None

index = startindex
while running:

	file_name = f'G00{index}.JPG'
	file_path = os.path.join(images_path, file_name)
	index += 1

	print('Trying file', file_name)

	if not os.path.exists(file_path):
		running = False
		break

	try:
		img = Image.open(file_path)
	except:
		continue

	exif_dict = img._getexif()

	try:
		GPS_tags = exif_dict[34853]
	except:
		continue


	north_or_south = GPS_tags[1]
	east_or_west = GPS_tags[3]

	latitude_raw = GPS_tags[2]
	longitude_raw = GPS_tags[4]

	altitude_raw = GPS_tags[6]

	time_raw = GPS_tags[7]
	date_raw = GPS_tags[29]


	latitude = latitude_raw[0][0] / latitude_raw[0][1] + latitude_raw[1][0] / (60*latitude_raw[1][1]) + latitude_raw[2][0] / (60*60*latitude_raw[2][1])
	longitude = longitude_raw[0][0] / longitude_raw[0][1] + longitude_raw[1][0] / (60*longitude_raw[1][1]) + longitude_raw[2][0] / (60*60*longitude_raw[2][1])
	altitude = altitude_raw[0] / altitude_raw[1]

	# these are strings
	year = date_raw[2:4]
	month = date_raw[5:7]
	day = date_raw[8:10]

	# these are numbers
	hour = time_raw[0][0] // time_raw[0][1]
	minute = time_raw[1][0] // time_raw[1][1]
	second = time_raw[2][0] // time_raw[2][1]

	if not file_created:
		file_created = True
		output_file_name = year + month + day + f'{hour:02}.CSV'
		print('Opening file', output_file_name)
		f = open(output_file_name, 'w')
		f.write('time, lat, lon, SOG [m/s], Alt\n')

	lat_int = int(latitude)
	lon_int = int(longitude)
	f.write(f'{hour:02}:{minute:02}:{second:02}, {lat_int:2}*{60*(latitude-lat_int):8.5f}\'' + north_or_south + f', {lon_int:3}*{60*(longitude-lon_int):8.5f}\'' + east_or_west + f', nan, {altitude}\n')

	#print('latitude', latitude)
	#print('longitude', longitude)
	#print('altitude', altitude)
	#print('year', year)
	#print('month', month)
	#print('day', day)
	#print('hour', hour)
	#print('minute', minute)
	#print('second', second)

	#if index > 4:
	#	break





