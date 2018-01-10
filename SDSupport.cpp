/*
This file is part of Ionlib.  Copyright (C) 2016  Tim Sweet

Ionlib is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Ionlib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Ionlib.If not, see <http://www.gnu.org/licenses/>.
*/
namespace ion
{
    //void printDirectory(File dir, int numTabs)
    //{
    //    while (true)
    //    {
    //
    //        File entry = dir.openNextFile();
    //        if (!entry)
    //        {
    //            // no more files
    //            break;
    //        }
    //        for (uint8_t i = 0; i < numTabs; i++)
    //        {
    //            SERIAL_DEBUG.print('\t');
    //        }
    //        SERIAL_DEBUG.print(entry.name());
    //        if (entry.isDirectory())
    //        {
    //            SERIAL_DEBUG.println("/");
    //            printDirectory(entry, numTabs + 1);
    //        } else
    //        {
    //            // files have sizes, directories do not
    //            SERIAL_DEBUG.print("\t\t");
    //            SERIAL_DEBUG.println(entry.size(), DEC);
    //        }
    //        entry.close();
    //    }
    //}
};