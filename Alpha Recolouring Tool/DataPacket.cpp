#include "DataPacket.h"

DataPacket::DataPacket()
{

    // Load font file into memory
    std::ifstream fontFile("font.ttf", std::ios::binary | std::ios::ate);
    if (!fontFile) {
        std::cerr << "Could not open font.ttf\n";
        return ;
    }

    std::streamsize size = fontFile.tellg();
    fontFile.seekg(0, std::ios::beg);

    if (size > 0 && size <= std::numeric_limits<std::streamsize>::max()) {
        std::vector<char> buffer(size);
        if (!fontFile.read(buffer.data(), size))
        {
            std::cerr << "Error reading font.ttf\n";
            return ;
        }

        // Now buffer contains the binary data of the font file

        // Open output file and write size and font data
        std::ofstream outputFile("font.dat", std::ios::binary);
        if (!outputFile) {
            std::cerr << "Could not open font.dat for writing\n";
            return ;
        }

        unsigned int sizeInt = static_cast<unsigned int>(size);  // convert to unsigned int
        outputFile.write(reinterpret_cast<char*>(&sizeInt), sizeof(sizeInt));  // write size
        outputFile.write(buffer.data(), buffer.size());  // write font data
        outputFile.close();  // explicitly close the file
    }
    else {
        std::cerr << "Font file is too large\n";
        return ;
    }

}
