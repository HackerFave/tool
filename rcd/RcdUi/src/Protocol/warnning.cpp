#include "warnning.h"
#include <cstring>

warnning::warnning()
{

}

std::vector<size_t> warnning::getWarningBit()
{
    std::vector<size_t> result;
    int index = 0;
    for (auto &i : warningVector)
    {
        _getWarningBit(i, result, (index * BITSET_LENGTH));
        index++;
    }
    return result;
}

bool warnning::testWarning(WarningBit _bit)
{
    int index = _bit / BITSET_LENGTH;
    int bitOffset = _bit % BITSET_LENGTH;
    return warningVector[index].test(bitOffset);
}

int warnning::loadFromRawData(const char *rawData, int len)
{
    int index = 0;
    warningVector.clear();
    for (int i = 0; i < BITSET_BYTE_LEN; i++)
    {
        warningVector.push_back(std::bitset<BITSET_LENGTH>(rawData[index++]));
    }
    std::memmove(&status, &rawData[index], sizeof(status));
    index += sizeof(status);
    return index;
}

int warnning::_getWarningBit(const std::bitset<BITSET_LENGTH> &_warn, std::vector<size_t> &outResult, int offset)
{
    if (_warn.any())
    {
        size_t pos = 0;
        while (pos < _warn.size())
        {
            if (_warn.test(pos))
            {
                outResult.push_back(pos + offset);
            }
            pos++;
        }
    }
    return 0;
}

