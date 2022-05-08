//
// Created by Nekbakht Zabirov on 08.05.2022.
//

#ifndef UNTITLED1_MYSERVER_H
#define UNTITLED1_MYSERVER_H


class MyServer {
public:
    typedef void (*OnLeftWingMove)(int);
    typedef void (*OnRightWingMove)(int);
    typedef void (*OnSpeedUp)(int);
    void setup();
    void run(OnLeftWingMove onLeftWingMove,
             OnRightWingMove onRightWingMove,
             OnSpeedUp onSpeedUp);
};


#endif //UNTITLED1_MYSERVER_H
