//
//  main.cpp
//  VRPN_mouse
//
//  Created by 劉獻章 on 2019/11/25.
//  Copyright © 2019 chimpanze. All rights reserved.
//
#include "/usr/local/Cellar/vrpn/07.34/include/vrpn_Analog.h"
#include "/usr/local/Cellar/vrpn/07.34/include/vrpn_Tracker.h"
#include "/usr/local/Cellar/vrpn/07.34/include/vrpn_Button.h"
#include <iostream>
using namespace std;
//temp for last status of analog tracker
static unsigned char temp;
//flag for status of actived or deactived
static unsigned char flag;
//unsigned char temp = 0b00000000;
//unsigned char flag = 0b00000011;



void VRPN_CALLBACK handle_analog( void* userData, const vrpn_ANALOGCB a )
{
    int nbChannels = a.num_channel;
    //use a byte to store position flag
    unsigned char pos = 0b00000000;
    cout << "Analog : ";
    //print mouse position
    for( int i=0; i < a.num_channel; i++ )
    {
        cout << a.channel[i] << " ";
        
    }
    //use bitwise to operate zone a-d
    if(a.channel[0]>0.5){
        pos = pos | 0b00000100;
    }else{
        pos = pos | 0b00001000;
    }
    if(a.channel[1]>0.5){
        pos = pos | 0b00000001;
    }else{
        pos = pos | 0b00000010;
    }
    switch (pos) {
        case 0b00001010:
            cout<<"A";
            break;
        case 0b00000110:
            cout<<"B";
            break;
        case 0b00001001:
            cout<<"C";
            break;
        case 0b00000101:
            cout<<"D";
            break;
        default:
            cout<<std::bitset<8>(pos);
            break;
    }
    //if current zone is different then before print changed Zone
    if(temp!=pos){
        cout<<" has changed Zone!";
        //for beep
        //cout << '\a';
    }
    //assigned current to last position
    temp = pos;
    cout << endl;
}

void VRPN_CALLBACK handle_button( void* userData, const vrpn_BUTTONCB b )
{
    //print button status
    cout << "Button '" << b.button << "': " << b.state << endl;
    if(b.button==2){
        //if it is right button click print right click
        cout <<"Right click!"<<endl;
        //for beep
        //cout << '\a';
        flag = 0b00000000;
    }else{
        //if it is left button click deactivated analog tracker
        flag = 0b00000001;
    }
}

int main(int argc, char* argv[])
{
    //assigned 2 tracker
    vrpn_Analog_Remote* vrpnAnalog = new vrpn_Analog_Remote("Mouse0@25.79.182.170:3883");
    vrpn_Button_Remote* vrpnButton = new vrpn_Button_Remote("Mouse0@25.79.182.170:3883");
    //assigned 2 change triger
    vrpnAnalog->register_change_handler( 0, handle_analog );
    vrpnButton->register_change_handler( 0, handle_button );
    //init the variable
    temp = 0b00000000;
    flag = 0b00000011;
    //loop
    while(1)
    {
        //if right click active analog handler
        if(flag==0b00000000){
            try{
                vrpnAnalog->register_change_handler( 0, handle_analog );
            }catch(const runtime_error){
                cout<<"runtime_error!"<<endl;
            }
            flag = 0b00000011;
        }
        //if left click deactivate analog handler
        if(flag==0b00000001){
            try{
                vrpnAnalog->unregister_change_handler( 0, handle_analog );
            }catch(const runtime_error){
                cout<<"runtime_error!"<<endl;
            }
            flag = 0b00000111;
        }
        //if it is active then we do tracking
        if(flag==0b00000011){
            try{
                vrpnAnalog->mainloop();
            }catch(const runtime_error){
                cout<<"runtime_error!"<<endl;
            }
        }
        //track button
        vrpnButton->mainloop();
    }
    
    return 0;
}



