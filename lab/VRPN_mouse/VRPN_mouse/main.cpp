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
char temp = 0b00000000;
char flag = 0b00000011;



void VRPN_CALLBACK handle_analog( void* userData, const vrpn_ANALOGCB a )
{
    int nbChannels = a.num_channel;
    char pos = 0b00000000;
    cout << "Analog : ";
    
    for( int i=0; i < a.num_channel; i++ )
    {
        cout << a.channel[i] << " ";
        
    }
    if(a.channel[0]>0.5){
        pos = pos | 00000100;
    }else{
        pos = pos | 00001000;
    }
    if(a.channel[1]>0.5){
        pos = pos | 00000001;
    }else{
        pos = pos | 00000010;
    }
    switch (pos) {
        case 0b00001000:
            cout<<"A";
            break;
        case 0b01001000:
            cout<<"B";
            break;
        case 0b00000001:
            cout<<"C";
            break;
        case 0b01000001:
            cout<<"D";
            break;
        default:
            cout<<std::bitset<8>(pos);
            break;
    }
    if(temp!=pos){
        cout<<" has changed Zone!";
    }
    temp = pos;
    cout << endl;
}

void VRPN_CALLBACK handle_button( void* userData, const vrpn_BUTTONCB b )
{
    cout << "Button '" << b.button << "': " << b.state << endl;
    if(b.button==2){
        cout <<"Right click!"<<endl;
        if(b.state==1){
            flag = 0b00000000;
        }
        flag = 0b00000000;
    }else{
        
            
        flag = 0b00000001;
        
        
        //vrpnAnalog->unregister_change_handler( 0, handle_analog );
    }
}

int main(int argc, char* argv[])
{
    
    vrpn_Analog_Remote* vrpnAnalog = new vrpn_Analog_Remote("Mouse0@25.79.182.170:3883");
    vrpn_Button_Remote* vrpnButton = new vrpn_Button_Remote("Mouse0@25.79.182.170:3883");
    
    vrpnAnalog->register_change_handler( 0, handle_analog );
    vrpnButton->register_change_handler( 0, handle_button );
    
    
    while(1)
    {
        
        if(flag==0b00000000){
            try{
                vrpnAnalog->register_change_handler( 0, handle_analog );
            }catch(const runtime_error){
                cout<<"runtime_error!"<<endl;
            }
            flag = 0b00000011;
        }
        if(flag==0b00000001){
            try{
                vrpnAnalog->unregister_change_handler( 0, handle_analog );
            }catch(const runtime_error){
                cout<<"runtime_error!"<<endl;
            }
            flag = 0b00000111;
        }
        if(flag==0b00000011){
            try{
                vrpnAnalog->mainloop();
            }catch(const runtime_error){
                cout<<"runtime_error!"<<endl;
            }
        }
        
        vrpnButton->mainloop();
    }
    
    return 0;
}



