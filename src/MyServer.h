//
// Created by Nekbakht Zabirov on 08.05.2022.
//

#ifndef UNTITLED1_MYSERVER_H
#define UNTITLED1_MYSERVER_H

    typedef void (*OnLeftWingMove)(int);
    typedef void (*OnRightWingMove)(int);
    typedef void (*OnSpeedUp)(int);

class MyServer {
public:

    void setup();
    void run(OnLeftWingMove onLeftWingMove,
             OnRightWingMove onRightWingMove,
             OnSpeedUp onSpeedUp);
};


#endif //UNTITLED1_MYSERVER_H
