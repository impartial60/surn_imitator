#include <QDebug>
#include <QByteArray>
#include <string.h>
#include "pointsofspace.h"

//=======================================================================================
//
// Координаты точки в земных прямоугольных координатах
//
//=======================================================================================

/*!
 * \brief Конструктор инициализирующий объект значениями координат.
 * \param x
 * \param y
 * \param z
 */
PointOfOrtogonal::PointOfOrtogonal(const double x, const double y, const double z)
{
    d.mMarker = cMarker;
    d.mX = x;
    d.mY = y;
    d.mZ = z;
}

/*!
 * \brief Конструктор копирующий по ссылке.
 */
PointOfOrtogonal::PointOfOrtogonal(const PointOfOrtogonal &val)
{
    *this = val;
    d.mMarker = cMarker;
}

/*!
 * \brief Конструктор
 */
PointOfOrtogonal::PointOfOrtogonal(const PointOfSpherical &val)
{
    *this = operator=(val);
    d.mMarker = cMarker;
}

/*!
 * \brief Конструктор преобразующий PointOfParametrical
 * \param val
 */
PointOfOrtogonal::PointOfOrtogonal(const PointOfParametrical &val)
{
    *this = operator =(val);
    d.mMarker = cMarker;
}

/*!
 * \brief Преобразующий QVariant из конструктор.
 */
PointOfOrtogonal::PointOfOrtogonal(const QVariant &val)
{
    setQVariant(val);
}

/*!
 * \brief Помещает объект в контейнер QVariant
 * \return
 */
QVariant PointOfOrtogonal::toQVariant() const
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.writeRawData((const char*)&d, sizeof(d));
    return data;
}

/*!
 * \brief Копирует значения с объекта упакованного в QVariant
 * \param val Ссылка на QVariant в который упакован объект PointOfOrtogonal.
 * \return
 */
bool PointOfOrtogonal::setQVariant(const QVariant &val)
{
    QByteArray dd = val.toByteArray();
    QDataStream inp(&dd, QIODevice::ReadOnly);
    if(inp.readRawData((char*)&d, sizeof(d)) == sizeof(d))
    {
        if(d.mMarker == cMarker)
            return true;
        else
            qCritical()<<"PointOfOrtogonal::setQVariant: Error! A received marker is "<<QString::number(d.mMarker,16) << " instead "<< QString::number(cMarker,16);
    }else
    {
        qCritical()<<"PointOfOrtogonal::setQVariant: Error! Size of data isn`t "<<sizeof(d);
    }
    clean();
    return false;
}

/*!
 * \brief Обнуляет все данные объекта этого класса.
 */
void PointOfOrtogonal::clean()
{
    bzero((uchar*)&d, sizeof(d));
    d.mMarker = cMarker;
}

/*!
 * \brief Выгрузка дынных этого класса в поток.
 * \param s ссылка на целевой поток.
 * \param p ссылка на выгружаемых объект этого класса.
 * \return
 */
QDataStream &operator<<(QDataStream &s, const PointOfOrtogonal &p)
{
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.d))), sizeof(p.d)) ;
    return s;
}

/*!
 * \brief Загрузка данных этого класса из потока.
 * \param s ссылка на поток с входящими данными.
 * \param p ссылка на целевой объект этого класса.
 * \return
 */
QDataStream &operator>>(QDataStream &s, PointOfOrtogonal &p)
{
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.d))), sizeof(p.d));
    return s;
}

/*!
 * \brief Формирование отладочной информации об объекте этого класса.
 * \param dbg
 * \param p
 * \return
 */
QDebug operator<<(QDebug dbg, const PointOfOrtogonal &p)
{
    dbg.nospace() << "PointOfOrtogonal(X="<<p.d.mX<<" Y="<<p.d.mY<<" Z="<<p.d.mZ<<")";
    return dbg.space();
}

/*!
 * \brief Вычисление расторяния до указанной точки
 * \param po ссылка на объект класса PointOfOrtogonal описывающий точку до которой нужно определить расстояние.
 * \return Расстояние до указанной точки в метрах.
 */
double PointOfOrtogonal::getDistanceTo(const PointOfOrtogonal &po) const
{
    PointOfOrtogonal lpo( *this - po);
    return( ::sqrt( lpo.getX() * lpo.getX() + lpo.getY() * lpo.getY() + lpo.getZ() * lpo.getZ()));
}

/*!
 * \brief Выполняет перенос точки из прямоуголной в параметрическую систему координат
 * \param course Значения курса для параметрической системы координат
 * \return Точку пересчитанную в парамтерическую систему координат.
 *
 * Выполняет поворот координат точки в параметрическую сиситему построенную для указанного
 * значения курса. Начала координат прямоугольной и параметрической систем совпадают.
 */
const PointOfParametrical PointOfOrtogonal::toParametrical(const double course) const
{
    return PointOfParametrical(-d.mX * cos(course) - d.mZ * sin(course),
                                d.mY,
                                d.mX * sin(course) - d.mZ * cos(course),
                                course);
}

/*!
 * \brief Выполняет перенос точки заданой объектом PointOfOrtogonal и курсом для парамтерической СК.
 * \param course Значение курса для парметрической СК.
 * \return Точку пересчитанную в парамтерическую систему координат.
 */
const PointOfParametrical PointOfOrtogonal::toParametrical(const PointOfOrtogonal &point, const double course)
{
  return point.toParametrical( course);
}

/**
 * @brief ператор сравнения.
 * @param p
 * @return
 */
bool PointOfOrtogonal::operator == (const PointOfOrtogonal &p) const
{
    return (p.d.mX == d.mX) && (p.d.mY == d.mY) && (p.d.mZ == d.mZ);
}

/*!
 * \brief Оператор умножения на double
 * \param lhv
 * \param rhv
 * \return
 */
const PointOfOrtogonal PointOfOrtogonal::operator*( const double rhv) const
{
    PointOfOrtogonal result( *this);
    result.d.mX *= rhv;
    result.d.mY *= rhv;
    result.d.mZ *= rhv;
    return result;
}

/*!
 * \brief Оператор сложения кординат  двух точек.
 * \param lhv
 * \param rhv
 * \return
 */
const PointOfOrtogonal PointOfOrtogonal::operator+( const PointOfOrtogonal &rhv) const
{
    PointOfOrtogonal result( *this);
    result.d.mX += rhv.d.mX;
    result.d.mY += rhv.d.mY;
    result.d.mZ += rhv.d.mZ;
    return result;
}

/*!
 * \brief Оператор вычитания координат двух точек.
 * \param lhv
 * \param rhv
 * \return
 */
const PointOfOrtogonal PointOfOrtogonal::operator-( const PointOfOrtogonal &rhv) const
{
    PointOfOrtogonal result( *this);
    result.d.mX -= rhv.d.mX;
    result.d.mY -= rhv.d.mY;
    result.d.mZ -= rhv.d.mZ;
    return result;
}

/*!
 * \brief Кросклассовый оператор присваивания
 *
 * Преобразует объект класса PointOfSpherical в объект класса PointOfOrtogonal.
 * \return Указатель на себя
 */
PointOfOrtogonal &PointOfOrtogonal::operator =(const PointOfSpherical &rhv)
{
    double rg;
    rg = rhv.getRhor();
    d.mX = rg * cos( rhv.getB());
    d.mY = rhv.getR() * sin( rhv.getE());
    d.mZ = rg * sin( rhv.getB());
    return *this;
}

/*!
 * \brief Преобразует объект класса PointOfParametrical в объект класса PointOfOrtogonal
 * \param val
 * \return
 */
PointOfOrtogonal &PointOfOrtogonal::operator =(const PointOfParametrical &rhv)
{
    double q( rhv.getQ());
    q *= -1.;
    double sinq = sin(q);
    double cosq = cos(q);
    d.mX = rhv.getL() * cosq - rhv.getP() * sinq;
    d.mY = rhv.getH();
    d.mZ = rhv.getL() * sinq + rhv.getP() * cosq;
    return *this;
}

//--------------------------------------------------------------------------------------
PointOfOrtogonal & PointOfOrtogonal::operator+=( const PointOfOrtogonal &rhv)
{
    d.mX += rhv.d.mX;
    d.mY += rhv.d.mY;
    d.mZ += rhv.d.mZ;
    return *this;
}

//--------------------------------------------------------------------------------------
PointOfOrtogonal & PointOfOrtogonal::operator-=( const PointOfOrtogonal &rhv)
{
    d.mX -= rhv.d.mX;
    d.mY -= rhv.d.mY;
    d.mZ -= rhv.d.mZ;
    return *this;
}

//--------------------------------------------------------------------------------------
PointOfOrtogonal & PointOfOrtogonal::operator+=( const PointOfSpherical &rhv)
{
    double y  = rhv.getR() * sin( rhv.getE());
    double rg = rhv.getRhor();
    double x  = rg * cos( rhv.getB());
    double z  = rg * sin( rhv.getB());
    d.mX += x;
    d.mY += y;
    d.mZ += z;
    return *this;
}

/*!
 * \brief PointOfOrtogonal::operator =
 * \param rhv
 * \return
 */
PointOfOrtogonal &PointOfOrtogonal::operator =(const PointOfOrtogonal &rhv)
{
    this->d = rhv.d;
    return *this;
}


//=======================================================================================
//
// Координаты точки в сферических корординатах
//
//=======================================================================================
PointOfSpherical::PointOfSpherical( const double e, const double b, const double r)
{
    setEBR(e, b, r);
    d.mMarker = cMarker;
}

PointOfSpherical::PointOfSpherical(const PointOfSpherical &val)
{
    *this = val;
    d.mMarker = cMarker;
}

PointOfSpherical::PointOfSpherical(const PointOfOrtogonal &val)
{
    *this = operator =(val);
    d.mMarker = cMarker;
}
/*!
 * \brief Преобразующий QVariant из конструктор.
 */
PointOfSpherical::PointOfSpherical(const QVariant &val)
{
    setQVariant(val);
    d.mMarker = cMarker;
}

/*!
 * \brief Помещает объект в контейнер QVariant
 * \return
 */
QVariant PointOfSpherical::toQVariant() const
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.writeRawData((const char*)&d, sizeof(d));
    return data;
}

/*!
 * \brief Копирует значения с объекта упакованного в QVariant
 * \param val Ссылка на QVariant в который упакован объект PointOfOrtogonal.
 * \return
 */
bool PointOfSpherical::setQVariant(const QVariant &val)
{
    QByteArray dd = val.toByteArray();
    QDataStream inp(&dd, QIODevice::ReadOnly);
    if(inp.readRawData((char*)&d, sizeof(d)) == sizeof(d))
    {
        if(d.mMarker == cMarker)
            return true;
        else
            qCritical()<<"PointOfSpherical::setQVariant: Error! A received marker is "<<QString::number(d.mMarker,16) << " instead "<< QString::number(cMarker,16);
    }else
    {
        qCritical()<<"PointOfSpherical::setQVariant: Error! Size of data isn`t "<<sizeof(d);
    }
    clean();
    return false;
}

/*!
 * \brief Обнуляет все данные объекта этого класса.
 */
void PointOfSpherical::clean()
{
    bzero((uchar*)&d, sizeof(d));
    d.mMarker = cMarker;
}

/*!
 * \brief Выгрузка дынных этого класса в поток.
 * \param s ссылка на целевой поток.
 * \param p ссылка на выгружаемых объект этого класса.
 * \return
 */
QDataStream &operator<<(QDataStream &s, const PointOfSpherical &p)
{
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.d))), sizeof(p.d)) ;
    return s;
}

/*!
 * \brief Загрузка данных этого класса из потока.
 * \param s ссылка на поток с входящими данными.
 * \param p ссылка на целевой объект этого класса.
 * \return
 */
QDataStream &operator>>(QDataStream &s, PointOfSpherical &p)
{
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.d))), sizeof(p.d));
    return s;
}

/*!
 * \brief Формирование отладочной информации об объекте этого класса.
 *
 * \note Значения углов выводятся в градусах.
 *
 *
 * \param dbg
 * \param p
 * \return
 */
QDebug operator<<(QDebug dbg, const PointOfSpherical &p)
{
    dbg.nospace() << "PointOfSpherical(e="<<p.d.mE/M_PI*180.<<" B="<<p.d.mB/M_PI*180.<<" R="<<p.d.mR<<")";
    return dbg.space();
}

/*!
 * \fn double PointOfSpherical::getDistanceTo( const PointOfSpherical &p);
 * \brief Вычисление расторяния до указанной точки.
 * \param p ссылка на объект класса PointOfSpherical описывающий точку до которой нужно определить расстояние.
 * \return Расстояние до указанной точки в метрах.
 */

/*!
 * \brief Установка значение координат точки.
 * \param e
 * \param b
 * \param r
 */
void PointOfSpherical::setEBR( double e, double b, double r)
{
    this->setE(e);
    this->setB(b);
    this->setR(r);
}

/*!
 * \brief Установка значение угла места, с приведением к допустимым значениям
 *
 * Допустимые значения угламеста +- 90 градусов.
 * \param val
 */
void PointOfSpherical::setE(const double val)
{
    double a(val);
    while( a > M_PI_2) a -= M_PI;
    while( a < -1.*M_PI_2) a += M_PI;
    d.mE = a;
}

/*!
 * Установка значения азимута с приведением к допустым значениям.
 * \param val
 */
void PointOfSpherical::setB(const double val)
{
    double a( val);
    while( a < 0.) a += 2.* M_PI;
    while( a >= 2. * M_PI) a -= 2.* M_PI;
    d.mB = a;
}


bool PointOfSpherical::operator == (const PointOfSpherical &p) const
{
    return (p.d.mE == d.mE) && (p.d.mB == d.mB) && (p.d.mR == d.mR);
}


const PointOfSpherical PointOfSpherical::operator+( const PointOfSpherical &rhv) const
{
    PointOfSpherical result( *this);
    result.setE( d.mE + rhv.d.mE);
    result.setB( d.mB + rhv.d.mB);
    result.setR( d.mR + rhv.d.mR);
    return result;
}

const PointOfSpherical PointOfSpherical::operator-( const PointOfSpherical &rhv) const
{
    PointOfSpherical result( *this);
    result.setE( d.mE - rhv.d.mE);
    result.setB( d.mB - rhv.d.mB);
    result.setR( d.mR - rhv.d.mR);
    return result;
}

PointOfSpherical & PointOfSpherical::operator+=( const PointOfSpherical &rhv)
{
    this->setE( d.mE + rhv.d.mE);
    this->setB( d.mB + rhv.d.mB);
    this->setR( d.mR + rhv.d.mR);
    return *this;
}

PointOfSpherical & PointOfSpherical::operator-=( const PointOfSpherical &rhv)
{
    this->setE( d.mE - rhv.d.mE);
    this->setB( d.mB - rhv.d.mB);
    this->setR( d.mR - rhv.d.mR);
    return *this;
}


PointOfSpherical & PointOfSpherical::operator=( const PointOfSpherical &rhv)
{
    d.mE = rhv.d.mE;
    d.mB = rhv.d.mB;
    d.mR = rhv.d.mR;
    return *this;
}

PointOfSpherical & PointOfSpherical::operator=( const PointOfOrtogonal &rhv)
{
    d.mR = d.mB = d.mE = 0.;
    double rg = rhv.d.mX * rhv.d.mX + rhv.d.mZ * rhv.d.mZ;
    double r  = sqrt( rg + rhv.d.mY * rhv.d.mY);
    rg = sqrt(rg);
    if( rg == 0. || r == 0.) return *this;
    this->setR(r);
    this->setE(::atan2( rhv.getY(), rg));
    this->setB(::atan2( rhv.getZ(), rhv.getX()));
    return *this;
}

//=======================================================================================
//
// Координаты точки в параметрических корординатах
//
//=======================================================================================

/*!
 * \class PointOfParametrical
 * \brief Класс описывающий точку в параметрической системе координат.
 *
 * Направление осей системы координат:
 * \list
 * \li Ось L- на цель.
 * \li Ось H - в верх.
 * \li Ось P - влево по курсу цели.
 * \endlist
 * Другие параметры:\b
 * Q - курс движения объекта. Изменяется в пределах 0 <= ... < 360 градусов.\b
 * RelativeBearing -курсовой угол движения цели (угол между направлением на станцию и курсом цели).
 * Изменяется в пределах -180...+180 градусов.
 * Курсовой угол движения можна рассматривать как ракурс цели. Для курсового угла 0 цель летит на нас
 * и мы видим ее нос. Если курсовой угол 180 градусов то цель удаляется от нас и мы видим ее "попу"
 *
 */


/*!
 * \brief Инициализирующий конструктор
 * \param l
 * \param h
 * \param p
 * \param q
 */
PointOfParametrical::PointOfParametrical(const double &l, const double &h, const double &p, const double &q)
{
    d.mL = l;
    d.mH = h;
    d.mP = p;
    d.mQ = q;
    d.mMarker = cMarker;
}

/*!
 * \brief Копирующий конструктор
 */
PointOfParametrical::PointOfParametrical(const PointOfParametrical &val)
{
    *this = val;
    d.mMarker = cMarker;
}

/*!
 * \brief Кроссклассовый преобразующий конструктор
 */
PointOfParametrical::PointOfParametrical(const PointOfOrtogonal &val)
{
    *this = operator =(val);
    d.mMarker = cMarker;
}

/*!
 * \brief Преобразующий QVariant из конструктор.
 */
PointOfParametrical::PointOfParametrical(const QVariant &val)
{
    setQVariant(val);
}

/*!
 * \brief Помещает объект в контейнер QVariant
 * \return
 */
QVariant PointOfParametrical::toQVariant() const
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.writeRawData((const char*)&d, sizeof(d));
    return data;
}

/*!
 * \brief Копирует значения с объекта упакованного в QVariant
 * \param val Ссылка на QVariant в который упакован объект PointOfOrtogonal.
 * \return
 */
bool PointOfParametrical::setQVariant(const QVariant &val)
{
    QByteArray dd = val.toByteArray();
    QDataStream inp(&dd, QIODevice::ReadOnly);
    if(inp.readRawData((char*)&d, sizeof(d)) == sizeof(d))
    {
        if(d.mMarker == cMarker)
            return true;
        else
            qCritical()<<"PointOfParametrical::setQVariant: Error! A received marker is "<<QString::number(d.mMarker,16) << " instead "<< QString::number(cMarker,16);
    }else
    {
        qCritical()<<"PointOfParametrical::setQVariant: Error! Size of data isn`t "<<sizeof(d);
    }
    clean();
    return false;
}

/*!
 * \brief Обнуляет все данные объекта этого класса.
 */
void PointOfParametrical::clean()
{
    bzero((uchar*)&d, sizeof(d));
    d.mMarker = cMarker;
}

/*!
 * \brief Выгрузка дынных этого класса в поток.
 * \param s ссылка на целевой поток.
 * \param p ссылка на выгружаемых объект этого класса.
 * \return
 */
QDataStream &operator<<(QDataStream &s, const PointOfParametrical &p)
{
    s.writeRawData(static_cast<const char*>(static_cast<const void*>(&(p.d))), sizeof(p.d)) ;
    return s;
}

/*!
 * \brief Загрузка данных этого класса из потока.
 * \param s ссылка на поток с входящими данными.
 * \param p ссылка на целевой объект этого класса.
 * \return
 */
QDataStream &operator>>(QDataStream &s, PointOfParametrical &p)
{
    s.readRawData(static_cast<char*>(static_cast<void*>(&(p.d))), sizeof(p.d));
    return s;
}

/*!
 * \brief Формирование отладочной информации об объекте этого класса.
 *
 * \note Значение курса выводится в градусах.
 *
 *
 * \param dbg
 * \param p
 * \return
 */
QDebug operator<<(QDebug dbg, const PointOfParametrical &p)
{
    dbg.nospace() << "PointOfParametrical(L="<<p.d.mL<<" H="<<p.d.mH<<" P="<<p.d.mP<<" Q="<<p.d.mQ/M_PI*180.<<")";
    return dbg.space();
}

/*!
 * \fn void PointOfParametrical::setLHPQ(const double &l, const double &h, const double &p, const double &q);
 * \brief Установка парамтричесих координат и курса
 * \param l
 * \param h
 * \param p
 * \param q
 */

/*!
 * \brief Метод устанавливающий значение курса
 * \param val Значение курса, в радианах.
 */
void PointOfParametrical::setQ(const double val)
{
    double q(val);
    while( q >= 2. * M_PI) q -= 2. * M_PI;
    while( q <  0.)        q += 2. * M_PI;
    d.mQ = q;
}

bool PointOfParametrical::operator == (const PointOfParametrical &p) const
{
    return (p.d.mL == d.mL) && (p.d.mH == d.mH) && (p.d.mP == d.mP) && (p.d.mQ == d.mQ);
}


/*!
 * \brief PointOfParametrical::operator +
 * \param rhv
 * \return
 */
const PointOfParametrical PointOfParametrical::operator +(const PointOfParametrical &rhv) const
{
    PointOfParametrical result(*this);
    result.d.mL += rhv.d.mL;
    result.d.mH += rhv.d.mH;
    result.d.mP += rhv.d.mP;
    result.setQ( this->d.mQ + rhv.d.mQ);
    return result;
}

/*!
 * \brief PointOfParametrical::operator -
 * \param rhv
 * \return
 */
const PointOfParametrical PointOfParametrical::operator -(const PointOfParametrical &rhv) const
{
    PointOfParametrical result(*this);
    result.d.mL -= rhv.d.mL;
    result.d.mH -= rhv.d.mH;
    result.d.mP -= rhv.d.mP;
    result.setQ( this->d.mQ - rhv.d.mQ);
    return result;
}

/*!
 * \brief PointOfParametrical::operator *
 * \param rhv
 * \return
 */
const PointOfParametrical PointOfParametrical::operator *(const double rhv) const
{
    PointOfParametrical result(*this);
    result.d.mL *= rhv;
    result.d.mH *= rhv;
    result.d.mP *= rhv;
    //result.d.mQ *= rhv;   Курс не изменется.
    return result;
}

/*!
 * \brief PointOfParametrical::operator +=
 * \param rhv
 * \return
 */
PointOfParametrical &PointOfParametrical::operator +=(const PointOfParametrical &rhv)
{
    this->d.mL += rhv.d.mL;
    this->d.mH += rhv.d.mH;
    this->d.mP += rhv.d.mP;
    this->setQ( this->d.mQ + rhv.d.mQ);
    return *(this);
}

/*!
 * \brief PointOfParametrical::operator -=
 * \param rhv
 * \return
 */
PointOfParametrical &PointOfParametrical::operator -=(const PointOfParametrical &rhv)
{
    this->d.mL -= rhv.d.mL;
    this->d.mH -= rhv.d.mH;
    this->d.mP -= rhv.d.mP;
    this->setQ( this->d.mQ - rhv.d.mQ);
    return *(this);
}

/*!
 * \brief PointOfParametrical::operator =
 * \param rhv
 * \return
 */
PointOfParametrical &PointOfParametrical::operator =(const PointOfParametrical &rhv)
{
    this->d = rhv.d;
    return *this;
}

/*!
 * \brief Оператор присваивания преобразующий координаты точки из прямоугольных в парамтерические координаты.
 *
 * При преобразованиив качестве курса берется направление из точки в центр координат.
 * \param rhv Объект класса PointOfOrtogonal.
 * \return
 */
PointOfParametrical &PointOfParametrical::operator =(const PointOfOrtogonal &rhv)
{
    double rh( sqrt( rhv.getX() * rhv.getX() + rhv.getZ() * rhv.getZ()));
    if( rh == 0.)
    {
        d.mL = 0.;
        d.mH = rhv.getY();
        d.mP = 0.;
        d.mQ = 0.;
        return *this;
    }

    d.mL = rh;
    d.mH = rhv.getY();
    d.mP = 0;
    this->setQ( ::atan2( rhv.getZ(), rhv.getX()) - M_PI); // Считаю, что неподвижная точка "лентит" на нас.
    return *this;
}

/*!
 * \brief Вычисление расторяния до указанной точки
 * \param p ссылка на объект класса PointOfParametrical описывающий точку до которой нужно определить расстояние.
 * \return Расстояние до указанной точки в метрах.
 */
double PointOfParametrical::getDistanceTo(const PointOfParametrical &p) const
{
    PointOfParametrical lp( *this - p);
    return( ::sqrt( lp.getL() * lp.getL() + lp.getH() * lp.getH() + lp.getP() * lp.getP()));
}
