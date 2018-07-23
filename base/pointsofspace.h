#ifndef POINTSOFSPACE_H
#define POINTSOFSPACE_H
#include <QDataStream>
#include <QVariant>
#include <cmath>

class PointOfSpherical;
class PointOfParametrical;

class PointOfOrtogonal
{
public:
    friend class PointOfSpherical;
    friend class PointOfParametrical;
    PointOfOrtogonal( const double x=0., const double y=0., const double z=0.);
    PointOfOrtogonal( const PointOfOrtogonal &);
    PointOfOrtogonal( const PointOfSpherical &);
    PointOfOrtogonal( const PointOfParametrical &);
    PointOfOrtogonal( const QVariant &);

    QVariant toQVariant() const;
    bool setQVariant(const QVariant &val);
    void clean();

    friend QDataStream &operator<<(QDataStream &s, const PointOfOrtogonal &p);
    friend QDataStream &operator>>(QDataStream &s, PointOfOrtogonal &p);

    friend QDebug operator<<(QDebug dbg, const PointOfOrtogonal &p);
    inline void bzero(uchar *m ,int len) {memset(m,0,len);}
    inline double getX() const { return d.mX; }
    inline double getY() const { return d.mY; }
    inline double getZ() const { return d.mZ; }
    double getDistanceTo( const PointOfOrtogonal &po)const;

    inline void   setX( const double val) { d.mX = val;}
    inline void   setY( const double val) { d.mY = val;}
    inline void   setZ( const double val) { d.mZ = val;}
    inline void   setXYZ( const double x, const double y, const double z) { d.mX = x; d.mY = y; d.mZ = z;}

    const PointOfParametrical toParametrical( const double course) const;
    static const PointOfParametrical toParametrical(const PointOfOrtogonal &point, const double course);

    bool operator == (const PointOfOrtogonal &p) const ;

    const PointOfOrtogonal operator +( const PointOfOrtogonal &rhv) const;
    const PointOfOrtogonal operator -( const PointOfOrtogonal &rhv) const;
    const PointOfOrtogonal operator *( const double rhv) const ;
    PointOfOrtogonal &operator+=( const PointOfOrtogonal &rhv);
    PointOfOrtogonal &operator-=( const PointOfOrtogonal &rhv);
    PointOfOrtogonal &operator+=( const PointOfSpherical &rhv);
    PointOfOrtogonal &operator=( const PointOfOrtogonal &rhv);
    PointOfOrtogonal &operator=( const PointOfSpherical &rhv);
    PointOfOrtogonal &operator=( const PointOfParametrical &rhv);

protected:
    /// >маркер в строчном представлении 'PoOr'.
    const static qint32 cMarker = 0x506f4f72;

    struct PData
    {
        qint32 mMarker;
        double mX;
        double mY;
        double mZ;
    } d __attribute__((packed));

};


//-----------------------------------------------------------------------------
class PointOfOrtogonal;

/*!
 * \class PointOfSpherical
 * \brief The PointOfSpherical class
 */

class PointOfSpherical
{
public:
    friend class PointOfOrtogonal;
    PointOfSpherical( const double e=0., const double b=0., const double r=0.);
    PointOfSpherical( const PointOfSpherical &);
    PointOfSpherical( const PointOfOrtogonal &);
    PointOfSpherical( const QVariant &);

    QVariant toQVariant() const;
    bool setQVariant(const QVariant &val);
    void clean();

    friend QDataStream &operator<<(QDataStream &s, const PointOfSpherical &p);
    friend QDataStream &operator>>(QDataStream &s, PointOfSpherical &p);

    friend QDebug operator<<(QDebug dbg, const PointOfSpherical &p);
    inline void bzero(uchar *m ,int len) {memset(m,0,len);}
    inline double getE() const { return d.mE; }
    inline double getB() const { return d.mB; }
    inline double getR() const { return d.mR; }
    inline double getRhor() const { return d.mR * cos( d.mE);}
    inline double getDistanceTo( const PointOfSpherical &p) const{ return static_cast<PointOfOrtogonal>(*this).getDistanceTo(static_cast<PointOfOrtogonal>(p));}

    inline void   setR( const double val) { if( val >= 0.) d.mR = val; else d.mR = 0.;}
    void          setE( const double val);
    void          setB( const double val);
    void          setEBR( const double, const double, const double);

    bool operator == (const PointOfSpherical &p) const;
    const PointOfSpherical operator +( const PointOfSpherical &rhv) const ;
    const PointOfSpherical operator -(  const PointOfSpherical &rhv)const;
    PointOfSpherical &operator+=(const PointOfSpherical &rhv);
    PointOfSpherical &operator-=(const PointOfSpherical &rhv);
    PointOfSpherical &operator=( const PointOfSpherical &rhv);
    PointOfSpherical &operator=( const PointOfOrtogonal &rhv);


protected:
    /// >маркер в строчном представлении 'PoSp'.
    const static qint32 cMarker = 0x506f5370;

    struct PData
    {
        qint32 mMarker;
        double mE;
        double mB;
        double mR;
    } d __attribute__((packed));

};


//-----------------------------------------------------------------------------
class PointOfParametrical
{
public:
    PointOfParametrical( const double &l=0., const double &h=0., const double &p=0., const double &q=0.);
    PointOfParametrical( const PointOfParametrical &);
    PointOfParametrical( const PointOfOrtogonal &);
    PointOfParametrical( const QVariant &);

    QVariant toQVariant() const;
    bool setQVariant(const QVariant &val);
    void clean();

    friend QDataStream &operator<<(QDataStream &s, const PointOfParametrical &p);
    friend QDataStream &operator>>(QDataStream &s, PointOfParametrical &p);

    friend QDebug operator<<(QDebug dbg, const PointOfParametrical &p);
    inline void bzero(uchar *m ,int len) {memset(m,0,len);}
    inline double getL() const { return d.mL;}
    inline double getH() const { return d.mH;}
    inline double getP() const { return d.mP;}
    inline double getQ() const { return d.mQ;}
    double getDistanceTo( const PointOfParametrical &p)const;
    inline double getRelativeBearing() const {return atan2(d.mP, d.mL);}  //!< Получение курсового угла.
    inline bool   isOvertake() const { return (d.mL < 0.);}               //!< Формирование признака "В догон"!

    inline void setL( const double &val) { d.mL = val;}
    inline void setH( const double &val) { d.mH = val;}
    inline void setP( const double &val) { d.mP = val;}
    void setQ( const double val);
    void setLHPQ( const double &l, const double &h, const double &p, const double &q) { d.mL = l; d.mH =h; d.mP =p; setQ(q);}

    bool operator == (const PointOfParametrical &p) const;
    const PointOfParametrical operator +( const PointOfParametrical &rhv) const;
    const PointOfParametrical operator -( const PointOfParametrical &rhv) const;
    const PointOfParametrical operator *( const double rhv) const;
    PointOfParametrical &operator+=(const PointOfParametrical &rhv);
    PointOfParametrical &operator-=(const PointOfParametrical &rhv);
    PointOfParametrical &operator=( const PointOfParametrical &rhv);
    PointOfParametrical &operator=( const PointOfOrtogonal &rhv);

protected:
    /// >маркер в строчном представлении 'PoPa'.
    const static qint32 cMarker = 0x506f5061;

    struct PData
    {
        qint32 mMarker;
        double mL;
        double mH;
        double mP;
        double mQ;
    } d __attribute__((packed));
};

#endif // POINTSOFSPACE_H
