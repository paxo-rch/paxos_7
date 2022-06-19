#ifndef FILES_HPP
#define FILES_HPP

void listDir(String dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = SD.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(file.name(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

bool createDir(String path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (SD.mkdir(path))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

bool removeDir(String path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (SD.rmdir(path))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

String readFile(String path)
{
    Serial.print("Reading file: "+ path + "\n");

    File file = SD.open(path);
    if (!file)
    {
        return "-1";
    }

    String str;

    while (file.available())
    {
        str += (char)file.read();
    }
    file.close();
    return str;
}

bool writeFile(String path, String message)
{
    File file = SD.open(path, FILE_WRITE);
    if (!file)
    {
        return 1;
    }
    if (file.print(message))
    {
        file.close();
        return 0;
    }
    else
    {
        file.close();
        return 1;
    }
}

bool appendFile(String path, String message)
{
    File file = SD.open(path, FILE_APPEND);
    if (!file)
    {
        return 1;
    }
    if (file.print(message))
    {
        file.close();
        return 0;
    }
    else
    {
        file.close();
        return 1;
    }
    
}

bool renameFile(String path1, String path2)
{
    if (SD.rename(path1, path2))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

bool deleteFile(String path)
{
    if (SD.remove(path))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*class SD_File
{
public:
    SD_File(String location);
    bool open();
    bool erase(String buffer);
    bool append(String buffer);
    bool clear();
    String get();

    File file;

private:
    String location;
};

SD_File::SD_File(String location)
{
    this->location = location;
}

bool SD_File::open()
{
    file = SD.open(location);
}

bool SD_File::erase(String buffer)
{
    SD.remove(location);
    append(buffer);
}

bool SD_File::append(String buffer)
{
    open();
    for (int i = 0; i < buffer.length(); i++)
        file.write(buffer[i]);
}

bool SD_File::clear()
{
    SD.remove(location);
}

String SD_File::get()
{
    String str="";
    while (file.available())
    {
        str+=char(file.read());
    }
    return str;
}*/

#endif