# Subtitle-Time-Edit
Provides simple interface to edit the time string of a subtitle (.srt) file so that all the subtitle texts of that file can be displayed earlier or later.

The main subtitle input file is not edited for backup purpose (if requires). Always a new edited file is created.

Might be very useful when the contents of the subtitle file need to be divided in parts. Can be done by giving sufficient delay time as time input. If delay time input results in negative appearance time of subtitle strings, those strings are discarded and the new edited file will only contain the second part of the file (subtitle strings with positive time appearance).
