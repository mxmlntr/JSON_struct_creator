/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  /home/visxim/CLionProjects/JSON_struct_creator/main.cpp
 *        \brief  main of the JSON_struct_creator
 *
 *      \details This project is for generating JSON files and the neccessary code (struct, archive, CRC, parse)
 *      to run it. The size can be predefined by the number of cycles.
 *      ATTENTION: These generated JSON files are with the only purpose of simulating different sizes JSON files,
 *      they depend on a repetitive algorithm.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

uint16_t str_obj_itr = 0;
uint32_t str_obj_appe_itr = 0;

uint16_t str_val_itr = 0;

//! Definition of the String which will define the JSON file structure
string object_names[10] = {"name","description","mode","version","status","path","buffer_size","id","number","length"};
string object_values[10] = {"JSON_creator","dummy json","shutdown","V1.283","running","~/etc/tmp/process1/running","2048","1337","1","2309763"};

//! resets the Values of the iterators
void resetValues()
{
    str_obj_itr = 0;
    str_obj_appe_itr = 0;
    str_val_itr = 0;
};

//! returns the next name from the object_name array
string get_string_object()
{
    if(str_obj_itr == 10)
    {
        str_obj_itr = 0;
    }
    string buf = object_names[str_obj_itr++];
   return buf.append(to_string(str_obj_appe_itr++));
}

//! returns the next value from the object_value array
string get_string_value()
{
    if(str_val_itr == 10)
    {
        str_val_itr = 0;
    }
    string buf = object_values[str_val_itr++];
    return buf;
}

//! Function for creating the synthetic JSON file with the given parameters
uint8_t createJSON(uint16_t length)
{
    ofstream jsn("/home/visxim/CLionProjects/ShareFolder/EXMPLE.json");

    jsn << "{\n";

    for(uint32_t j = 0; j<length/10; j++)
    {
        for(uint16_t i = 0; i<6;i++)
        {
            string object = get_string_object();
            string value = get_string_value();
            object = R"(")" + object + R"(")" + ": " + R"(")" + value + R"(")";
            object = object + "," + "\n";
            jsn << object;
        }
        for(uint16_t i = 0; i<4;i++)
        {
            string object = get_string_object();
            string value = get_string_value();
            object = R"(")" + object + R"(")" + ": " + value;
            if(((length/10)-j) == 1 && i==3)
            {
                object = object + "\n";
            }
            else
            {
                object = object + "," + "\n";
            }
            jsn << object;
        }
    }
    jsn << "}\n";
    jsn.close();
    resetValues();
    return 0;
};

//! Function for creating the struct for the matching JSON file
uint8_t createStruct(uint16_t length)
{
    ofstream strct("createdStruct.cpp");

    strct << "typedef struct {\n";

    for(uint16_t i = 0; i<(length/10);i++)
    {
        for(uint16_t i = 0; i<6;i++)
        {
            string object = get_string_object();
            strct << "string " + object + ";" + "\n";
        }
        for(uint16_t i = 0; i<4;i++)
        {
            string object = get_string_object();
            strct << "int " + object + ";" + "\n";
        }
    }

    strct << "int checksum;\n";
    strct << "}\n";

    strct.close();
    resetValues();
    return 0;
};

//! Function for creating the Archive for the matching JSON file
uint8_t createArchive(uint16_t length)
{
    ofstream arc("createdArchive.cpp");

    arc <<  "//!function for serializing the struct\ntemplate <typename Archive>\nvoid serialize(Archive& ar, const unsigned int version)\n{\n";
    for(uint16_t i = 0; i<(length/10);i++)
    {
        for(uint16_t i = 0; i<10;i++)
        {
            string object = get_string_object();
            arc << "ar & " + object + ";" + "\n";
        }
    }
    arc << "ar & checksum;\n";
    arc << "}\n";
    arc.close();
    resetValues();
    return 0;
};
//! Function for creating the CRC for the matching JSON file
uint8_t createCRC(uint16_t length)
{
    ofstream crc("createdCRC.cpp");
    crc  <<    "//!Internal struct for CRC creation, generated by JSON_struct_creator\n int calcCRC() {\nCRC crc;\n";
    for(uint16_t i = 0; i<(length/10);i++)
    {
        for(uint16_t i = 0; i<10;i++)
        {
            string object = get_string_object();
            crc << "crc(" + object + ");" + "\n";
        }
    }
    crc << "return crc.get();\n}\n";
};

//! Function for creating the parse information for the matching JSON file
uint16_t createParse(uint16_t length)
{
    ofstream prs("createdParse.cpp");
    for(uint16_t i = 0; i<(length/10);i++)
    {
        for(uint16_t i = 0; i<6;i++)
        {
            string object = get_string_object();
            prs << "data." + object + R"( = doc[")" + object + R"("].GetString();)" + "\n";
        }
        for(uint16_t i = 0; i<4;i++)
        {
            string object = get_string_object();
            prs << "data." + object + R"( = doc[")" + object + R"("].GetInt();)" + "\n";
        }
    }
    resetValues();
    return 0;
};

//! main
int main()
{
    //! Declaration of the size of the JSON file 100 ~ 2,5kb
    uint16_t size = 10000;
    //! Creating all neccessary objects NOTICE: JSON file will be saved to ShareFolder
    createJSON(size);
    createStruct(size);
    createArchive(size);
    createParse(size);
    createCRC(size);
    return 0;
}
