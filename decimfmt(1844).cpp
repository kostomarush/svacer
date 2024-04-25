void DecimalFormat::doFastFormatInt32(int32_t input, bool isNegative, UnicodeString &output) const
{
    U_ASSERT(fields->canUseFastFormat);
    if (isNegative)
    {
        output.append(fields->fastData.cpMinusSign);
        U_ASSERT(input != INT32_MIN); // handled by callers
        input = -input;
    }
    // Cap at int32_t to make the buffer small and operations fast.
    // Longest string: "2,147,483,648" (13 chars in length)
    static constexpr int32_t localCapacity = 13;
    char16_t localBuffer[localCapacity];
    char16_t *ptr = localBuffer + localCapacity;
    int8_t group = 0;
    int8_t minInt = (fields->fastData.minInt < 1) ? 1 : fields->fastData.minInt;
    for (int8_t i = 0; i < fields->fastData.maxInt && (input != 0 || i < minInt) && (ptr > localBuffer); i++) // Добавлено условие: (ptr > localBuffer)
    {
        if (group++ == 3 && fields->fastData.cpGroupingSeparator != 0)
        {
            *(--ptr) = fields->fastData.cpGroupingSeparator;
            group = 1;
        }
        std::div_t res = std::div(input, 10);
        *(--ptr) = static_cast<char16_t>(fields->fastData.cpZero + res.rem);
        input = res.quot;
    }
    int32_t len = localCapacity - static_cast<int32_t>(ptr - localBuffer);
    output.append(ptr, len);
}