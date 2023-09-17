#pragma once
#include <iostream>
#include <string>
#include <fstream>

struct eo_position
{
    float x;
    float y;
    float z;
};

eo_position get_position(char* memory)
{
    eo_position output;
    // If we get the entire memory address
    if (sizeof(memory) >= 10000)
    {
        // 0x010203
        // 0x030201
        // 103
        // 301

        // 805605C0 --> base address?
        // 8064E494 / 0x64E494 --> Y level?
        // 8064E494 / 0x64E4D4 --> Y level? --> 4 sets of 2 HEX to the left, X; 4 sets of 2 HEX to the right (of Y), Z     


        // Y LEVEL:
        char* test = &memory[0x64E4D4];
        // myfile << test[0] << test[1] << test[2] << test[3];
        char y_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { y_char_value[i] = test[3 - i]; }
        float y_position;
        memcpy(&y_position, &y_char_value, sizeof(y_position));
        std::cout << "Y Address: " << y_position << std::endl;

        // X LEVEL:
        char* testX = &memory[0x64E4D0];
        // myfile << test[0] << test[1] << test[2] << test[3];
        char x_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { x_char_value[i] = testX[3 - i]; }
        float x_position;
        memcpy(&x_position, &x_char_value, sizeof(x_position));
        std::cout << "X Address: " << x_position << std::endl;

        // Z LEVEL:
        char* testZ = &memory[0x64E4D8];
        // myfile << test[0] << test[1] << test[2] << test[3];
        char z_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { z_char_value[i] = testZ[3 - i]; }
        float z_position;
        memcpy(&z_position, &z_char_value, sizeof(z_position));
        std::cout << "z Address: " << z_position << std::endl;

        output.x = x_position;
        output.y = y_position;
        output.z = z_position;
        return output;
    }

    if (sizeof(memory) < 10000)
    {
        // 0x010203
        // 0x030201
        // 103
        // 301

        // 805605C0 --> base address?
        // 8064E494 / 0x64E494 --> Y level?
        // 8064E494 / 0x64E4D4 --> Y level? --> 4 sets of 2 HEX to the left, X; 4 sets of 2 HEX to the right (of Y), Z     


        // Y LEVEL:
        char* test = &memory[0x4];
        // myfile << test[0] << test[1] << test[2] << test[3];
        char y_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { y_char_value[i] = test[3 - i]; }
        float y_position;
        memcpy(&y_position, &y_char_value, sizeof(y_position));
        std::cout << "Y Address: " << y_position << std::endl;

        // X LEVEL:
        char* testX = &memory[0x0];
        // myfile << test[0] << test[1] << test[2] << test[3];
        char x_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { x_char_value[i] = testX[3 - i]; }
        float x_position;
        memcpy(&x_position, &x_char_value, sizeof(x_position));
        std::cout << "X Address: " << x_position << std::endl;

        // Z LEVEL:
        char* testZ = &memory[0x8];
        // myfile << test[0] << test[1] << test[2] << test[3];
        char z_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { z_char_value[i] = testZ[3 - i]; }
        float z_position;
        memcpy(&z_position, &z_char_value, sizeof(z_position));
        std::cout << "z Address: " << z_position << std::endl;
        std::ofstream myfile;
        //myfile.open("test3.txt", std::ios::out | std::ios::binary);
        //myfile << x_position << y_position << z_position;
        //myfile.close();

        output.x = x_position;
        output.y = y_position;
        output.z = z_position;
        return output;
    }
}




std::string get_map(char* input_bytes)
{
    // Locations:
    // Offset: +92BA0F92
    // Aquarium: 130
    // Nineball: 121           
    // Gatama Atoll: 132        
    // Cicero's strait: 139
    // North coast of canada: 81
    // Weddell sea: 0
    // Cortica River: 248
    // Zahhab Region, Red Sea: 131 

    if (sizeof(input_bytes) >= 400)
    {
        // Z LEVEL:
        std::ofstream myfile;
        myfile.open("test.txt", std::ios::out | std::ios::binary);
        // myfile << test[0] << test[1] << test[2] << test[3];
        char temp_char_value[4] = { ' ', ' ', ' ', ' ' };
        // Copy the bytes from the array to the Y position, in reverse Endianness to make sure the numbers line up.
        for (int i = 0; i < 4; i++) { temp_char_value[i] = input_bytes[3 - i]; }
        int map_integer;
        myfile << temp_char_value[0] << temp_char_value[1] << temp_char_value[2] << temp_char_value[3];
        std::cout << std::hex << temp_char_value[0] << temp_char_value[1] << temp_char_value[2] << temp_char_value[3] << std::dec;
        memcpy(&map_integer, &temp_char_value, sizeof(map_integer));
        std::cout << "Map int: " << map_integer << std::endl;
        return "unknown";
        myfile.close();
    }
    if (sizeof(input_bytes) < 400)
    {
        // Locations: (4-byte set)
        // first byte:
        //  in boat: 30
        //  underwater: 10
        //  ashore: 40
        // Offset: +8064E76C
        // Aquarium: 
        // Nineball: 20 00 00 00
        // Gatama Atoll: 10 00 00 04
        // Cicero's strait: 10 00 00 06
        // North coast of canada: 10 00 00 0D
        // Weddell sea: 10 00 00 0B
        // Cortica River: 10 00 00 0E
        // Zahhab Region, Red Sea: 10 00 00 08
        
        switch ((int)input_bytes[0]) {
        case 0x0:
            return "Position loading..."; // Loading
        case 0x30:
            switch ((int)input_bytes[3]) {
            case 0:
                return "In boat (Nineball)";
            case 0x04:
                return "In boat (Gatama Atoll)";
            case 0x06:
                return "In boat (Cicero's Strait)";
            case 0x0D:
                return "In boat (North coast of Canada)";
            case 0x0B:
                return "In boat (Wedell Sea)";
            case 0x0E:
                return "In boat (Cortica River)";
            case 0x08:
                return "In boat (Red Sea)";
            }
            
        case 0x40:
            return "Ashore";
        }

        switch ((int)input_bytes[3]) {
        case 0:
            return "Nineball";
        case 0x04:
            return "Gatama Atoll";
        case 0x06:
            return "Cicero's Strait";
        case 0x0D:
            return "North coast of Canada";
        case 0x0B:
            return "Wedell Sea";
        case 0x0E:
            return "Cortica River";
        case 0x08:
            return "Red Sea";
        }


        return "unknown";
    }
}