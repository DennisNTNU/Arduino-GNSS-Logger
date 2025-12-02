
#include "SSD1306.hpp"
#include "si7021.hpp"
#include <SD.h>
#include <Wire.h>


unsigned long ts = 0;

byte SD_STATUS_LED_PIN = 4;
byte FIX_STATUS_LED_PIN = 5;
byte BUTTON_PIN = 6;

byte SPI_SD_CHIP_SELECT_PIN = 7;



// General GPS parsing variables
int nmea_msgs_seen = 0;
int nmea_msgs_seen_10k = 0;
char string[10] = "";


// GPS PARSE STATE MACHINE
int nmea_parse_state = 1;

#define FLUSH_UNTIL_NEXT_NMEA_MESSAGE_START 1

#define FINDING_SPECIFIC_NMEA_MESSAGE 2
const char desired_nmea_message[8] = "__RMC__";
//const char desired_nmea_message[8] = "__GGA__";
int desired_nmea_message_index = 0;

char nmea_message_id[8] = "";

int current_nmea_field = 1; // Type GGA has 14 fields (14 commas) plus a checksum at the end (not preceded by a comma)
int nmea_field_str_index = 0;

#define FOUND_NMEA_MESSAGE_RMC 3
char nmea_msg_date[8] = "";
char nmea_msg_time[16] = "";
char nmea_msg_lat[16] = "";
char nmea_msg_lon[16] = "";
char nmea_msg_sog[8] = "";
char nmea_msg_fix_status[10] = "V";
bool RMC_message_update = false;
bool has_fix = false;
int rmc_msgs_parsed = 0;


#define FOUND_NMEA_MESSAGE_GGA 4
char nmea_msg_fix_quality[8] = "";
char nmea_msg_sats[8] = "";
char nmea_msg_altitude[8] = "";
char nmea_msg_geoid_separation[8] = "";
bool GGA_message_update = false;



// SD CARD LOGGING GLOBALS
    //Sd2Card card;
    //SdVolume volume;
    //SdFile root;
File myFile;
const int chipSelect_SD = SPI_SD_CHIP_SELECT_PIN;
bool initialized_SD = false;
char status_string_SD[16] = "";
char filename[32] = "";

byte log_subsample = 3; // decimation factor
byte log_index = 1;



// Si7021 variables
float temperature_humidity_buffer[2] = {1.0, 2.0};

// other
int loop_iterations = 0;


// RMC = a specific nmea message
// Each message has that this program parses has their own dedicated buffers
void clear_rmc_buffers()
{
  memset(nmea_msg_date, 0, 8);
  memset(nmea_msg_time, 0, 16);
  memset(nmea_msg_lat, 0, 16);
  memset(nmea_msg_lon, 0, 16);
  memset(nmea_msg_sog, 0, 8);
  memset(nmea_msg_fix_quality, 0, 8);
}
void clear_gga_buffers()
{
  memset(nmea_msg_fix_quality, 0, 8);
  memset(nmea_msg_sats, 0, 8);
  memset(nmea_msg_altitude, 0, 8);
  memset(nmea_msg_geoid_separation, 0, 8);
}

void clear_nmea_buffers()
{
  clear_rmc_buffers();
  clear_gga_buffers();
}

void initSD()
{
  if (!SD.begin(chipSelect_SD))
  {
    Serial.println(F("SD init err"));
    sprintf(status_string_SD, "SD init err");
  }
  else
  {
    Serial.println(F("SD init success"));
    sprintf(status_string_SD, "SD inited");
    initialized_SD = true;
  }
}

void setup()
{
  //pinMode(2, INPUT_PULLUP); // these are I2C pins
  //pinMode(3, INPUT_PULLUP); // these are I2C pins
  pinMode(SD_STATUS_LED_PIN, OUTPUT);
  pinMode(FIX_STATUS_LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);

  pinMode(SPI_SD_CHIP_SELECT_PIN, OUTPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);


  digitalWrite(SD_STATUS_LED_PIN, HIGH);
  digitalWrite(FIX_STATUS_LED_PIN, HIGH);

  Wire.begin();
  Wire.setClock(400000);

  //Serial.begin(9600);
  Serial.begin(19200);
  //Serial.begin(38400);
  //Serial.begin(115200);
  //Serial1.begin(230400);
  //Serial1.begin(307200); // invalid baud
  //Serial1.begin(460800); // doesnt work
  //Serial1.begin(500000);
  //Serial1.begin(576000); // doesnt work
  //Serial1.begin(921600); // doesnt work
  //Serial1.begin(1000000); // doesnt work
  //Serial1.begin(1152000); // doesnt work
  //Serial1.begin(2000000); // doesnt work

  Serial.print(F("Hello\r\n"));  
  delay(100);

  ts = millis();

  initSD();

  oled_setup(128, 64);
  delay(20);
  oled_setTestPattern();
  delay(200);
  oled_clear();
  oled_writeFont();
  delay(200);

  oled_clear();

  oled_writeText(status_string_SD, 1, 0);

  clear_nmea_buffers();

  Serial.flush();

  digitalWrite(SD_STATUS_LED_PIN, LOW);
  digitalWrite(FIX_STATUS_LED_PIN, LOW);
}

void loop()
{
  parse_nmea();
  Serial.flush();



  if (RMC_message_update && GGA_message_update)
  {
    update_coords_oled();
  }


  // This will be run only once every second ish, no matter the (variable) loop period
  unsigned long ts1 = millis();
  if (ts1 - ts > 1000)
  {
    // No longer show this, in favor of humidity & temperature
    //num2str4positive(nmea_msgs_seen_10k, string);
    //num2str4positive(nmea_msgs_seen, &(string[4]));
    //oled_writeText(string, 64, 7);


    byte a = digitalRead(BUTTON_PIN);
    if (a)
    {
      ;
    }
    else
    {
      log_subsample++;
      if (log_subsample > 5)
      {
        log_subsample = 1;
      }
    }


    num2str2positive(log_subsample, string);
    oled_writeText(string, 37, 7);
    oled_writeText("Decim:", 1, 7);

    ts = ts1;
    digitalWrite(SD_STATUS_LED_PIN, (initialized_SD) ? false : true);
    digitalWrite(FIX_STATUS_LED_PIN, has_fix);
    has_fix = false;

    loop_iterations++;
    if (loop_iterations >= log_subsample)
    {
      loop_iterations = 0;
      readDataSi7021(temperature_humidity_buffer);

      num2str2positive((int)temperature_humidity_buffer[0], string);
      string[2] = '.';
      num2str2positive((temperature_humidity_buffer[0] - (int)temperature_humidity_buffer[0])*100, &(string[3]));
      string[5] = 0;

      oled_writeText(string, 60, 7);

      num2str2positive((int)temperature_humidity_buffer[1], string);
      string[2] = '.';
      num2str2positive((temperature_humidity_buffer[1] - (int)temperature_humidity_buffer[1])*100, &(string[3]));
      string[5] = 0;
      
      oled_writeText(string, 95, 7);
    }
  }

  // Needs to have a small delay (rather than large), else there
  // will be serial interface buffer overflows and GNSS serial data is lost
  delay(1);

}

void update_coords_oled()
{
  if (nmea_msg_fix_status[0] == 'A')
  {
    Serial.print(nmea_msg_time);
    Serial.print(F(" There is a GPS fix: "));
    Serial.println(nmea_msg_fix_status);
    has_fix = true;
  }

  // 104631.00,6326.63235,N,   01026.37737,E
  // 10:46:31, 63*26.63235'N,N,010*26.37737'E,E
  nmea_msg_time[12] = '\0';
  nmea_msg_time[11] = 'C';
  nmea_msg_time[10] = 'T';
  nmea_msg_time[9] = 'U';
  nmea_msg_time[8] = '\0';
  nmea_msg_time[7] = nmea_msg_time[5];
  nmea_msg_time[6] = nmea_msg_time[4];
  nmea_msg_time[5] = ':';
  nmea_msg_time[4] = nmea_msg_time[3];
  nmea_msg_time[3] = nmea_msg_time[2];
  nmea_msg_time[2] = ':';
  //    "hhmmss" 
  // -> "hh:mm:ss"

  oled_writeText(nmea_msg_time, 70, 5);


  //    "ddmmyy"
  // -> " 20yy.mm.dd"
  char date_str[12] = "";
  date_str[0] = '2';
  date_str[1] = '0';
  date_str[2] = nmea_msg_date[4];
  date_str[3] = nmea_msg_date[5];
  date_str[4] = '.';
  date_str[5] = nmea_msg_date[2];
  date_str[6] = nmea_msg_date[3];
  date_str[7] = '.';
  date_str[8] = nmea_msg_date[0];
  date_str[9] = nmea_msg_date[1];
  date_str[10] = '\0';

  oled_writeText(date_str, 2, 5);


  nmea_msg_lat[13] = '\0';
  nmea_msg_lat[12] = nmea_msg_lat[10];
  nmea_msg_lat[11] = '\'';
  nmea_msg_lat[10] = nmea_msg_lat[9];
  nmea_msg_lat[9] = nmea_msg_lat[8];
  nmea_msg_lat[8] = nmea_msg_lat[7];
  nmea_msg_lat[7] = nmea_msg_lat[6];
  nmea_msg_lat[6] = nmea_msg_lat[5];
  nmea_msg_lat[5] = nmea_msg_lat[4];
  nmea_msg_lat[4] = nmea_msg_lat[3];
  nmea_msg_lat[3] = nmea_msg_lat[2];
  //nmea_msg_lat[2] = ' ';
  nmea_msg_lat[2] = -5;
  if (nmea_msg_lat[0] == '0')
  {
    nmea_msg_lat[0] = ' ';
  }

  oled_writeText(" ", 2, 2);
  oled_writeString(nmea_msg_lat);


  nmea_msg_lon[14] = '\0';
  nmea_msg_lon[13] = nmea_msg_lon[11];
  nmea_msg_lon[12] = '\'';
  nmea_msg_lon[11] = nmea_msg_lon[10];
  nmea_msg_lon[10] = nmea_msg_lon[9];
  nmea_msg_lon[9] = nmea_msg_lon[8];
  nmea_msg_lon[8] = nmea_msg_lon[7];
  nmea_msg_lon[7] = nmea_msg_lon[6];
  nmea_msg_lon[6] = nmea_msg_lon[5];
  nmea_msg_lon[5] = nmea_msg_lon[4];
  nmea_msg_lon[4] = nmea_msg_lon[3];
  //nmea_msg_lon[3] = ' ';
  nmea_msg_lon[3] = -5;
  if (nmea_msg_lon[0] == '0')
  {
    nmea_msg_lon[0] = ' ';
    if (nmea_msg_lon[1] == '0')
    {
      nmea_msg_lon[1] = ' ';
    }
  }

  oled_writeText(nmea_msg_lon, 2, 3);


  char good_fix = nmea_msg_fix_status[0];
  nmea_msg_fix_status[0] = 'F';
  nmea_msg_fix_status[1] = 'i';
  nmea_msg_fix_status[2] = 'x';
  nmea_msg_fix_status[3] = ':';
  if (good_fix == 'A')
  {
    nmea_msg_fix_status[4] = 'G';
    nmea_msg_fix_status[5] = 'o';
    nmea_msg_fix_status[6] = 'o';
    nmea_msg_fix_status[7] = 'd';
    nmea_msg_fix_status[8] = '\0';
  }
  else
  {
    nmea_msg_fix_status[4] = 'B';
    nmea_msg_fix_status[5] = 'a';
    nmea_msg_fix_status[6] = 'd';
    nmea_msg_fix_status[7] = ' ';
    nmea_msg_fix_status[8] = '\0';
  }
  oled_writeText(nmea_msg_fix_status, 2, 1);



  nmea_msg_sog[6] = '\0';
  //oled_writeText(nmea_msg_sog, 2, 6);// dont need miles per hour indicator
  float sog_meter_ps = 0.514*atof(nmea_msg_sog);
  int sog_int_whole = sog_meter_ps;
  int sog_int_frac = (sog_meter_ps - sog_int_whole)*1000.0;
  //snprintf(nmea_msg_sog, 10, "%6.4f", sog_mps);
  sprintf(nmea_msg_sog, "%2i.%03i", sog_int_whole, sog_int_frac);

  oled_writeText(nmea_msg_sog, 2, 4);
  oled_writeString("m/s");


  nmea_msg_sats[7] = 0;
  oled_writeText("Sats:", 2, 6);
  oled_writeString(nmea_msg_sats);


  nmea_msg_altitude[7] = 0;
  oled_writeText(nmea_msg_altitude, 64, 4);


  nmea_msg_geoid_separation[7] = 0;
  oled_writeText("Geoid:", 56, 6);
  oled_writeString(nmea_msg_geoid_separation);


  num2str4positive(rmc_msgs_parsed, string);
  oled_writeText(string, 64, 1);


  RMC_message_update = false;
  
  if (initialized_SD && good_fix == 'A')
  {
    if (log_index++ == (log_subsample-1))
    {
      if (!myFile)
      {
        sprintf(filename, "%.6s__.csv", nmea_msg_date);
        filename[0] = nmea_msg_date[4];
        filename[1] = nmea_msg_date[5];
        filename[2] = nmea_msg_date[2];
        filename[3] = nmea_msg_date[3];
        filename[4] = nmea_msg_date[0];
        filename[5] = nmea_msg_date[1];

        filename[6] = nmea_msg_time[0];
        filename[7] = nmea_msg_time[1];
        Serial.print(F("New file: "));
        Serial.println(filename);
        myFile = SD.open(filename, FILE_WRITE);
        if (myFile)
        {
          myFile.println("time, lat, lon, SOG [m/s], Alt");
          char temp = filename[8];
          filename[8] = 0;
          oled_writeText(filename, 64, 0);
          filename[8] = temp;
        }
      }

      if (myFile)
      {
        Serial.println(F("writing GPS sample to file"));
        char temp[18] = "";
        sprintf(temp, "%.8s, ", nmea_msg_time);
        myFile.print(temp);
        nmea_msg_lat[2] = '*';
        sprintf(temp, "%.13s, ", nmea_msg_lat);
        myFile.print(temp);
        nmea_msg_lon[3] = '*';
        sprintf(temp, "%.14s, ", nmea_msg_lon);
        myFile.print(temp);
        sprintf(temp, "%.6s, ", nmea_msg_sog);
        myFile.print(temp);
        sprintf(temp, "%.6s", nmea_msg_altitude);
        myFile.println(temp);
        myFile.flush();
        //myFile.close();
      }
      else
      {
        Serial.println(F("Error opening file on SD card"));
        oled_writeText("SD write err", 40, 1);
      }
      log_index = 0;
    }
  }
  clear_nmea_buffers();
}

void parse_nmea()
{
  while (Serial.available() > 0)
  {
    byte nmea_char = Serial.read();
    //num2str4positive(nmea_parse_state, string);
    //writeText_SPI(string, 64, 1);
    switch (nmea_parse_state)
    {
    case FLUSH_UNTIL_NEXT_NMEA_MESSAGE_START:
      if (nmea_char == '$')
      {
        // Message start found
        nmea_msgs_seen += 1;
        if (nmea_msgs_seen >= 10000)
        {
          nmea_msgs_seen_10k += 1;
          nmea_msgs_seen = 0;
        }
        desired_nmea_message_index = 0;
        nmea_parse_state = FINDING_SPECIFIC_NMEA_MESSAGE;
      }
      break;
    case FINDING_SPECIFIC_NMEA_MESSAGE:
      if (desired_nmea_message_index < 2)
      {
        // Skipping the 'talker ID' (first two bytes) of the nmea message
        desired_nmea_message_index++;
        continue;
      }
      else
      {
        nmea_message_id[desired_nmea_message_index-2] = nmea_char;
        desired_nmea_message_index++;
      }

      if (desired_nmea_message_index >= 5)
      {
        if (nmea_message_id[0] == 'R' && 
            nmea_message_id[1] == 'M' && 
            nmea_message_id[2] == 'C')
        {
          nmea_parse_state = FOUND_NMEA_MESSAGE_RMC;
          current_nmea_field = 0;
          clear_rmc_buffers();
        }
        else if (nmea_message_id[0] == 'G' && 
                 nmea_message_id[1] == 'G' && 
                 nmea_message_id[2] == 'A')
        {
          nmea_parse_state = FOUND_NMEA_MESSAGE_GGA;
          current_nmea_field = 0;
          clear_gga_buffers();
        }
        else
        {
          desired_nmea_message_index = 0;
          nmea_parse_state = FLUSH_UNTIL_NEXT_NMEA_MESSAGE_START;
        }
      }
      break;
    case FOUND_NMEA_MESSAGE_RMC:
      parse_nmea_msg_rmc(nmea_char);
      break;
    case FOUND_NMEA_MESSAGE_GGA:
      parse_nmea_msg_gga(nmea_char);
      break;
    }
  }
}

void parse_nmea_msg_gga(char nmea_msg_char)
{

  // $GNGGA,104631.00,6326.63235,N,01026.37737,E,1,12,0.83,46.7,M,39.9,M,,*7F
  //        1          2           3  4            5  6  7   8     9    10  11   12 13 14 15
  // $GNGGA,104631.00, 6326.63235, N, 01026.37737, E, 1, 12, 0.83, 46.7, M, 39.9, M,  ,   *7F
  if (nmea_msg_char == ',')
  {
    current_nmea_field++;
    if (current_nmea_field == 10 || current_nmea_field == 12)
    {
      return;
    }
    nmea_field_str_index = 0;
  }
  else
  {
    switch (current_nmea_field)
    {
      // parsing time from RMC message
    case 1: //nmea_msg_time[nmea_field_str_index++] = nmea_msg_char;
      break;
      // parsing lat from RMC message
    case 2: //nmea_msg_lat[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 3:
      // parsing lon from RMC message
    case 4: //nmea_msg_lon[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 5:
      break;
    case 6:
      nmea_msg_fix_quality[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 7:
      nmea_msg_sats[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 8: // hDOP
      break;
    case 9: // altitude and alt units
    case 10:
      nmea_msg_altitude[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 11: // geoid separation and geoid separation units
    case 12:
      nmea_msg_geoid_separation[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 13:
    case 14:
    default:
      nmea_parse_state = FLUSH_UNTIL_NEXT_NMEA_MESSAGE_START;
      GGA_message_update = true;
      break;
    }
  }
}

void parse_nmea_msg_rmc(char nmea_msg_char)
{

  // $GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146,,100117,,,A\*7B
  //        1         2          3 4           5 6     7 8      9 10 11 12
  // $GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146, ,100117,  ,  , A\*7B
  if (nmea_msg_char == ',')
  {
    current_nmea_field++;
    if (current_nmea_field == 4 || current_nmea_field == 6)
    {
      return;
    }
    nmea_field_str_index = 0;
  }
  else
  {
    switch (current_nmea_field)
    {
    case 1:
      nmea_msg_time[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 2:
      nmea_msg_fix_status[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 3:
    case 4:
      nmea_msg_lat[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 5:
    case 6:
      nmea_msg_lon[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 7: // speed over ground
      nmea_msg_sog[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 8: // ?? Track made good, degrees true ??
      break;
    case 9:
      nmea_msg_date[nmea_field_str_index++] = nmea_msg_char;
      break;
    case 10: // stop parsing after field 10 (by omitting break and falling through to default)
    case 11:
    case 12:
    case 13:
    case 14:
    default:
      nmea_parse_state = FLUSH_UNTIL_NEXT_NMEA_MESSAGE_START;
      RMC_message_update = true;
      rmc_msgs_parsed++;
      break;
    }
  }
}








// convert an integer of size up to 9999 to a string
void num2str4positive(int num, char* str)
{
  // constrain num to below 10000
  if (num > 10000)
  { 
    int temp = num / 10000;
    num -= temp*10000;
  }

  char leading_zeros = 0;
  char first_non_zero = -1;
  byte i = 0;

  // most sig digit
  byte dig = num / 1000;
  if (dig == 0 && first_non_zero == -1)
  {
    leading_zeros++;
  }
  else
  {
    first_non_zero = i;
  }
  str[i++] = dig + 48;
  num -= dig*1000;
  
  
  // second most sig digit
  dig = num / 100;
  if (dig == 0 && first_non_zero == -1)
  {
    leading_zeros++;
  }
  else
  {
    first_non_zero = i;
  }
  str[i++] = dig + 48;
  num -= dig*100;
  
  // third most sig digit
  dig = num / 10;
  if (dig == 0 && first_non_zero == -1)
  {
    leading_zeros++;
  }
  else
  {
    first_non_zero = i;
  }
  str[i++] = dig + 48;
  num -= dig*10;

  // least sig digit
  str[i++] = num + 48;

  str[i++] = 0;

  while (leading_zeros > 0)
  {
    str[--leading_zeros] = ' ';
  }
}




// convert an integer of size up to 99 to a string
void num2str2positive(int num, char* str)
{
  // constrain num to below 100
  if (num > 100)
  {
    int temp = num / 100;
    num -= temp*100;
  }

  char leading_zeros = 0;
  char first_non_zero = -1;
  byte i = 0;

  // most sig digit
  byte dig = num / 10;
  if (dig == 0 && first_non_zero == -1)
  {
    leading_zeros++;
  }
  else
  {
    first_non_zero = i;
  }
  str[i++] = dig + 48;
  num -= dig*10;

  // least sig digit
  str[i++] = num + 48;

  str[i++] = 0;

  while (leading_zeros > 0)
  {
    str[--leading_zeros] = ' ';
  }
}
