#ifndef BUILDDATA_H
#define BUILDDATA_H

#include <QString>
#include <QStringList>

class BuildData
{
public:
    BuildData();
    void read(const char * filename);
    bool isEmpty(void);
    void getVersionParts(void);
    void splitVersion(void);
    void setVersion(const char * v);

    friend bool operator<(const BuildData& l, const BuildData& r);

private:
    QString _version;
    QStringList _partslist;
};

inline bool operator> (const BuildData& lhs, const BuildData& rhs){ return rhs < lhs; }
inline bool operator<=(const BuildData& lhs, const BuildData& rhs){ return !(lhs > rhs); }
inline bool operator>=(const BuildData& lhs, const BuildData& rhs){ return !(lhs < rhs); }

#endif // BUILDDATA_H
