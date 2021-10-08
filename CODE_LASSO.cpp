#include <simplecpp>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <conio.h>
#include <time.h>
//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_
#include <fstream>
#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>
#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05
#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)
#define EPSILON 2
#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5
using namespace simplecpp;
void show_VOILA();
        //Prototype: A function to throw an exclaimation on successful catch
class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__

int randomnumber(int upper, int lower){
    ::srand(time(0));
    return (rand() % (upper - lower + 1)) + lower;
}
    //For generating random numbers in a given range

class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, bool clr_code) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin(clr_code);
            //color code for setting color of bombs and balls
  }

  void initCoin(bool);
  void resetCoin(double,double,double,double);
  void resetCoin2(double coin_speed, double coin_angle_deg, double a_y);
            //For a diff set of parameters
  void randomizeXY(){
    coin_start_x=100+(PLAY_X_WIDTH)*0.05*randomnumber(17,4);
    coin_start_y=PLAY_Y_HEIGHT-10*randomnumber(5,0);
  }         //Random positions for coins

}; // End class Coin

//resetCoin2() defn
void Coin::resetCoin2(double coin_speed, double coin_angle_deg, double a_y) {
  coin_ax = 0;
  coin_ay = a_y;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  }

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Coin *the_coin2;
  Coin *the_coin3;
  int num_coins;
  int successes=0;

  void initLasso();
  //void initLasso2();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void yank2();
        //two coin yank
  void yank3();
        //three coin yank
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  int check_for_coin(Coin *coin);
  int check_for_coin(Coin *coin, Coin *coin2);
  int check_for_coin(Coin *coinPtr, Coin *coinPtr2, Coin *coinPtr3);
  int getNumCoins() { return num_coins; }
  int get_success() { return successes;}
  void null_success() {successes=0;}
        //LOL, I wish it was as simple :p

}; // End class Lasso

#endif

//coin.h
//coin initialization; bool for bomb vs coin
void Coin::initCoin(bool i) {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  if(i==false)coin_circle.setColor(COLOR(250,210,0));
  else coin_circle.setColor(COLOR(250,0,0));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin(double coin_speed = COIN_SPEED,double coin_angle_deg = COIN_ANGLE_DEG, double ax_from_prog=0, double ay_from_prog=COIN_G) {
  coin_ax = ax_from_prog;
  coin_ay = ay_from_prog;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

//lasso.cpp
void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

 void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(250,0,0));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(99,65,32));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  the_coin2 = NULL;
  the_coin3 = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(99,65,32));

  lasso_band.setColor(COLOR(135,42,221));
  draw_lasso_band();

} // End Lasso::initLasso()


void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin();
  }
} // End Lasso::yank()
void Lasso::yank2() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin2(COIN_SPEED,90+1.6*randomnumber(4,1),COIN_G-randomnumber(4,0));
  }
  if(the_coin2 != NULL) {
    num_coins++;
    the_coin2->resetCoin2(COIN_SPEED-randomnumber(5,-5),90-1.6*randomnumber(3,1),COIN_G-randomnumber(4,0));
  }
} // End Lasso::yank2()
void Lasso::yank3() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin2(COIN_SPEED-randomnumber(3,-3),90+1.6*randomnumber(4,2),COIN_G);
  }
  if(the_coin2 != NULL) {
    num_coins++;
    the_coin2->resetCoin2(COIN_SPEED-randomnumber(5,-5),90+1.6*randomnumber(3,1),COIN_G-randomnumber(4,0));
  if(the_coin3 != NULL) {
    num_coins++;
    the_coin3->resetCoin2(COIN_SPEED-randomnumber(5,-5),90+1.6*randomnumber(3,1),COIN_G-randomnumber(4,0));
  }
  }
} // End Lasso::yank3()
void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

/*Overloaded check_for_coin functions*/
int Lasso::check_for_coin(Coin *coinPtr, Coin *coinPtr2) {
  int ret=0;
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr ->getYPos();
  double coin_x2 = coinPtr2->getXPos();
  double coin_y2 = coinPtr2->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double xdiff2 = (lasso_x - coin_x2);
  double ydiff2 = (lasso_y - coin_y2);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  double distance2 = sqrt((xdiff2*xdiff2)+(ydiff2*ydiff2));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
    ret++;
    successes++;
    show_VOILA();
  }
  if(distance2 <= LASSO_RADIUS) {
    the_coin2 = coinPtr2;
    the_coin2->getAttachedTo(this);
    ret++;
    successes++;
    show_VOILA();
  }
  return ret;
} // End Lasso::check_for_coin()
int Lasso::check_for_coin(Coin *coinPtr, Coin *coinPtr2, Coin *coinPtr3) {

  int ret=0;
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr ->getYPos();
  double coin_x2 = coinPtr2->getXPos();
  double coin_y2 = coinPtr2->getYPos();
  double coin_x3 = coinPtr3->getXPos();
  double coin_y3 = coinPtr3->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double xdiff2 = (lasso_x - coin_x2);
  double ydiff2 = (lasso_y - coin_y2);
  double xdiff3 = (lasso_x - coin_x3);
  double ydiff3 = (lasso_y - coin_y3);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  double distance2 = sqrt((xdiff2*xdiff2)+(ydiff2*ydiff2));
  double distance3 = sqrt((xdiff3*xdiff3)+(ydiff3*ydiff3));

  if(distance3 <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
    successes--;
    return -1;
  }
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
    ret++;
    successes++;
    show_VOILA();
  }
  if(distance2 <= LASSO_RADIUS) {
    the_coin2 = coinPtr2;
    the_coin2->getAttachedTo(this);
    ret++;
    successes++;
    show_VOILA();
  }

  return ret;
} // End Lasso::check_for_coin()
int Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
    show_VOILA();
    successes++;
    return 1;
  }
  return 0;
} // End Lasso::check_for_coin()
//Exiting function
void game_exit(int flag){
    cout<<"Exit code = "<<flag;
    wait(2);
    exit(flag);
}
/*Instructions*/
void showtext_begin(){
    Rectangle BG(WINDOW_X/2,WINDOW_Y/2,600,600);
    BG.setColor(COLOR("WHITE"));
    BG.setFill();
    Text begin1(WINDOW_X/2,WINDOW_Y/2,"CONTROLS");
    Text begin2(WINDOW_X/2,WINDOW_Y/2+30,"'t' -> Loop Throw                           'y' -> Yanking the Loop              ");
    Text begin3(WINDOW_X/2,WINDOW_Y/2+60,"'[' -> Decrease Throw-Angle             ']' -> Increase Throw-Angle          ");
    Text begin4(WINDOW_X/2,WINDOW_Y/2+90,"'-' -> Reduce Speed before throwing         '=' -> Increase speed before throwing ");
    Text begin5(WINDOW_X/2,WINDOW_Y/2+120,"'q'-> Exit menu--------CLICK ANYWHERE TO EXIT AND START THE GAME            ");
    Text begin6(WINDOW_X/2,WINDOW_Y/2+150,"Throw the lasso and try to catch the balls acc to the level. ");
    Text begin7(WINDOW_X/2,WINDOW_Y/2+180,"If you drop the lasso, or allow a ball to drop without yanking, you loose a life. Also,");
    Text begin8(WINDOW_X/2,WINDOW_Y/2+200,"don't let the loop cross the rightmost wall, aka, the wall of death (LOL) ATB");

    getClick();
}


//Et Voila!!
void show_VOILA(){
    Text voila(WINDOW_X/2,WINDOW_Y/2,"VERY WELL!!");
    wait(1);
}


//Exit menu
void show_post_turn(int totalscore){
    Text post_turn2(WINDOW_X/2,WINDOW_Y/2+30,"Click on X to exit");
    Text post_turn(WINDOW_X/2,WINDOW_Y/2,"To continue, hit 't' and click anywhere else");
    int click=getClick();
        if((click/65536<=(WINDOW_X-5)&&(click/65536>=(WINDOW_X-25)))&&((click%65536<=(25))&&(click%65536>=(5)))){
            char dum[256];
            sprintf(dum, "Net Score : %d", totalscore);
            Text netscore(WINDOW_X/2, WINDOW_Y/2+60, dum);
            getClick();
            exit(0);
        }


    //wait(3);

}

//Gameover and final score
void gameover(int totalscore){
 Text go1(WINDOW_X/2,WINDOW_Y/2,"You have exhausted all your lives. GAME OVER");
 wait(1);
 char dum[256];
 sprintf(dum, "Net Score : %d", totalscore);
 Text netscore(WINDOW_X/2, WINDOW_Y/2+30, dum);
 getClick();
 game_exit(0);
}

//First thing one sees in the game initcanvas
void welcomescreen(){
    Text w1(WINDOW_X/2,WINDOW_Y/2,"Welcome! Get ready with ya lasso to grab the coins and win the game");
    Text w2(WINDOW_X/2,WINDOW_Y/2+30,"Click on 'Instructions' to view instructions");
    Text w3(WINDOW_X/2,WINDOW_Y/2+60,"Else, hit 't' and click anywhere else to throw the lasso");
    Text w4(WINDOW_X/2-30,WINDOW_Y/2+130,"Instructions");
    //cout<<WINDOW_X<<endl<<WINDOW_Y;
    int click=getClick();
    if((click/65536>=(WINDOW_X/2-70)&&(click/65536<=(WINDOW_X/2+10)))&&((click%65536>=(WINDOW_Y/2+125))&&(click%65536<=(WINDOW_Y/2+140)))){
            showtext_begin();

        }

}

//Level pass msg
void levelpass(int lvl){
   Rectangle lvl1(WINDOW_X/2,WINDOW_Y/2,textWidth("Congratulations!! You've passed level one"),textWidth("Congratulations!! You've passed level one") );
        //Rectangle to give a photoclick-like feel
   switch(lvl){
   case (1) : {
                    Text lvl1pass(WINDOW_X/2,WINDOW_Y/2,"Congratulations!! You've passed the first level");
                    wait(2);
                    break;
            }
   case (2) : {
                Text lvl2pass(WINDOW_X/2,WINDOW_Y/2,"Congratulations!! You've passed the second level");
                wait(2);
                break;
            }
   case 3 : {
                Text lvl3pass(WINDOW_X/2,WINDOW_Y/2,"Congratulations!!! You've won the game!!!!!!");
                wait(2);
                break;}
   default:break;
   }
   //wait(3);

}

//Level intro
void level_opening(int lvl){
   switch(lvl){
   case 1 : {
            Text lvl1passA(WINDOW_X/2,WINDOW_Y/2,"Level 1: Easy Peasy \'Lemon\' Squeezy");
            Text lvlpassB(WINDOW_X/2,WINDOW_Y/2+30,"Catch all the yellow balls");
            wait(3);
            }
           break;
   case 2 : {
            Text lvl2passA(WINDOW_X/2,WINDOW_Y/2,"Level 2: Fountain of balls");
            Text lvl2passB(WINDOW_X/2,WINDOW_Y/2+30,"Catch one ball, get one point; two balls, two points");
            Text lvl2passC(WINDOW_X/2,WINDOW_Y/2+60,"Drop one loose one point, drop two, loose two points");
            wait(3);
            }
           break;
   case 3 : {
            Text lvl3passA(WINDOW_X/2,WINDOW_Y/2,"Level 3: Bombs");
            Text lvl3passB(WINDOW_X/2,WINDOW_Y/2+30,"Catch only the yellow ones or loose a point");
            Text lvl3passC(WINDOW_X/2,WINDOW_Y/2+60,"Remember, RED balls are bombs. Do not collect them");
            //Text lvl3passD(WINDOW_X/2,WINDOW_Y/2+90,"This time, the wall of death will just take away all that was yours if you have a ball or two (GameOver;)");
            wait(4);
            }
           break;
   default:break;
   }

}

//Bomb kaboom display
void kaboom(Coin& coin){
    Rectangle boomr(coin.getXPos(),coin.getYPos(),textWidth("KABOOM!!!"),textHeight());
    boomr.setColor(COLOR(250,0,0));
    boomr.setFill();
    Text boom(coin.getXPos(),coin.getYPos(),"KABOOM!!!");
    wait(2);
}


// Level functions ahead
int game_main_lvl1(int totalscore){
  welcomescreen();
  level_opening(1);
  wait(1);
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR(0,0,250));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR(0,0,250));
  Line LOF(PLAY_X_WIDTH+100,PLAY_Y_HEIGHT,100+PLAY_X_WIDTH,PLAY_Y_START);
  LOF.setColor(COLOR(250,0,0));
  /*Close button*/
  Rectangle close(WINDOW_X-15,15,20,20);
  close.setColor(COLOR(250,0,0));
  close.setFill();
  Text close_t(WINDOW_X-15,15,"X");
  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins : %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  char ScoreStr[256];
  sprintf(ScoreStr, "Score : %d", lasso.get_success());
  Text mainScore(WINDOW_X-50, 50,ScoreStr);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta,false);
  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  int yanks=10;
  char YANKS[256];
  sprintf(YANKS, "Lives Left : %d", yanks);
  Text yanks_left(WINDOW_X-50, 80,YANKS);
        //yanks= lives left. Name for historical reasons
  int coin_status=0;
  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, open exit menu
  for(;;) {

    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {

      char c = charFromEvent(e);

      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
                    lasso.unpause();
                    break;
      case 'y':{
                    lasso.yank();
                    //cout<<"COIN STAUS: "<<lasso.check_for_coin(&coin)<<endl;
                    if(coin_status==0)yanks--;
                    if(yanks<=0) {
                        gameover(totalscore+lasso.get_success());
                    }
                }
                break;
      case 'l':
                lasso.loopit();
                coin_status=lasso.check_for_coin(&coin);
                wait(STEP_TIME*5);
                break;
      case '[':
                if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
                break;
      case ']':
                if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
                break;
      case '-':
                if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
                break;
      case '=':
                if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
                break;
      case 'q':
                {
                    lasso.pause();show_post_turn(totalscore+lasso.get_success());lasso.unpause();;
                    break;
                }
      default:
                break;
      }

    }

    lasso.nextStep(stepTime);   //Probably velocity changer

    coin.nextStep(stepTime);   //velociy changer
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
        coin.randomizeXY();
        coin_status=0;
        coin.unpause();
      }
    }

        if(coin.getYPos() > PLAY_Y_HEIGHT||coin.getXPos()> (100+PLAY_X_WIDTH)) {
                if(coin_status==0)yanks--;
                if(yanks<=0) {
                    gameover(totalscore+lasso.get_success());
                }
        coin_status=0;
        coin.resetCoin();


        last_coin_jump_end = currTime;
        }

        else if(((PLAY_Y_HEIGHT-lasso.getYPos() )<= EPSILON||lasso.getXPos()> (100+PLAY_X_WIDTH))&&(coin_status==0)){
                yanks--;
                if(yanks<=0) {
                    gameover(totalscore+lasso.get_success());
                }
	    coin_status=0;
	    lasso.yank();
        }
        sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
        coinScore.setMessage(coinScoreStr);
        sprintf(ScoreStr, "Score : %d", lasso.get_success());
        mainScore.setMessage(ScoreStr);

        sprintf(YANKS, "Lives Left : %d", yanks);
        yanks_left.setMessage(YANKS);

        currTime += stepTime;
        wait(stepTime);

    //Levels part
        if(lasso.get_success()>=10) {
            levelpass(1);
            return totalscore+lasso.get_success();
    }
  }
  return 0;// End for(;;)
//

}
int game_main_lvl2(int totalscore){

  welcomescreen();
  level_opening(2);
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR(0,0,250));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR(0,0,250));
  /*Close button*/
  Rectangle close(WINDOW_X-15,15,20,20);
  close.setColor(COLOR(250,0,0));
  close.setFill();
  Text close_t(WINDOW_X-15,15,"X");
  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins : %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  char ScoreStr[256];
  sprintf(ScoreStr, "Score : %d", lasso.get_success());
  Text mainScore(WINDOW_X-50, 50,ScoreStr);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta,false);
  Coin coin2(coin_speed, coin_angle_deg+5, coin_ax+2, coin_ay-3, paused, rtheta,false);
                //Second coin
  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  int yanks=20;
  char YANKS[256];
  sprintf(YANKS, "Lives Left : %d", yanks);
  Text yanks_left(WINDOW_X-50, 80,YANKS);
        //yanks= lives left. Name for historical reasons
  int coin_status=0;
  for(;;) {

    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {

      char c = charFromEvent(e);

      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
        case 't':
                    lasso.unpause();
                    break;
        case 'y':{
                    lasso.yank2();
                    lasso.check_for_coin(&coin, &coin2);
                    coin.resetCoin2(COIN_SPEED-randomnumber(5,3),90,COIN_G-randomnumber(4,0));
                    //coin3.resetCoin2(COIN_SPEED-randomnumber(8,-8),90-1.6*randomnumber(3,-3),COIN_G-randomnumber(4,0));
                    coin2.resetCoin2(COIN_SPEED-randomnumber(-3,-5),90+1.6*randomnumber(5,3),COIN_G-randomnumber(4,0));
                    if(coin_status==0)yanks-=1;
                    if(yanks<=0) {
                        gameover(totalscore+lasso.get_success());
                    }
        }
                    break;
        case 'l':
                    lasso.loopit();
                    coin_status=lasso.check_for_coin(&coin,&coin2);
                    wait(STEP_TIME*5);
                    break;
        case '[':
                    if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
                    break;
        case ']':
                    if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
                    break;
        case '-':
                    if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
                    break;
        case '=':
                    if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
                    break;
        case 'q':
                    {lasso.pause();show_post_turn(totalscore+lasso.get_success());lasso.unpause();;
                        break;}
        default:
                    break;
      }
    }
    lasso.nextStep(stepTime);   //Probably velocity changer

    coin.nextStep(stepTime);   //velociy changer
    coin2.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
    coin.randomizeXY();
    coin_status=0;
	coin.unpause();
      }
    }
    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
    coin2.randomizeXY();
    coin_status=0;
	coin2.unpause();
      }
    }
    if(((coin2.getYPos() > PLAY_Y_HEIGHT||coin2.getXPos()> (100+PLAY_X_WIDTH)))||(coin.getYPos() > PLAY_Y_HEIGHT||coin.getXPos()> (100+PLAY_X_WIDTH))){
    if(coin.getYPos() > PLAY_Y_HEIGHT||coin.getXPos()> (100+PLAY_X_WIDTH)) {
      if(coin_status==0) yanks-=1;
	  if(yanks<=0) {
        gameover(totalscore+lasso.get_success());
	  }
	  coin_status=0;
      coin.resetCoin2(coin_speed,90,COIN_G);
                //90, so that it is a bit easy
      last_coin_jump_end = currTime;
    }
    if(coin2.getYPos() > PLAY_Y_HEIGHT||coin2.getXPos()> (100+PLAY_X_WIDTH)) {
      if(coin_status==0) yanks-=1;
	  if(yanks<=0) {
        gameover(totalscore+lasso.get_success());
	  }
	  coin_status=0;
      coin2.resetCoin2(coin_speed,95+randomnumber(5,2),COIN_G-4);
                        //randomnumber() just for the sake of some randomness
      last_coin_jump_end = currTime;
    }
    }
    else if (((PLAY_Y_HEIGHT-lasso.getYPos() )<= EPSILON||lasso.getXPos()> (100+PLAY_X_WIDTH))&&(coin_status==0)){
        yanks--;
	    if(yanks<=0) {
            gameover(totalscore+lasso.get_success());
	    }
	    coin_status=0;
	    lasso.yank2();
    }
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    sprintf(ScoreStr, "Score : %d", lasso.get_success());
    mainScore.setMessage(ScoreStr);

    sprintf(YANKS, "Lives Left : %d", yanks);
    yanks_left.setMessage(YANKS);
    //stepCount++;
    currTime += stepTime;
    wait(stepTime);
    //Levels part
    if(lasso.get_success()>=10) {
        levelpass(2);
        return totalscore+lasso.get_success();
    }
  } // End for(;;)
 return 0;

}
int game_main_lvl3(int totalscore){
  welcomescreen();
  level_opening(3);
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR(0,0,250));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR(0,0,250));
  /*Close button*/
  Rectangle close(WINDOW_X-15,15,20,20);
  close.setColor(COLOR(250,0,0));
  close.setFill();
  Text close_t(WINDOW_X-15,15,"X");
  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins : %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  char ScoreStr[256];
  sprintf(ScoreStr, "Score : %d", lasso.get_success());
  Text mainScore(WINDOW_X-50, 50,ScoreStr);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta,false);
  Coin coin2(coin_speed, coin_angle_deg+5, coin_ax+2, coin_ay-3, paused, rtheta,false);
  Coin coin3(coin_speed, coin_angle_deg-5, coin_ax-2, coin_ay+3, paused, rtheta,true);
                //The three coins
  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  int yanks=30;
  char YANKS[256];
  sprintf(YANKS, "Lives Left : %d", yanks);
  Text yanks_left(WINDOW_X-50, 80,YANKS);
        //yanks= lives left. Name for historical reasons
  int coin_status=0;
  for(;;) {

    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {

      char c = charFromEvent(e);

      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
        case 't':
                    lasso.unpause();
                    break;
        case 'y':{
                    lasso.yank3();
                    coin.resetCoin2(COIN_SPEED-randomnumber(5,-5),90,COIN_G-randomnumber(4,0));
                    coin3.resetCoin2(COIN_SPEED-randomnumber(8,-8),90-1.6*randomnumber(5,4),COIN_G-randomnumber(4,0));
                    coin2.resetCoin2(COIN_SPEED-randomnumber(5,-5),90+1.6*randomnumber(3,2),COIN_G-randomnumber(4,0));
                    //lasso.check_for_coin(&coin, &coin2, &coin3);
                    if(coin_status==0)yanks-=1;
                    if(yanks<=0) {
                        gameover(totalscore+lasso.get_success());
                        coin_status=0;
                    }
        }
                    break;
        case 'l':
                    lasso.loopit();
                    coin_status=lasso.check_for_coin(&coin,&coin2,&coin3);
                    if(coin_status==-1) {
                            yanks--;
                            coin_status=0;
                            kaboom(coin3);
                            lasso.yank3();
                    }
                    wait(STEP_TIME*5);
                    break;
        case '[':
                    if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
                    break;
        case ']':
                    if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
                    break;
        case '-':
                    if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
                    break;
        case '=':
                    if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
                    break;
        case 'q':
                    {lasso.pause();show_post_turn(totalscore+lasso.get_success());lasso.unpause();;
                    break;}
        default:
                    break;
      }
    }
    lasso.nextStep(stepTime);   //Probably velocity changer

    coin.nextStep(stepTime);   //velociy changer
    coin2.nextStep(stepTime);
    coin3.nextStep(stepTime);
    if(coin.isPaused()) {
        if((currTime-last_coin_jump_end) >= COIN_GAP) {
            coin.randomizeXY();
            coin_status=0;
            coin.unpause();
      }
    }
    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
            coin2.randomizeXY();
            coin_status=0;
            coin2.unpause();
      }
    }
    if(coin3.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
            coin3.randomizeXY();
            coin_status=0;
            coin3.unpause();
      }
    }
    if(((coin2.getYPos() > PLAY_Y_HEIGHT||coin2.getXPos()> PLAY_X_WIDTH))||(coin.getYPos() > PLAY_Y_HEIGHT||coin.getXPos()> (100+PLAY_X_WIDTH))||(coin3.getYPos() > PLAY_Y_HEIGHT||coin3.getXPos()> (100+PLAY_X_WIDTH))){
        if(coin.getYPos() > PLAY_Y_HEIGHT||coin.getXPos()> PLAY_X_WIDTH) {
            if(coin_status==0) yanks-=1;
            if(yanks<=0) {
                gameover(totalscore+lasso.get_success());
            }
            coin.resetCoin2(COIN_SPEED-randomnumber(5,-5),90,COIN_G-randomnumber(4,0));
            last_coin_jump_end = currTime;
        }
        if(coin3.getYPos() > PLAY_Y_HEIGHT||coin.getXPos()> (100+PLAY_X_WIDTH)) {
            coin3.resetCoin2(COIN_SPEED-randomnumber(8,-8),90-1.6*randomnumber(3,2),COIN_G-randomnumber(4,0));
            last_coin_jump_end = currTime;
        }
        if(coin2.getYPos() > PLAY_Y_HEIGHT||coin2.getXPos()> (100+PLAY_X_WIDTH)) {
            if(coin_status==0) yanks-=1;
            if(yanks<=0) {
            gameover(totalscore+lasso.get_success());
        }
        coin2.resetCoin2(COIN_SPEED-randomnumber(5,-5),90+1.6*randomnumber(5,3),COIN_G-randomnumber(4,0));
        last_coin_jump_end = currTime;
        }
        coin_status=0;
        }
        else if (((PLAY_Y_HEIGHT-lasso.getYPos() )<= EPSILON||lasso.getXPos()> (100+PLAY_X_WIDTH))&&(coin_status==0)){
        yanks--;
	    if(yanks<=0) {
            gameover(totalscore+lasso.get_success());
	    }
	    coin_status=0;
	    lasso.yank();
    }
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);
    sprintf(ScoreStr, "Score : %d", lasso.get_success());
    mainScore.setMessage(ScoreStr);

    sprintf(YANKS, "Lives Left : %d", yanks);
    yanks_left.setMessage(YANKS);
    //stepCount++;
    currTime += stepTime;
    wait(stepTime);
    //Levels part
    if(lasso.get_success()>=10) {
        levelpass(3);
        return totalscore+lasso.get_success();
    }
  } // End for(;;)
 return 0;
}
void game_entry(){      //Game launcher
    int totalscore=0;
    cout<<"Game Started:::";
    cout<<"\n *        **     ****   ****     ***"
        <<"\n *       *  *   *      *        *   *"
        <<"\n *      *    *   ****   ****    *   *"
        <<"\n *     ********      *       *  *   *"
        <<"\n **** *        * ****   ****     ***";
    cout<<endl;
    char p_name[21];
    cin.ignore(sizeof(char));
    cout<<"Enter your name: ";
    cin.getline(p_name,20);
    cout<<"\n Very well!! The game will launch now";
    wait(3);
    initCanvas("Lasso", WINDOW_X, WINDOW_Y);
    Text nameT(WINDOW_X-100-textWidth(p_name)-10,20,"NAME: ");
    sprintf(p_name, p_name,0);
    Text yanks_left(WINDOW_X-100, 20,p_name);
    totalscore=game_main_lvl1(totalscore);
    system("cls");
    cout<<"\n *        **     ****   ****     ***"
        <<"\n *       *  *   *      *        *   *"
        <<"\n *      *    *   ****   ****    *   *"
        <<"\n *     ********      *      *   *   *"
        <<"\n **** *        * ****   ****     ***";
    cout<<endl;

    cout<<endl
        <<"Whoa!! You cleared the first level. \nNow, We'll throw two balls from a single spout. Score 10 points to win"
        <<endl;

    totalscore=game_main_lvl2(totalscore);
    system("cls");
    cout<<"\n *        **     ****   ****     ***"
        <<"\n *       *  *   *      *        *   *"
        <<"\n *      *    *   ****   ****    *   *"
        <<"\n *     ********      *       *  *   *"
        <<"\n **** *        * ****   ****     ***";
    cout<<endl;
    cout<<endl
        <<"Now, I must admit, you are smart!! You cleared the second level too. \nNow, We'll throw THREE balls from a single spout. Score 10 points to win"
        <<endl;
    totalscore=game_main_lvl3(totalscore);
    char dum[256];
    sprintf(dum, "Net Score : %d", totalscore);
    Text netscore(WINDOW_X/2, WINDOW_Y/2, dum);
    Text end_t(WINDOW_X/2,WINDOW_Y/2+30,"CLICK ANYWHERE TO EXIT");
    getClick();
    exit(0);

}

//Main Function:
main_program {

cout<<"Welcome!! "
    <<"\n1. To enter the game, enter 'p'"
    <<"\n2. To exit, enter '.'\n";

char choice;
while(true){
cin>>choice;
switch (choice){
case 'p': game_entry(); break;
case '.': game_exit(0); break;
default : break;

}

}

} // End main_program
