#ifndef MOTIONPOINTSOFSPASE_H
#define MOTIONPOINTSOFSPASE_H
#include <QDataStream>
#include "pointsofspace.h"

/****************************************************************************************
 *
 *                              class MotionPointOfOrtogonal
 *
 ****************************************************************************************/

class MotionPointOfSpherical;
class MotionPointOfParametrical;

class MotionPointOfOrtogonal : public PointOfOrtogonal
{
public:
    friend class MotionPointOfSpherical;
    friend class MotionPointOfParametrical;

    MotionPointOfOrtogonal( const double x=0., const double y=0., const double z=0.,
                            const double vx=0.,const double vy=0.,const double vz=0.);
    MotionPointOfOrtogonal(const PointOfOrtogonal &, const PointOfOrtogonal & );
    MotionPointOfOrtogonal( const MotionPointOfSpherical &);
    MotionPointOfOrtogonal( const MotionPointOfParametrical &);
    MotionPointOfOrtogonal( const QVariant &);

    QVariant toQVariant() const;
    bool setQVariant(const QVariant &val);
    void clean();

    friend QDataStream &operator<<(QDataStream &s, const MotionPointOfOrtogonal &p);
    friend QDataStream &operator>>(QDataStream &s, MotionPointOfOrtogonal &p);

    friend QDebug operator<<(QDebug dbg, const MotionPointOfOrtogonal &p);

    inline void bzero(uchar *m ,int len) {memset(m,0,len);}

    inline double getVx() const { return v.mVx;}
    inline double getVy() const { return v.mVy;}
    inline double getVz() const { return v.mVz;}
    inline double getV() const { return sqrt( v.mVx * v.mVx + v.mVy * v.mVy + v.mVz * v.mVz);}
    inline double getVhor() const { return sqrt( v.mVx * v.mVx + v.mVz * v.mVz);}
    inline double getQ() const { return atan2( v.mVz, v.mVx);}
    inline double getPitch() const { return atan2( v.mVy, getVhor());}
    const PointOfOrtogonal getCoords() const;
    const PointOfOrtogonal getVelocities() const;
    MotionPointOfOrtogonal getPredicted( const double) const;

    inline void setVx( const double val) {v.mVx = val;}
    inline void setVy( const double val) {v.mVy = val;}
    inline void setVz( const double val) {v.mVz = val;}
    inline void setVxVyVz( const double vx, const double vy, const double vz) {v.mVx= vx; v.mVy=vy; v.mVz=vz;}
    inline void setXYZVxVyVz( const double x, const double y, const double z,
                              const double vx, const double vy, const double vz) {d.mX= x; d.mY= y; d.mZ=z; v.mVx= vx; v.mVy=vy; v.mVz=vz;}

    MotionPointOfOrtogonal &operator =( const PointOfOrtogonal &);
    MotionPointOfOrtogonal &operator =( const MotionPointOfSpherical &);
    MotionPointOfOrtogonal &operator =( const MotionPointOfParametrical&);

protected:
    /// >маркер в строчном представлении 'MPOr'.
    const static qint32 cMarker = 0x4d504f72;

    struct
    {
        qint32 mMarker;
        double mVx;
        double mVy;
        double mVz;
    } v __attribute__((packed));
};

/****************************************************************************************
 *
 *                      class MotionPointOfSpherical
 *
 ****************************************************************************************/

class MotionPointOfSpherical : public PointOfSpherical
{
public:
    friend class PointOfOrtogonal;
    friend class PointOfParametric;

    MotionPointOfSpherical( const double e=0., const double b=0., const double r=0.,
                            const double ve=0., const double vb=0., const double vr=0.);
    MotionPointOfSpherical( const PointOfSpherical &pos);
    MotionPointOfSpherical( const MotionPointOfOrtogonal &mpos);
    MotionPointOfSpherical( const QVariant &);

    QVariant toQVariant() const;
    bool setQVariant(const QVariant &val);
    void clean();

    friend QDataStream &operator<<(QDataStream &s, const MotionPointOfSpherical &p);
    friend QDataStream &operator>>(QDataStream &s, MotionPointOfSpherical &p);

    friend QDebug operator<<(QDebug dbg, const MotionPointOfSpherical &p);
    inline void bzero(uchar *m ,int len) {memset(m,0,len);}

    inline double getVe() const { return v.mVe;}
    inline double getVb() const { return v.mVb;}
    inline double getVr() const { return v.mVr;}
    double getV() const;
    inline double getVrhor() const { return v.mVr * cos(d.mE);}
    const PointOfSpherical getCoords() const;
    MotionPointOfSpherical getPredicted( const double) const;

    inline void setVe( const double val) { v.mVe = val;}
    inline void setVb( const double val) { v.mVb = val;}
    inline void setVr( const double val) { v.mVr = val;}
    inline void setVeVbVr( const double ve, const double vb, const double vr) { v.mVe = ve; v.mVb = vb; v.mVr = vr;}
    inline void setEBRVeVbVr(const double e, const double b, const double r,
                             const double ve, const double vb, const double vr ) { d.mE= e; d.mB= b; d.mR= r; v.mVe= ve; v.mVb= vb; v.mVr= vr;}
    void setCoords( const PointOfSpherical &val) { d.mE = val.getE(); d.mB = val.getB(); d.mR = val.getR();}
    void setVelocities( const PointOfSpherical &val) { v.mVe = val.getE(); v.mVb = val.getB(); v.mVr = val.getR();}

    MotionPointOfSpherical &operator =( const PointOfSpherical &);
    MotionPointOfSpherical &operator =( const MotionPointOfOrtogonal &);
    MotionPointOfSpherical &operator =( const MotionPointOfParametrical &);

    double safeAtan2( const double s, const double c);

protected:
    /// >маркер в строчном представлении 'MPSp'.
    const static qint32 cMarker = 0x4d505370;

    struct
    {
        qint32 mMarker;
        double mVe;
        double mVb;
        double mVr;
    } v __attribute__((packed));
};

/****************************************************************************************
 *
 *                              class MotionPointOfParametric
 *
 ****************************************************************************************/

class MotionPointOfParametrical : public PointOfParametrical
{
public:
    MotionPointOfParametrical( const double l= 0., const double h= 0., const double p= 0., const double q= 0.,
                               const double vl=0., const double vp=0., const double vh=0.);
    MotionPointOfParametrical( const MotionPointOfOrtogonal &);
    MotionPointOfParametrical( const MotionPointOfSpherical &);
    MotionPointOfParametrical( const QVariant &);

    QVariant toQVariant() const;
    bool setQVariant(const QVariant &val);
    void clean();

    inline void bzero(uchar *m ,int len) {memset(m,0,len);}

    friend QDataStream &operator<<(QDataStream &s, const MotionPointOfParametrical &p);
    friend QDataStream &operator>>(QDataStream &s, MotionPointOfParametrical &p);

    friend QDebug operator<<(QDebug dbg, const MotionPointOfParametrical &p);

    inline double getVl() const { return v.mVl;}
    inline double getVh() const { return v.mVh;}
    inline double getVp() const { return v.mVp;}
    inline double getVhor() const { return sqrt( v.mVl*v.mVl + v.mVp*v.mVp);}
    inline double getV() const { return sqrt( v.mVl*v.mVl + v.mVh*v.mVh + v.mVp*v.mVp);}
    inline double getPitch() const { return atan2( getVh(), getVhor());}
    const PointOfParametrical getCoords() const;
    const PointOfParametrical getVelocity() const;
    MotionPointOfParametrical getPredicted( const double) const;

    inline void setVl( const double val) { v.mVl = val;}
    inline void setVh( const double val) { v.mVh = val;}
    inline void setVp( const double val) { v.mVp = val;}
    inline void setVlVhVp(const double vl=0., const double vp=0., const double vh=0.) { v.mVl = vl;  v.mVh = vh;  v.mVp = vp;}
    inline void setLHPQVlVhVp(const double l= 0., const double h= 0., const double p= 0., const double q= 0.,
                              const double vl=0., const double vh=0., const double vp=0.) { d.mL = l; d.mH =h; d.mP =p; setQ(q); v.mVl = vl;  v.mVh = vh;  v.mVp = vp;}

    MotionPointOfParametrical &operator =( const PointOfParametrical &);
    MotionPointOfParametrical &operator =( const MotionPointOfOrtogonal &);
    MotionPointOfParametrical &operator =( const MotionPointOfSpherical &);

protected:
    /// >маркер в строчном представлении 'MPPa'.
    const static qint32 cMarker = 0x4d504d61;

    struct
    {
        qint32 mMarker;
        double mVl;
        double mVh;
        double mVp;
    } v __attribute__((packed));
};

#endif // MOTIONPOINTSOFSPASE_H
