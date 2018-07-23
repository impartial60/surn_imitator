#include "motionpointsofspase.h"

#include <QDebug>
/*!
 * \class MotionPointOfOrtogonal
 * \brief Класс описывающий подвижную точку в земной прямоугольной
 *
 * Направление осей системы координат:
 * \list
 * \li Ось X - на Север.
 * \li Ось Y - в Верх.
 * \li Ось Z - на Восток.
 * \endlist
 *
 */


/*!
 * \brief Конструктор инициализирующий объект координатами и скоростями, которые задаются аргументами.
 * \param x
 * \param y
 * \param z
 * \param vx
 * \param vy
 * \param vz
 */


MotionPointOfOrtogonal::MotionPointOfOrtogonal(const double x, const double y, const double z, const double vx, const double vy, const double vz) :
    PointOfOrtogonal( x, y, z)
{
    v.mVx = vx;
    v.mVy = vy;
    v.mVz = vz;
    v.mMarker = cMarker;
}

/*!
 * \brief Конструктор инициализирующий объект получая значения координат и скоростей из двух объектов PointOfOrtogonal.
 * \param p Набор координат
 * \param v Набор скоростей
 */
MotionPointOfOrtogonal::MotionPointOfOrtogonal(const PointOfOrtogonal &p, const PointOfOrtogonal &s)
{
    *this = p;
    v.mVx = s.getX();
    v.mVy = s.getY();
    v.mVz = s.getZ();
    v.mMarker = cMarker;
}

/*!
 * \brief Конструктор преобразующий MotionPointOfOrtogonal в MotionPointOfSpherical
 */
MotionPointOfOrtogonal::MotionPointOfOrtogonal( const MotionPointOfSpherical &val)
{
    *this = operator =( val);
    v.mMarker = cMarker;
}

/*!
 * \brief Конструктор преобразующий MotionPointOfParametrical в MotionPointOfOrtogonal
 * \param val
 */
MotionPointOfOrtogonal::MotionPointOfOrtogonal(const MotionPointOfParametrical &val)
{
    *this = operator =( val);
    v.mMarker = cMarker;
}

/*!
 * \brief Преобразующий QVariant из конструктор.
 */
MotionPointOfOrtogonal::MotionPointOfOrtogonal(const QVariant &val)
{
    setQVariant(val);
}

/*!
 * \brief Помещает объект в контейнер QVariant
 * \return
 */
QVariant MotionPointOfOrtogonal::toQVariant() const
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.writeRawData((const char*)&d, sizeof(d));
    out.writeRawData((const char*)&v, sizeof(v));
    return data;
}

/*!
 * \brief Копирует значения с объекта упакованного в QVariant
 * \param val Ссылка на QVariant в который упакован объект PointOfOrtogonal.
 * \return
 */
bool MotionPointOfOrtogonal::setQVariant(const QVariant &val)
{
    QByteArray dd = val.toByteArray();
    QDataStream inp(&dd, QIODevice::ReadOnly);
    if(inp.readRawData((char*)&d, sizeof(d)) == sizeof(d))
    {
        if(inp.readRawData((char*)&v, sizeof(v)) == sizeof(v))
        {
            if(v.mMarker == cMarker)
                return true;
            else
                qCritical()<<"MotionPointOfOrtogonal::setQVariant: Error! A received marker is "<<QString::number(v.mMarker,16) << " instead "<< QString::number(cMarker,16);
        }else
        {
            qCritical()<<"MotionPointOfOrtogonal::setQVariant: Error! Size of \"v\" data isn`t "<<sizeof(d);
        }
    }else
    {
        qCritical()<<"MotionPointOfOrtogonal::setQVariant: Error! Size of \"d\" data isn`t "<<sizeof(d);
    }
    clean();
    return false;
}
void bzero(uchar *m ,int len) {memset(m,0,len);}

/*!
 * \brief Обнуляет все данные объекта этого класса.
 */
void MotionPointOfOrtogonal::clean()
{
    bzero((uchar*)&d, sizeof(d));
    bzero((uchar*)&v, sizeof(v));
    v.mMarker = cMarker;
}

/*!
 * \brief Выгрузка дынных этого класса в поток.
 * \param s ссылка на целевой поток.
 * \param p ссылка на выгружаемых объект этого класса.
 * \return
 */
QDataStream &operator<<(QDataStream &s, const MotionPointOfOrtogonal &p)
{
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.d))), sizeof(p.d)) ;
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.v))), sizeof(p.v)) ;
    return s;
}

/*!
 * \brief Загрузка данных этого класса из потока.
 * \param s ссылка на поток с входящими данными.
 * \param p ссылка на целевой объект этого класса.
 * \return
 */
QDataStream &operator>>(QDataStream &s, MotionPointOfOrtogonal &p)
{
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.d))), sizeof(p.d));
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.v))), sizeof(p.v));
    return s;
}

/*!
 * \brief Формирование отладочной информации об объекте этого класса.
 * \param dbg
 * \param p
 * \return
 */
QDebug operator<<(QDebug dbg, const MotionPointOfOrtogonal &p)
{
    dbg.nospace() << "MotionPointOfOrtogonal(X="<<p.d.mX<<" Y="<<p.d.mY<<" Z="<<p.d.mZ<<" Vx="<<p.v.mVx<<" Vy="<<p.v.mVy<<" Vz="<<p.v.mVz<<")";
    return dbg.space();
}

/*!
 * \fn double MotionPointOfOrtogonal::getQ();
 * \brief Получение курса движения точки
 * \return Значение курса в радианах.
 */

/*!
 * \fn double MotionPointOfOrtogonal::getV()
 * \brief Получение полной скорости (модуль вектора скорости).
 * \return Значение полной скорости
 */

/*!
 * \fn double MotionPointOfOrtogonal::getPitch();
 * \brief Получение тангажа движения точки
 * \return Значение тангажа в радианах.
 */

/*!
 * \brief MotionPointOfOrtogonal::getCoords
 * \return
 */
const PointOfOrtogonal MotionPointOfOrtogonal::getCoords() const
{
    PointOfOrtogonal coords( d.mX, d.mY, d.mZ);
    return coords;
}

/*!
 * \brief MotionPointOfOrtogonal::getVelocity
 * \return
 */
const PointOfOrtogonal MotionPointOfOrtogonal::getVelocities() const
{
    PointOfOrtogonal velocities( v.mVx, v.mVy, v.mVz);
    return velocities;
}

/*!
 * \brief Получение предсказанного (пролонгированного) состояния подвижной для указанного приращения времени.
 * \param dt Значение приращения времени в секундах.
 * \return Объект MotionPointOfOrtogonal.
 */
MotionPointOfOrtogonal MotionPointOfOrtogonal::getPredicted(const double dt) const
{
    MotionPointOfOrtogonal po( *this);

    po.d.mX += po.v.mVx * dt;
    po.d.mY += po.v.mVy * dt;
    po.d.mZ += po.v.mVz * dt;

    return po;
}

/*!
 * \brief Кроссклассовый оператор присваивания.
 * \param rhv
 * \return
 */
MotionPointOfOrtogonal &MotionPointOfOrtogonal::operator =(const PointOfOrtogonal &rhv)
{
    d.mX = rhv.getX();
    d.mY = rhv.getY();
    d.mZ = rhv.getZ();
    v.mMarker = cMarker;
    return *this;
}

/*!
 * \fn void MotionPointOfOrtogonal::setVx( const double &val);
 * \brief Установка значения линейной скорости Vx
 * \param val Значение линейной скорости.
 */

/*!
 * \fn void MotionPointOfOrtogonal::setVy( const double &val);
 * \brief Установка значения линейной скорости setVy
 * \param val Значение линейной скорости.
 */

/*!
 * \fn void MotionPointOfOrtogonal::setVz( const double &val);
 * \brief Установка значения линейной скорости setVz
 * \param val Значение линейной скоростию=.
 */

/*!
 * \fn void MotionPointOfOrtogonal::setXYZVxVyVz( const double &x, const double &y, const double &z, const double &vx, const double &vy, const double &vz);
 * \brief Установка значений координат и скорости.
 * \param x
 * \param y
 * \param z
 * \param vx
 * \param vy
 * \param vz
 */

/*!
 * \brief Кросклассовый (MotionPointOfOrtogonal > MotionPointOfSpherical) оператор присваивания.
 *
 * Присваивает объект MotionPointOfSpherical объекту MotionPointOfOrtogonal.
 * \param rhv
 * \return
 */
MotionPointOfOrtogonal &MotionPointOfOrtogonal::operator =(const MotionPointOfSpherical &rhv)
{
    *this = static_cast<PointOfOrtogonal>(rhv);
    v.mMarker = cMarker;

    double horR( rhv.getRhor()); // Линейная горизонтальная радиальная скорость.
    double tanVb( ::tan(rhv.getVb()));
    double horVe( d.mY * ::tan(rhv.getVe()));  // Горизинтальная составляющая скорости Ve.
    double horVre( rhv.getVrhor() - horVe);

    if( rhv.getR() == 0)
    {
        v.mVx = 0.;
        v.mVy = 0.;
        v.mVz = 0.;

        return *this;
    }
    v.mVy = horR * ::tan( rhv.getVe()) + rhv.getVr() * d.mY / rhv.getR();

    if( horR == 0)
    {
        v.mVx = 0.;
        v.mVz = 0.;

        return *this;
    }
    v.mVx = d.mX * horVre / horR - d.mZ * tanVb;
    v.mVz = d.mZ * horVre / horR + d.mX * tanVb;

    return *this;
}

/*!
 * \brief Преобразующий оператор присваивания
 * \param rhv Состояние подвижной точки в параметрических координатах.
 * \return Указатель на соответсвующий объект MotionPointOfOrtogonal
 */
MotionPointOfOrtogonal &MotionPointOfOrtogonal::operator =(const MotionPointOfParametrical &rhv)
{
    double q( rhv.getQ());
    v.mMarker = cMarker;

    q *= -1.;
    double sinq = sin(q);
    double cosq = -1.*cos(q);

    d.mX = rhv.getL() * cosq - rhv.getP() * sinq;
    d.mY = rhv.getH();
    d.mZ = rhv.getL() * sinq + rhv.getP() * cosq;

    v.mVx = rhv.getVl() * cosq - rhv.getVp() * sinq;
    v.mVy = rhv.getVh();
    v.mVz = rhv.getVl() * sinq + rhv.getVp() * cosq;

    return *this;
}



/****************************************************************************************
 *
 *                                  class MotionPointOfSpherical
 *
 ****************************************************************************************/

/*!
 * \class MotionPointOfSpherical
 * \brief Класс описывающий подвижною точку в сферической системе координат.
 *
 * Класс корректно работает с точками координатами которых находятся в следующих пределах:
 * \list
 * \li Угол места -90 < ... <= 90 градусов
 * \li Азимут 0 <= ... < 360 градусов
 * \li Дальность 0 <= ...
 * \endlist
 */
MotionPointOfSpherical::MotionPointOfSpherical(const double e, const double b, const double r,
                                               const double ve, const double vb, const double vr) :
    PointOfSpherical( e, b, r)
{
    v.mVe = ve;
    v.mVb = vb;
    v.mVr = vr;
    v.mMarker = cMarker;
}

/*!
 * \brief Конструктор инициализирующий параметры точки.
 * \param pos
 */
MotionPointOfSpherical::MotionPointOfSpherical( const PointOfSpherical &pos) :
PointOfSpherical(pos)
{
    v.mVe = v.mVb = v.mVr = 0;
    v.mMarker = cMarker;
}

/*!
 * \brief Инициализирующий конструктор
 * \param val
 */
MotionPointOfSpherical::MotionPointOfSpherical(const MotionPointOfOrtogonal &mpos) :
    PointOfSpherical( static_cast< const PointOfOrtogonal &>(mpos))
{
    *this = operator =( mpos);
    v.mMarker = cMarker;
}

/*!
 * \brief Преобразующий QVariant из конструктор.
 */
MotionPointOfSpherical::MotionPointOfSpherical(const QVariant &val)
{
    setQVariant(val);
    v.mMarker = cMarker;
}

/*!
 * \brief Помещает объект в контейнер QVariant
 * \return
 */
QVariant MotionPointOfSpherical::toQVariant() const
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.writeRawData((const char*)&d, sizeof(d));
    out.writeRawData((const char*)&v, sizeof(v));
    return data;
}

/*!
 * \brief Копирует значения с объекта упакованного в QVariant
 * \param val Ссылка на QVariant в который упакован объект MotionPointOfSpherical.
 * \return
 */
bool MotionPointOfSpherical::setQVariant(const QVariant &val)
{
    QByteArray dd = val.toByteArray();
    QDataStream inp(&dd, QIODevice::ReadOnly);
    if(inp.readRawData((char*)&d, sizeof(d)) == sizeof(d))
    {
        if(inp.readRawData((char*)&v, sizeof(v)) == sizeof(v))
        {
            if(v.mMarker == cMarker)
                return true;
            else
                qCritical()<<"MotionPointOfSpherical::setQVariant: Error! A received marker is "<<QString::number(v.mMarker,16) << " instead "<< QString::number(cMarker,16);
        }else
        {
            qCritical()<<"MotionPointOfSpherical::setQVariant: Error! Size of \"v\" data isn`t "<<sizeof(d);
        }
    }else
    {
        qCritical()<<"MotionPointOfSpherical::setQVariant: Error! Size of \"d\" data isn`t "<<sizeof(d);
    }
    clean();
    return false;
}

/*!
 * \brief Обнуляет все данные объекта этого класса.
 */
void MotionPointOfSpherical::clean()
{
    bzero((uchar*)&d, sizeof(d));
    bzero((uchar*)&v, sizeof(v));
    v.mMarker = cMarker;
}

/*!
 * \brief Выгрузка дынных этого класса в поток.
 * \param s ссылка на целевой поток.
 * \param p ссылка на выгружаемых объект этого класса.
 * \return
 */
QDataStream &operator<<(QDataStream &s, const MotionPointOfSpherical &p)
{
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.d))), sizeof(p.d)) ;
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.v))), sizeof(p.v)) ;
    return s;
}

/*!
 * \brief Загрузка данных этого класса из потока.
 * \param s ссылка на поток с входящими данными.
 * \param p ссылка на целевой объект этого класса.
 * \return
 */
QDataStream &operator>>(QDataStream &s, MotionPointOfSpherical &p)
{
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.d))), sizeof(p.d));
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.v))), sizeof(p.v));
    return s;
}

/*!
 * \brief Формирование отладочной информации об объекте этого класса.
 * \param dbg
 * \param p
 * \return
 */
QDebug operator<<(QDebug dbg, const MotionPointOfSpherical &p)
{
    dbg.nospace() << "MotionPointOfSpherical(E="<<p.d.mE/M_PI*180.<<" B="<<p.d.mB/M_PI*180.<<" R="<<p.d.mR<<" Ve="<<p.v.mVe/M_PI*180.<<" Vb="<<p.v.mVb/M_PI*180.<<" Vr="<<p.v.mVr<<")";
    return dbg.space();
}

/*!
 * \brief Получение модуля полной скорости
 * \return
 */
double MotionPointOfSpherical::getV() const
{
    return (MotionPointOfOrtogonal( *this).getV());
}

/*!
 * \brief Получение объекта класса PointOfSpherical с координатами.
 * \return PointOfSpherical с координатами.
 */
const PointOfSpherical MotionPointOfSpherical::getCoords() const
{
    PointOfSpherical c( d.mE, d.mB, d.mR);
    return c;
}

/*!
 * \brief Получение предсказанного состояния подвижной точки для указанного приращения времени.
 * \param dt Значение приращения времени в секундах.
 * \return Объект MotionPointOfSpherical
 */
MotionPointOfSpherical MotionPointOfSpherical::getPredicted(const double dt) const
{
    MotionPointOfSpherical ps( *this);

    ps.setE( ps.d.mE + ps.v.mVe * dt);
    ps.setB( ps.d.mB + ps.v.mVb * dt);
    ps.setR( ps.d.mR + ps.v.mVr * dt);

    return ps;
}

/*!
 * \fn void MotionPointOfSpherical::setVe( const double &val);
 * \brief Установка значения угловой скорости по углу места.
 * \param val Значение угловой скорости в рад/сек.
 */

/*!
 * \fn void MotionPointOfSpherical::setVb( const double &val);
 * \brief Установка значения угловой скорости по азимуту.
 * \param val Значение угловой скорости в рад/сек.
 */

/*!
 * \fn void MotionPointOfSpherical::setVr( const double &val);
 * \brief Установка значения радиальной скорости (по дальности)
 * \param val Значение линейной скорости в м/сек.
 */

/*!
 * \fn void MotionPointOfSpherical::setEBRVeVbVr(const double &e, const double &b, const double &r,const double &ve, const double &vb, const double &vr );
 * \brief Установка значений угловых координат и скоростей.
 * \param e
 * \param b
 * \param r
 * \param ve
 * \param vb
 * \param vr
 */

/*!
 * \brief Преобразующий оператор присваивания
 * \param rhv
 * \return
 */
MotionPointOfSpherical &MotionPointOfSpherical::operator =( const PointOfSpherical &rhv)
{
    d.mE = rhv.getE();
    d.mB = rhv.getB();
    d.mR = rhv.getR();
    v.mMarker = cMarker;
    return *this;
}


/*!
 * \brief MotionPointOfSpherical::operator =
 * \param orto
 * \return
 */
MotionPointOfSpherical &MotionPointOfSpherical::operator =(const MotionPointOfOrtogonal &rhv)
{
    *this = static_cast<PointOfOrtogonal>(rhv);
    v.mMarker = cMarker;
    if( d.mR == 0)
    {
        this->setEBRVeVbVr( 0., 0., 0., 0., 0., 0.);
        return *this;
    }

    v.mVr = (rhv.getX() * rhv.getVx() + rhv.getY() * rhv.getVy() + rhv.getZ() * rhv.getVz()) / d.mR;
    double rh( this->getRhor());
    if( rh == 0)
    {
        v.mVe = 0.;
        v.mVb = 0.;
        return *this;
    }
    v.mVb = this->safeAtan2((rhv.getVz() * rhv.getX() - rhv.getVx() * rhv.getZ()) / rh, rh);

    //v.mVe = this->safeAtan2( (rhv.getVy() - (v.mVr * rhv.getY() / d.mR)) * d.mR / rh , d.mR);
    v.mVe = this->safeAtan2((rhv.getVy() * d.mR - v.mVr * rhv.getY()) / rh , d.mR);

    return *this;
}

/*!
 * \brief Преобразующее присваивание.
 * \return
 */
MotionPointOfSpherical &MotionPointOfSpherical::operator =(const MotionPointOfParametrical &rhv)
{
    MotionPointOfOrtogonal po(rhv);
    this->operator =( po);
    v.mMarker = cMarker;
    return *this;
}

/*!
 * \brief Арктангенс с проверками на нулевые значения агрументов
 *
 *  Если хотя бы один из аргументов нулевой метод возвращает нуль.
 *
 * \param s длина противолежащего катета
 * \param c длина прилежащего катета.
 * \return Значение соответсвеющего угла в радианах.
 */
double MotionPointOfSpherical::safeAtan2(const double s, const double c)
{
    if( s == 0. || c == 0.) return 0.;
    return ::atan2(s, c);
}

/****************************************************************************************
 *
 *                                  class MotionPointOfParametric
 *
 ****************************************************************************************/


/*!
 * \class MotionPointOfParametric
 * \brief Класс описывающий подвижною точку в параметрической системе координат.
 *
 * Направление осей системы координат:
 * \list
 * \li Ось L- на цель.
 * \li Ось H - в верх.
 * \li Ось P - влево по курсу цели.
 * \endlist
 * Q - курс движения объекта.
 *
 * \note При равномерном прямолинейном полете скорость Vp равна нулю. И только в случае непрямолинейного
 * полета значение скорости изменения параметра не равно нулю.
 *
 *
 */


/*!
 * \brief Инициализирующий конструктор
 * \param l
 * \param h
 * \param p
 * \param q
 * \param vl
 * \param vp
 * \param vh
 */
MotionPointOfParametrical::MotionPointOfParametrical(const double l, const double h, const double p, const double q,
                                                     const double vl, const double vp, const double vh) :
    PointOfParametrical( l, h, p, q)
{
    v.mVl = vl;
    v.mVh = vh;
    v.mVp = vp;
    v.mMarker = cMarker;
}

/*!
 * \brief Преобразующий конструктор.
 * \param val
 */
MotionPointOfParametrical::MotionPointOfParametrical(const MotionPointOfOrtogonal &val)
{
    *this = operator =(val);
    v.mMarker = cMarker;
}

/*!
 * \brief Преобразующий конструктор.
 * \param val
 */
MotionPointOfParametrical::MotionPointOfParametrical(const MotionPointOfSpherical &val)
{
    this->operator =(val);
    v.mMarker = cMarker;
}

/*!
 * \brief Преобразующий QVariant из конструктор.
 */
MotionPointOfParametrical::MotionPointOfParametrical(const QVariant &val)
{
    setQVariant(val);
    v.mMarker = cMarker;
}

/*!
 * \brief Помещает объект в контейнер QVariant
 * \return
 */
QVariant MotionPointOfParametrical::toQVariant() const
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.writeRawData((const char*)&d, sizeof(d));
    out.writeRawData((const char*)&v, sizeof(v));
    return data;
}

/*!
 * \brief Копирует значения с объекта упакованного в QVariant
 * \param val Ссылка на QVariant в который упакован объект PointOfOrtogonal.
 * \return
 */
bool MotionPointOfParametrical::setQVariant(const QVariant &val)
{
    QByteArray dd = val.toByteArray();
    QDataStream inp(&dd, QIODevice::ReadOnly);
    if(inp.readRawData((char*)&d, sizeof(d)) == sizeof(d))
    {
        if(inp.readRawData((char*)&v, sizeof(v)) == sizeof(v))
        {
            if(v.mMarker == cMarker)
                return true;
            else
                qCritical()<<"MotionPointOfParametrical::setQVariant: Error! A received marker is "<<QString::number(v.mMarker,16) << " instead "<< QString::number(cMarker,16);
        }else
        {
            qCritical()<<"MotionPointOfParametrical::setQVariant: Error! Size of \"v\" data isn`t "<<sizeof(d);
        }
    }else
    {
        qCritical()<<"MotionPointOfParametrical::setQVariant: Error! Size of \"d\" data isn`t "<<sizeof(d);
    }
    clean();
    return false;
}

/*!
 * \brief Обнуляет все данные объекта этого класса.
 */
void MotionPointOfParametrical::clean()
{
    bzero((uchar*)&d, sizeof(d));
    bzero((uchar*)&v, sizeof(v));
    v.mMarker = cMarker;
}

/*!
 * \brief Выгрузка дынных этого класса в поток.
 * \param s ссылка на целевой поток.
 * \param p ссылка на выгружаемых объект этого класса.
 * \return
 */
QDataStream &operator<<(QDataStream &s, const MotionPointOfParametrical &p)
{
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.d))), sizeof(p.d)) ;
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.v))), sizeof(p.v)) ;
    return s;
}

/*!
 * \brief Загрузка данных этого класса из потока.
 * \param s ссылка на поток с входящими данными.
 * \param p ссылка на целевой объект этого класса.
 * \return
 */
QDataStream &operator>>(QDataStream &s, MotionPointOfParametrical &p)
{
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.d))), sizeof(p.d));
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.v))), sizeof(p.v));
    return s;
}

/*!
 * \brief Формирование отладочной информации об объекте этого класса.
 * \param dbg
 * \param p
 * \return
 */
QDebug operator<<(QDebug dbg, const MotionPointOfParametrical &p)
{
    dbg.nospace() << "MotionPointOfParametrical(L="<<p.d.mL<<" H="<<p.d.mH<<" P="<<p.d.mP<<" Q="<<p.d.mQ/M_PI*180.
                  <<" RB=" << p.getRelativeBearing()/M_PI*180. <<" Vl="<<p.v.mVl<<" Vh="<<p.v.mVh<<" Vp="<<p.v.mVp<<")";
    return dbg.space();
}

/*!
 * \fn double getPitch();
 * \brief Получение угла тангажа.
 * \return Угол тангажа в радианах.
 */

/*!
 * \brief Получение параметрических координат.
 * \return Объект класса PointOfParametrical
 */
const PointOfParametrical MotionPointOfParametrical::getCoords() const
{
    PointOfParametrical pp( *this);
    return pp;
}

/*!
 * \brief Получение скоростей в виде объекта класса PointOfParametrical.
 *
 * Кроме значений скоростей заполняется и значение курса.
 * \return
 */
const PointOfParametrical MotionPointOfParametrical::getVelocity() const
{
    PointOfParametrical pp;
    pp.setLHPQ( v.mVl, v.mVh, v.mVp, d.mQ);
    return pp;
}

/*!
 * \brief Получение пркдсказанного состояния подвижной точки для указанного приращения времени.
 * \param dt Значение приращения времени в секундах.
 * \return
 */
MotionPointOfParametrical MotionPointOfParametrical::getPredicted(const double dt) const
{
    MotionPointOfParametrical pp( *this);
    pp.d.mL += pp.v.mVl * dt;
    pp.d.mH += pp.v.mVh * dt;
    pp.d.mP += pp.v.mVp * dt;

    return pp;
}

/*!
 * \brief Преобразующий оператор присваивания
 * \param rhv
 * \return
 */
MotionPointOfParametrical &MotionPointOfParametrical::operator =( const PointOfParametrical &rhv)
{
    d.mL = rhv.getL();
    d.mH = rhv.getH();
    d.mP = rhv.getP();
    d.mQ = rhv.getQ();
    v.mMarker = cMarker;

    return *this;
}


/*!
 * \brief Оператор присваивания преобразующий объект PointOfOrtogonal к PointOfParametrical
 * \return
 */
MotionPointOfParametrical &MotionPointOfParametrical::operator =(const MotionPointOfOrtogonal &rhv)
{
    v.mMarker = cMarker;

    double vh( rhv.getVhor());
    //if( vh < 1.)
    if (vh == 0.) { // Точка не имеет горизонтальной скорости.
        d.mL = sqrt( rhv.getX() * rhv.getX() + rhv.getZ() * rhv.getX());
        d.mH = rhv.getY();
        d.mP = 0.;
        this->setQ( ::atan2( rhv.getZ(), rhv.getX()) - M_PI); // Считаю, что неподвижная точка "летит" на нас.

        v.mVl = 0.;
        //v.mVh = (fabs(rhv.getVy()) < 1.) ? 0. : rhv.getVy();
        v.mVh = rhv.getVy();
        v.mVp = 0.;
    } else {   // Точка имеет горизонтальную скорость.
        double sinq = rhv.getVz() / vh;
        double cosq = -1. * rhv.getVx() / vh;
        d.mL = rhv.getX() * cosq - rhv.getZ() * sinq;
        d.mH = rhv.getY();
        d.mP = rhv.getX() * sinq + rhv.getZ() * cosq;

        this->setQ( ::atan2( rhv.getVz(), rhv.getVx()));

        v.mVl = rhv.getVx() * cosq - rhv.getVz() * sinq;
        v.mVh = rhv.getVy();
        v.mVp = rhv.getVx() * sinq + rhv.getVz() * cosq;
    }

    return *this;
}

/*!
 * \brief Преобразующий оператор присваивания.
 * \param rhv
 * \return
 */
MotionPointOfParametrical &MotionPointOfParametrical::operator =(const MotionPointOfSpherical &rhv)
{
    MotionPointOfOrtogonal po( rhv);
    this->operator =(po);
    v.mMarker = cMarker;

    return *this;
}

