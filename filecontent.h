#ifndef FILECONTENT_H
#define FILECONTENT_H

#include <QVector>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>

#include <QListWidgetItem>

struct StringMeta
{
    uint64_t id;
    uint64_t size;
    uint64_t no;
    uint64_t ffff;
    uint64_t idk012;
};

struct LanguageMeta
{
    uint32_t id;
    uint32_t size;
};

struct LanguageContent
{
    uint32_t sizes[14];
    QVector<StringMeta> meta;
    QVector<uint64_t> str_sizes;
    QVector<QString> strings;
    std::string random;
};

struct StringItem : public QListWidgetItem
{
    int index;

    StringItem(QString const& str, QListWidget *parent, int index)
        : QListWidgetItem(str, parent)
        , index(index)
    { }
};

class FileContent
{
    template<typename T>
    static void binread(std::istream& is, T& data)
    {
        constexpr std::size_t size = sizeof(T);
        char buf[size];
        is.read(buf, size);
        std::memcpy(&data, buf, size);
    }

    template<typename T, std::size_t N>
    static void binread(std::istream& is, T (&data)[N])
    {
        constexpr std::size_t size = sizeof(T);
        for(std::size_t i = 0; i < N; ++i)
        {
            char buf[size];
            is.read(buf, size);
            std::memcpy(data+i, buf, size);
        }
    }

public:

    int lang_cnt;
    QVector<LanguageMeta> sections;
    QVector<LanguageContent> languages;

    FileContent() = default;

    FileContent(std::istream& is)
    {
        binread(is, lang_cnt);
        binread(is, lang_cnt);

        for(int i = 0; i < lang_cnt; ++i)
        {
            LanguageMeta lm;
            binread(is, lm);
            sections.push_back(lm);
        }

        for(int i = 0; i < lang_cnt; ++i)
        {
            LanguageContent lc;
            binread(is, lc.sizes);

            uint32_t strings_cnt = lc.sizes[2];

            for(uint32_t j = 0; j < strings_cnt; ++j)
            {
                StringMeta sm;
                binread(is, sm);
                lc.meta.push_back(sm);
            }

            uint32_t rand_cnt = lc.sizes[3];
            uint32_t rand_miss = 0;

            for(uint32_t j = 0; j < rand_cnt; ++j)
            {
                uint64_t ss[2];
                binread(is, ss[0]);
                binread(is, ss[1]);
                if(ss[0] != 0)
                    lc.str_sizes.push_back(ss[0]);
                else
                {
                    ++rand_miss;
                }
            }

            rand_cnt -= rand_miss;

            uint64_t strings_len = lc.sizes[4];
            std::unique_ptr<char[]> buf{new char[strings_len]};
            is.read(buf.get(), strings_len);

            const char * str = buf.get() + 1;
            for(uint32_t j = 1; j < rand_cnt; ++j)
            {
                if(j == 640)
                {
                    // if is only for breakpoint
                }

                std::size_t len = lc.str_sizes[j] - lc.str_sizes[j-1];
                lc.strings.push_back(QString::fromUtf8(str, len - 1));
                str += len;
            }

            uint32_t rand_size = lc.sizes[5];
            std::unique_ptr<char[]> buf2{new char[rand_size]};
            is.read(buf2.get(), rand_size);

            lc.random = std::string(buf2.get(), rand_size);

            languages.push_back(lc);
        }
    }
};

#endif // FILECONTENT_H
