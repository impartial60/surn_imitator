#ifndef FORM_MNEMOSHEM_H
#define FORM_MNEMOSHEM_H
#include <QMainWindow>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkAddressEntry>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>
#include <QNetworkSession>
#include <QColor>
#include <QPainter>
#include <QPaintEngine>
#include <QFile>

//#include <QtCharts/QChartView>
//#include <QtCharts/QPolarChart>

#include "posemath/posemath.h"
#include "base/motionpointsofspase.h"
#include "base/clockrealtime.h"
#include "base/converter.h"
//#include "QCustomplot/qcustomplot.h"
#include "src/tp.h"
//#include "confsrv.h"
//#include "../cubeparsrv/parsrv.h"
#define radius_earth8 6377649.96584

#define START_PACK _Pragma ("pack(push, 1)")
#define END_PACK   _Pragma ("pack(pop)")

#define N_ID_enc_new        0x3F // (63) 	При наладке адрес энкодера по умолчанию
#define N_ID_incl           0x3E // (62) 	Адрес инклинометра (при запуске нового и в процессе работы одинаковый)
#define N_ID_enc_azimuth    0x3D // (61)	Энкодер азимута.
#define N_ID_enc_elv        0x3C // (60) 	Энкодер угла места
#define N_ID_lenze_az       0x3B // (59) 	Привод азимута.
#define N_ID_lenze_elv      0x3A // (58)	Привод угла места.
#define N_ID_PLC            0x39 // (57)	PLC Schneider.




#define port_125 50000

#define max_x   1000
#define min_x   -20

#define max_y  360
#define min_y   -10


#define cmax_vel_az  15.0*10800
#define cmax_acc_az  8.0*10800
#define cmax_vel_um  15.0*10800
#define cmax_acc_um  8.0*10800

#define debug_app
//#define local_server
//#define test_no_p //test without transform coordinates

#define main_clock_works 20
#define time_outs 10

#define grad_to_radians(n) (M_PI/180.0)*n
#define radians_to_grad(n) (180.0/M_PI)*n

namespace Ui {
class Form_mnemoshem;
}

class Form_mnemoshem : public QMainWindow
{
    Q_OBJECT

public:

    explicit Form_mnemoshem(QWidget *parent = 0);
    ~Form_mnemoshem();

    bool get_state_imitator();
    bool get_No_pusk();
    bool get_Test_mode();
    bool get_Target();
    bool get_Coord_TK();
    bool get_Clog_1C31M1();
    bool get_Link();
    bool get_Start();
    bool get_Mode_II_in();

    double get_epsilon_target();  //угол места цели, измеряемый СН
    double get_vel_epsilon_target();  //vel угол места целi

    double get_beta_target();     //угол азимута цели
    double get_vel_beta_target();     //vel угол азимута цели

    double get_eps_target();

    double get_distance_target();
    double get_velocity_target();

    void set_epsilon_target(double v);  //угол места цели, измеряемый СН
    void set_vel_epsilon_target(double v);  //vel угол места целi


    void set_beta_target(double v);     //угол азимута цели
    void set_vel_beta_target(double v);     //vel угол азимута цели

    void set_distance_target(double v);
    void set_velocity_target(double v);

    void target_motion(double time);

// ----------------------------------------
    double get_DoG_imt();     //дальность
    double get_beta_imt();     //угол азимута цели
    double get_Ho_imt();
    double get_Vo_imt();
    double get_So_imt();
    double get_To_imt();
//-----------------------------------------
    inline double get_encoder_az(int az) {return (double)az/10800.0;}
    inline double get_encoder_um(int um) {return (double)um/10800.0;}

    inline double get_resolver_az(int az) {return (double)az/10000.0;}
    inline double get_resolver_um(int um) {return (double)um/10000.0;}

    inline double get_integrator_az(int irg) {return (double)irg/10000.0;}
    inline double get_integrator_um(int irg) {return (double)irg/10000.0;}
 //--------------------------------------------------------------------
    inline double get_encoder_az_p(int az) {return ((360.0/65536.0)*(double)(az&0x0000ffff));}
    inline double get_encoder_um_p(int um) {return ((360.0/65536.0)*(double)(um&0x0000ffff));}

    inline double get_resolver_az_p(int az) {return (double)az/10000.0;}
    inline double get_resolver_um_p(int um) {return (double)um/10000.0;}
//------------------------05.06.2018 for test
    inline double get_resolver_um_ptst(double um,double mum)
    {
        if(um-mum < 0.0)
            return mum-360.0;
        else
        return mum;}
//-----------------------------------------------------------

    inline double get_integrator_az_p(int irg) {return (double)irg/10000.0;}
    inline double get_integrator_um_p(int irg) {return (double)irg/10000.0;}


    inline int32_t double_to_lenze(double n){return ((int)(n*10000));}
    inline int32_t double_to_lenze_eps(double n){return ((int)(n*10000));}

    inline int32_t double_to_lenze_p(double n){return ((int)(n*10000));}
    inline int32_t double_to_lenze_eps_p(double n){return ((int)(n*10000));}

    double polynom(double x) {return
            (((x*x*x)*4.51561467696784e-7)-((x*x)*0.000198086819613)+(x*0.02827381765438)-0.040144620068631);}

    inline int32_t double_to_lenze_eps_p_poly( double n) {return ((int)((n-polynom(n))*10000.0));}


    inline double get_ortogonal_X() {return p_ortogonal->getX();}
    inline double get_ortogonal_Z() {return p_ortogonal->getZ();}
    inline double get_ortogonal_Y() {return p_ortogonal->getY();}

    inline double get_ortogonal_vX() {return p_ortogonal->getVx();}
    inline double get_ortogonal_vZ() {return p_ortogonal->getVz();}
    inline double get_ortogonal_vY() {return p_ortogonal->getVy();}

    double coord_tr(double deg,double min,double sec) {return (deg*3600+min*60+sec)/3600;}
    /* static */
    double distanceBetween(double lat1, double long1, double lat2, double long2)
    {
      // returns distance in meters between two positions, both specified
      // as signed decimal-degrees latitude and longitude. Uses great-circle
      // distance computation for hypothetical sphere of radius 6372795 meters.
      // Because Earth is no exact sphere, rounding errors may be up to 0.5%.

      double delta = rad(long1-long2);
      double sdlong = sin(delta);
      double cdlong = cos(delta);
      lat1 = rad(lat1);
      lat2 = rad(lat2);
      double slat1 = sin(lat1);
      double clat1 = cos(lat1);
      double slat2 = sin(lat2);
      double clat2 = cos(lat2);
      delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
      delta = sq(delta);
      delta += sq(clat2 * sdlong);
      delta = sqrt(delta);
      double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
      delta = atan2(delta, denom);
      return delta * radius_earth8;//6372795; 6377649.96584
    }

    double courseTo(double lat1, double long1, double lat2, double long2)
    {
      // returns course in degrees (North=0, West=270) from position 1 to position 2,
      // both specified as signed decimal-degrees latitude and longitude.
      // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.

      double dlon = rad(long2-long1);//long2-long1
      lat1 = rad(lat1);
      lat2 = rad(lat2);
      double a1 = sin(dlon) * cos(lat2);
      double a2 = sin(lat1) * cos(lat2) * cos(dlon);
      a2 = cos(lat1) * sin(lat2) - a2;
      a2 = atan2(a1, a2);
      if (a2 < 0.0)
      {
        a2 += M_PI*2;//TWO_PI;
      }
      return degrees(a2);
    }
//---------------------------------------------
    double get_angle();
//---------------------------------------------
public slots:

    void slot_setup_default();
    void slot_target_motion();
    void slot_timer_on();
    void slot_udpSocket();

    void slot_timeout();
    void az_sync(int st);
        void slot_udpServer();
protected:
            void paintEvent(QPaintEvent *pe) override;
private:
    double x_i;
    double z_i ;
    double y_i ;
    double vx_i;
    double vz_i;
    double vy_i;
    double tmp_p;

    int count_frameI;
    int index;
    bool enable_pos;
    double az_enc_zero;
    double um_enc_zero;
    double az_res_zero;
    double um_res_zero;

 //   QFile file;
    FILE *fp;
    tp_t  ptaz={0,0,0,0,0,0,0},ptum={0,0,0,0,0,0,0},ptmaz,ptmum,pteaz,pteum;

 //  QCustomPlot *cplt;
     const QPoint h[3] = {
        QPoint(3, 4),
        QPoint(-3, 4),
        QPoint(0, -90)
    };
     const QPoint m[3] = {
        QPoint(3, 4),
        QPoint(-3, 4),
        QPoint(0, -90)
    };

     const QPoint eh[3] = {
        QPoint(3, 4),
        QPoint(-3, 4),
        QPoint(0, -90)
    };
     const QPoint em[3] = {
        QPoint(3, 4),
        QPoint(-3, 4),
        QPoint(0, -90)
    };
     const QPoint itg[3] = {
        QPoint(3, 4),
        QPoint(-3, 4),
        QPoint(0, -90)
    };

    MotionPointOfOrtogonal *p_ortogonal;
    MotionPointOfSpherical *p_spherical;

    PM_CARTESIAN pos;
    PM_CARTESIAN vel;

    PM_SPHERICAL s_pos;
    PM_SPHERICAL s_vel;
    QTimer      *servo, *mono;
    ClockRealTime   rt;

    Ui::Form_mnemoshem *ui;

    QUdpSocket   *udpSocket;
//#ifdef local_server
    QUdpSocket   *udpServer;
    double encoder_base;
    double current_encoder=252.166;
//#endif
    int cycleSync = 10;

    QHostAddress ha_device;
    QHostAddress hb_device[5];
    QHostAddress hb_device_r[5];

  //  QHostAddress hb_device_br;
    quint16      port_dev = port_125;
    quint16      port_dev_tmp;

    quint16      port_recv[5];
    //IP Адреса устройств Enhernet. (Порт 50000)

    QHostAddress ip_apu;// = QHostAddress(QString("10.5.0.100"));// 	 АПУ -
    QHostAddress ip_unv;// = QHostAddress(QString("10.5.0.101"));// 	 УНВ -
    QHostAddress ip_p1 ;// = QHostAddress(QString("10.5.0.102"));// 	 Пусковая 1.
    QHostAddress ip_p2 ;// = QHostAddress(QString("10.5.0.103"));// 	 Пусковая 2.
    QHostAddress ip_p3 ;// = QHostAddress(QString("10.5.0.104"));// 	 Пусковая 3.
    QHostAddress ip_p4 ;// = QHostAddress(QString("10.5.0.105"));// 	 Пусковая 4.
    QHostAddress ip_tmp ;

//Структура данных обмена — АПУ->УНВ  и пусковых
START_PACK
struct apu_slave
{
    const quint32 header = 0xaa55aa55;
/*4 */quint32 ID_packet;
/*8 */int32_t angle_pos_az;               //DINT 	(32) 		 Угловая координата азимута.
/*12*/int32_t angle_pos_elv ;            //DINT 	(32) 		 Угловая координата угла места.
/*16*/int32_t angle_speed_az ;           //DINT 	(32) 		 Угловая скорость по азимуту.
/*20*/int32_t angle_speed_elv ;          //DINT 	(32) 		 Угловая скорость по углу места.
/*24*/quint32 send_SDO1;                 //UDINT[2] (64)         данные для записи по кодам/субкодвм. На передачу.
/*28*/quint32 send_SDO2;                 //UDINT[2] (64)	 	 данные для записи по кодам/субкодвм На передачу.
/*30*/quint16 code_az;       //WORD (16)		Номер кода  в запросе.(азимут).(Номер кода внутри блока управления приводом)
/*32*/quint16 subcode_az;    //WORD (16)		Номер субкода  в запросе.(азимут) (Номер кода внутри блока управления приводом)
/*34*/quint16 code_elv;      //WORD (16)		Номер кода  в запросе.(угол места).(Номер кода внутри блока управления приводом)
/*36*/quint16 subcode_elv;   //WORD (16)		Номер субкода  в запросе.(угол места) (Номер кода внутри блока управления приводом)
/*40*/quint32 data_az;       //UNION(32)		Данные для записи.(угол места)
/*44*/quint32 data_elv;      //UNION(32)		Данные для записи.(азимут)
/*48*/quint32                //DWORD (32) 		Управление приводом
    az_on:1,//бит 0 		Включение привода азимута
    elv_on:1,//бит 1 		Включение привода угла места
    az_en:1,//бит 2 		Разрешение работы интегратора азимута
    elv_en:1,//бит 3 		Разрешение работы интегратора угла места
    az_int_clear:1,//бит 4 		Сброс интегратора азимута
    elv_int_clear:1,//бит 5 		Сброс интегратора угла места
    az_pos_speed_controll:1,//бит 6 		Управление по позиции — 0 по скорости 1 (азимут)
    elv_pos_speed_controll:1,//бит 7 		Управление по позиции — 0 по скорости 1 (угол места)
    az_error_clear:1,//бит 8 		Сброс ошибки привода азимута
    elv_error_clear:1,//бит 9		Сброс ошибки привода угла места
    CAN_SDO_ADR:3,//биты 10-12 	Адрес устройства на CAN шине при обмене SDO .
    az_code_write:1,//Бит 13	 	Признак записи для кодов (азимут).
    elv_code_write:1,//Бит 14	 	Признак записи для кодов (угол места).

    sc_UV10_on:1,//bit 15           Включение/выключение сканнера УВ10
    sc_UV11_on:1,//bit 16           Включение/выключение сканнера УВ11

    az_code_read:1,//Бит 17         Признак чтения для кодов (азимут).
    elv_code_read:1,//Бит 18	 	Признак чтения для кодов (угол места).
    sync_int_az:1,  //Бит 19        синхронизация энкодера и резольвера азимута
    sync_int_um:1;  //Бит 20        синхронизация энкодера и резольвера угла места
  //  not_used:11;        //bit 19-31

///*70*/const char ID_string[22] = {'K','r','a','m','a','r','e','n','k','o','S','h','c','h','e','r','b','a','k','o','v',0};//{"КрамаренкоЩербаков"};

};
END_PACK
//Структура данных обмена —УНВ  и пусковые →АПУ
START_PACK
struct slave_apu
{
    const quint32 header = 0x55aa55aa;
quint32 ID_packet;
int32_t enc_angle_pos_az; //DINT 	(32) 		 Угловая координата азимута. (по энкодеру)
int32_t enc_angle_pos_elv;//DINT 	(32) 		 Угловая координата угла места.(по энкодеру)
int32_t enc_angle_speed_az ;//DINT 	(32) 		 Угловая скорость по азимуту.(по энкодеру)
int32_t enc_angle_speed_elv ; //DINT 	(32) 		 Угловая скорость по углу места.(по энкодеру)
quint32 motor_encoder_az;     //DWORD (32) Энкодер двигателя азимута
quint32 motor_encoder_elv;    //DWORD (32) Энкодер двигателя угла места
quint32 receive_SDO1;         // SDO 1,2 На прием.
quint32 receive_SDO2;
//uint64_t receive_SDO;           //UDINT[2] (64)		 SDO На прием.
quint16 code_az;       //WORD (16)		Номер кода  в ответе.(азимут).(Номер кода внутри блока управления приводом)
quint16 subcode_az;             //WORD (16)		Номер субкода  в ответе.(азимут) (Номер кода внутри блока управления приводом)
quint16 code_elv;      //WORD (16)		Номер кода  в ответе.(угол места).(Номер кода внутри блока управления приводом)
quint16 subcode_elv;   //WORD (16)		Номер субкода  в ответе (угол места) (Номер кода внутри блока управления приводом)
quint32 data_az;       //UNION(32)		Данные по коду.(азимут)
quint32 data_elv;      //UNION(32)		Данные по коду.(угол места)
quint32 az_errors;     //UDINT (32) 		Код ошибки привода azimuth
quint32 elv_errors;     //UDINT (32) 		Код ошибки привода elv

quint32            //UDINT (32) 		Состояние привода.
    az_error:1,     //бит 0 		Ошибка привода азимута
    elv_error:1,    //бит 1 		Ошибка привода угла места
    drv_present:1,  //биты 2 07/07/2018 признак наличия приводов по "или" азим. и угол места
    err_rd_az:1,//3
    err_wr_az:1,//4
    err_rd_um:1,//5
    err_wr_um:1,//6
    status_rd_az:1,//7   //биты 7-8 	статусы чтения/записи кодов .
    status_wr_az:1,//8
    status_rd_um:1,//9   //биты 9-10 	статусы чтения/записи кодов .
    status_wr_um:1,//10
 //   rez2:21;        //биты 11-31 	резерв.
                        //состояние унв/пск
     k_l_raz:1,  //11                 //концевик лесницы/ручка азимута
     k_b_rum:1,  //12                 //концевик переднего балкона/ручка угла места
     k_bz:1,     //13               //концевик заднего балкона
     k_umV:1,   //14                //угол места предел низ
     k_umn:1,   //15                //угол места предел верх
     b_otb:6;  //16-21                 //Биты с ОТБ 9 только для пушек

//char ID_string[22] = {'S','h','c','h','e','r','b','a','k','o','v','K','r','a','m','a','r','e','n','k','o',0};//{"КрамаренкоЩербаков"};

};
END_PACK
apu_slave apu_unv,apu_p1,apu_p2,apu_p3,apu_p4,tmp_mas,apu_apu;// = {0,"КрамаренкоЩербаков"};
slave_apu unv,p1,p2,p3,p4,tmp_slv;
int cnt=0;
};
#endif // FORM_MNEMOSHEM_H
