#include "SURN_Imitator.h"
#include "ui_form_mnemoshem.h"




QString off_fld      =  "background-color: rgb(125,125,125);border-radius: 0;border: 1px solid black;";
QString on_fld       =  "background-color: rgb(0,255,0);border-radius: 0;border: 1px solid black;";
QString on_fld_yelow =  "background-color: rgb(255,255,0);border-radius: 0;border: 1px solid black;";
QString on_fld_red   =  "background-color: rgb(255,0,0);border-radius: 0;border: 1px solid black;";
QString on_fld_orang =  "background-color: rgb(255,128,0);border-radius: 0;border: 1px solid black;";

QString off_bkgc ="background-color: rgb(125,125,125)";
QString on_bkgc = "background-color: rgb(0,255,0)";
QString keyUp  =  "border-image: url(:/new/pic1/img/Key.jpg); color: rgb(255,255,255)";
QString keyDn  =  "border-image: url(:/new/pic1/img/Key_dwn.jpg); color: rgb(255,255,255)";

Form_mnemoshem::Form_mnemoshem(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form_mnemoshem)
{

       ui->setupUi(this);

//ui->gr->paintEngine()->drawLines(QLine(1,1,20,20),1);







    ha_device.setAddress("10.5.0.100");
#ifdef local_server
    ip_apu.setAddress("127.0.0.7");//
    ip_unv.setAddress("127.0.0.1");//

#else
    ip_apu.setAddress("10.5.0.225");//
    ip_unv.setAddress("10.5.0.101");//
#endif
    ip_p1 .setAddress("10.5.0.102");//
    ip_p2 .setAddress("10.5.0.103");//
    ip_p3 .setAddress("10.5.0.104");//
    ip_p4 .setAddress("10.5.0.105");//


 //   hb_device_br = QHostAddress(QString("10.5.0.255"));
    port_dev  = port_125;



     udpSocket = new QUdpSocket(this);


     udpSocket->bind(ip_apu,port_dev,QAbstractSocket::DontShareAddress);



#ifdef local_server
    udpServer = new QUdpSocket(this);

    udpServer->bind(ip_unv,port_dev/*,QAbstractSocket::DontShareAddress*/);

     connect(this->udpServer,SIGNAL(readyRead()),this,SLOT(slot_udpServer()));
#endif

     servo = new QTimer();
     mono  = new QTimer();
     mono->setSingleShot(true);

     mono->start(main_clock_works);

    ui->lineEdit_3->setText("10000.0");
    ui->lineEdit_4->setText("45.0");
    ui->lineEdit_5->setText("4000");

    ui->lineEdit_6->setText("0");
    ui->lineEdit_7->setText("180");
    ui->lineEdit_8->setText("0");

    p_ortogonal = new MotionPointOfOrtogonal();
    p_spherical = new MotionPointOfSpherical();

    double D_C_G    = get_DoG_imt();
    double B_C      = get_beta_imt();
    double H_C      = get_Ho_imt();
    double V_C      = get_Vo_imt();
    double S_C      = get_So_imt();
    double O_C      = get_To_imt();
    double X_I      =   D_C_G * cos(grad_to_radians(B_C));
    double Z_I      =   D_C_G * sin(grad_to_radians(B_C));
    //double Y_I      =   H_C - pmSq(D_C_G) * 0.0000000588;
    //double E_C      =   radians_to_grad(atan2(D_C_G,Y_I));
    double vX_I     =   V_C * cos(grad_to_radians(S_C))*cos(grad_to_radians(O_C));
    double vZ_I     =   V_C * sin(grad_to_radians(S_C))*cos(grad_to_radians(O_C));\
    double vY_I     =   V_C * sin(grad_to_radians(O_C));

    double X_GS     =   0;
    double Z_GS     =   0;
    double H_GS     =   0;

    //--------------------TARGET Parameters in CK-42
        double X_GC     =   X_I + X_GS;
        double Z_GC     =   Z_I + Z_GS;
        double H_GC     =   H_C + H_GS;
        double vX_GC    =   vX_I;
        double vZ_GC    =   vZ_I;
        double vH_GC    =   vY_I;
    //--------------------MZCK-I
        double x_i      =   X_GC - X_GS;
        double z_i      =   Z_GC - Z_GS;
        double y_i      =   H_GC - H_GS - (pmSq(x_i) + pmSq(z_i)) * 0.0000000588;
        double vx_i     =   vX_GC;
        double vz_i     =   vZ_GC;
        double vy_i     =   vH_GC;  // (41)
    //-----------------------------


    ui->lineEdit_9->setText(QString::number(x_i));
    ui->lineEdit_10->setText(QString::number(z_i));
    ui->lineEdit_11->setText(QString::number(y_i));

    ui->lineEdit_12->setText(QString::number(vx_i));
    ui->lineEdit_13->setText(QString::number(vz_i));
    ui->lineEdit_14->setText(QString::number(vy_i));


    p_ortogonal->setXYZ(ui->lineEdit_9->text().toDouble(),ui->lineEdit_10->text().toDouble(),ui->lineEdit_11->text().toDouble());
    p_ortogonal->setVxVyVz(ui->lineEdit_12->text().toDouble(),ui->lineEdit_13->text().toDouble(),ui->lineEdit_14->text().toDouble());
    *p_spherical = *p_ortogonal;//преобразование ортогональной системы координат в сферическую

//    ui->lineEdit_43->setText(QString::number(p_spherical->getR()));
 //   ui->lineEdit_46->setText(QString::number(p_spherical->getVr()));
//    ui->lineEdit_45->setText(QString::number(p_spherical->getE()));
//    ui->lineEdit->setText(QString::number(p_spherical->getVe()));

 //   ui->lineEdit_44->setText(QString::number(p_spherical->getB()));
//    ui->lineEdit_2->setText(QString::number(p_spherical->getVb()));

    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(slot_setup_default()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(slot_timer_on()));
    connect(this->udpSocket,SIGNAL(readyRead()),this,SLOT(slot_udpSocket()));

    connect(servo,SIGNAL(timeout()),this,SLOT(slot_target_motion()));
    connect(servo,SIGNAL(timeout()),this,SLOT(update()));

    connect(mono,SIGNAL(timeout()),this,SLOT(slot_timeout()));
    connect(ui->led_49,SIGNAL(stateChanged(int)),this,SLOT(az_sync(int)));

    ptaz.max_acc = cmax_acc_az;
    ptaz.max_vel = cmax_vel_az;
    ptum.max_acc = cmax_acc_um;
    ptum.max_vel = cmax_vel_um;

//    QNetworkConfigurationManager mgr;
//    QNetworkConfiguration nconfig = mgr.defaultConfiguration();
//    QNetworkSession session ( nconfig );
//    QNetworkInterface ninter = session.interface();
//    QList<QNetworkAddressEntry> laddr = ninter.addressEntries();
//    for ( QList<QNetworkAddressEntry> ::const_iterator it = laddr.begin(); it != laddr.end(); ++it )
//    {
//        qDebug() << it->ip() << endl;
//    }

fp=fopen("Elevation.txt","w");

}

Form_mnemoshem::~Form_mnemoshem()
{
    fclose(fp);
    delete servo;
    delete mono;
    delete udpSocket;
    delete p_ortogonal;
    delete p_spherical;

    delete ui;
}

void Form_mnemoshem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
  //  painter.setWindow(QRect(609,269,221,221));
    painter.setViewport(QRect(609,269,221,221));
    QColor hc(100, 0, 0);
    QColor mc(0, 100, 80, 191);
    QColor ehc(180, 10, 0);
    QColor emc(0, 180, 10, 191);
    QColor mt(20, 20, 20, 191);
     QColor itgc(255, 255, 255, 191);
      //  int side = qMin(width(), height());
    painter.setRenderHint(QPainter::Antialiasing);
    //! [12] //! [13]
    painter.translate(width() / 2, height() / 2);
    //! [13] //! [14]
//    painter.scale(side / 200.0, side / 200.0);
      painter.scale(6.0, 3.0);
    //! [9] //! [14]

    //! [15]
    painter.setPen(Qt::NoPen);
    //! [15] //! [16]
    painter.setBrush(hc);
    //! [16]

    //! [17] //! [18]
    painter.save();
    //! [17] //! [19]
    painter.rotate(get_resolver_az(unv.motor_encoder_az));
    painter.drawConvexPolygon(h, 3);
    painter.restore();
    //! [18] //! [19]
    painter.setBrush(mc);

//! [24]
    painter.save();
    painter.rotate(get_resolver_um(unv.motor_encoder_elv));
    painter.drawConvexPolygon(m, 3);
    painter.restore();
    //! [20]
//-------------------------------------------
    painter.setBrush(ehc);

//! [24]
    painter.save();
    painter.rotate(get_encoder_az(unv.enc_angle_pos_az));
    painter.drawConvexPolygon(eh, 3);
    painter.restore();

    painter.setBrush(emc);

//! [24]
    painter.save();
    painter.rotate(get_encoder_um(unv.enc_angle_pos_elv));
    painter.drawConvexPolygon(em, 3);
    painter.restore();

//    painter.setBrush(itgc);

////! [24]
//    painter.save();
//    painter.rotate(get_integrator_az(unv.enc_angle_speed_az));
//    painter.drawConvexPolygon(itg, 3);
//    painter.restore();
//------------------------------------------------------

    painter.setPen(mt);
    //! [20] //! [21]

    for (int i = 0; i < 12; ++i) {
        painter.drawLine(90, 0, 96, 0);
        painter.rotate(30.0);
    }


}
 void Form_mnemoshem::az_sync(int st)
 {
 if(st == 2)
 {
az_enc_zero=get_encoder_az(unv.enc_angle_pos_az);
az_res_zero=get_resolver_az((unv.motor_encoder_az));
um_enc_zero=get_encoder_um(unv.enc_angle_pos_elv);
um_res_zero=get_resolver_um((unv.motor_encoder_elv));

 }

 }

void Form_mnemoshem::slot_timer_on()
{
    if(ui->led->isChecked())
        this->servo->start(main_clock_works);
    else
        this->servo->stop();
}

void Form_mnemoshem::slot_setup_default()
{

current_encoder = ui->stenc->text().toDouble();
cycleSync = 10;

double D_C_G    = get_DoG_imt();
double B_C      = get_beta_imt();
double H_C      = get_Ho_imt();
double V_C      = get_Vo_imt();
double S_C      = get_So_imt();
double O_C      = get_To_imt();
double X_I      =   D_C_G * cos(grad_to_radians(B_C));
double Z_I      =   D_C_G * sin(grad_to_radians(B_C));
//double Y_I      =   H_C - pmSq(D_C_G) * 0.0000000588;
//double E_C      =   radians_to_grad(atan2(D_C_G,Y_I));
double vX_I     =   V_C * cos(grad_to_radians(S_C))*cos(grad_to_radians(O_C));
double vZ_I     =   V_C * sin(grad_to_radians(S_C))*cos(grad_to_radians(O_C));\
double vY_I     =   V_C * sin(grad_to_radians(O_C));

double X_GS     =   0;
double Z_GS     =   0;
double H_GS     =   0;

//--------------------TARGET Parameters in CK-42
    double X_GC     =   X_I + X_GS;
    double Z_GC     =   Z_I + Z_GS;
    double H_GC     =   H_C + H_GS;
    double vX_GC    =   vX_I;
    double vZ_GC    =   vZ_I;
    double vH_GC    =   vY_I;
//--------------------MZCK-I
    x_i      =   X_GC - X_GS;
    z_i      =   Z_GC - Z_GS;
    y_i      =   H_GC - H_GS - (pmSq(x_i) + pmSq(z_i)) * 0.0000000588;
    vx_i     =   vX_GC;
    vz_i     =   vZ_GC;
    vy_i     =   vH_GC;  // (41)
//-----------------------------QHostAddress


ui->lineEdit_9->setText(QString::number(x_i));
ui->lineEdit_10->setText(QString::number(z_i));
ui->lineEdit_11->setText(QString::number(y_i));

ui->lineEdit_12->setText(QString::number(vx_i));
ui->lineEdit_13->setText(QString::number(vz_i));
ui->lineEdit_14->setText(QString::number(vy_i));

p_ortogonal->setXYZ(x_i,y_i,z_i);
p_ortogonal->setVxVyVz(vx_i,vy_i,vz_i);

#ifdef test_no_p
//--------------------------for test 21.05.2018
set_beta_target(get_beta_imt());
ui->lineEdit_44->setText(QString::number(get_eps_target()));
//---------------------------------------------
#endif
 *p_spherical = *p_ortogonal;//преобразование ортогональной системы координат в сферическую
ptaz.max_acc = cmax_acc_az;
ptaz.max_vel = cmax_vel_az;
ptum.max_acc = cmax_acc_um;
ptum.max_vel = cmax_vel_um;

}

 bool Form_mnemoshem::get_state_imitator()  {return ui->led->isChecked();}

 //bool Form_mnemoshem::get_state_imitator()  {return (ui->led_49->state());}
 bool Form_mnemoshem::get_No_pusk()         {return (ui->led_49->isChecked());}
 bool Form_mnemoshem::get_Test_mode()       {return (ui->led_52->isChecked());}
 bool Form_mnemoshem::get_Target()          {return (ui->led_46->isChecked());}
 bool Form_mnemoshem::get_Coord_TK()        {return (ui->led_48->isChecked());}
 bool Form_mnemoshem::get_Clog_1C31M1()     {return (ui->led_50->isChecked());}
 bool Form_mnemoshem::get_Link()            {return (ui->led_51->isChecked());}
 bool Form_mnemoshem::get_Start()           {return (ui->led_45->isChecked());}
// bool Form_mnemoshem::get_Mode_II_in()      {return (ui->led_47->isChecked());}

 double Form_mnemoshem::get_epsilon_target() {return (ui->lineEdit_44->text().toDouble());}  //угол места цели, измеряемый СН
 double Form_mnemoshem::get_vel_epsilon_target() {return (ui->lineEdit_2->text().toDouble());}
 double Form_mnemoshem::get_beta_target()    {return (ui->lineEdit_45->text().toDouble());}  //угол азимута цели
 double Form_mnemoshem::get_vel_beta_target()    {return (ui->lineEdit->text().toDouble());}  //угол азимута цели

 //-------------------------01/06/2018
 double Form_mnemoshem::get_eps_target()    {return (ui->stum->text().toDouble());}  //угол места для теста цели
//-----------------------------------------
 double Form_mnemoshem::get_distance_target(){return (ui->lineEdit_43->text().toDouble());}
 double Form_mnemoshem::get_velocity_target(){return (ui->lineEdit_46->text().toDouble());}

 double Form_mnemoshem::get_DoG_imt(){return (ui->lineEdit_3->text().toDouble());}     //дальность
 double Form_mnemoshem::get_beta_imt(){return (ui->lineEdit_4->text().toDouble());}     //угол азимута цели
 double Form_mnemoshem::get_Ho_imt(){return (ui->lineEdit_5->text().toDouble());}
 double Form_mnemoshem::get_Vo_imt(){return (ui->lineEdit_6->text().toDouble());}
 double Form_mnemoshem::get_So_imt(){return (ui->lineEdit_7->text().toDouble());}
 double Form_mnemoshem::get_To_imt(){return (ui->lineEdit_8->text().toDouble());}


 void Form_mnemoshem::set_epsilon_target(double v) {ui->lineEdit_44->setText(QString::number(v));}  //угол места цели, измеряемый СН
 void Form_mnemoshem::set_vel_epsilon_target(double v) {ui->lineEdit_2->setText(QString::number(v));}

 void Form_mnemoshem::set_beta_target(double v)    {ui->lineEdit_45->setText(QString::number(v));}  //угол азимута цели
 void Form_mnemoshem::set_vel_beta_target(double v)    {ui->lineEdit->setText(QString::number(v));}  //угол азимута цели

 void Form_mnemoshem::set_distance_target(double v){ui->lineEdit_43->setText(QString::number(v));}
 void Form_mnemoshem::set_velocity_target(double v){ui->lineEdit_46->setText(QString::number(v));}    


 //double Form_mnemoshem::get_angle()    {return (ui->lineEdit_15->text().toDouble());}  //угол test


 void Form_mnemoshem::slot_target_motion()
 {

   //  int spu_num = ui->spu_num->value();
     double time=rt.interval();//0.02;
  //  qDebug() << time;
     tp_update(&ptaz,time);
     tp_update(&ptum,time);

     if(ui->U_GPS->isChecked())
     {
         if(ui->use_this->isChecked())
         {
             ui->lineEdit_3->setText(QString::number(
                             distanceBetween(
                                             coord_tr(ui->lineEdit_15->text().toDouble(),
                                                      ui->lineEdit_16->text().toDouble(),
                                                      ui->lineEdit_17->text().toDouble()),
                                              coord_tr(ui->lineEdit_18->text().toDouble(),
                                                       ui->lineEdit_19->text().toDouble(),
                                                       ui->lineEdit_20->text().toDouble()),
                                              coord_tr(ui->lineEdit_21->text().toDouble(),
                                                       ui->lineEdit_22->text().toDouble(),
                                                       ui->lineEdit_23->text().toDouble()),
                                              coord_tr(ui->lineEdit_24->text().toDouble(),
                                                       ui->lineEdit_25->text().toDouble(),
                                                       ui->lineEdit_26->text().toDouble()))));
             ui->lineEdit_4->setText(QString::number(
                                        courseTo(
                                            coord_tr(ui->lineEdit_15->text().toDouble(),
                                                     ui->lineEdit_16->text().toDouble(),
                                                     ui->lineEdit_17->text().toDouble()),
                                             coord_tr(ui->lineEdit_18->text().toDouble(),
                                                      ui->lineEdit_19->text().toDouble(),
                                                      ui->lineEdit_20->text().toDouble()),
                                             coord_tr(ui->lineEdit_21->text().toDouble(),
                                                      ui->lineEdit_22->text().toDouble(),
                                                      ui->lineEdit_23->text().toDouble()),
                                             coord_tr(ui->lineEdit_24->text().toDouble(),
                                                      ui->lineEdit_25->text().toDouble(),
                                                      ui->lineEdit_26->text().toDouble()))));

             ui->lineEdit_5->setText(QString::number(ui->h2->text().toDouble()-
                                                     ui->h1->text().toDouble()));
         }
         else
         {
 ui->lineEdit_3->setText(QString::number(
                 distanceBetween(ui->la1->text().toDouble(),
                                 ui->lo1->text().toDouble(),
                                 ui->la2->text().toDouble(),
                                 ui->lo2->text().toDouble())));
 ui->lineEdit_4->setText(QString::number(
                            courseTo(ui->la1->text().toDouble(),
                                 ui->lo1->text().toDouble(),
                                 ui->la2->text().toDouble(),
                                 ui->lo2->text().toDouble())));
 ui->lineEdit_5->setText(QString::number(ui->h2->text().toDouble()-
                                         ui->h1->text().toDouble()));
}

     }

    // *p_ortogonal = p_ortogonal->getPredicted(time);
      *p_spherical = p_spherical->getPredicted(time);
    // *p_spherical = *p_ortogonal;//преобразование ортогональной системы координат в сферическую


     ui->lineEdit_9->setText(QString::number(p_ortogonal->getX()));
     ui->lineEdit_10->setText(QString::number(p_ortogonal->getY()));
     ui->lineEdit_11->setText(QString::number(p_ortogonal->getZ()));

     ui->lineEdit_12->setText(QString::number(p_ortogonal->getVx()));
     ui->lineEdit_13->setText(QString::number(p_ortogonal->getVy()));
     ui->lineEdit_14->setText(QString::number(p_ortogonal->getVz()));




     double sdaz = ui->azm->text().toDouble()-ui->raz->text().toDouble();
  //   if(sdaz <0.0)
 //    ui->daz->setText(QString::number(360.0+sdaz));
 //    else
     ui->daz->setText(QString::number(sdaz));

     double sdum = ui->eum->text().toDouble()-ui->rum->text().toDouble();

  //   if(sdum <0.0)
 //    ui->dum->setText(QString::number(360.0+sdum));
  //   else
     ui->dum->setText(QString::number(sdum));




     set_distance_target(p_spherical->getR());

     if(fabs(radians_to_grad(p_spherical->getE())) >= 90.0)
    {
#ifndef test_no_p
         set_beta_target(radians_to_grad(p_spherical->getB())+180.0);
 set_epsilon_target(90.0-(radians_to_grad(p_spherical->getE())-90.0));

#endif
     }
     else
     {
 #ifndef test_no_p
     set_beta_target(radians_to_grad(p_spherical->getB()));
     set_epsilon_target(radians_to_grad(p_spherical->getE()));

#endif
        }


     set_velocity_target(p_spherical->getVr());
     set_vel_epsilon_target(radians_to_grad(p_spherical->getVe()));
     set_vel_beta_target(radians_to_grad(p_spherical->getVb()));



switch (ui->spu_num->value()) {
case 0:
    ui->azm->setText(QString::number(get_encoder_az(unv.enc_angle_pos_az)));
    ui->raz->setText(QString::number(get_resolver_az(unv.motor_encoder_az)));
    ui->eum->setText(QString::number(get_encoder_um(unv.enc_angle_pos_elv)));
    ui->rum->setText(QString::number(get_resolver_um(unv.motor_encoder_elv)));

    ui->iaz->setText(QString::number(get_integrator_az(unv.enc_angle_speed_az)));
    ui->ium->setText(QString::number(get_integrator_um(unv.enc_angle_speed_elv)));

    ui->err_az->setCheckState(Qt::CheckState(unv.err_rd_az | unv.err_wr_az) );
    ui->err_elv->setCheckState(Qt::CheckState(unv.err_rd_um | unv.err_wr_um) );

    ui->ok_az->setCheckState(Qt::CheckState(unv.status_rd_az | unv.status_wr_az));
    ui->ok_elv->setCheckState(Qt::CheckState(unv.status_rd_um | unv.status_wr_um));

    ui->data_az->setText(QString::number(unv.receive_SDO1));
    ui->data_elv->setText(QString::number(unv.receive_SDO2));



    apu_unv.ID_packet = cnt++;//1234567890;
#ifdef test_no_p
   apu_unv.angle_pos_az =  double_to_lenze(get_beta_target());
   apu_unv.angle_pos_elv = double_to_lenze_eps_p(get_epsilon_target());

#else
   apu_unv.angle_pos_az = double_to_lenze(radians_to_grad(p_spherical->getB())/*get_beta_imt()*/); //DINT 	(32) 		 Угловая координата азимута.
   apu_unv.angle_pos_elv = double_to_lenze_eps(radians_to_grad(p_spherical->getE())) ;//DINT 	(32) 		 Угловая координата угла места.

#endif
    apu_unv.angle_speed_az = 0;// -(int)((51.2*60.0*65536.0)/360.0);           //DINT 	(32) 		 Угловая скорость по азимуту.
    apu_unv.angle_speed_elv = 1 ;          //DINT 	(32) 		 Угловая скорость по углу места.


    apu_unv.code_az=ui->code->text().toInt();
    apu_unv.subcode_az=ui->subcode->text().toInt();
    apu_unv.code_elv=ui->code->text().toInt();
    apu_unv.subcode_elv=ui->subcode->text().toInt();

    apu_unv.data_az=ui->DataWrite->text().toInt();
    apu_unv.data_elv=ui->DataWrite->text().toInt();

    apu_unv.send_SDO1=ui->DataWrite->text().toInt();
    apu_unv.send_SDO2=ui->data_type->text().toInt();

apu_unv.az_on=ui->led_49->checkState()?1:0;
apu_unv.elv_on=ui->led_52->checkState()?1:0;//:1,
apu_unv.az_en=ui->led_50->checkState()?1:0;//:1,//бит 2 		Р
apu_unv.elv_en=ui->led_50->checkState()?1:0;//:1,//бит 3 		Р
apu_unv.az_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 4
apu_unv.elv_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 5
apu_unv.az_pos_speed_controll=0;//:1,/
apu_unv.elv_pos_speed_controll=0;//:1,
apu_unv.az_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 8
apu_unv.elv_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 9
apu_unv.CAN_SDO_ADR=0;//:3,//биты 10-1
apu_unv.az_code_write=ui->write_az->checkState()?1:0;//:1,//Бит 13
apu_unv.elv_code_write=ui->write_elv->checkState()?1:0;//:1;//Бит 14
apu_unv.sc_UV10_on=ui->led_51->checkState()?1:0;//:1,//бит 15
apu_unv.sc_UV11_on=ui->led_45->checkState()?1:0;//:1,//бит 16
apu_unv.az_code_read=ui->read_az->checkState()?1:0;
apu_unv.elv_code_read=ui->read_elv->checkState()?1:0;

//apu_unv.not_used =0;
udpSocket->writeDatagram((char*)&this->apu_unv,sizeof(apu_unv), ip_unv,port_dev);

    break;
case 1:
    ui->azm->setText(QString::number(get_encoder_az_p(p1.enc_angle_pos_az)));
    ui->raz->setText(QString::number(get_resolver_az_p(p1.motor_encoder_az)));
    ui->eum->setText(QString::number(get_encoder_um_p(p1.enc_angle_pos_elv)));
    ui->rum->setText(QString::number(get_resolver_um_p(p1.motor_encoder_elv)));

    ui->iaz->setText(QString::number(get_integrator_az_p(p1.enc_angle_speed_az)));
    ui->ium->setText(QString::number(get_integrator_um_p(p1.enc_angle_speed_elv)));




    ui->err_az->setCheckState(Qt::CheckState(p1.err_rd_az | p1.err_wr_az) );
    ui->err_elv->setCheckState(Qt::CheckState(p1.err_rd_um | p1.err_wr_um) );

    ui->ok_az->setCheckState(Qt::CheckState(p1.status_rd_az | p1.status_wr_az));
    ui->ok_elv->setCheckState(Qt::CheckState(p1.status_rd_um | p1.status_wr_um));

    ui->data_az->setText(QString::number(p1.receive_SDO1));
    ui->data_elv->setText(QString::number(p1.receive_SDO2));

    apu_p1.ID_packet = cnt++;//1234567890;
#ifdef test_no_p
   apu_p1.angle_pos_az = double_to_lenze_p(get_beta_target());
   apu_p1.angle_pos_elv = double_to_lenze_eps_p_poly(get_epsilon_target());
#else
   apu_p1.angle_pos_az = double_to_lenze_p(radians_to_grad(p_spherical->getB())/*get_beta_imt()*/); //DINT 	(32) 		 Угловая координата азимута.
   apu_p1.angle_pos_elv = double_to_lenze_eps_p(radians_to_grad(p_spherical->getE())) ;//DINT 	(32) 		 Угловая координата угла места.

#endif

   apu_p1.code_az=ui->code->text().toInt();
   apu_p1.subcode_az=ui->subcode->text().toInt();
   apu_p1.code_elv=ui->code->text().toInt();
   apu_p1.subcode_elv=ui->subcode->text().toInt();
   apu_p1.data_az=ui->DataWrite->text().toInt();
   apu_p1.data_elv=ui->DataWrite->text().toInt();
   apu_p1.send_SDO1=ui->DataWrite->text().toInt();
   apu_p1.send_SDO2=ui->data_type->text().toInt();

    apu_p1.angle_speed_az = 0;// -(int)((51.2*60.0*65536.0)/360.0);           //DINT 	(32) 		 Угловая скорость по азимуту.
    apu_p1.angle_speed_elv = 1 ;          //DINT 	(32) 		 Угловая скорость по углу места.
   // apu_p1.send_SDO1=2;
   // apu_p1.send_SDO2=3;
   // apu_p1.code_az=4;
   // apu_p1.subcode_az=5;
   // apu_p1.code_elv=6;
   // apu_p1.subcode_elv=7;
  //  apu_p1.data_az=8;
  //  apu_p1.data_elv=9;

    apu_p1.az_on=ui->led_49->checkState()?1:0;
    apu_p1.elv_on=ui->led_52->checkState()?1:0;//:1,
    apu_p1.az_en=ui->led_50->checkState()?1:0;//:1,//бит 2 		Р
    apu_p1.elv_en=ui->led_50->checkState()?1:0;//:1,//бит 3 		Р
    apu_p1.az_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 4
    apu_p1.elv_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 5
    apu_p1.az_pos_speed_controll=0;//:1,/
    apu_p1.elv_pos_speed_controll=0;//:1,
    apu_p1.az_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 8
    apu_p1.elv_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 9
    apu_p1.CAN_SDO_ADR=0;//:3,//биты 10-1
    apu_p1.az_code_write=ui->write_az->checkState()?1:0;//:1,//Бит 13
    apu_p1.elv_code_write=ui->write_elv->checkState()?1:0;//:1;//Бит 14
    apu_p1.sc_UV10_on=ui->led_51->checkState()?1:0;//:1,//бит 15
    apu_p1.sc_UV11_on=ui->led_45->checkState()?1:0;//:1,//бит 16
    apu_p1.az_code_read=ui->read_az->checkState()?1:0;
    apu_p1.elv_code_read=ui->read_elv->checkState()?1:0;

//apu_p1.not_used =0;
udpSocket->writeDatagram((char*)&this->apu_p1,sizeof(apu_p1), ip_p1,port_dev);

    break;
case 2:
    ui->azm->setText(QString::number(get_encoder_az_p(p2.enc_angle_pos_az)));
    ui->raz->setText(QString::number(get_resolver_az_p(p2.motor_encoder_az)));
    ui->eum->setText(QString::number(get_encoder_um_p(p2.enc_angle_pos_elv)));
    ui->rum->setText(QString::number(get_resolver_um_p(p2.motor_encoder_elv)));

    ui->iaz->setText(QString::number(get_integrator_az_p(p2.enc_angle_speed_az)));
    ui->ium->setText(QString::number(get_integrator_um_p(p2.enc_angle_speed_elv)));

    ui->err_az->setCheckState(Qt::CheckState(p2.err_rd_az | p2.err_wr_az) );
    ui->err_elv->setCheckState(Qt::CheckState(p2.err_rd_um | p2.err_wr_um) );

    ui->ok_az->setCheckState(Qt::CheckState(p2.status_rd_az | p2.status_wr_az));
    ui->ok_elv->setCheckState(Qt::CheckState(p2.status_rd_um | p2.status_wr_um));

    ui->data_az->setText(QString::number(p2.receive_SDO1));
    ui->data_elv->setText(QString::number(p2.receive_SDO2));

    apu_p2.ID_packet = cnt++;//1234567890;
#ifdef test_no_p
   apu_p2.angle_pos_az = double_to_lenze_p(get_beta_target());
   apu_p2.angle_pos_elv = double_to_lenze_eps_p(get_epsilon_target());
#else
   apu_p2.angle_pos_az = double_to_lenze_p(radians_to_grad(p_spherical->getB())/*get_beta_imt()*/); //DINT 	(32) 		 Угловая координата азимута.
   apu_p2.angle_pos_elv = double_to_lenze_eps_p(radians_to_grad(p_spherical->getE())) ;//DINT 	(32) 		 Угловая координата угла места.

#endif
   if(apu_p2.angle_pos_az == 0)
      apu_p2.angle_speed_az = 0;

   apu_p2.code_az=ui->code->text().toInt();
   apu_p2.subcode_az=ui->subcode->text().toInt();
   apu_p2.code_elv=ui->code->text().toInt();
   apu_p2.subcode_elv=ui->subcode->text().toInt();
   apu_p2.data_az=ui->DataWrite->text().toInt();
   apu_p2.data_elv=ui->DataWrite->text().toInt();
   apu_p2.send_SDO1=ui->DataWrite->text().toInt();
   apu_p2.send_SDO2=ui->data_type->text().toInt();

    apu_p2.angle_speed_az = 0;// -(int)((51.2*60.0*65536.0)/360.0);           //DINT 	(32) 		 Угловая скорость по азимуту.
    apu_p2.angle_speed_elv = 1 ;          //DINT 	(32) 		 Угловая скорость по углу места.
   // apu_p2.send_SDO1=2;
   // apu_p2.send_SDO2=3;
   // apu_p2.code_az=4;
  //  apu_p2.subcode_az=5;
  //  apu_p2.code_elv=6;
  //  apu_p2.subcode_elv=7;
   // apu_p2.data_az=8;
   // apu_p2.data_elv=9;

    apu_p2.az_on=ui->led_49->checkState()?1:0;
    apu_p2.elv_on=ui->led_52->checkState()?1:0;//:1,
    apu_p2.az_en=ui->led_50->checkState()?1:0;//:1,//бит 2 		Р
    apu_p2.elv_en=ui->led_50->checkState()?1:0;//:1,//бит 3 		Р
    apu_p2.az_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 4
    apu_p2.elv_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 5
    apu_p2.az_pos_speed_controll=0;//:1,/
    apu_p2.elv_pos_speed_controll=0;//:1,
    apu_p2.az_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 8
    apu_p2.elv_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 9
    apu_p2.CAN_SDO_ADR=0;//:3,//биты 10-1
    apu_p2.az_code_write=ui->write_az->checkState()?1:0;//:1,//Бит 13
    apu_p2.elv_code_write=ui->write_elv->checkState()?1:0;//:1;//Бит 14
    apu_p2.sc_UV10_on=ui->led_51->checkState()?1:0;//:1,//бит 15
    apu_p2.sc_UV11_on=ui->led_45->checkState()?1:0;//:1,//бит 16
    apu_p2.az_code_read=ui->read_az->checkState()?1:0;
    apu_p2.elv_code_read=ui->read_elv->checkState()?1:0;

//apu_p2.not_used =0;
udpSocket->writeDatagram((char*)&this->apu_p2,sizeof(apu_p2), ip_p2,port_dev);

    break;
case 3:
    ui->azm->setText(QString::number(get_encoder_az_p(p3.enc_angle_pos_az)));
    ui->raz->setText(QString::number(get_resolver_az_p(p3.motor_encoder_az)));
    ui->eum->setText(QString::number(get_encoder_um_p(p3.enc_angle_pos_elv)));
    ui->rum->setText(QString::number(get_resolver_um_p(p3.motor_encoder_elv)));

    ui->iaz->setText(QString::number(get_integrator_az_p(p3.enc_angle_speed_az)));
    ui->ium->setText(QString::number(get_integrator_um_p(p3.enc_angle_speed_elv)));

    ui->err_az->setCheckState(Qt::CheckState(p3.err_rd_az | p3.err_wr_az) );
    ui->err_elv->setCheckState(Qt::CheckState(p3.err_rd_um | p3.err_wr_um) );

    ui->ok_az->setCheckState(Qt::CheckState(p3.status_rd_az | p3.status_wr_az));
    ui->ok_elv->setCheckState(Qt::CheckState(p3.status_rd_um | p3.status_wr_um));

    ui->data_az->setText(QString::number(p3.receive_SDO1));
    ui->data_elv->setText(QString::number(p3.receive_SDO2));

    apu_p3.ID_packet = cnt++;//1234567890;
#ifdef test_no_p
   apu_p3.angle_pos_az = double_to_lenze_p(get_beta_target());
   apu_p3.angle_pos_elv = double_to_lenze_eps_p(get_epsilon_target());
#else
   apu_p3.angle_pos_az = double_to_lenze_p(radians_to_grad(p_spherical->getB())/*get_beta_imt()*/); //DINT 	(32) 		 Угловая координата азимута.
   apu_p3.angle_pos_elv = double_to_lenze_eps_p(radians_to_grad(p_spherical->getE())) ;//DINT 	(32) 		 Угловая координата угла места.

#endif

   apu_p3.code_az=ui->code->text().toInt();
   apu_p3.subcode_az=ui->subcode->text().toInt();
   apu_p3.code_elv=ui->code->text().toInt();
   apu_p3.subcode_elv=ui->subcode->text().toInt();
   apu_p3.data_az=ui->DataWrite->text().toInt();
   apu_p3.data_elv=ui->DataWrite->text().toInt();
   apu_p3.send_SDO1=ui->DataWrite->text().toInt();
   apu_p3.send_SDO2=ui->data_type->text().toInt();

    apu_p3.angle_speed_az = 0;// -(int)((51.2*60.0*65536.0)/360.0);           //DINT 	(32) 		 Угловая скорость по азимуту.
    apu_p3.angle_speed_elv = 1 ;          //DINT 	(32) 		 Угловая скорость по углу места.
  //  apu_p3.send_SDO1=2;
  //  apu_p3.send_SDO2=3;
 //   apu_p3.code_az=4;
 //   apu_p3.subcode_az=5;
 //   apu_p3.code_elv=6;
  //  apu_p3.subcode_elv=7;
//    apu_p3.data_az=8;
//    apu_p3.data_elv=9;

    apu_p3.az_on=ui->led_49->checkState()?1:0;
    apu_p3.elv_on=ui->led_52->checkState()?1:0;//:1,
    apu_p3.az_en=ui->led_50->checkState()?1:0;//:1,//бит 2 		Р
    apu_p3.elv_en=ui->led_50->checkState()?1:0;//:1,//бит 3 		Р
    apu_p3.az_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 4
    apu_p3.elv_int_clear=ui->led_48->checkState()?1:0;//:1,//бит 5
    apu_p3.az_pos_speed_controll=0;//:1,/
    apu_p3.elv_pos_speed_controll=0;//:1,
    apu_p3.az_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 8
    apu_p3.elv_error_clear=ui->led_46->checkState()?1:0;//:1,//бит 9
    apu_p3.CAN_SDO_ADR=0;//:3,//биты 10-1
    apu_p3.az_code_write=ui->write_az->checkState()?1:0;//:1,//Бит 13
    apu_p3.elv_code_write=ui->write_elv->checkState()?1:0;//:1;//Бит 14
    apu_p3.sc_UV10_on=ui->led_51->checkState()?1:0;//:1,//бит 15
    apu_p3.sc_UV11_on=ui->led_45->checkState()?1:0;//:1,//бит 16
    apu_p3.az_code_read=ui->read_az->checkState()?1:0;
    apu_p3.elv_code_read=ui->read_elv->checkState()?1:0;

//apu_p3.not_used =0;
udpSocket->writeDatagram((char*)&this->apu_p3,sizeof(apu_p3), ip_p3,port_dev);

    break;


default:
    break;
}

#ifdef debug_app

//     if(index++ >= 1000)
//     {index=0;enable_pos=true;/*state=0;*/ // rt_time=0.0;

//     cplt->graph(0)->clearData();cplt->graph(0)->removeDataBefore(count_frameI); //cplt->replot();
//     cplt->graph(1)->clearData();cplt->graph(1)->removeDataBefore(count_frameI); //cplt->replot();

//     cplt->graph(2)->clearData();cplt->graph(2)->removeDataBefore(count_frameI); //cplt->replot();
//     cplt->graph(3)->clearData();cplt->graph(3)->removeDataBefore(count_frameI); //cplt->replot();

//     cplt->graph(4)->clearData();cplt->graph(4)->removeDataBefore(count_frameI); //cplt->replot();
//     cplt->graph(5)->clearData();cplt->graph(5)->removeDataBefore(count_frameI); //cplt->replot();

//     cplt->graph(6)->clearData();cplt->graph(6)->removeDataBefore(count_frameI); //cplt->replot();
//     cplt->graph(7)->clearData();cplt->graph(7)->removeDataBefore(count_frameI); //cplt->replot();


//     cplt->replot();


//     count_frameI=0;
//     }
//else{
//   cplt->graph(2)->addData(count_frameI,get_encoder_az(unv.enc_angle_pos_az));
//   cplt->graph(3)->clearData();
//   cplt->graph(3)->addData(count_frameI,get_encoder_az(unv.enc_angle_pos_az));


//   cplt->graph(6)->addData(count_frameI,get_encoder_um(unv.enc_angle_pos_elv));
//   cplt->graph(7)->clearData();
//   cplt->graph(7)->addData(count_frameI,get_encoder_um(unv.enc_angle_pos_elv));

//   cplt->graph(0)->addData(count_frameI,get_resolver_az(unv.motor_encoder_az));
//   cplt->graph(1)->clearData();
//   cplt->graph(1)->addData(count_frameI,get_resolver_az(unv.motor_encoder_az));

//   cplt->graph(4)->addData(count_frameI,get_resolver_um(unv.motor_encoder_elv));
//   cplt->graph(5)->clearData();
//   cplt->graph(5)->addData(count_frameI,get_resolver_um(unv.motor_encoder_elv));


// ///  vtm=(int)((p2.curr_vel/pith_oborot)*60.0);



//   cplt->replot();
//   count_frameI++;
//     }

#endif

 }
//int tm;
 void Form_mnemoshem::slot_udpSocket()
 {
     int lendg;

     while (udpSocket->hasPendingDatagrams()) {

     lendg = udpSocket->readDatagram((char*)&this->tmp_slv,sizeof(tmp_slv),&ip_tmp,&port_dev_tmp);}

       mono->start(main_clock_works+time_outs);

       ui->label_53->setText(QString("Link"));
       ui->label_53->setStyleSheet(on_fld);
#ifdef local_server

//       tmp_slv.enc_angle_pos_az*=10800;
//       tmp_slv.enc_angle_pos_elv*=10800;
//       tmp_slv.motor_encoder_az*=10000;
//       tmp_slv.motor_encoder_elv*=10000;


#endif
         if(port_dev_tmp == port_125)
         {
       if(ip_tmp == ip_unv){
               ::memcpy(&unv,&tmp_slv,sizeof(unv));
       if(ui->wrt->checkState())
       fprintf(fp,"%f,%f\n",get_encoder_um_p(p1.enc_angle_pos_elv),
                            get_resolver_um_ptst(get_encoder_um_p(p1.enc_angle_pos_elv),get_resolver_um_p(p1.motor_encoder_elv)));
       }
       if(ip_tmp == ip_p1){
               ::memcpy(&p1,&tmp_slv,sizeof(p1));

       }
       if(ip_tmp == ip_p2)
               ::memcpy(&p2,&tmp_slv,sizeof(p2));
       if(ip_tmp == ip_p3)
               ::memcpy(&p3,&tmp_slv,sizeof(p3));
       if(ip_tmp == ip_p4)
               ::memcpy(&p4,&tmp_slv,sizeof(p4));

        }


 }

void Form_mnemoshem::slot_timeout()
{
   ui->label_53->setText(QString("Link of"));
   ui->label_53->setStyleSheet(on_fld_red);
}

//#ifdef local_server
 void Form_mnemoshem::slot_udpServer()
 {
     int len;
     while (udpServer->hasPendingDatagrams()) {

     len = udpServer->readDatagram((char*)&this->tmp_mas,sizeof(tmp_mas),&ip_tmp,&port_dev_tmp);}

     mono->start(main_clock_works+time_outs);
     if(port_dev_tmp == port_125)
     {
         if(ip_tmp == ip_apu)
                 ::memcpy(&apu_apu,&tmp_mas,sizeof(apu_apu));
     }

     ptaz.enable = apu_apu.az_on?1:0;
     ptum.enable = apu_apu.elv_on?1:0;

     ptaz.pos_cmd = apu_apu.angle_pos_az;
     ptum.pos_cmd = apu_apu.angle_pos_elv;

     double t=current_encoder*10800.0 + ptaz.curr_pos;
             if(t >= 360.0)
               t -= 360.0;
           //  t *= 10800.0/10000.0;

     unv.enc_angle_pos_az = (int) (t);// ptaz.curr_pos;//*10800;
     unv.enc_angle_pos_elv =(int) ptum.curr_pos;//*10800;
     unv.motor_encoder_az = (int) ptaz.curr_pos;//*10000;
     unv.motor_encoder_elv =(int) ptum.curr_pos;//*10000;


 udpServer->writeDatagram((char*)&this->unv,sizeof(unv), ip_apu, port_dev);

     ui->label_53->setText(QString("Link"));
     ui->label_53->setStyleSheet(on_fld);




 }
//#endif
